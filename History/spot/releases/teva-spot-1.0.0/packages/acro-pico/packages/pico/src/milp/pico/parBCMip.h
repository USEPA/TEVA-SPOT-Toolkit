/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file parBCMip.h
 *
 * Defines the pico::parBCMip class.
 */

#ifndef pico_parBCMip_H
#define pico_parBCMip_H

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <utilib/PackBuf.h>
#include <pico/parBCMipBase.h>
#include <pico/parMilp.h>
#include <pico/BCMip.h>

namespace pico {


class parBCMipNode; // Forward reference

class parBCMip: virtual public parMILP, virtual public BCMip, virtual public parBCMipBase
{
 public:
  // For now we can just use the parMILP pack method.  There are a lot
  // of data members in BCMip, but they're all initialized from info
  // available on the processor (e.g. from static initialization) or
  // are initialized to "zero" states
  //  virtual void pack(PackBuffer& outBuffer);
  //  virtual void unpack(UnPackBuffer& inBuffer);

  // Estimated size of a packed subproblem.

  virtual int spPackSize();

  bool setup(int& argc, char**& argv, BCMipProblem& _MIP)
        {
	  setBCMipProblem(_MIP);
	  return parMILP::setup(argc,argv,_MIP);
        }

  void MILPInit(bool initVB=true)
    {
      // parMILP doesn't seem to have a MILPInit
    if (initVB)
       BCMip::MILPInit(true);
    // doesn't seem to be anything specific for parBCMip
    }

  pebbl::parallelBranchSub* blankParallelSub();

  virtual pebbl::branchSub* blankSub();

  // As with parMILP, construct an empty instance to be initialized
  // from a message or an MPS file

  parBCMip()
    {
      // TODO: more general, safe, nice way to override defaults
      using_cuts = false;
    }

  virtual ~parBCMip() {}

    /// Note: should this include the VB flag?
  void reset(bool VBflag=true)
	{
        BCMip::reset(VBflag);
        parMILP::reset(false);
        }

  // Statistics printout details.  Inserted by JE but probably not
  // exactly right.

  void printAllStatistics(std::ostream& stream = std::cout)
    {
      BCMip::printAllStatistics(stream);
      parMILP::printRampUpHeurStats(stream);
    };

  virtual void preprocess();

   // This is used by parBCMipNode instances for packing binding cuts,
   // and by the cut server thread.  num_to_pack -1 means to use the
   // length of the array We may eventually want to move this when we
   // do the threads, but the nodes at least can access it from here.
  // This cannot be a method of picoRowCut, since we want the serial code
  // (even when configued with mpi) to run without linking in the parallel
  // base class (which is where the global array of "cut constructors" is)


  // were both static.  Try without to get this to compile
   void packCuts(BasicArray<picoRowCut *> &to_pack, 
		 PackBuffer &outBuffer,
		 int num_to_pack = -1);

   // Returns the number of cuts unpacked
   int unPackCuts(BasicArray<picoRowCut *> &unpack_array, UnPackBuffer &inBuffer);

   void packCutQueue(PackBuffer &outBuffer);
   // This appends the cut queue information to what's already there
   void unpackCutQueue(UnPackBuffer &inBuffer);

   // Throw away all cuts and clean up.  Used in parallel ramp up for processors other than 0.
   void dropCutQueue();

   // During ramp up, processors can divide the work of running serial cut finders.
   void rampUpCutting(parBCMipNode *curr_problem);
   // Determine which cut finders are eligible to run now.
   void getEligibleFinders(BasicArray<cutFinder *> eligibleFinders, parBCMipNode *curr_problem,
			   size_type &numEligibleFinders);
};

class parBCMipNode: virtual public parBCMipBase, virtual public parMILPNode, virtual public BCMipNode
{
public:

  virtual parBCMip*  pbcGlobal() const { return globalPtr; };
  parMILP*           pmGlobal()  const { return globalPtr; };
  MILP*              mglobal()   const { return globalPtr; };
  pebbl::parallelBranching* pGlobal()   const { return globalPtr; };
  pebbl::branching*         bGlobal()   const { return globalPtr; };

  void pack(PackBuffer& outBuffer);
  void packChild(PackBuffer& outBuffer,int whichChild);
  void unpack(UnPackBuffer& inBuffer);

  // Have to override the normal procedure to handle ramp up

  PicoLPInterface::problemState boundComputationGuts();

  void getBasis(BasisArray& basisBuffer) const
    {
    BCMipNode::getBasis(basisBuffer);
    }

  PicoLPInterface::problemState boundingSolve()
    {
      PicoLPInterface::problemState thisState = parMILPNode::boundingSolve();

      if (!rampingUp() || !bcGlobal()->using_cuts)
	return(thisState);
      // Processors other than rank 0 didn't actually solve the LP, and even processor
      // 0 didn't if it read the solution from a file.  Have to set the solver interface
      // as though it did just solve it for CGL routines to work.
      if (uMPI::rank > 0 || (isRoot() && pmGlobal()->useSavedRootSolution()))
	restoreLPSolver();
      return(thisState);
    }

  void parBCMipNodeFromparBCMip(parBCMip* master_, bool initVB = true)
    {
    globalPtr = master_;
    if (initVB)
      {
      BCMipNodeFromBCMip(master_); // this one initializes MILPNode and branchSub
      parMILPNodeFromParMILP(master_, false);
      }
    }

  void parBCMipNodeAsChildOf(parBCMipNode* parent, bool initVB=true)
  {
  globalPtr = parent->pbcGlobal();
  if (initVB)
    {
    BCMipNodeAsChildOf(parent, true);
    parMILPNodeAsChildOf(parent, false);
    }
  }

  ~parBCMipNode() { };

  parallelBranchSub* makeParallelChild(int whichChild)
    {
    parBCMipNode *child = new parBCMipNode;
    child->parBCMipNodeAsChildOf(this);
    // This initializes milpNode, etc.  parMILPNode
    // has no special childInit routine
    BCMipNode::childInit(child, whichChild);
    return(child);
    };

  bool isWarmStart()
  {
    {return(parMILPNode::isWarmStart() || parMILPNode::IwasLast());};
  }

// We can get pseudocost information in splitting (initialization)
// or bounding, so overide these methods to include a call to broadcast
// pseudocosts if appropriate.
void boundComputation(double *controlParam)
{
  DEBUGPRX(100,pbcGlobal(),"From boundComputation\n");
  BCMipNode::boundComputation(controlParam);
  pbcGlobal()->sendPcosts();
};

int splitComputation()
{
  DEBUGPRX(100,pbcGlobal(),"From splitComputation\n");
  int temp = BCMipNode::splitComputation();
  pbcGlobal()->sendPcosts();
  return(temp);
};


pebbl::branchSub *makeChild(int whichChild)
  {
    return(makeParallelChild(whichChild));
  }

virtual void postPseudoCostInit(IntVector &initVars,
				bool dontRemoveAllBranchChoices,
				int &numBranches)
{
  // I don't think the order matters
  parMILPNode::postPseudoCostInit(initVars,dontRemoveAllBranchChoices, numBranches);
  BCMipNode::postPseudoCostInit(initVars,dontRemoveAllBranchChoices, numBranches);
}


// A list of choices between ambiguous inheritance to make the compiler happy
// Let's see if we can do without this after working on the multiple virtual
// inheritance issues

//void setRootComputation()
//  {parMILPNode::setRootComputation();};

//bool candidateSolution()
//{return(BCMipNode::candidateSolution());};

protected:
  parBCMip *globalPtr;

};

inline pebbl::parallelBranchSub* parBCMip::blankParallelSub()
{
      MEMDEBUG_START_NEW("parBCMipNode");
      parBCMipNode* tmp = new parBCMipNode;
#ifdef ACRO_VALIDATING
 if (tmp == NULL)
   EXCEPTION_MNGR(std::runtime_error, "Failed to allocate a new parBCMipNode\n");
#endif
      tmp->parBCMipNodeFromparBCMip(this);
      MEMDEBUG_END_NEW("parBCMipNode");
      return (pebbl::parallelBranchSub *)tmp;
}

// Not sure this is used (as opposed to parallelBlankSub), but added for
// completeness.  The compiler was confused about which class to inherit
// from, so just make one for this class

inline pebbl::branchSub* parBCMip::blankSub()
{
MEMDEBUG_START_NEW("parBCMipNode")
parBCMipNode * tmp = new parBCMipNode;
#ifdef ACRO_VALIDATING
 if (tmp == NULL)
   EXCEPTION_MNGR(std::runtime_error, "Failed to allocate a new parBCMipNode\n");
#endif
tmp->parBCMipNodeFromparBCMip(this);
MEMDEBUG_END_NEW("parBCMipNode")
return (pebbl::branchSub *) tmp;
};

} // namespace pico

#endif
#endif
