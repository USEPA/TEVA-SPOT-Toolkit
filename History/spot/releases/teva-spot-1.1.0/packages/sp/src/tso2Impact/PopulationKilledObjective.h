#ifndef tevasp_PopulationKilledObjective_h
#define tevasp_PopulationKilledObjective_h

#include "ObjectiveBase.h"


/// Trait/Functional class for the Murray et al. health impacts - 
/// fatalities - objective
class PopulationKilledObjective : public ObjectiveBase
{
public:

  // Constructor.
  PopulationKilledObjective(PMem mem):
    myCurrentFatalities(0.0),
    myLastComputedTimeStep(-1),
    myLastImpactTimeStep(-1)
  {
    assessMem = mem;
    myNetInfo = NULL;
    myDoseDim[0] = 0;
    myDoseDim[1] = 0;
  }

  // Copy constructor.
  PopulationKilledObjective(const PopulationKilledObjective &other):
    ObjectiveBase(other),
    myCurrentFatalities(other.myCurrentFatalities),
    myTAIFileName(other.myTAIFileName),
    myLastComputedTimeStep(other.myLastComputedTimeStep),
    myLastImpactTimeStep(other.myLastImpactTimeStep)
  {
    // we don't copy assessMem and myNetInfo
    copyDoses(other);
  }

  /// Destructor.
  virtual ~PopulationKilledObjective()
  {
    
    freeDoses();
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
        copyDoses(other);
      }
    return *this;
  }

  virtual void update(utilib::AnyValue v) 
  {
	// we'll assume that the old assessMem was deleted 
	assessMem = utilib::any_cast<PMem>(v);
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
      myResponses = new float * [nnodes];
    
      for (int i=0; i<nnodes; i++){
        myDoses[i] = new float [nsteps];
        myResponses[i] = new float [nsteps];
      }    

      myDoseDim[0] = nnodes;
      myDoseDim[1] = nsteps;
    }
  }
  ///
  void setDoseResponse(int nodeId, int timeStep)
  {
    memcpy(assessMem->u->dos, myDoses[nodeId], sizeof(float) * timeStep);
    memcpy(assessMem->u->res, myResponses[nodeId], sizeof(float) * timeStep);

    float lastDose = assessMem->u->dos[timeStep-1];
    float lastResponse = assessMem->u->res[timeStep-1];

    for (int i=timeStep; i<myDoseDim[1]; i++){
      assessMem->u->dos[i] = lastDose;
      assessMem->u->res[i] = lastResponse;
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
      data.res = myResponses[i];

      Dose(nsteps, assessMem->net,assessMem->dr,&data,&assessMem->node[i]);
      Response(assessMem->net,assessMem->dr,&data);
    }
  }
  /// 
  void freeDoses()
  {
    if (myDoseDim[0]){
      for (int i=0; i<myDoseDim[0]; i++){
        delete [] myDoses[i];
        delete [] myResponses[i];
      }
      delete [] myDoses;
      delete [] myResponses;
      myDoseDim[0] = 0;
      myDoseDim[1] = 0;
    }
  } 
  ///
  void copyDoses(const PopulationKilledObjective &peo)
  {
    freeDoses();
    int nnodes = peo.myDoseDim[0];
    int nsteps = peo.myDoseDim[1];
  
    if ((nnodes > 0) && (nsteps > 0)){
      myDoses = new float *  [nnodes];
      myResponses = new float *  [nnodes];

      for (int i=0; i<nnodes; i++){
        myDoses[i] = new float [nsteps];
        myResponses[i] = new float [nsteps];
        memcpy(myDoses[i], peo.myDoses[i], sizeof(float) * nsteps);
        memcpy(myResponses[i], peo.myResponses[i], sizeof(float) * nsteps);
      }
      myDoseDim[0] = nnodes;
      myDoseDim[1] = nsteps;
    }
  }

  ///
  virtual void init(const std::string &outputPrefix,PNetInfo net,PNodeInfo nodes,PLinkInfo links,double detectionDelay, int numScenarios=-1)
  {
    ObjectiveBase::init(outputPrefix,net,nodes,links,detectionDelay,numScenarios);
    myCurrentFatalities=0.0;

    Population(assessMem->net,assessMem->dr,assessMem->node);
    IngestionRate(assessMem->net,assessMem->node);
    initialize(assessMem->net,assessMem->dr,assessMem->xa,assessMem->ta);
    initXA(&myInterimXA,(assessMem->dr)->maxsteps);

    myLastComputedTimeStep=-1;
    myLastImpactTimeStep=-1;

    initDoses();
  }

  ///
  virtual void resetForScenario(PNetInfo net, PNodeInfo nodes, PLinkInfo links,
				PSource sources)
  {
    ObjectiveBase::resetForScenario(net,nodes,links,sources);
    myCurrentFatalities=0.0;

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
  PMem assessMem;
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

  /// Tables of doses and responses by node ID by time step
  float **myDoses;
  float **myResponses;
  int myDoseDim[2];

private:

  /// re-compute the health impact statistics for 
  /// the input time-step; results are stored in 
  /// member variables.
  void recompueNumFatalities(int theNewTimeStep)
  {
    resetXA(&myInterimXA,assessMem->dr->maxsteps);

    memset(&myImpactHist,0,sizeof(HIST));

    // Time-dependent cumulative expected dose and disease response 
    for(int i=0;i<assessMem->net->info->nnodes;i++) 
      {
	if(assessMem->node[i].info->nz[0]) 
	  {
            // copy precomputed doses & responses into assessMem.u
            setDoseResponse(i, ((theNewTimeStep==INT_MAX) ? myDoseDim[1] : theNewTimeStep));
	    DiseaseRM(assessMem->net,assessMem->dr,assessMem->u);
	  }
	Accumulate(i,assessMem->node[i].info->nz[0],assessMem->net,assessMem->dr,assessMem->node[i].pop,assessMem->u,&myInterimXA,assessMem->ta,&myImpactHist);
      }

    AccumulateFinal(assessMem->net,assessMem->dr,assessMem->xa,&myInterimXA);

    myCurrentFatalities=myImpactHist.fh;
  }

};

#endif
