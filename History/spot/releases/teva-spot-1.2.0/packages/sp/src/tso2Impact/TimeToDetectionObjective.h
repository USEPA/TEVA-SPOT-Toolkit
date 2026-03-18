#ifndef tevasp_TimeToDetectionObjective_h
#define tevasp_TimeToDetectionObjective_h

#include "ObjectiveBase.h"


/// Trait/Functional class for the time-to-detection objective //
class TimeToDetectionObjective : public ObjectiveBase
{
public:

  /// Constructor.
  TimeToDetectionObjective(void):
    myTotalTime(0.0),
    myStartTime(0.0)
  {
  }

  /// Copy constructor.
  TimeToDetectionObjective(const TimeToDetectionObjective &other):
    ObjectiveBase(other),
    myTotalTime(other.myTotalTime),
    myStartTime(other.myStartTime)
  {
  }

  /// Destructor.
  virtual ~TimeToDetectionObjective(void)
  {
    // no dynamic memory to worry about
  }

  /// Assignment operator.
  TimeToDetectionObjective &operator=(const TimeToDetectionObjective &other)
  {
    if(this!=&other)
      {
	ObjectiveBase::operator=(other);
	myTotalTime=other.myTotalTime;
	myStartTime=other.myStartTime;
      }
    return *this;
  }

  ///
  virtual void init(const std::string &outputPrefix,PNetInfo net,PNodeInfo nodes,PLinkInfo links,double detectionDelay, int numScenarios=-1)
  {
    ObjectiveBase::init(outputPrefix,net,nodes,links,detectionDelay,numScenarios);
    myTotalTime=0.0;
    myStartTime=0.0;
  }

  ///
  virtual void resetForScenario(PNetInfo net, PNodeInfo nodes, PLinkInfo links,
				PSource sources)
  {
    ObjectiveBase::resetForScenario(net,nodes,links,sources);
    if (net->nsources > 1) {
	cerr << "ERROR: time-to-detection objective can't handle multiple "
		"sources." << endl;
	exit(1);
    }
    myStartTime = sources[0].SourceStart / 60.0;
    myTotalTime=0.0;
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
  virtual void addNominalImpact(int thisNodeIndex,int thisTimeStep,double reportStep)
  {
    double relTime = thisTimeStep*reportStep - myStartTime;
    updateImpacts(thisNodeIndex,relTime,thisTimeStep*reportStep);

    updateSensorEvaluation(thisNodeIndex,thisTimeStep*reportStep);
  }

  ///
  virtual void addUndetectedImpact(int maxTimeStep,double reportStep)
  {
    myTotalTime=maxTimeStep*reportStep - myStartTime;

    updateImpacts(UNDETECTED_NODE_INDEX,myTotalTime,maxTimeStep*reportStep);

    updateSensorEvaluation(UNDETECTED_NODE_INDEX,myTotalTime);
  }

  ///
  static std::string ourImpactOutputFileSuffix;

  ///
  static std::string ourEvaluationOutputFileSuffix;

private:

  /// The number of minutes since the event was initiated.
  /// The units are in minutes. defaults to 0.0.
  double myTotalTime;

  double myStartTime;

};

#endif
