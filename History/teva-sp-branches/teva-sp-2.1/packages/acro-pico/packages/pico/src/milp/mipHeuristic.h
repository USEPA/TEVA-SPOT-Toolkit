/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file mipHeuristic.h
 *
 * Defines the pico::mipHeuristic class.
 */

#ifndef pico_mipHeuristic_h
#define pico_mipHeuristic_h

#include <acro_config.h>
#include <utilib/RNG.h>
#include <utilib/BasicArray.h> 
#include <utilib/DoubleVector.h>
#include <utilib/SimpleSplayTree.h>
#include <CoinPackedVector.hpp>
#include <CoinShallowPackedVector.hpp>
#include <pico/PicoLPSolver.h>
#include <pico/bbhConfig.h>
#include <pico/lpSetupObj.h>


class OsiSolverInterface;

#if !defined(COUGAR) && !defined(RS6K) && !defined(REDSTORM)
  class utilib::RNG;
#endif


namespace pico {

  using utilib::DoubleVector;
  
  class MILP;
  class MILPNode;
  class MILPProblem;
  class MeritFunction;
  class parMILP;
  class parMILPNode;


  // This auxilliary class is used in ramp-up, to control "pre-splitting"
  // on multiple varliables.  It is used only by the parallel code,
  // hence requires ACRO_HAVE_MPI

#ifdef ACRO_HAVE_MPI

  class preSplitNode
    {
    public:

      // All data members public accessible

      preSplitNode* parent;
      int           depth;
      int           splitVar;
      branch_type   branchDir;
      int           numSpawns;
      IntVector     branchList;
      parMILP*      global;

      // Constructor from a B&B node and a variable ordering

      preSplitNode(parMILPNode* sp, BasicArray<int>& pi);

      // Constructor from a parent node

      preSplitNode(preSplitNode* parent_,
		   branch_type   branchDir_,
		   adjSetObj&    adjSet,
		   bool          incrementSpawnCount=true);

      // Constructor from a aibling node

      preSplitNode(preSplitNode* sibling,
		   branch_type   branchDir_);

      // Compare two nodes to see which is the best to subdivide

      int compare(const preSplitNode& other) const;

      // Is this node is eligible to subdivide further

      bool active() const;

      // Is this a leaf node of the presplitting tree?

      bool isLeaf() const { return numSpawns == 0; };

      // Is this the root of the tree?

      bool isRoot() const { return depth == 0; };

      // Dump information to a stream

      void dump(const char* heading,std::ostream& os) const;

    };

#endif
  

  // The main heuristic class

  class MIPHeuristic: public CommonIO
    {
    protected:

      enum { findHighest=1, findLowest=-1};

      // Data members
      
      SimpleSplayTree<double> population;
      double heurObjBound;
      double weight;
      int numInts;
      int nc, nr;
      DoubleVector c;
      PicoLPSolver solOwner;
      PicoLPInterface* sol;
      utilib::PM_LCG  myRNG;
      utilib::Uniform myUniform;
      BasicArray<lpSetupObj*> heurPool;
      BasicArray<variableType> heurVType;
      MILP* mGlobal;
      MILPProblem* MIP;
      bool   globalBoundKnown;
      double globalBound;
      BBHConfig& cfg;      
      BasicArray<int> intList;
      long totalHeurIter;
      int numCalls;
      MeritFunction* mf;
      bool canonicalOnly;
      int cNzCount;
      IntVector cSparseNdx;
      DoubleVector cSparse;
      CoinShallowPackedVector cVector;
      int runCounter;
      int heurPoolSize;

      // These members relate to adding columns duplicating general integers
      int extraCols;
      int totalInts;
      int totalCols;
      int maxExtraCols;
      int maxTotalCols;
      int maxTotalInts;
      int oldTotalInts;

      IntVector cloneIndex;
      IntVector sign;
      IntVector colIndices;

      IntVector allIndices;

      // These are bookkeeping for ramp-up (parallel only)

    public:
      int sumDepth;
      int rampUpCalls;
      int rampUpSuccesses;

      // Methods

    protected:

      void init(PicoLPSolver& lp);
      void copyLP(PicoLPSolver& lp);
      void addObjRow();
      void rebuildLP();
      void addCol(CoinPackedVector& newCol,int j,int signVal);

      bool decideToRun(MILPNode* sp);

      double score(double bound,double mfValue);
      double randomizedScore(MILPNode* sp);
      double randomizedScore(lpSetupObj* ptr);
      double scanPool(int& slot,int sense=findHighest);

      void setObjBound(double lpBound);
      void setObjBound(MILPNode* sp);

    public:

      ~MIPHeuristic();

      MIPHeuristic( MILP* mGlobal_,
		    bool  useCanonicalOnly = false,
		    int   mainDebugLevel = 0);

      void inform(MILPNode* sp);
      void remove(MILPNode* sp);
      void update(MILPNode* sp);
      void globalBoundSet(bool globalBoundKnown_,double globalBound_);

      void possibleRun(MILPNode* sp);

      void feedIn(MILPNode* sp);

      bool run( DoubleVector& newSol, 
		double& newValue,
		const DoubleVector& solution,
		bool wantCanonical=false);
   
      bool parRun();

      void updateIncumbent(DoubleVector& newSol, double& newVal);

      bool emptyFlag;

#ifdef ACRO_HAVE_MPI  // Used only in parallel code

      // This is mostly obsolete -- ramp-up run with one possible branch
      // branch_up=0, branch_down=1, no_branch=2
      void rampUpRun(MILPNode* sp, 
		     int direction, 
		     int branchChoice=-1,
		     bool wantCanonical=false);

      // This is the newer, psSplitNode tree-driven version
      void rampUpRun(MILPNode* sp, 
		     preSplitNode* leafNode,
		     bool wantCanonical=false);
#endif

#ifdef ACRO_VALIDATING

      // Detailed debug stuff

      static void dumpSolBasis(OsiSolverInterface* solObj,
			       std::ostream&            stream,
			       const char*         header);

      static void dumpCuts(OsiCuts&    cutPile,
			   std::ostream&    stream,
			   const char* header);

      static char basisChar(CoinWarmStartBasis::Status s);

      void checkLP(const char* tag) { sol->checkColInfo(tag); };
     
#endif

    }; 


} // namespace pico

#endif
