#ifndef tevasp_TimeToDetectionObjective_h
#define tevasp_TimeToDetectionObjective_h

#include "ObjectiveBase.h"


/// Trait/Functional class for the time-to-detection objective //
class TimeToDetectionObjective : public ObjectiveBase
{
public:

  /// Constructor.
  TimeToDetectionObjective(void):
    myCurrentTime(0.0)
  {
  }

  /// Copy constructor.
  TimeToDetectionObjective(const TimeToDetectionObjective &other):
    ObjectiveBase(other),
    myCurrentTime(other.myCurrentTime)
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
	myCurrentTime=other.myCurrentTime;
      }
    return *this;
  }

  ///
  virtual void init(const std::string &outputPrefix,PNetInfo net,PNodeInfo nodes,PLinkInfo links,double detectionDelay)
  {
    ObjectiveBase::init(outputPrefix,net,nodes,links,detectionDelay);
    myCurrentTime=0.0;
  }

  ///
  virtual void resetForScenario(void)
  {
    ObjectiveBase::resetForScenario();
    myCurrentTime=0.0;
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
    updateImpacts(thisNodeIndex,thisTimeStep*reportStep,thisTimeStep*reportStep);

    updateSensorEvaluation(thisNodeIndex,thisTimeStep*reportStep);
  }

  ///
  virtual void addUndetectedImpact(int maxTimeStep,double reportStep)
  {
    myCurrentTime=maxTimeStep*reportStep;

    updateImpacts(UNDETECTED_NODE_INDEX,myCurrentTime,maxTimeStep*reportStep);

    updateSensorEvaluation(UNDETECTED_NODE_INDEX,myCurrentTime);
  }

  ///
  static std::string ourImpactOutputFileSuffix;

  ///
  static std::string ourEvaluationOutputFileSuffix;

private:

  /// The number of minutes since the attack was initiated.
  /// The units are in minutes. defaults to 0.0.
  double myCurrentTime;

};

#endif
