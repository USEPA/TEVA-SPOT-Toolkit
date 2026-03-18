#ifndef tevasp_PopulationKilledObjective_h
#define tevasp_PopulationKilledObjective_h

#include "ObjectiveBase.h"


/// Trait/Functional class for the Murray et al. health impacts - 
/// fatalities - objective
class PopulationKilledObjective : public ObjectiveBase
{
public:

  // Constructor.
  PopulationKilledObjective(void):
    myCurrentFatalities(0.0),
    myLastComputedTimeStep(-1),
    myLastImpactTimeStep(-1)
  {
  }

  // Copy constructor.
  PopulationKilledObjective(const PopulationKilledObjective &other):
    ObjectiveBase(other),
    myCurrentFatalities(other.myCurrentFatalities),
    myTAIFileName(other.myTAIFileName),
    myLastComputedTimeStep(other.myLastComputedTimeStep),
    myLastImpactTimeStep(other.myLastImpactTimeStep)
  {
  }

  /// Destructor.
  virtual ~PopulationKilledObjective(void)
  {
    TSO_Close(&myMem.tso);
  }

  // Assignment operator.
  PopulationKilledObjective &operator=(const PopulationKilledObjective &other)
  {
    if(this!=&other)
      {
	ObjectiveBase::operator=(other);
	myCurrentFatalities=other.myCurrentFatalities;
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
    myCurrentFatalities=0.0;

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
    initXA(&myInterimXA,(myMem.dr)->maxsteps);

    myLastComputedTimeStep=-1;
    myLastImpactTimeStep=-1;
  }

  ///
  virtual void resetForScenario(void)
  {
    ObjectiveBase::resetForScenario();
    myCurrentFatalities=0.0;

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
	recompueNumFatalities(myLastImpactTimeStep);
	myLastComputedTimeStep=myLastImpactTimeStep;
      }

    updateImpacts(thisNodeIndex,myCurrentFatalities,thisTimeStep*reportStep);
    
    updateSensorEvaluation(thisNodeIndex,myCurrentFatalities);
  }

  ///
  virtual void addUndetectedImpact(int maxTimeStep,double reportStep)
  {
    // the threat simulator code uses INT_MAX as the
    // 'not detected' parameter value.
    if(myLastComputedTimeStep!=maxTimeStep)
      {
	recompueNumFatalities(INT_MAX);
	myLastComputedTimeStep=maxTimeStep;
      }

    updateImpacts(UNDETECTED_NODE_INDEX,myCurrentFatalities,maxTimeStep*reportStep);

    updateSensorEvaluation(UNDETECTED_NODE_INDEX,myCurrentFatalities);
  }

  ///
  static std::string ourImpactOutputFileSuffix;

  ///
  static std::string ourEvaluationOutputFileSuffix;

protected:

  /// The number of fatalities for the current scenario.
  /// Defaults to 0.0.
  double myCurrentFatalities;

  /// The name of the TAI input file.
  /// Defaults to "".
  std::string myTAIFileName;
  
  /// The "mother" data structure for the health impacts code
  Mem myMem;
  ///
  PNetInfo  myNetInfo;
  ///
  XA myInterimXA;
  ///
  HIST myImpactHist;

  /// The last time-step for which health impacts were actually computed.
  /// This defaults to -1. This enables caching of results for
  /// all nodes at which detection occurs for a single time-step.
  int myLastComputedTimeStep;

  /// The last time-step for which an impact due to a non-zero
  /// quality concentration at a node was reported. The current 
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

  /// re-compute the health impact statistics for 
  /// the input time-step; results are stored in 
  /// member variables.
  void recompueNumFatalities(int theNewTimeStep)
  {
    resetXA(&myInterimXA,myMem.dr->maxsteps);

    memset(&myImpactHist,0,sizeof(HIST));

    // Time-dependent cumulative expected dose and disease response 
    for(int i=0;i<myMem.net->info->nnodes;i++) 
      {
	if(myMem.node[i].info->nz[0]) 
	  {
	    Dose(theNewTimeStep,myMem.net,myMem.dr,myMem.u,&myMem.node[i]);
	    Response(myMem.net,myMem.dr,myMem.u);
	    DiseaseRM(myMem.net,myMem.dr,myMem.u);
	  }
	Accumulate(i,myMem.node[i].info->nz[0],myMem.net,myMem.dr,myMem.node[i].pop,myMem.u,&myInterimXA,myMem.ta,&myImpactHist);
      }

    AccumulateFinal(myMem.net,myMem.dr,myMem.xa,&myInterimXA);

    myCurrentFatalities=myImpactHist.fh;
  }

};

#endif
