#ifndef tevasp_PopulationExposedObjective_h
#define tevasp_PopulationExposedObjective_h

#include "ObjectiveBase.h"


/// Trait/Functional class for the population exposed (i.e., sick) objective
class PopulationExposedObjective : public ObjectiveBase
{
public:

  /// Constructor.
  PopulationExposedObjective(PMem mem):
    myCurrentSickened(0.0),
    myLastComputedTimeStep(-1),
    myLastImpactTimeStep(-1)
  {
    assessMem = mem;
    myNetInfo = NULL;
    myDoseDim[0] = 0; 
    myDoseDim[1] = 0; 
  }

  /// Copy constructor.
  PopulationExposedObjective(const PopulationExposedObjective &other):
    ObjectiveBase(other),
    myCurrentSickened(other.myCurrentSickened),
    myTAIFileName(other.myTAIFileName),
    myLastComputedTimeStep(other.myLastComputedTimeStep),
    myLastImpactTimeStep(other.myLastImpactTimeStep)
  {
    // we don't copy assessMem and myNetInfo
    copyDoses(other);
  }

  /// Destructor.
  virtual ~PopulationExposedObjective(void)
  {
    freeDoses();
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
        copyDoses(other);
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
  void initDoses()
  {
    freeDoses();

    int nnodes = assessMem->net->info->nnodes;
    int nsteps = assessMem->dr->maxsteps;

    if ((nnodes > 0) && (nsteps > 0)){
      myDoses = new float * [nnodes];

      for (int i=0; i<nnodes; i++){
        myDoses[i] = new float [nsteps];
        memset(myDoses[i], 0, sizeof(float) * nsteps);
      }    
    myDoseDim[0] = nnodes;
    myDoseDim[1] = nsteps;
    }
  }
  ///
  void calcDoses()
  {
    U data;
    int nnodes = myDoseDim[0];
    int nsteps = myDoseDim[1];

    for (int i=0; i<nnodes; i++){
      data.dos = myDoses[i];
      Dose(nsteps, assessMem->net,assessMem->dr,&data,&assessMem->node[i]); 
    }
  }
  ///
  void freeDoses()
  {
    if (myDoseDim[0]){
      for (int i=0; i<myDoseDim[0]; i++){
        delete [] myDoses[i];
      }
      delete [] myDoses;
      myDoseDim[0] = 0;
      myDoseDim[1] = 0;
      myDoses = NULL;
    }
  }
  ///
  void copyDoses(const PopulationExposedObjective &peo)
  {
    freeDoses();

    if (peo.myDoseDim[0] > 0){
      myDoses = new float *  [peo.myDoseDim[0]];
      for (int i=0; i<peo.myDoseDim[0]; i++){
        myDoses[i] = new float [peo.myDoseDim[1]];
        memcpy(myDoses[i], peo.myDoses[i], sizeof(float) * peo.myDoseDim[1]);
      }
      myDoseDim[0] = peo.myDoseDim[0];
      myDoseDim[1] = peo.myDoseDim[1];
    }
  }

  ///
  virtual void init(const std::string &outputPrefix,PNetInfo net,PNodeInfo nodes,PLinkInfo links,double detectionDelay)
  {
    if (myNetInfo){
      return; // init should not be called twice, memory would leak
    }

    ObjectiveBase::init(outputPrefix,net,nodes,links,detectionDelay);
    myCurrentSickened=0.0;

    myNetInfo = net;
/*
    assessMem->nodeinfo = nodes;
    assessMem->linkinfo = links;

    Population(assessMem->net,assessMem->dr,assessMem->node);
    IngestionRate(assessMem->net,assessMem->node);
    initialize(assessMem->net,assessMem->dr,assessMem->xa,assessMem->ta);

*/
    initDoses();

    myLastComputedTimeStep=-1;
    myLastImpactTimeStep=-1;
  }

  ///
  virtual void resetForScenario(PNetInfo net, PNodeInfo nodes, PLinkInfo links,
				PSource sources)
  {
    ObjectiveBase::resetForScenario(net,nodes,links,sources);
    myCurrentSickened=0.0;

    myLastComputedTimeStep=-1;
    myLastImpactTimeStep=-1;

    assessMem->net->info = net;
    assessMem->nodeinfo = nodes;
    assessMem->sources = sources;

    calcDoses();
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
  PMem assessMem;

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

  /// Table of doses by node ID by time step
  float **myDoses;
  int myDoseDim[2];

private:

  /// Re-compute the #-sickened statistics for the input
  /// time-step; results are stored in the member variables.
  void recomputeNumberSickened(int theNewTimeStep)
  {
    double sumPopulationImpacted(0.0);

    if (theNewTimeStep > myDoseDim[1]){
     theNewTimeStep = myDoseDim[1];
    }

    for(int i=0;i<assessMem->net->info->nnodes;i++) 
      {
	if(assessMem->node[i].info->nz[0]) // someone is consuming at this node
	  {
            double maxDose(myDoses[i][theNewTimeStep-1]); 

	    // response is the fraction of the population exposed
	    double maxResponse(DoseResponse(maxDose,assessMem->dr)); 

	    sumPopulationImpacted+=(maxResponse*assessMem->node[i].pop);
	  }
      }

    myCurrentSickened=sumPopulationImpacted;
  }

};

#endif
