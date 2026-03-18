/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

//
// mipHeuristic.cpp
//
// 
//
// 
//

#include <acro_config.h>
#include <CoinPackedMatrix.hpp>
#include <utilib/_math.h>
#include <utilib/PM_LCG.h>
#include <pico/PicoLPInterface.h>
#include <pico/MILPProblem.h>
#include <pico/mipHeuristic.h>
#include <pico/milp.h> 
#include <pico/milpNode.h>
#include <pico/meritFunction.h>
#include <pico/parMilp.h>
#include <pico/PicoCglGomory.hpp>

using namespace std;

namespace pico {

  #if !defined(WITHOUT_COMMONIO)
  using utilib::Flush;
  #endif

  MIPHeuristic::MIPHeuristic( MILP* mGlobal_,
			      bool  useCanonicalOnly,
			      int   mainDebugLevel) : 
    heurObjBound(1e30), 
    weight(0), 
    numInts(0), 
    sol(NULL), 
    myRNG(0),
    myUniform(&myRNG),
    mGlobal(mGlobal_),
    MIP(mGlobal_->MIP),
    globalBoundKnown(false),
    globalBound(0),
    cfg(mGlobal_->heurCfg),
    totalHeurIter(0l), 
    numCalls(0),
    mf(NULL),
    canonicalOnly(useCanonicalOnly),
    runCounter(0),
    oldTotalInts(-1),  // -1 forces creating new merit function on first use
    sumDepth(0),
    rampUpCalls(0),
    rampUpSuccesses(0),
    emptyFlag(true)
 { 
    if (cfg.parameter_initialized("bbhDebug"))
      setDebug(cfg.bbhDebug);
    else
      setDebug(mainDebugLevel);

    heurPoolSize = cfg.bbhMaxPool;
    DEBUGPR(50,ucout << "Heuristic pool size is " << heurPoolSize << endl);
    heurPool.resize(heurPoolSize);
    for (int i=0; i<heurPoolSize; i++)
      heurPool[i] = NULL;

    int randomSeed = cfg.bbhSeedAlpha;
#ifdef ACRO_HAVE_MPI
    if (uMPI::running())
      randomSeed += uMPI::rank;
#endif
    myRNG.reseed(randomSeed);
    double junk = myUniform();
    for(int i=0; i<5; i++)
      junk = myUniform();
    
    init( MIP->lp );

  } 
  

  MIPHeuristic::~MIPHeuristic()
  {
    if ( mf != NULL ) delete mf;    
    for(int i=0; i<heurPoolSize; i++)
      if (heurPool[i] != NULL)
	delete heurPool[i];
  }


  //========================
  
  void MIPHeuristic::init(PicoLPSolver& lp )
  {
    DEBUGPR(10, ucout << "MIPHeuristic::init() " << endl);

    numInts = MIP->numIntegerVars();

    nc = lp()->getNumCols();
    nr = lp()->getNumRows();

    DEBUGPR(20,ucout << "nc=" << nc << ", nr=" << nr << endl);

    maxExtraCols = 2*cfg.bbhAddCols*(MIP->numGeneralIntVars());
    maxTotalCols = nc + maxExtraCols;
    maxTotalInts = numInts + maxExtraCols;

    DEBUGPR(20,ucout << "maxExtraCols=" << maxExtraCols
	    << " maxTotalCols=" << maxTotalCols
	    << " maxTotalInts=" << maxTotalInts << endl);

    cloneIndex.resize(maxExtraCols);
    sign.resize(maxExtraCols);
    colIndices.resize(maxExtraCols);
    allIndices.resize(maxTotalCols);

    // The allIndices array contains just ascending integers

    for(int i=0; i<maxTotalCols; i++)
      allIndices[i] = i;

    // The colIndices array just holds the indices of all possible
    // added columns (we probably won't use all of it!)

    for(int i=0; i<maxExtraCols; i++)
      colIndices[i] = nc+i;

    // The intList array holds the indices of all integer variables.
    // We set this up so it holds all possible added variables too.

    intList.resize(maxTotalInts);
    for(int i=0; i<numInts; i++)
      intList[i] = MIP->integerVarNdx[i];
    for(int i=0; i<maxExtraCols; i++)
      intList[numInts + i] = nc + i;

    // Copy the linear programming interface object

    copyLP(lp);

    DEBUGPR(50,ucout << "Analyzing objective function...\n");

    const double objSense = lp()->getObjSense();
    
    c.resize(maxTotalCols);
    const double* cPtr = lp()->getObjCoefficients();
    for(int i=0; i<nc; i++)
      c[i] = *(cPtr++);
    for(int i=nc; i<maxTotalCols; i++)
      c[i] = 0;

    cNzCount = 0;
    cSparse.resize(nc);
    cSparseNdx.resize(nc);
    
    //Defines what coefficients are to be considered zero in the LP objective
    double nzTol = cfg.bbhObjectiveTol;    // = 1e-8;
    
    //Compute the scale
    double sumc    = 0.0;
    double suminvc = 0.0;
    for (int i = 0; i < nc; i++) 
      {
	double ci = c[i];
	double aci = fabs(ci);
	if (aci >= nzTol) 
	  {
	    sumc += aci;
	    suminvc += 1.0 / aci;
	    cSparseNdx[cNzCount] = i;
	    cSparse[cNzCount++] = objSense * ci;
	  }
      }

    cSparse.resize(cNzCount);
    cSparseNdx.resize(cNzCount);
    cVector.setVector(cNzCount,cSparseNdx.data(),cSparse.data());
    
    //Set the weight for the original objective
    //Note: may want to use the number of integer variables minus those
    //fixed in a particular subproblem
    weight = objSense * sqrt(suminvc/sumc) 
      * (cfg.bbhWeight * MIP->numIntegerVars()) / cNzCount;

    // Make an array to hold variable types of the modified problem
    // All added variables should be binary.  The original general integer
    // variables become essentially binary, but may be offset from the 
    // interval [0,1], so they stay official "general integer".
    heurVType.resize(maxTotalCols);
    for(int j=0; j<nc; j++)
      heurVType[j] = MIP->vtype[j];
    for(int j=nc; j<maxTotalCols; j++)
      heurVType[j] = binary;

    DEBUGPR(50, ucout<<"weight="<<weight<<" cNzCount="<<cNzCount);
    DEBUGPRP(50, ucout<<"\ncSparseNdx: ");
    DEBUGPRP(50, for (int j=0;j<cNzCount;j++) ucout<<cSparseNdx[j]<<" ");
    DEBUGPR(50, ucout << endl);
    
    DEBUGPRP(50, ucout<<"\ncSparse:    ");
    DEBUGPRP(50, for (int j=0;j<cNzCount;j++) ucout<<cSparse[j]<<"   ");
    DEBUGPR(50, ucout << endl);

    // Objective row is no longer added at this point.  It is
    // added anew after each setupLP call.

    DEBUGPR(50, ucout << "MIPHeuristic::init() is done\n");    
    
  }


  //+++++++++++++++++++++++++++++++++++++++++++++++
  
  void MIPHeuristic::copyLP(PicoLPSolver& lp)
  {
    DEBUGPR(50,ucout << "MIPHeuristic::copyLP(lp)\n");
    
    // According to Bill, this copies the LP object
    solOwner = lp;
    sol = solOwner();

    DEBUGPR(90,sol->dumpColInfo("Start copyLP"));
    
    sol->setSolving();          // Most of the time, we're not cutting
    sol->setObjSense(pebbl::minimize); // Always minimize

    // Code below should be extraneous now; check with Bill/Cindy %%%%%%%%
    // Should we set a flag here saying we're not cutting now? %%%%%%%%
    
    //For now, set sol's integrality info here
    //(may have to set it depending on a subproblem)

//     DEBUGPR(50, ucout << "Initial sol's integrality info: " << endl;
// 	    for (int i = 0; i < nc; i++) 
// 	       ucout << sol->isContinuous(i) << " " 
// 	             << sol->isInteger(i) << endl);

//     sol->setInteger((MIP->integerVarNdx).data(), MIP->numIntegerVars()); 
    
//     DEBUGPR(50, ucout <<"MIP's integrality info: " << endl
// 	    << "MIP->numIntegerVars()=" << MIP->numIntegerVars() << endl;;
//             for (int i = 0; i < MIP->numIntegerVars(); i++) 
//                 ucout << (MIP->integerVarNdx).data()[i] << " ");
    
//     DEBUGPR(50, ucout << "sol's integrality info after setInteger(): " << endl;
// 	    for (int i = 0; i < nc; i++)
// 	        ucout << sol->isContinuous(i) << " ");
//     DEBUGPR(50, ucout << endl);

    sol->setDblParam(OsiPrimalTolerance, cfg.bbhFeasibilityTol);
    sol->setIntParam(OsiMaxNumIteration, cfg.bbhMaxHeurPivots);    

    sol->setHintParam(OsiDoReducePrint, true, OsiHintDo); //Reduce sol's print

    DEBUGPR(50, ucout<<"checking bbhConfig " << cfg.bbhFeasibilityTol
	    << " " << cfg.bbhMaxHeurPivots << " ");
    DEBUGPR(50, ucout<< cfg.bbhObjectiveTol << " "<< cfg.bbhWeight <<endl);
  }
  

  //+++++++++++++++++++++++++++++++++++++++++++++++
  
  void MIPHeuristic::addObjRow()
  {
    DEBUGPR(50, ucout << "MIPHeuristic::addObjRow(): heurObjBound=" 
	    << heurObjBound << endl);

    // Add the objective row as a constraint 
    // (will always correspond to minimization,
    // so need to set the upper bound in a subproblem) 

    sol->addRow(cVector,'L',heurObjBound,0);
  }


  //+++++++++++++++++++++++++++++++++++++++++++++++
  
  void MIPHeuristic::rebuildLP()
  {
    DEBUGPR(10,ucout << "Rebuilding heuristic LP\n");
    copyLP(MIP->lp);
  }
  

  //++++++++++++++++++++++++----------------------------  

  void MIPHeuristic::feedIn(MILPNode* sp)
  {
    DEBUGPR(10,ucout << "MIPHeuristic::feedIn()\n");

    int    slot       = -1;
    double worstScore = scanPool(slot,findLowest);
    double spScore    = randomizedScore(sp);
    DEBUGPR(20,ucout << "worstScore=" << worstScore << ", slot=" << slot
	    << ", spScore=" << spScore << endl);
    if (spScore > worstScore)
      {
	DEBUGPR(10,ucout << "Replacing slot " << slot << endl);
	if (heurPool[slot] != NULL)
	  {
	    bool found;
	    population.remove(heurPool[slot]->mfValue,found);
	    //if (!found)
	      //Warning(errmsg("Missing merit function value for heurPool"));
	    delete heurPool[slot];
	  }
	heurPool[slot] = sp->makeLPSetupObj();
	population.add(heurPool[slot]->mfValue);
	DEBUGPR(50,ucout << "Now contains " << heurPool[slot] << endl);
	emptyFlag = false;
      }
  }


  //++++++++++++++++++++++--------------------------------

  bool MIPHeuristic::parRun()
  {
    DoubleVector newSol; 
    double newValue;
    DoubleVector boundList;
    DoubleVector solution;
    BasisArray basis;

    DEBUGPR(2, ucout << "MIPHeuristic::parRun() " << endl);

    // Find the element of the mini-pool with the highest (randomized)
    // score.  A negative score means there was nothing there, so
    // bail out with no solution.
 
   int    slot = -1;
   double bestScore = scanPool(slot,findHighest);
   DEBUGPR(10,ucout << "bestScore=" << bestScore << ", slot=" << slot << endl);
   if (bestScore < 0)
     {
       DEBUGPR(10,ucout << "Nothing to do -- bailing out\n");
       emptyFlag = true;
       return false;
     }
 
   // If we get here, then we found something in heurPool to round.
   // use it to set up the LP.

   heurPool[slot]->setupLP(sol);
   setObjBound(heurPool[slot]->bound);

   // Now run the heuristic and see if we get anything.

   bool feasibleSolFound =  run(newSol,
				newValue,
				heurPool[slot]->savedSolution);

   if (feasibleSolFound)  
     {
       DEBUGPR( 1, ucout<<"^^^^^ MIPHeuristic found a feasible solution "
		<< newValue<<" ^^^^^ " << endl);
       DEBUGPR( 4, ucout << "^^^^^  solution = "<< newSol << endl );
       updateIncumbent(newSol,newValue);
     }

 
   if (cfg.bbhPoolRemove)
     {
       delete heurPool[slot];
       heurPool[slot] = NULL;
     }

   return feasibleSolFound;
   
  }
  

  //++++++++++++++++++++++--------------------------------

  bool MIPHeuristic::run( DoubleVector& newSol, 
			  double&       newValue,
			  const DoubleVector& solution,
			  bool wantCanonical)
  {
    DEBUGPR(5, ucout << "MIPHeuristic::run() solution.size()=" 
	    << solution.size() << endl);

    DEBUGPR(90,sol->dumpColInfo("Start run"));
#ifdef ACRO_VALIDATING
    DEBUGPR(150,dumpSolBasis(sol,ucout,"Start of heuristic:"));
#endif

    // Now throw in a constraint on the objective.  This used to 
    // happen much earlier.  Note that a basis should already be 
    // loaded into "sol".

    addObjRow();
    int startRows = sol->getNumRows();

    // Tightens the bounds on all general integer variables
    // to a single unit, but possibly add bracketing binary 
    // variables.

    extraCols = 0;

    DoubleVector lower(numInts);
    DoubleVector upper(numInts);
    const double* lowerPtr = sol->getColLower();
    const double* upperPtr = sol->getColUpper();
    for(int i=0; i<numInts; i++)
      {
	int j = intList[i];
	lower[i] = lowerPtr[j];
	upper[i] = upperPtr[j];
      }

    for(int i=0; i<numInts; i++)
      {
	int j = intList[i];
	if (MIP->vtype[j] == generalInteger)
	  {
	    int lb = lround(lower[i]);
	    int ub = lround(upper[i]);

	    DEBUGPR(100,ucout << "i=" << i << " j=" << j 
		    << " lb=" << lb << " ub=" << ub << endl);

	    if (ub - lb > 1)
	      {
		int newLB = lb;
		int newUB = ub;
		double val  = solution[i];  // Note: solution is just integer
		int    ival = lround(val);  // variables here.
		if (ival == lb)
		  newUB = lb + 1;
		else if (ival == ub)
		  newLB = ub - 1;
		else
		  {
		    newLB = lround(max(floor(val),(double) lb));
		    newUB = min(newLB+1,ub);
		  }
		if (newLB > lb)
		  sol->setColLower(j,(double) newLB);
		if (newUB < ub)
		  sol->setColUpper(j,(double) newUB);

		DEBUGPR(30,ucout << 'x' << j << '=' << val 
			<< ": [" << lb << ',' << ub << "] --> ["
			<< newLB << ',' << newUB << "]\n");

		// Add duplicate columns if necessary and allowd

		int addColsUp   = min(cfg.bbhAddCols, ub - newUB);
		int addColsDown = min(cfg.bbhAddCols, newLB - lb);

		if (addColsUp + addColsDown > 0)
		  {
		    CoinPackedVector newCol;
		    newCol = sol->getMatrixByCol()->getVector(j);
		    DEBUGPR(30,ucout << "Adding " << addColsUp 
			    << " up duplicates and " << addColsDown
			    << " down duplicates\n");
		    for(int k=0; k<addColsUp; k++)
		      addCol(newCol,j,1);
		    if (addColsDown > 0)
		      {
			newCol *= -1.0;
			for(int k=0; k<addColsDown; k++)
			  addCol(newCol,j,-1);
		      }
		  }
	      }
	    else
	      DEBUGPR(30,ucout << 'x' << j << '=' << solution[i]
		      << ": [" << lb << ',' << ub << "] unchanged\n");
	  }
      }

    DEBUGPR(90,sol->dumpColInfo("After adding columns: "));

    totalCols = nc + extraCols;      // extraCols is incremented by "addCol"
    totalInts = numInts + extraCols;

    // Dump information if need be

    DEBUGPR(60,for(int i=0; i<numInts; i++) 
	    ucout << "Bounds for x" << intList[i] << " are [" 
	    << (sol->getColLower())[intList[i]] << ','
	    << (sol->getColUpper())[intList[i]] << ']'
	    << endl);
    DEBUGPR(65,for(int i=0; i<extraCols; i++)
	    ucout << "Bounds for x" << intList[numInts+i] << " ("
	    << sign[i] << ") copy of x" << cloneIndex[i] << " are [" 
	    << (sol->getColLower())[intList[numInts+i]] << ','
	    << (sol->getColUpper())[intList[numInts+i]] << ']'
	    << endl);

    DEBUGPR(5,ucout << "MIPHeuristic::nr=" << nr 
	    << " heurObjBound=" << heurObjBound << endl);

    // Create a merit function if the number of variables has changed.
    // Also reload the variable types

    if (totalInts != oldTotalInts)
      {
	if (mf != NULL)
	  delete mf;
	if (canonicalOnly)  
	  mf = new CanonicalMeritFunction( totalCols, intList.data(), totalInts, 
					   sol->getColLower() );
	else 
	  mf = new MeritFunction( totalCols, intList.data(), totalInts,
				  sol->getColLower(), &myRNG );

	// Not sure this is needed, but make array exactly the right size.
	heurVType.resize(totalCols);
	for(int j=nc; j<totalCols; j++)
	  heurVType[j] = binary;

	// Make sure variable types are set!
	sol->setVariableTypes(heurVType);
      }

    DEBUGPR(20,ucout << "About to 'randomize' merit function.\n");

    // If wantCanonical is set, use canonical merit function.
    // Otherwise, try to randomize it.

    if (wantCanonical)
      mf->canonicalAlpha(sol->getColLower());
    else
      mf->randomAlpha( sol->getColLower(), &myRNG );

    DEBUGPRP(50,ucout << "Alpha after randomization:");
    DEBUGPRP(50,for(int i=0; i<totalInts; i++) 
	     ucout << ' ' << (mf->getAlpha())[i]);
    DEBUGPR(50,ucout << endl);

    //Compute the gradient at the initial point (all components)

    DoubleVector d(totalCols);
    DEBUGPR(50,ucout << "Gradient computation: totalCols=" 
	    << totalCols << endl);

    for (int i = 0; i < totalCols; i++) 
	d[i] = weight * c[i];
    for (int i = 0; i < numInts; i++) 
      d[intList[i]] += mf->derivative(i, solution[i]);

    // Added variables all start at zero

    for (int i = 0; i < extraCols; i++)
      d[nc + i] += mf->derivative(numInts+i, 0.0);
    
    DEBUGPR(50,ucout << "Gradient computed\n");

    //Set the gradient at the initial point as the linear model's objective
    sol->setObjCoeffSet(allIndices.data(), 
			allIndices.data() + totalCols, 
			d.data());

    // No cutoff on the modified objective itself
    sol->setDblParam(OsiPrimalObjectiveLimit,-MAXDOUBLE);
    sol->setDblParam(OsiDualObjectiveLimit,MAXDOUBLE);

    int majorIter = 0; //Major iteration counter
    picoCglGomory gomGen; // Use gomory cuts -- our globalized version!
                          // Do not use PICO-wrapped version for now, since
                          // we are not sure of having a BCMIP to feed it. 
    gomGen.setLimit(totalCols); //Cuts can be as long as necessary
    gomGen.setAway(0.001); //Variables with fractionality>0.001 will be used

    //Maximum allowed number of major FW iterations
    int maxMajorIter = cfg.bbhMaxFWIter;
    DEBUGPR(5,ucout << "maxMajorIter = " << maxMajorIter << endl);
    //Maximum number of simplex iterations to burn
    int maxSimplexIter = cfg.bbhMaxHeurPivots; 
    DEBUGPR(5,ucout << "maxSimplexIter = " << maxSimplexIter << endl);

    int simplexIter = 0; //Simplex iteration counter
    //Defines what is considered to be 0 of the merit function
    double mfTol = cfg.bbhMeritFunctionTolerance; //1e-5; 
    double mfAtX = HUGE_VAL; //Merit function value at the current primal vector
    //Indicates if optimality was reached in the last resolve()
    bool optimality = true; 
    //Gradient components for integer variables
    DoubleVector grad(totalInts);
    DoubleVector x(totalCols);
    //Maximum number of cuts (if exceeded, new cuts will not be generated)
    int maxCuts = cfg.bbhMaxCuts;
    DEBUGPR(5,ucout << "maxCuts = " << maxCuts << endl);
    int nCuts = 0; //Current number of cuts

#ifdef ACRO_VALIDATING
    DEBUGPR(150,dumpSolBasis(sol,ucout,"After setup:"));
#endif

    while (majorIter < maxMajorIter) 
      {
	// Solve the LP using the gradient of the concave objective
	// JE -- catch exceptions and bail out of heuristic.
	// Try to use primal method, and solve instead of resolve (Aug 2005)
	// Primal ought to be good since we're already feasible and we're
	// just messing with the objective function.

	sol->setLPMethod(PicoLPInterface::primalSimplex);
	try
	  {
	    sol->solve();
	  }
	catch(const char* str)
	  {
	    DEBUGPR(1,ucout << "Heuristic LP solver threw: " << str << endl);
	    rebuildLP();
	    return false;
	  }
	
	majorIter++;
	int iter = sol->getIterationCount();
	simplexIter += iter;
	optimality = sol->isProvenOptimal();
	
#ifdef ACRO_VALIDATING
	DEBUGPR(150,dumpSolBasis(sol,ucout,"After resolve():"));
#endif

	if (!optimality) 
	  {
	    DEBUGPR(5, ucout << "resolve() unsuccessful, terminating." << endl 
		    << "sol->isProvenPrimalInfeasible()=" 
		    << sol->isProvenPrimalInfeasible() << endl);
	    break;
	  }
	
	// Get solution into "x" -- this used to be as below, but
        // that allowed memory errors to sneak through...
	// Formerly: CoinCopyN(sol->getColSolution(), nc, x.data());
	
	const double* colSolPtr = sol->getColSolution();
	for (int i=0; i<totalCols; i++)
	  x[i] = *(colSolPtr++);

	mfAtX = mf->value(x.data());

	DEBUGPR(5, ucout << "Major iteration " << majorIter 
		<< ": lp iter.= " << iter);
	DEBUGPR(5, ucout << " MF= " << mfAtX << endl);
	
	if (mfAtX < mfTol)
	  {
	    DEBUGPR(5,ucout << "Merit function below tolerance\n");
	    break;
	  }
	if ((maxSimplexIter > 0) && (simplexIter >= maxSimplexIter)) 
	  {
	    DEBUGPR(5,ucout << simplexIter << " pivots: above limit\n");
	    break;
	  }

	if (iter == 0) 
	  {

	    // Recomputation of the gradient did not change 
	    // optimality status of the vertex
	    if (nCuts >= maxCuts)
	      { 
		DEBUGPR(5, ucout << "Cut limit exceeded, terminating." 
			<< endl);
		break;
	      } 

	    DEBUGPR(5, ucout << "Generating cuts: ");

	    // Do this the old CGL way for now...
	    OsiCuts cuts;
	    sol->setCutting();
	    gomGen.generateCuts(*sol, cuts);
	    sol->setSolving();  
	    int cutsThisTime = cuts.sizeCuts();
	    DEBUGPR(5, ucout << cutsThisTime << endl);
	    if (cutsThisTime == 0) break;

#ifdef ACRO_VALIDATING
	    DEBUGPR(150,dumpCuts(cuts,ucout,"Cuts obtained:"));
	    
	    DEBUGPR(150,dumpCuts(cuts,ucout,"Cuts obtained:"));
#endif
	    
	    //Apply cuts
	    DEBUGPR(5, ucout << "Applying cuts: ");

	    // This is also done the old CGL way for the time being
	    OsiSolverInterface::ApplyCutsReturnCode acRc = sol->applyCuts(cuts);
	    nCuts = sol->getNumRows() - startRows;
	    
	    DEBUGPR(5, ucout << acRc.getNumInconsistent() 
		    << " were inconsistent" << endl);
	    DEBUGPR(5, ucout << acRc.getNumInconsistentWrtIntegerModel() 
		    << " were inconsistent for this problem" << endl);
	    DEBUGPR(5, ucout
		    << acRc.getNumInfeasible() << " were infeasible" << endl);
	    DEBUGPR(5, ucout << acRc.getNumIneffective() 
		    << " were ineffective" << endl);
	    DEBUGPR(5, ucout << acRc.getNumApplied() 
		    << " were applied" << endl << endl);
	   
	    DEBUGPRP(5, ucout << "Reoptimizing: ");

	    // Use dual simplex now; it's probably faster

	    sol->setLPMethod(PicoLPInterface::dualSimplex);
	    try
	      {
		sol->solve();
	      }
	    catch(const char* str)
	      {
		DEBUGPR(1,ucout << "Heuristic LP solver (cuts) threw: " 
			<< str << endl);
		rebuildLP(); 
		DEBUGPR(1,ucout << "Emergency LP rebuild complete.\n");
		return false;
	      }
	    
	    iter = sol->getIterationCount();
	    simplexIter += iter;
	    optimality = sol->isProvenOptimal();
	    
	    DEBUGPR(5, ucout << "lp iter.= " << iter << endl);
	    
#ifdef ACRO_VALIDATING
	    DEBUGPR(150,dumpSolBasis(sol,ucout,"After cut resolve():"));
#endif

	    if (!optimality)
	      {
		DEBUGPR(5, ucout << endl 
			<< "resolve() after cuts unsuccessful, terminating." 
			<< endl
			<< "sol->isProvenPrimalInfeasible()=" 
			<< sol->isProvenPrimalInfeasible() << endl);
		break;
	      }
	    
	    CoinCopyN(sol->getColSolution(), totalCols, x.data());
	    mfAtX = mf->value(x.data());
	  }

	//Recompute the gradient (only "integer" components change)
	mf->gradient(x.data(), grad.data());
	for (int i = 0; i < totalInts; i++) 
	  {
	    grad[i] += weight * c[intList[i]];
	  }
	//Set the gradient at the new point as the linear model's objective
	sol->setObjCoeffSet(intList.data(), 
			    intList.data() + totalInts, 
			    grad.data());    
      }

    DEBUGPR(5,ucout << "End loop: major iterations = " << majorIter << endl);

    // Clean up cuts -- now *including* the objective constraint and
    // any cuts added when the lpSetupObj object was called.

    nCuts = sol->getNumRows() - nr;
    DEBUGPR(10,ucout << "Cleaning up " << nCuts << " cuts\n");
    IntVector rowIndices(nCuts);
    for (int i = 0; i < nCuts; i++) 
      rowIndices[i] = nr + i;
    sol->deleteRows(nCuts,rowIndices.data());

    // Also, throw away any added columns

    if (extraCols > 0)
      sol->deleteCols(extraCols,colIndices.data());

    DEBUGPR(90,sol->dumpColInfo("After deleteCols"));

    // Check if we got a solution

    bool feasibleSolFound;
    if (mfAtX < mfTol && optimality) 
      {
	DEBUGPR(10,ucout << "Found something: " << x << endl);
	
	// EUREKA!!! We found something feasible!!!
	// Fold added columns back into original ones

	for(int i=0; i<extraCols; i++)
	  x[cloneIndex[i]] += x[nc+i]*sign[i];

	DEBUGPR(10,ucout <<"Compacted to: " << x << endl);

	// Check value of new solution

	double value = 0.0;
	for (int i = 0; i < nc; i++) 
	  value += x[i] * c[i];

	DEBUGPR(5, ucout << endl 
		<< "MIPHeuristic found feasible solution with value " 
		<< value << endl);
	feasibleSolFound = true;
	newValue = value;
	newSol = DoubleVector(nc);
	for(int i=0; i<nc; i++)
	  newSol[i] = x[i];
	
	DEBUGPR(5, ucout << "value= "<<newValue<<" solution "<< newSol);
      } 
    else
      {
	DEBUGPR(5, ucout << endl << "Tough luck :-((((" << endl);
	feasibleSolFound = false;
      }

    DEBUGPR(5, ucout << endl << "MIPHeuristic finished" << endl);
    
    //Update cumulative iteration counter
    totalHeurIter += simplexIter;
    //Update call counter
    numCalls++;
    
    return feasibleSolFound;
  }


  // Little routine to add a column

  void MIPHeuristic::addCol(CoinPackedVector& newCol,
			    int j,
			    int signVal)
  {
    sol->addCol(newCol, 0.0, 1.0, signVal*weight*c[j]);
    // setVTypes should handle this: sol->setInteger(nc + extraCols);

    c[nc + extraCols]     = signVal*c[j];
    cloneIndex[extraCols] = j;
    sign[extraCols]       = signVal;

    extraCols++;
  }


  // This method lets the heuristic know that a new B&B subproblem has
  // entered the pool.  In this heuristic, we just register that subproblem's
  // merit value in the population

  void MIPHeuristic::inform(MILPNode* sp)
  {
    DEBUGPR(50,ucout << "Adding mfVal=" << sp->mfValue << " to population for "
	    << sp << endl);
    population.add(sp->mfValue);
  }
  

  // Lets the heuristic know a given B&B subproblem is no longer around.

  void MIPHeuristic::remove(MILPNode* sp)
  {
    bool found;
    DEBUGPR(50,ucout << "Removing mfVal=" << sp->mfValue 
	    << " from population for " << sp << endl);
    population.remove(sp->mfValue, found);
    if (!found)
      {
	ucout << "Could not find merit function value " << sp->mfValue
	      << " in population for subproblem " << sp << endl;
	//Warning("Missing merit function value");
      }
  }


  // Something major has happend to this B&B subproblem

  void MIPHeuristic::update(MILPNode* sp)
  {
    DEBUGPR(50,ucout << "mfValue update for " << sp << endl);
    remove(sp);
    double v = 0.0;
    //For now, just use quadratic MF
    for (int i = 0; i < numInts; i++) 
      {
	double x = sp->solution[i];
	double xFrac = x - floor(x);
	v += xFrac * (1 - xFrac);
        DEBUGPR(200,ucout << i << ' ' << x << ' ' 
		<< xFrac << ' ' << v << endl);
      }
    DEBUGPR(50,ucout << "Calculated and added " << v << endl);
    sp->mfValue = v; 
    population.add(v);
  }


  // Lets the heuristic know about global bound information

  void MIPHeuristic::globalBoundSet(bool globalBoundKnown_,double globalBound_)
  {
    globalBoundKnown = globalBoundKnown_;
    globalBound      = globalBound_;
    DEBUGPR(50,ucout << "Heuristic global bound info set to " 
	    << globalBoundKnown << ' ' << globalBound << endl);
  }


  // This method either runs the heuristic, or decides it doesn't want to

  void MIPHeuristic::possibleRun(MILPNode* sp)
  {
    if (decideToRun(sp))
      {
	DEBUGPR(10,ucout << "Proceeding with heuristic run...\n");
#ifdef ACRO_VALIDATING
	DEBUGPR(160,dumpSolBasis(sol,ucout,"Just before setupLP:"));
#endif
	sp->setupLP(sol);
#ifdef ACRO_VALIDATING
	DEBUGPR(160,dumpSolBasis(sol,ucout,"Just after setupLP:"));
#endif
	setObjBound(sp);
	DoubleVector newSol;
	double newValue;
	int heurStatus = run(newSol,newValue,sp->solution);
	if (heurStatus)
	  {
	    DEBUGPR( 1, ucout << "***** MIPHeuristic found a solution: "
		     << "value = "  << newValue << " {bound-count=" 
		     << sp->bGlobal()->subCount[pebbl::pebblBase::beingBounded] 
		     << "} ***** " << endl);
	    DEBUGPR( 4, ucout << "solution = "<< newSol << endl );
	    updateIncumbent(newSol,newValue);
	  }
      }
  }


  // This checks that the problem really is feasible and better than
  // the incumbent.  If so, it does a real updateIncumbent.
  // There is some duplication of logic here from MILPNode, and at
  // first I tried to consolidate it, but it got really gnarly!

  void MIPHeuristic::updateIncumbent(DoubleVector& newSol,double& newValue)
  {
    DEBUGPR(10,ucout << "MIPHeuristic feasibility re-check, obj="
	    << newValue << endl);

    __LPSolver_Interface* mainLP = mGlobal->lp;
    int i;

    // Save the old bounds

    DoubleVector saveUpper(numInts);
    DoubleVector saveLower(numInts);
    const double* lower = mainLP->getColLower();
    const double* upper = mainLP->getColUpper();
    for(int i=0; i<numInts; i++)
      {
	int j = MIP->integerVarNdx[i];
	saveLower[i] = lower[j];
	saveUpper[i] = upper[j];
      }
    
   // Round all the integer variables to exactly integer values -- 
   // they should be within integerTolerance already!

   for (i=0; i < numInts; i++) 
     {
       int j = MIP->integerVarNdx[i];
       int intVal = lround(newSol[j]);
       newSol[j] = intVal;
       // Fix lower and upper bounds (doesn't matter if general or binary)
       // bounds of continuous variables are never changed
       mainLP->setColLower(j, intVal);
       mainLP->setColUpper(j, intVal);
       DEBUGPR(100,ucout << "Set LP bounds of LP column " << j 
	       << " to " << intVal << endl);
     }
   DEBUGPR(100,ucout << "Integer variables rounded.\n" << Flush);

   // Check the beast by running an LP to determine if things look OK.

   PicoLPInterface::solutionMethod oldMethod = mainLP->getLPMethod();
   mainLP->setLPMethod(PicoLPInterface::primalSimplex);
   DEBUGPR(300,ucout << "Method set" << endl << Flush);
   mainLP->solve(true);
   DEBUGPR(300,ucout << "Problem solved" << endl << Flush);
   PicoLPInterface::problemState recheckState = mainLP->state();

   if (recheckState == PicoLPInterface::solved)
     {
       newValue = mainLP->getObjValue();
       DEBUGPR(10, ucout << "Solution passes feasibility check, value="
	       << newValue << endl);

       if ((mGlobal->sense)*(mGlobal->fathomValue() - newValue) > 0) 
	 mGlobal->updateIncumbent(newSol,(mGlobal->sense)*newValue); 
       else 
	 {
	   DEBUGPR(3,ucout << "Warning: heuristic found solution "
		   "worse than incumbent" << endl);
	   // Warning("Heuristic solution worse than incumbent");
	 }
     }
   else
     {
       DEBUGPR(3,ucout << "Warning: heuristic found infeasible solution."
	       << endl);
     }
   DEBUGPR(90,sol->checkColInfo("After updateIncumbent")); 

   // Restore old bounds.  Probably this could be done more efficiently!

   for(int i=0; i<numInts; i++)
     {
       int j = MIP->integerVarNdx[i];
       mainLP->setColBounds(j,saveLower[i],saveUpper[i]);
       DEBUGPR(100,ucout << "Resetting LP bounds of column " << j 
	       << " to " << saveLower[i] << " and " << saveUpper[i] << endl);
     }
   DEBUGPR(100,ucout.flush());

   mainLP->setLPMethod(oldMethod);
   mGlobal->forceColdStart();

  }
  


  // Figures out RHS of objective cut added to LP

  void MIPHeuristic::setObjBound(MILPNode* sp)
  {
    setObjBound(sp->lpBound);
  }


  void MIPHeuristic::setObjBound(double lpBound)
  {
    //Set the bound on the objective value
    //Note: This relative tolerance stuff is tricky: double check

    //Assume minimization in heuristic
    double newValue = (mGlobal->sense)*(mGlobal->incumbentValue);

    double rtol = max(mGlobal->relTolerance,cfg.bbhRelGapCutoff );
    double tol  = (mGlobal->gapDenom(lpBound))*rtol;

    if (mGlobal->absTolerance > tol ) 
      newValue -= mGlobal->absTolerance;
    else 
      newValue -= tol;

    newValue = min(cfg.bbhAPrioriObj,newValue);

    if (sol->getObjSense()*(heurObjBound - newValue) > 0) 
      heurObjBound = newValue;
  }


  // Figures out if we want to turn on the heuristic

  bool MIPHeuristic::decideToRun(MILPNode* sp)
  {
    if (totalHeurIter >= cfg.bbhMasterIter)
    {
      //Heuristic has run out of gas
      DEBUGPR(10, ucout << "Master iteration limit exceeded." << endl);
      return false;
    }
  
    // Always run on the first bbhMinCalls things we encounter

    if (numCalls <= cfg.bbhMinCalls)
      return true;

    // Otherwise, make a (hairy) probablistic decision

    double p    = score(sp->lpBound,sp->mfValue);
    double uVal = myUniform();
    DEBUGPR(10,ucout << "p=" << p << " u=" << uVal << endl);
    return (uVal < p);
    
  }


  // Scoring system for subproblems.  Used to make probabilistic run 
  // decisions.  Usual return values are in [0,1].  A score of -1 means
  // the problem is not runnable.

  double MIPHeuristic::score(double bound,double mfValue)
  {
    DEBUGPR(55,ucout << "Scoring bound=" << bound << ", mfValue="
	    << mfValue << endl);

    double problemAbsGap = mGlobal->absGap(bound);
    double problemRelGap = mGlobal->relGap(bound);
    double gapRatio = 1.0;  //Ratio of the problem and global absolute gaps
    double im      ;        //Integrality measure quantile
      
    //Compute gapRatio

    gapRatio = 1.0;
    if (globalBoundKnown && (abs(globalBound) < MAXDOUBLE)) 
      {
	DEBUGPR(100,ucout << "problemAbsGap=" << problemAbsGap
		<< ", globalBound=" << globalBound
		<< ", absGap(globalBound)=" << mGlobal->absGap(globalBound)
		<< endl);
	gapRatio = problemAbsGap / mGlobal->absGap(globalBound);
      } 
       
    //Compute integrality measure quantile

    im = (double) population.rank(mfValue) / population.size();
    DEBUGPR(100,ucout << "population.size()=" << population.size() << endl);
      
    DEBUGPR(100,ucout << "problemRelGap=" << problemRelGap 
	    << ", gapRatio=" << gapRatio << ", im=" << im << endl);

    // Check cutoffs

    double relGapCutoff   = cfg.bbhRelGapCutoff;
    double gapRatioCutoff = cfg.bbhGapRatioCutoff;
    double imCutoff       = cfg.bbhIntMeasureCutoff;
 
    if (problemRelGap <= relGapCutoff ||
	gapRatio <= gapRatioCutoff    ||
	im >= imCutoff)
      {
	DEBUGPR(20,ucout << "Failed cutoffs " << relGapCutoff
		<< '/' << gapRatioCutoff << '/' << imCutoff << endl);
	return -1;
      }

    DEBUGPR(100,ucout << "Cutoffs passed\n");

    // Do bizarre probabilistic calculation

    double p = cfg.bbhPMax;
    //Adjust by a "relative gap factor"
    p *= pow((problemRelGap - relGapCutoff) / (1.0 - relGapCutoff),
	     cfg.bbhRelGapExp);
    //Adjust by a "gap ratio factor"
    p *= pow((gapRatio - gapRatioCutoff) / (1.0 - gapRatioCutoff),
	     cfg.bbhGapRatioExp);
    //Adjust by a "integrality measure factor"
    p *= pow((imCutoff - im) / imCutoff, 
	     cfg.bbhIntMeasureExp);

    DEBUGPR(60,ucout << " score=" << p << endl);

    return p;
  }
  

  // This version runs on subproblems and randomizes its result

  double MIPHeuristic::randomizedScore(MILPNode* sp)
  {
    DEBUGPRP(20,ucout << "Scoring " << sp << "... ");
    double p = score(sp->lpBound,sp->mfValue);
    if (p > 0)
      p *= myUniform();
    DEBUGPRP(55,ucout << "randomizedScore(sp) Returning " << p << endl);
    return p;
  }
  

  // This version also randomizes its result, but runs on lpSetupObj 
  // objects.  A null pointer returns "-1" for "not runnable"

  double MIPHeuristic::randomizedScore(lpSetupObj* ptr)
  {
    if (ptr == NULL)
      return -1;
    double p = score(ptr->bound,ptr->mfValue);
    if (p > 0)
      p *= myUniform();
    DEBUGPRP(55,ucout << "randomizedScore(ptr) returning "  << p << endl);
    return p;
  }
  

  // Scan the mini-pool for the element with the best (sense=1) or 
  // worst (sense=-1) *randomize* score.

  double MIPHeuristic::scanPool(int& slot,int sense)
  {
    slot = 0;
    double extremum = randomizedScore(heurPool[0]);
    DEBUGPR(50,ucout << "MIPHeuristic::scanPool: slot 0 " 
	    << heurPool[0] << " score=" << extremum << endl);
    for (int i=1; i<heurPoolSize; i++)
      {
	double p = randomizedScore(heurPool[i]);
	DEBUGPR(50,ucout << "MIPHeuristic::scanPool: slot " << i 
		<< ' ' << heurPool[i] << " score=" << p << endl);
	if (sense*(extremum - p) < 0)
	  {
	    extremum = p;
	    slot     = i;
	  }
      }
    return extremum;
  }


  // The fullowing methods are used only from the parallel code and
  // and are not created when configure without MPI

#ifdef ACRO_HAVE_MPI

  // Old version of ramp-up heuristic that branches on perhaps one variable
  // This is only used now if we are doing no presplitting.

  void MIPHeuristic::rampUpRun(MILPNode* sp, 
			       int direction, 
			       int branchChoice,
			       bool wantCanonical)
  {
    DEBUGPR(20,ucout <<  "Ramp-up heuristic run: direction=" << direction
	    << " branchChoice=" << branchChoice << endl);
    rampUpCalls++;

    sp->setupLP(sol);
    setObjBound(sp);
    DoubleVector newSol;
    double newValue;
    int heurStatus;

    // The following "if" shouldn't ever be invoked now...

    if (direction == branch_down)
      { 
	sp->adjustLPBounds( sol, branchChoice, branch_down );
	DEBUGPR(4,ucout << "RampUp: branch_down  branchChoice="
		<< branchChoice );
	sumDepth++;
      }
    else if (direction == branch_up)
      { 
	sp->adjustLPBounds( sol, branchChoice, branch_up );
	DEBUGPR(4,ucout << "RampUp:  branch_up     branchChoice="
		<< branchChoice );
	sumDepth++;
      }

    heurStatus = run(newSol,newValue,sp->solution,wantCanonical);
 
    if (heurStatus)
      {
	DEBUGPR( 1, ucout << endl 
		 << "##### rampUpMIPHeuristic found value = " 
		 << newValue << " #####" << endl);
	DEBUGPR( 4, ucout << "#####  solution = "<< newSol << endl );
	updateIncumbent(newSol,newValue);
	rampUpSuccesses++;
      }

  }


  // Run ramp-up heuristic from a leaf of the presplitting tree

  void MIPHeuristic::rampUpRun(MILPNode* sp, 
			       preSplitNode* leafNode,
			       bool wantCanonical)
  {
    DEBUGPR(20,leafNode->dump("Running ramp-up heuristic from node: ",ucout));

    sumDepth += leafNode->depth;
    rampUpCalls++;

    // Set up the LP bounds and basis

    sp->setupLP(sol);

    // Make all the bound modifications tracing from this leaf node to the 
    // root

    for(preSplitNode* thisNode=leafNode; 
	!thisNode->isRoot(); 
	thisNode=thisNode->parent)
      sp->adjustLPBounds(sol,thisNode->splitVar,thisNode->branchDir);

    setObjBound(sp);
    DoubleVector newSol;
    double newValue;
    int heurStatus;

    heurStatus = run(newSol,newValue,sp->solution,wantCanonical);
 
    if (heurStatus)
      {
	DEBUGPR( 1, ucout << endl 
		 << "##### rampUpMIPHeuristic (depth="
		 << leafNode->depth << ") found value = " 
		 << newValue << " #####" << endl);
	DEBUGPR( 4, ucout << "#####  solution = "<< newSol << endl );
	updateIncumbent(newSol,newValue);
	rampUpSuccesses++;
      }
    
  }


  // Constructor to create a root preSplitting node from a subproblem

  preSplitNode::preSplitNode(parMILPNode* sp,BasicArray<int>& pi) :
    parent(NULL),
    depth(0),
    splitVar(-1),
    branchDir(no_branch),
    numSpawns(0),
    branchList(sp->nFrac + sp->nSetCandidates),
    global(sp->pmGlobal())
  {
    int nFrac = sp->nFrac;
    for (size_type k=0; k<branchList.size(); k++)
      {
	int j = pi[k];
	if (j < nFrac)
	  branchList[k] = sp->FracVarNdx[j];
	else
	  branchList[k] = sp->setCandidateNdx[j - nFrac] 
	                    + global->MIP->numIntegerVars();
      }
  }


  // Constructor to make a presplitting node from a parent
  // Construct list of all remaining branches from parent, omitting
  // those "adjacent" to the current split.

  preSplitNode::preSplitNode(preSplitNode* parent_,
			     branch_type   branchDir_,
			     adjSetObj&    adjSet,
			     bool          incrementSpawnCount) :
    parent(parent_),
    depth(parent->depth + 1),
    branchDir(branchDir_),
    numSpawns(0),
    global(parent->global)
  {
    int pSpawns = parent->numSpawns;
    if (incrementSpawnCount)
      (parent->numSpawns)++;
    IntVector& parentList = parent->branchList;
    int pSize = parentList.size();
    splitVar = parentList[pSpawns];
    branchList.resize(pSize - pSpawns);
    int bListSize = 0;
    for(int ndx=pSpawns+1; ndx<pSize; ndx++)
      {
	int bval = parentList[ndx];
	if (!adjSet[splitVar].contains(bval))
	  branchList[bListSize++] = bval;
      }
    branchList.resize(bListSize);
  }


  // Constructor to make a sibling identical except for branch direction
  // The allowable branch list just shares memory with the sibling.

  preSplitNode::preSplitNode(preSplitNode* sibling,branch_type branchDir_) :
    parent(sibling->parent),
    depth(sibling->depth),
    splitVar(sibling->splitVar),
    branchDir(branchDir_),
    numSpawns(0),
    global(sibling->global)
  {
    branchList &= sibling->branchList;
  }


  // Code to compare to presplit nodes

  int preSplitNode::compare(const preSplitNode& other) const
  {
    // If active status different, the active node is better

    int diff = ((int) other.active()) - ((int) active());
    if (diff != 0)
      return diff;

    // If active status the same, the better one has fewer spawns

    diff = numSpawns - other.numSpawns;
    if (diff != 0)
      return diff;

    // If number of spawns the same, higher in tree is better
    
    return depth - other.depth;
  }


  // Predicate to test whether you can do any more presplitting on a node

  bool preSplitNode::active() const
  {
    if (depth >= global->heurCfg.bbhPreSplitDepth)
      return false;
    return numSpawns < (int) branchList.size();
  }


  // Code to dump contents of a presplit node

  void preSplitNode::dump(const char* heading,ostream& os) const
  {
    os << heading << "depth=" << depth  
       << (active() ? " active" : " inactive") << endl;
    for(const preSplitNode* node=this; !node->isRoot(); node=node->parent)
      {
	if (node->branchDir == branch_up)
	  os << " up-";
	else if (node->branchDir == branch_down)
	  os << " down-";
	os << node->splitVar;
      }
    if (!isRoot())
      os << endl;
    os << "Branch list: " << branchList;
  }
    
#endif      // ACRO_HAVE_MPI

#ifdef ACRO_VALIDATING

  // Debug stuff...

  void MIPHeuristic::dumpSolBasis(OsiSolverInterface* solObj,
				  ostream&            stream,
				  const char*         header)
  {
    stream << header << endl;
    const double* x = solObj->getColSolution();
    // The following is UGLY, but this is only debugging...
    CoinWarmStartBasis* b = (CoinWarmStartBasis*) solObj->getWarmStart();
    int i=0;
    for(; i<solObj->getNumCols(); i++)
      {
	stream << 'x' << i << '=' << x[i] << ':' 
	       << basisChar(b->getStructStatus(i));
	if ((i % 5) == 4) 
	  stream << endl;
	else 
	  stream << '\t';
      }
    if ((i % 5) != 0)
      stream << endl;
    stream << "Slack status: ";
    for(i=0; i<solObj->getNumRows(); i++)
      stream << basisChar(b->getArtifStatus(i));
    stream << endl;
  }


  void MIPHeuristic::dumpCuts(OsiCuts&    cutPile,
			      ostream&    stream,
			      const char* header)
  {
    stream << header << endl;
    for(int i=0; i<cutPile.sizeRowCuts(); i++)
      {
	stream << "Cut#" << i << ": ";
	OsiRowCut* cutp = cutPile.rowCutPtr(i);
	stream << cutp->lb() << " <= ";
	const CoinPackedVector& row = cutp->row();
	int ne = row.getNumElements();
	const double* val = row.getElements();
	const int*    ndx = row.getIndices();
	for(int j=0; j<ne; j++)
	  {
	    double a = val[j];
	    if (a != 0)
	      {
		if (j == 0)
		  stream << a;
		else if (a < 0)
		  stream << " - " << -a;
		else 
		  stream << " + " << a;
	      }
	    stream << 'x' << ndx[j];
	  }
	stream << " <= " << cutp->ub() << endl;
      }
  }
      

  char MIPHeuristic::basisChar(CoinWarmStartBasis::Status s)
  {
    switch(s)
      {
      case CoinWarmStartBasis::isFree:
	return 'F';
      case CoinWarmStartBasis::basic:
	return 'B';
      case CoinWarmStartBasis::atUpperBound:
	return 'U';
      case CoinWarmStartBasis::atLowerBound:
	return 'L';
      default:
	return '?';
      }
  }

#endif

} // namespace pico
