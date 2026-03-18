#ifndef tevasp_PopulationExposedObjective_h
#define tevasp_PopulationExposedObjective_h

#include "ObjectiveBase.h"


/// Trait/Functional class for the population exposed (i.e., sick) objective
class PopulationExposedObjective : public ObjectiveBase
{
public:

  /// Constructor.
  PopulationExposedObjective(void):
    myCurrentSickened(0.0),
    myLastComputedTimeStep(-1),
    myLastImpactTimeStep(-1)
  {
  }

  /// Copy constructor.
  PopulationExposedObjective(const PopulationExposedObjective &other):
    ObjectiveBase(other),
    myCurrentSickened(other.myCurrentSickened),
    myTAIFileName(other.myTAIFileName),
    myLastComputedTimeStep(other.myLastComputedTimeStep),
    myLastImpactTimeStep(other.myLastImpactTimeStep)
  {
  }

  /// Destructor.
  virtual ~PopulationExposedObjective(void)
  {
    TSO_Close(&myMem.tso);
  }

  /// Assignment operator.
  PopulationExposedObjective &operator=(const PopulationExposedObjective &other)
  {
    if(this!=&other)
      {
	ObjectiveBase::operator=(other);
	myCurrentSickened=other.myCurrentSickened;
	myTAIFileName=other.myTAIFileName;
	myLastComputedTimeStep=other.myLastComputedTimeStep;
	myLastImpactTimeStep=other.myLastImpactTimeStep;
      }
    return *this;
  }

  ///
  void setTAIFileName(const char *theFileName)
  {
    myTAIFileName=theFileName;
  }

  ///
  void setTSOFileOrDirectory(const char *theName)
  {
    myTSOFileOrDirectory=theName;
  }

  ///
  void setTSOPattern(const char *thePattern)
  {
    myTSOPattern=thePattern;
  }

  ///
  virtual void init(const std::string &outputPrefix,PNetInfo net,PNodeInfo nodes,PLinkInfo links,double detectionDelay)
  {
    ObjectiveBase::init(outputPrefix,net,nodes,links,detectionDelay);
    myCurrentSickened=0.0;

    memset(&myMem,0,sizeof(Mem));

    PInputFileData taiIFD=loadTAIFileData(myTAIFileName.c_str());

    if(myTSOPattern.size()==0)
      {
	myMem.tso=TSO_Open((char*)myTSOFileOrDirectory.c_str(),0);
      }
    else
      {
	myMem.tso=TSO_Open((char*)myTSOFileOrDirectory.c_str(),(char*)myTSOPattern.c_str());
      }

    TSO_ReadPrologue(myMem.tso,&myNetInfo,&myMem.nodeinfo,&myMem.linkinfo);
    InitializeAssessMemory(&myMem,myNetInfo);
    processTAIData(taiIFD,&myMem);
    freeInputFileData(&taiIFD);

    Population(myMem.net,myMem.dr,myMem.node);
    IngestionRate(myMem.net,myMem.node);
    initialize(myMem.net,myMem.dr,myMem.xa,myMem.ta);

    myLastComputedTimeStep=-1;
    myLastImpactTimeStep=-1;
  }

  ///
  virtual void resetForScenario(void)
  {
    ObjectiveBase::resetForScenario();
    myCurrentSickened=0.0;

    myLastComputedTimeStep=-1;
    myLastImpactTimeStep=-1;

    (void) TSO_ReadSimulationResults(myMem.tso,myMem.net->info,myMem.nodeinfo,myMem.sources);
  }

  ///
  virtual const std::string &impactFilenameSuffix(void)
  {
    return ourImpactOutputFileSuffix;
  }

  ///
  virtual const std::string &evaluationFilenameSuffix(void)
  {
    return ourEvaluationOutputFileSuffix;
  }

  ///
  virtual void updateImpactStatisticsDueToNode(int thisNodeIndex,double thisNodeQuality,PNodeInfo nodes,PLinkInfo links,int thisTimeStep,double reportStep)
  {
    myLastImpactTimeStep=thisTimeStep;
  }

  ///
  virtual void addNominalImpact(int thisNodeIndex,int thisTimeStep,double reportStep)
  {
    if(myLastComputedTimeStep!=myLastImpactTimeStep)
      {
	recomputeNumberSickened(myLastImpactTimeStep);
	myLastComputedTimeStep=myLastImpactTimeStep;
      }

    updateImpacts(thisNodeIndex,myCurrentSickened,thisTimeStep*reportStep);
    
    updateSensorEvaluation(thisNodeIndex,myCurrentSickened);
  }

  ///
  virtual void addUndetectedImpact(int maxTimeStep,double reportStep)
  {
    // the threat simulator code uses INT_MAX as the
    // 'not detected' parameter value.
    if(myLastComputedTimeStep!=maxTimeStep)
      {
	recomputeNumberSickened(INT_MAX);
	myLastComputedTimeStep=maxTimeStep;
      }

    updateImpacts(UNDETECTED_NODE_INDEX,myCurrentSickened,maxTimeStep*reportStep);

    updateSensorEvaluation(UNDETECTED_NODE_INDEX,myCurrentSickened);
  }

  ///
  static std::string ourImpactOutputFileSuffix;

  ///
  static std::string ourEvaluationOutputFileSuffix;

protected:

  /// The number of sickened for the current scenario.
  /// This defaults to 0.0.
  double myCurrentSickened;

  /// The name of the TAI input file.
  /// This defaults to "".
  std::string myTAIFileName;
  
  /// The "mother" data structure for the health impacts code
  Mem myMem;

  ///
  PNetInfo  myNetInfo;

  /// The last time-step for which health impacts were actually computed.
  /// THis defaults to -1. This enables caching of results for
  /// all nodes at which detection occurs for a single time-step.
  int myLastComputedTimeStep;

  /// The last time-step for which an impact due to a non-zero
  /// quality concentration at a node was reported. the current 
  /// implementation simply uses the Murray et al. code as-is,
  /// without incremental (and computationally more efficient)
  /// computation of the fatality count. by tracking this value,
  /// it is possible to compute health impacts only when new
  /// node hits are recorded, in the addNominalImpact() and 
  /// addUndetectedImpact() methods. defauls to -1.
  int myLastImpactTimeStep;

  /// The tso input file names. in maintaining the Murry et al.
  /// code base, it is necessary to simultaneously open and 
  /// read the same TSO File that is read in the main tso2Impact
  /// process-scenario loop. both values default to "".
  std::string myTSOFileOrDirectory;

  ///
  std::string myTSOPattern;

private:

  /// Re-compute the #-sickened statistics for the input
  /// time-step; results are stored in the member variables.
  void recomputeNumberSickened(int theNewTimeStep)
  {
    double sumPopulationImpacted(0.0);

    for(int i=0;i<myMem.net->info->nnodes;i++) 
      {
	if(myMem.node[i].info->nz[0]) // someone is consuming at this node
	  {
	    // compute expected cumulative dose over time (dosage vectors are monotonically non-decreasing)
	    Dose(theNewTimeStep,myMem.net,myMem.dr,myMem.u,&myMem.node[i]); 
	    double maxDose(myMem.u->dos[myMem.dr->maxsteps-1]); 
	    double maxResponse(DoseResponse(maxDose,myMem.dr)); // response is the fraction of the population exposed
	    sumPopulationImpacted+=(maxResponse*myMem.node[i].pop);
	  }
      }

    myCurrentSickened=sumPopulationImpacted;
  }

};

#endif
