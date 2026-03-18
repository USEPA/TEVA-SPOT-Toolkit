/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// branching.cpp
//
// Implements lowest level common functions for branching algorithms
// (except those inlined in branching.h).
//
// Jonathan Eckstein
//


#include <acro_config.h>
#include <utilib/_math.h>
#include <utilib/nicePrint.h>
#include <utilib/seconds.h>
#include <pebbl/branching.h>

using namespace std;

extern "C" void pebbl_abort_handler(int code)
{
#if defined(SOLARIS) | defined(LINUX)
//
// We call the p4_error routine to allow MPI to cleanup itself!
//
//const char* tmp = "Signal Caught by PEBBL";
//p4_error(const_cast<char*>(tmp),code);
#endif

#ifdef ACRO_HAVE_MPI
MPI_Abort(MPI_COMM_WORLD,-1);
abort();
exit(0);
#endif
}



using namespace utilib;

namespace pebbl {


string branching::version_info = "Unknown version";


// An initializer for branchSubId

void branchSubId::branchSubIdFromBranching(branching* global)
  {
  serial = ++(global->probCounter);
#ifdef ACRO_HAVE_MPI
  creatingProcessor = uMPI::running() ? uMPI::rank : 0;
#else
  creatingProcessor = 0;
#endif
};


bool coreSPInfo::canFathom()
{
DEBUGPRX(100,bGlobal(),
	"Checking on fathoming "; debugPrint(ucout); ucout << endl);
return (state == dead) || bGlobal()->canFathom(bound);
}


// This is used to compare subproblems in a way that varies
// depending on whether we're doing initial diving.

int coreSPInfo::dynamicSPCompare(const coreSPInfo& sp2) const
{
  DEBUGPRX(200,bGlobal(),"Dynamic compare of "; debugPrint(ucout);
           ucout << " and "; sp2.debugPrint(ucout); ucout << endl);

  // If the subproblems are clearly identical, bail out and say
  // They're indistinguishable.

  if (this==&sp2)
    return 0;

  // See if we are doing initial diving
  if (this->bGlobal()->initialDive && 
      (fabs(this->bGlobal()->incumbentValue) == MAXDOUBLE))
    {
      // OK -- we are trying an initial dive and there is no incumbent,
      // so do not use the usual compare method...

      // First test the integrality measure if the user wants to
      // dive on integrality.

      if (bGlobal()->integralityDive && 
	  (integralityMeasure != sp2.integralityMeasure))
	{
	  if (integralityMeasure < sp2.integralityMeasure)
	    return -1;
	  else
	    return 1;
	}

      // Either the user wants to dive on pure depth, or the integrality
      // measures were identical.
      
      if (depth != sp2.depth)
	{
	  if (depth > sp2.depth)
	    return -1;
	  else
	    return 1;
	}
  
    }
  
  // Either no initial diving, or things are identical from the viewpoint of 
  // initial diving.  Just use the regular compare function.

  return compare(sp2);

};

int coreSPInfo::compare(const coreSPInfo& sp2) const
{
  DEBUGPRX(200,bGlobal(),"Comparing "; debugPrint(ucout);
           ucout << " and "; sp2.debugPrint(ucout); ucout << endl);

  // Bail out if the subproblems are a priori identical

  if (this==&sp2) 
    return 0;

  // Decide on bound, if different

  if (bound != sp2.bound) 
    {
      if ((bGlobal()->sense)*(bound - sp2.bound) < 0)
	return -1;
      else 
	return 1;
    }

  // Try to decide based on integrality measure

  if (integralityMeasure != sp2.integralityMeasure)
    {
      if (integralityMeasure < sp2.integralityMeasure)
	return -1;
      else
	return 1;
    }

  // Impose a strong ordering based on serial number and creating processor

  if (id.serial != sp2.id.serial)
    {
      if (id.serial < sp2.id.serial)
	return -1;
      else
	return 1;
    }
 
  if (id.creatingProcessor != sp2.id.creatingProcessor)
    {
      if (id.creatingProcessor < sp2.id.creatingProcessor)
	return -1;
      else
	return 1;
    }

  // These subproblems look indistinguishable.  Hopefully they are just
  // different tokens of the same base subproblem.

  return 0;

};


// Guts of the standard constructors for "branching"

// If anything to do with the sense (optimization direction), and related
// things, such as the bluff change, look at effects on MILP::readIn

void branching::branchingInit(optimType direction,
			     double relTolSet,
			     double absTolSet,
			     double bluff,
			     bool bluffReal)
{
  sense  = direction;
  if (relTolSet != -1.0)
     relTolerance = relTolSet;
  if (absTolSet != -1.0)
     absTolerance = absTolSet;

 if (!bluffReal)
    {
    if (parameter_initialized("startIncumbent"))
      incumbentValue = startIncumbent;
    else incumbentValue = direction*MAXDOUBLE;
    }
  else
    {
    incumbentValue = bluff;
    // Originally, this was always set, since this would avoid checks
    // in the reset method, but this won't work when there are two
    // instances of PEBBL running simultaneously that have, say,
    // different objective senses, and therefore different initial
    // incumbent values (they share the parameter table).  Till that
    // changes, only use the incumbentValue to signal use of a bluff
    // and avoid using the bluff or startHeuristic parameter when using
    // multiple PEBBL instances.
    //
    // WEH - with the use of ParameterSet ... this should not be
    // necessary
    set_parameter("startIncumbent",incumbentValue);
    }

  branchingInitGuts();
 
}

void branching::branchingInitGuts()
{

  for (int i=0; i<numStates; i++)
    subCount[i] = 0;

  probCounter       = 0;
  initialGuessValue = sense*MAXDOUBLE;
  currentSP         = NULL;
  needPruning       = false;

  vout = 0;
  statusLinePrecision = (int) ceil(-log10(max(relTolerance,1e-16)));
}


void branching::reset(bool resetVB)
{
  branchingInitGuts();
  if (parameter_initialized("startIncumbent"))
    incumbentValue = startIncumbent;
  else incumbentValue = sense*MAXDOUBLE;
  if (pool) {
     pool->load().reset();
     pool->reset();
     }
  //
  // This logic turns off standard fathoming if we're performing enumeration
  // of near-optimal solutions.  After we have confirmed that we have
  // 'enough' near-optimal points, then fathoming will be turned back
  // on.
  //
  if ((EnumerationCount > 1) || (EnumerationRelTolerance != -1.0) ||
	(EnumerationAbsTolerance != -1.0))
     performing_enumeration = true;
}


// Standard serial read-in code.  Returns true if we can continue, false if
// we have to bail out.

bool branching::setup(int& argc, char**& argv)
{
  startTime = CPUSeconds();
  startWall = WallClockSeconds();

  if (!processParameters(argc,argv,min_num_required_args))
    return false;
  set_parameters(plist,false);
  if (!checkParameters(argv[0]))
    return false;
  if (!setupProblem(argc,argv))
    return false;
  if (plist.unused() > 0) {
    ucout << "\nERROR: unused parameters: " << std::endl;
    plist.write_unused_parameters(ucout);
    ucout << utilib::Flush;
    return false;
  }
  return true;
}


bool branching::processParameters(int& argc, 
				  char**& argv, 
				  unsigned int min_num_required_args__)
{
  solver_name = argv[0];
  if (!parameters_registered) 
    {
      register_parameters();
      parameters_registered=true;
    }

  if ((argc > 1) && (strcmp(argv[1],"--version")==0)) 
    {
      ucout << endl << version_info << endl << endl;
      return false;
    }

  plist.process_parameters(argc,argv,min_num_required_args__);

  // Set the name of the problem to be the first thing on the command
  // line that does not look like an argument.  The setupProblem method
  // can overwrite this later.

  if ((argc > 1) && (argv[1] != NULL))
    setName(argv[1]);

  return true;
}


void branching::serialPrintSolution(const char* header,
				    const char* footer,
				    std::ostream& outStream)
{
  int oldPrecision = outStream.precision();
  outStream.precision(statusLinePrecision);
  outStream << header << "Solution:  Value = " << incumbentValue << '\n';
  outStream << footer;
  outStream.precision(oldPrecision);
}


// Default destructor.

branching::~branching() 
{
  if (pool)
    delete pool;
}


// This sets the official name of the problem by chewing up the
// filename.  It can be overridden.  This version just finds the last
// "/" in the name and removes it and everything before it.

void branching::setName(const char* cname)
{
#if defined (TFLOPS)
  problemName = cname;
  int i=problemName.size();
  while (i >= 0) {
    if (cname[i] == '/') break;
    i--;
    }
  if (i >= 0)
     problemName.erase(0,i+1);
#else
  problemName = cname;
  size_type i = problemName.rfind("/");
  if (i != string::npos)
    problemName.erase(0,i+1);
#endif
}


// Fathoming-related stuff...


// Standard fathoming function.

bool branching::canFathom(double boundValue) 
{
  double gap   = absGap(boundValue);
  double denom = gapDenom(boundValue);

  DEBUGPR(1000, ucout << "branching::canFathom - Starting test incumbent = " 
	  << incumbentValue << " bound = " << boundValue 
	  << " Performing Enumeration: " << performing_enumeration << endl);

  if ((EnumerationAbsTolerance != -1.0) && (gap < -EnumerationAbsTolerance)) 
    {
      DEBUGPR(1000, ucout << "branching::canFathom - Fathoming with "
	      "EnumerationAbsTolerance! gap = " << gap 
	      << " < -EnumerationAbsTolerance = " 
	      << (-EnumerationAbsTolerance) << endl);
      return true;
    }

  if ((EnumerationRelTolerance != -1.0) && (denom > 0) && 
      (gap <= -EnumerationRelTolerance*denom)) 
    {
      DEBUGPR(1000, ucout << "branching::canFathom - Fathoming "
	      << "with EnumerationRelTolerance! gap = " << gap 
	      << " <= -EnumerationRelTolerance*denom = " 
	      << (-EnumerationRelTolerance*denom) << endl);
      return true;
    }

  //
  // TODO: think about the case where all the LP bounds are exactly
  // equal the value of the best solution, _AND_ we have enough points.
  // We _should_ be able to stop in this situation!
  //
  if ((EnumerationCount > 1) && 
      (((fathomValue() - boundValue)*sense) < absTolerance) &&
      (enumeration_count() >= (unsigned int) EnumerationCount)) 
    {
      DEBUGPR(1000, ucout << "branching::canFathom - Fathoming because "
	      "enough points have been found and the absolute tolerance "
	      "of the worst point dictates a fathom" << endl);
      return true; 
    }

  if (performing_enumeration) {
      DEBUGPR(1000, ucout << "branching::canFathom - NO Fathoming because "
	      "still performing enumeration" << endl);
     return false;
     }

  if (gap < absTolerance) 
    {
      DEBUGPR(1000, ucout << "branching::canFathom - Fathoming  gap = " 
	      << gap << " < absTolerance = " << absTolerance << endl);
      return true;
    }

  if (denom == 0) 
    {
      DEBUGPR(1000, ucout << "branching::canFathom - Fathoming  denom = 0"
	     << endl);
      return true;
    }

  bool status = gap < MAXDOUBLE && denom < MAXDOUBLE && gap <= relTolerance*denom;

  if (status == true) 
    {
      DEBUGPR(1000, ucout << "branching::canFathom - Fathoming  gap = " 
	      << gap << " <= relTolerance*denom = " 
	      << relTolerance*denom << endl);
    }
  else
    DEBUGPR(1000, ucout << "branching::canFathom - Performed Test: gap = " 
	    << gap << " absTolerance = " << absTolerance << " denom = " 
	    << denom << " relTolerance*denom = " 
	    << relTolerance*denom << endl);

  return status;
}


// Compute the relative gap.

double branching::relGap(double boundValue)
{
  double denom = gapDenom(boundValue);
  if (denom == 0)
    return 0;
  return absGap(boundValue)/denom;
}


// Default (and typical) implementation of makeRoot operation.

branchSub* branching::makeRoot() 
{
  branchSub* root = blankSub();
  root->setRoot();
  if (valLogOutput())
    root->valLogCreatePrint(NULL);
  return root;
}

void branchSub::computeBound()
{
double dummy = MAXDOUBLE;
computeBound(&dummy);
}

void branchSub::computeBound(double* controlParam)
{
  DEBUGPR(10,ucout << "Trying to bound " << this << '\n');
  if (state == boundable)
    setState(beingBounded);
  else if (state != beingBounded)
    EXCEPTION_MNGR(runtime_error, "Attempt to bound " << stateString(state) << " subproblem");
  MEMORY_TRACK_SP(this,"Before bound computation");
  boundComputation(controlParam);
  MEMORY_TRACK_SP(this,"Bound computation");
  DEBUGPR(10,ucout << "Result is " << this << '\n');
  if (valLogOutput() && ((state == bounded) || (state == dead)))
    valLogBoundPrint();
  if ((state == bounded) && !canFathom() && candidateSolution())
    updateIncumbent();
}


  
// Standard code shell around problem splitting computation.

int branchSub::splitProblem()
{
  if (state == bounded)
    setState(beingSeparated);
  else if (state != beingSeparated)
    EXCEPTION_MNGR(runtime_error, "Attempt to split " << stateString(state) << " subproblem");
  DEBUGPRP(10,ucout << "Splitting: " << this << ' ');
  MEMORY_TRACK_SP(this,"Before split");
  int n = splitComputation();
  MEMORY_TRACK_SP(this,"After split");
  if (canFathom())
    {
      childrenLeft = totalChildren = 0;
      DEBUGPR(10,ucout << "was fathomed during splitComputation().\n");
      if (valLogOutput())
	valLogSplitPrint();
    }
  else if (state == separated)
    {
      childrenLeft = totalChildren = n;
      DEBUGPR(10,ucout << "has " << totalChildren << " children.\n");
      if (valLogOutput())
	valLogSplitPrint();
      return totalChildren;
    }
  return 0;
}


// Standard code shell around production of new problems.

branchSub* branchSub::child(int whichChild)
{
  if (state != separated)
    EXCEPTION_MNGR(runtime_error, "Attempt to extract child of " << stateString(state) << " problem");
  if (childrenLeft <= 0)
    EXCEPTION_MNGR(runtime_error, "Tried to make too many children of a subproblem");
  MEMORY_TRACK(bGlobal()->probCounter + 1,"Before making child");
  branchSub* c = makeChild(chooseChild(whichChild));
  MEMORY_TRACK_SP(c,"Made child");
  childrenLeft--;
  DEBUGPR(10,ucout << "Extracted child " 
	  << totalChildren - childrenLeft
	  << " of "  << this << '\n');
  DEBUGPR(10,ucout << "Made " << c << '\n');
  if (valLogOutput())
    c->valLogCreatePrint(this);
  return c;
}


//  Destructor-like thing.  This can be overridden to something that
//  is efficient about memory allocation.

void branchSub::recycle()
{
  if (valLogOutput())
    valLogDestroyPrint();
  MEMORY_TRACK_SP(this,"Before recycling subproblem");
#ifdef MEMORY_TRACKING
  int temp = id.serial;
#endif
  delete this;
  MEMORY_TRACK(temp,"Recycled subproblem");
};


// Minimal implementation of setRoot() (to tell a problem it's a root).
// Typically called from a more elaborate, specific setRoot in a
// derived class.

void branchSub::setRoot()
{
  branchSubInit(bGlobal(),boundable);
  bound = bGlobal()->aPrioriBound();
  depth = 1;
  id.branchSubIdFromBranching(bGlobal());
  setRootComputation();
}



// Standard code shell around bound computation.
// The basic engine for all search techniques.


double branching::searchFramework(spHandler* handler)
{
  // Miscellaneous initializations

  vout = valLogFile();                // Null if no validation log.
  valLogInit();

  lastHeurPrint = 0;
  hlog = (heurLog ? new ofstream("heur.log",ios::out) : NULL);

  double lastPrintTime = WallClockSeconds();
  int    lastPrint     = 0;

  abortReason = NULL;

  //
  // Create the pool -- this was in branchingInit, but it couldn't see
  // the parameters yet!
  //
  if ( depthFirst )
    pool = new doublyLinkedPool<branchSub,loadObject>(true); //stack
  else if ( breadthFirst )
    pool = new doublyLinkedPool<branchSub,loadObject>(false); //queue
  else
    pool = new heapPool<branchSub,loadObject,DynamicSPCompare<branchSub> >();

  pool->load().init(this);
  pool->setGlobal(this);

  incHeuristicExists = haveIncumbentHeuristic();

  nextOutputTime   = WallClockSeconds() + earlyOutputMinutes*60;
  lastSolValOutput = sense*MAXDOUBLE;

  // If no handler has passed in, create one according to standard
  // command line parameters.  If one was passed in, make sure its
  // global pointer gets linked back correctly to the branching object.

  int ownHandler = FALSE;
  if (handler == NULL)
    {
      ownHandler = TRUE;
      if (lazyBounding)
	handler = new lazyHandler;
      else if (eagerBounding)
	handler = new eagerHandler;
      else
	handler = new hybridHandler;
    }
  
  handler->setGlobal(this);

  // Preprocess problem, make a root subproblem, and put it in the pool.

  preprocess();
  {
    branchSub *tmp = makeRoot();
    pool->insert(tmp);
  }

  // Guess initial solution

  initialGuessValue = initialGuess();
  if (sense*initialGuessValue < sense*incumbentValue)
    incumbentValue = initialGuessValue;

  MEMORY_BASELINE;

  // The main loop -- process problems until the pool is empty.

  while (haveCurrentSP() || (pool->size() > 0))
    {
      DEBUGPR(20,ucout << "Load is " << load() << '\n');

      if (!haveCurrentSP())
	{ 
	  loadCurrentSP( pool->remove() );
	}
      DEBUGPR(20,ucout << "Load is " << load() << '\n');

      if (currentSP->canFathom())
	eraseCurrentSP();
      else
	{
	  DEBUGPR(5, ucout << "Executing " << currentSP->id.creatingProcessor 
		  << ":" << currentSP->id.serial 
		  << " bound=" << currentSP->bound 
		  << " state= " << currentSP->state 
		  << " depth=" << currentSP->depth << "\n");
	  handler->execute();
	  if (haveCurrentSP() && !(currentSP->forceStayCurrent()))
	    unloadCurrentSPtoPool();
	  if (needPruning)
	    {
	      DEBUGPR(20,ucout << "Pruning.\n");
	      pool->prune();
	      needPruning = false;
	    }
	}

      if ((earlyOutputMinutes > 0) && serialNeedEarlyOutput())
	{
	  directSolutionToFile();
	  recordEarlyOutput(incumbentValue);
	}

      statusPrint(lastPrint,lastPrintTime);

      DEBUGPR(2000,
	      ucout << "Pool scan: \n";
	      int s = pool->size();
	      pool->resetScan();
	      for(int i=0; i<s; i++)
	      ucout << "  " << pool->scan() << endl;);

      if (shouldAbort(subCount[beingBounded]))
	{
	  if (haveCurrentSP())
	    unloadCurrentSPtoPool();
	  loadObject l = updatedLoad();
	  printAbortStatistics(l);
	  pool->clear();             // This will terminate the loop.
	}

    }

  // Clean up, record time, and close log files.

  if (ownHandler)
    delete handler;
  
  searchTime = CPUSeconds() - startTime;
  if (valLogOutput())
    {
      valLogFathomPrint();
      delete vout;
    }
  if (hlog)
    delete hlog;

  return incumbentValue;
}


// Abort stuff

bool branching::shouldAbort(int boundCount)
{
  if ((maxSPBounds > 0) && 
      (boundCount > maxSPBounds))
    {
      if (!abortReason)
	abortReason = "too many nodes bounded";
      return true;
    }
  if ((maxCPUMinutes > 0) && 
      (CPUSeconds() - startTime > maxCPUMinutes*60))
    {
      if (!abortReason)
	abortReason = "too much CPU time";
      return true;
    }
  if ((maxWallMinutes > 0) && 
      (WallClockSeconds() - startWall > maxWallMinutes*60))
    {
      if (!abortReason)
	abortReason = "too much wall clock time";
      return true;
    }
  return false;
}


void branching::printAbortStatistics(loadObject& load)
{
  if (!printAbortMessage)
    return;
  ucout << "\nABORTED: " << abortReason << endl << endl;
  ucout << load.boundedSPs << " subproblem bounds started\n";
  ucout << load.count() << " subproblems still in pool\n";
  if (haveIncumbent())
    {
      ucout << "Best solution value: " << incumbentValue << endl;
      if (!load.boundUnknown())
	{
	  ucout << "Gap: " << absGap(load.aggregateBound);
	  ios::fmtflags defaultFlags = ucout.setf(ios::fixed,ios::floatfield);
	  int oldPrec = ucout.precision(3);
	  ucout << " (" << 100*relGap(load.aggregateBound) << "%)\n";
	  ucout.precision(oldPrec);
	  ucout.setf(defaultFlags,ios::floatfield);
	}
    }
  else
    ucout << "No solution found\n";
}


// Handler stuff...

void spHandler::heuristic()
{
//
// TODO: why not just call haveIncumbentHeuristic() here?
//
if (global->incHeuristicExists)
   p->incumbentHeuristic();
}

// This is called if we try to release a subproblem to another processor, 
// but we're running in serial.

void spHandler::releaseChild()
{
  EXCEPTION_MNGR(runtime_error, 
		 "Tried to release a child subproblem from serial code");
}


// This is a common pattern in the lazy and hybrid handlers

void spHandler::spinOffChild()
{
  if (shouldRelease())    // This will always be false in serial.
    releaseChild();       // Will only execute in parallel versions.
  else 
    {
      getChild();
      if (c->canFathom())
	eraseChild();
      else
	insertChild();
    }
}


// Handler for "lazy" search technique.  
// Split every subproblem as soon as it is bounded, extract all children,
// and put them in the pool.

void lazyHandler::execute()
{
  setProblem();

  DEBUGPR(50,ucout << "Entering lazy handler for " << p << '\n');
  
  switch(state())
    {
    case boundable:
      
    case beingBounded:

      computeBound();
      if (canFathom())
	{
	  erase();
	  break;
	}
      if (state() == beingBounded)
	break;
      
    case bounded:

      heuristic();
     
    case beingSeparated:

      split();
      if (canFathom())
	{
	  erase();
	  break;
	}
      if (state() == beingSeparated)
	break;

    case separated:

      heuristic();

      for (int childCount = p->childrenLeft; 
	   (childCount > 0) && !canFathom();
	   childCount--)
	spinOffChild();

    case dead:

      erase();
      break;

    default:

      EXCEPTION_MNGR(runtime_error, 
		     "Lazy search encountered unknown state " << state());

    }

  DEBUGPR(50,ucout << "Exiting lazy handler.\n");

}



// Handler for "hybrid" search technique, which is the most natural for This
// subproblem representation.  Whenever you pick up a subproblem, just try 
// to advance it one state or pull out one child.

void hybridHandler::execute()
{
  setProblem();
  
  DEBUGPR(50,ucout << "Entering hybrid handler for " << p << '\n');
  
  switch(state())
    {
    case boundable:
      
    case beingBounded:

      computeBound();
      if (canFathom())
	{
	  erase();
	}
      else if (state() == bounded)
	heuristic();
      break;
     
    case bounded:

    case beingSeparated:

      split();
      if (canFathom())
	erase();
      else if (state() == separated)
	heuristic();
      break;

    case separated:

      // Pull out children until the parent's bound changes.
      // This should result in sensible things happening without
      // having to use the softReposition method, which we don't 
      // use any more.
      
      {
	double startBound = p->bound;
	while (stillNeedSP() && (p->bound == startBound))
	  spinOffChild();
      }

      if (stillNeedSP())
	break;

    case dead:

      erase();
      break;

    default:

      EXCEPTION_MNGR(runtime_error, 
		     "Hybrid search encountered unknown state " << state());

    }

  DEBUGPR(50,ucout << "Exiting hybrid handler.\n");
 
}


// Handler for "eager" search.  As soon as a bounded problem comes out
// of the pool, expand all its subproblems.

void eagerHandler::execute()
{
  setProblem();
  
   DEBUGPR(50,ucout << "Entering eager handler for " << p << '\n');
  
   switch(state())
    {
    case boundable:               // Actually, should never come here...
      
    case beingBounded:

      setToBoundCurrent();
      if (boundSubHandler())
	{
	  erase();
	  break;
	}
      
    case bounded:

      heuristic();
     
    case beingSeparated:

      split();
      if (canFathom())
	{
	  erase();
	  break;
	}
      if (state() == beingSeparated)
	break;
      heuristic();

    case separated:

      while((p->childrenLeft > 0) && !canFathom())
	{
	  getChild();
	  setToBoundChild();
	  if (boundSubHandler())
	    eraseChild();
	  else
	    insertChild();
	}

    case dead:

      erase();
      break;

    default:

      EXCEPTION_MNGR(runtime_error, "Eager search encountered unknown state " << state());

    }

   DEBUGPR(50,ucout << "Exiting eager handler.\n");

}


// Routines for loading and unloading subproblems

void branching::loadCurrentSP(branchSub* p)
{
#ifdef ACRO_VALIDATING
  if (haveCurrentSP())
    EXCEPTION_MNGR(runtime_error, "Attempt to load a subproblem when one is already loaded");
  DEBUGPR(100,ucout << "Loading " << p << '\n');
#endif
  currentSP   = p;
  currentSPId = p->id;
  p->makeCurrentEffect();
}


void branching::unloadCurrentSP()
{
#ifdef ACRO_VALIDATING
  if (!haveCurrentSP())
    EXCEPTION_MNGR(runtime_error, "Tried to unload current subproblem when none existed");
  DEBUGPR(100,ucout << "Unloading " << currentSP << '\n');
#endif
  previousSPId = currentSPId;
  currentSP->noLongerCurrentEffect();
  currentSP = NULL;
}


void branching::eraseCurrentSP()
{
  branchSub* p = currentSP;
  unloadCurrentSP();
  p->recycle();
}


// loadObject functions that take the current subproblem into account

loadObject branching::load()
{
  loadObject l = pool->load();
  if (haveCurrentSP())
    l += *currentSP;

  return l;
};


loadObject branching::updatedLoad()
{
  loadObject l = pool->updatedLoad();
  if (haveCurrentSP())
    l += *currentSP;

  return l;
};


// To print status lines.

void branching::statusPrint(int&        lastPrint,
			    double&     lastPrintTime,
			    const char* tag) 
{
  loadObject l = updatedLoad();
  statusPrint(lastPrint,lastPrintTime,l,tag);
}


void branching::statusPrint(int&        lastPrint,
			    double&     lastPrintTime,
			    loadObject& l,
			    const char* tag)
{
  DEBUGPR(400,ucout << "statusPrint: count=" << l.boundedSPs
	  << ", last=" << lastPrint << (l.mismatch() ? ", mismatch" : "")
	  << ".\n");

  int needPrint = FALSE;

  if ((statusPrintCount > 0) &&
      (l.boundedSPs >= lastPrint + statusPrintCount))
    needPrint = TRUE;

  double now = WallClockSeconds();
  if ((statusPrintSeconds > 0) &&
      (now >= lastPrintTime + statusPrintSeconds))
    needPrint = TRUE;

  if (needPrint)
    {
#ifdef MEMUTIL_PRESENT
      l.setMemory();
#endif      
      statusLine(l,tag);
      lastPrint     = l.boundedSPs;
      lastPrintTime = now;
    }

  // Mikhail's heuristic log stuff

  if (hlog && (l.boundedSPs > lastHeurPrint)) 
    {
      int oldPrecision = ucout.precision();
      *hlog << l.boundedSPs;
      *hlog << " " << CPUSeconds() - startTime;
	hlog->precision(statusLinePrecision);
      *hlog << " " << l.incumbentValue;
      if (!l.boundUnknown() &&
	  (l.count() > 0) &&
	  (sense*l.aggregateBound < MAXDOUBLE))
	{
	  *hlog << " " << l.aggregateBound;
	  hlog->setf(ios::fixed,ios::floatfield);
	  hlog->precision(3);
	  *hlog << " " << 100*relGap(l.aggregateBound);
	  hlog->setf((ios::fmtflags)0,ios::floatfield);
	}
      else
	{
	  *hlog << " " << sense * MAXDOUBLE;
	  *hlog << " 100.000";
	}
      hlog->precision(oldPrecision);
      *hlog << endl;
      lastHeurPrint = l.boundedSPs;
    }
}



void branching::statusLine(loadObject& l, const char* tag) 
{
  int oldPrecision = ucout.precision();
  ucout << tag << '#' << l.boundedSPs;
  ucout << " pool=" << l.count();
  if (l.countIncomplete())
    ucout << '+';
  else
    ucout << ' ';
#ifdef MEMUTIL_PRESENT
  {
    ios::fmtflags defaultFlags = ucout.setf(ios::fixed,ios::floatfield);
    ucout.precision(2);
    ucout << ' ' << l.memUsed << "MB";
    ucout.setf(defaultFlags,ios::floatfield);
  }
#endif
  ucout.precision(statusLinePrecision);
  ucout << " inc=";
  if (l.incumbentValue == sense*MAXDOUBLE)
    ucout << "(none)";
  else
    ucout << l.incumbentValue;
  if (l.mismatch())
    ucout << "(x)";
  if (!l.boundUnknown() && 
      (l.count() > 0) && 
      (sense*l.aggregateBound < MAXDOUBLE))
    {
      ucout << " bnd=" << l.aggregateBound;
      {
#if 0
	long defaultFlags = ucout.setf(ios::fixed,ios::floatfield);
#else
	ios::fmtflags defaultFlags = ucout.setf(ios::fixed,ios::floatfield);
#endif
	ucout.precision(3);
	ucout << " gap=" << 100*relGap(l.aggregateBound) << '%';
	ucout.setf(defaultFlags,ios::floatfield);
      }
    }
  ucout.precision(oldPrecision);
  ucout << '\n' << Flush;
}


int branching::serialNeedEarlyOutput()
{
  if (WallClockSeconds() < nextOutputTime)
    return FALSE;
  return sense*(incumbentValue - lastSolValOutput) < 0;
}


void branching::recordEarlyOutput(double outputVal)
{
  double now = WallClockSeconds();
  DEBUGPR(1,ucout << "Early output completed: inc=" << outputVal
	  << " time=" << now << endl);
  nextOutputTime   = now + earlyOutputMinutes*60;
  lastSolValOutput = outputVal;
}


void branching::solutionToFile()
{
  // If we already wrote the solution, do nothing.

  if ((earlyOutputMinutes > 0) && (lastSolValOutput == incumbentValue))
    return;

  // Otherwise, write it.

  directSolutionToFile();
}


void branching::directSolutionToFile()
{
  ostream* outStreamP = openSolutionFile();
  serialPrintSolution("","",*outStreamP);
  closeSolutionFile(outStreamP);
}


ostream* branching::openSolutionFile()
{
  DEBUGPR(50,ucout << "Opening solution file\n");
  ofstream* toReturn = new ofstream("temp-sol.dat",ios::out);
  if (toReturn->bad())
    ucout << "****** Warning ******** could not open solution file.\n";
  return toReturn;
}


void branching::closeSolutionFile(ostream* fileStream)
{
  delete fileStream;
  string solFileName(problemName);
  if (solFileName.length() > 0)
    solFileName += ".";
  solFileName += "solution.dat";
  int delcode = remove(solFileName.c_str());     // Required for Windows OS
  rename("temp-sol.dat",solFileName.c_str());
  DEBUGPR(50,ucout << "Closed solution file " << solFileName << endl);
}


void branching::printSPStatistics(ostream& stream)
{
  printSPTable(stream,subCount,17);
}


void branching::printTimings(ostream& stream)
{
int oldPrecision = stream.precision(1);
stream.setf(ios::fixed,ios::floatfield);
stream << "Total Runtime= " << searchTime << " seconds" << endl;
stream << "Total Time= " << (CPUSeconds() - startTime) << " seconds" << endl;
stream.unsetf(ios::floatfield);
stream.precision(oldPrecision);
}



void branching::serialPrintSolValue(std::ostream& stream)
{
  if (!performing_enumeration) 
    {
      int oldPrecision = stream.precision(20);
      stream << endl << "Final Solution:  Value = " << incumbentValue << endl;
      stream.precision(oldPrecision);
    }
  stream << endl;
}


void branching::printAllStatistics(ostream& stream)
{
  printSPStatistics(stream);
  stream << endl;
  printTimings(stream);
}


void branching::printSPTable(ostream& stream,
			     int* table,
			     int nameWidth)
{
  stream << "Subproblems\n";
  hyphens(stream,11) << '\n';
  int numWidth = digitsNeededFor(table[boundable]);
  for(int stateCode = boundable; stateCode < numStates; stateCode++)
    printSPLine(stream,
		table[stateCode],
		stateString(stateCode),
		table[boundable],
		nameWidth,
		numWidth);
  stream << '\n';
}


void branching::printSPLine(ostream&    stream,
			    int         n,
			    const char* name,
			    int         denominator,
			    int         nameWidth,
			    int         numWidth)
{
  stream.width(nameWidth);
  stream.setf(ios::left,ios::adjustfield);
  stream << name;
  stream.setf(ios::right,ios::adjustfield);
  stream << "  ";
  stream.width(numWidth);
  stream << n << "  ";
  printPercent(stream,n,denominator) << '\n';
}


//  Validation Log Code.

ostream* branching::valLogFile()
{
  if (validateLog)
    return new ofstream("val00000.log",ios::out);
  else
    return 0;
}


void branching::valLogInit()
{
  if (!vout)
    return;
  vout->setf(ios::scientific,ios::floatfield);  // Output bounds at very
  vout->precision(20);                          // high precision.
}


void branching::valLogFathomPrint()
{
  *vout << "fathoming " << incumbentValue << ' ' << (int) sense << ' ';
  *vout << relTolerance << ' ' << absTolerance << '\n';
}


bool branching::checkParameters(char* progName)
{
  if (version_flag) {
    ucout << endl << branching::version_info << endl << endl;
    return false;
  }
  if (help_parameter)
    {
      write_usage_info(progName,cout);
      return false;
    }
  if (debug_solver_params) 
    {
      ucout << "---- PEBBL Parameters ----" << endl;
      write_parameter_values(ucout);
      ucout << endl << utilib::Flush;
    }
  if (use_abort) 
    {
      utilib::exception_mngr::set_mode(utilib::exception_mngr::Abort);
      utilib::exception_mngr::set_exit_function(&exit_fn);
      signal(SIGFPE,pebbl_abort_handler);
    }
  return true;
}


void branching::write_usage_info(char* progName,std::ostream& os) const
{
  writeCommandUsage(progName,os);
  os << endl;
  plist.write_registered_parameters(os);
  os << endl;
}


void branching::writeCommandUsage(char* progName,std::ostream& os) const
{
  os << "\nUsage: " << progName 
     << " { --parameter=value ... }";
  if (min_num_required_args == 1)
    os << " <problem data file>";
  else if (min_num_required_args == 1)
    os << " <" << min_num_required_args << " problem data files>";
  os << endl;
}


void branching::solve()
{
  double run_start = CPUSeconds();
  search();
  searchTime = CPUSeconds() - run_start;

  //
  // Final I/O
  //

  printSolValue(ucout);
  if (printFullSolution)
    printSolution("","\n",ucout);

  printAllStatistics();
  ucout << endl;

  solutionToFile();

  if (abortReason)
    ucout << "RUN ABORTED: " << abortReason << endl << endl;
}


void branchSub::valLogWriteID(char separator)
{
  *vout << valLogProc() << ' ' << id.serial;
  if (separator)
    *vout << separator;
}


void branchSub::valLogWriteBound(char separator)
{
  if ((state == dead) && !(bGlobal()->canFathom(bound)))
    *vout << bGlobal()->sense*MAXDOUBLE;
  else
    *vout << bound;
  if (separator)
    *vout << separator;
}


void branchSub::valLogCreatePrint(branchSub* parent)
{
  *vout << "create ";
  valLogWriteID(' ');
  valLogWriteBound(' ');
  if (parent)
    parent->valLogWriteID();
  else
    *vout << "-1 -1";
  valLogCreateExtra(parent);
  *vout << endl;
}


void branchSub::valLogBoundPrint()
{
  *vout << "bound ";
  valLogWriteID(' ');
  valLogWriteBound();
  valLogBoundExtra();
  *vout << endl;
}


void branchSub::valLogSplitPrint()
{
  *vout << "split ";
  valLogWriteID(' ');
  *vout << totalChildren << ' ';
  valLogWriteBound();
  valLogSplitExtra();
  *vout << endl;
}


void branchSub::valLogDestroyPrint()
{
  *vout << "destroy ";
  valLogWriteID(' ');
  *vout << childrenLeft << ' ';
  valLogWriteBound();
  valLogDestroyExtra();
  *vout << endl;
}


//  To help with debug printing


void coreSPInfo::debugPrint(ostream& s) const
{
  int oldPrecision = 0;
  if (bGlobal()->debugPrecision > 0)
    oldPrecision = s.precision(bGlobal()->debugPrecision);
  s << id << bound << " (" << stateString(state) << ')';
  if (bGlobal()->printIntMeasure)
    s << "<im=" << integralityMeasure << '>';
  if (bGlobal()->printDepth)
    s << "<d=" << depth << '>';
  if (bGlobal()->debugPrecision > 0)
    s.precision(oldPrecision);
}

void coreSPInfo::CSPInitfromBranching(branching *global)
{
  //initialize
id.branchSubIdFromBranching(global);
// Integrality measure set in (empty) constructor
}

void coreSPInfo::CPSInitAsCopy(coreSPInfo& toCopy)
{
id = toCopy.id;
bound = toCopy.bound;
integralityMeasure = toCopy.integralityMeasure;
state = toCopy.state;
depth = toCopy.depth;
}

branchSub::branchSub() 
{ 
  poolPtr = 0; 
  childrenLeft = 1;
  state = boundable;
};
  

void branchSub::branchSubAsChildOf(branchSub* parent)
{
  CSPInitfromBranching(parent->bGlobal());
  branchSubInit(parent->bGlobal(),boundable);
  bound = parent->bound;
  integralityMeasure = parent->integralityMeasure;
  depth = parent->depth + 1;
};

} // namespace pebbl

ostream& operator<<(ostream& s, const pebbl::branchSubId& spid)
{
  s << '{';
#ifdef ACRO_HAVE_MPI
  if (uMPI::running())
    s << spid.creatingProcessor << ':';
#endif
  return s << spid.serial << '}';
}

