#ifndef tevasp_VolumeContaminatedWaterConsumedObjective_h
#define tevasp_VolumeContaminatedWaterConsumedObjective_h

#include "ObjectiveBase.h"

/// Trait/Functional class for the volume of contamined water consumed objective
class VolumeContaminatedWaterConsumedObjective : public ObjectiveBase
{
public:

  /// Constructor.
  VolumeContaminatedWaterConsumedObjective(void):
    myAggregateConsumption(0.0)
  {
  }

  /// Copy constructor.
  VolumeContaminatedWaterConsumedObjective(const VolumeContaminatedWaterConsumedObjective &other):
    ObjectiveBase(other),
    myAggregateConsumption(other.myAggregateConsumption)
  {
  }

  /// Destructor.
  virtual ~VolumeContaminatedWaterConsumedObjective(void)
  {
    // no dynamic memory to worry about
  }

  /// Assignment operator.
  VolumeContaminatedWaterConsumedObjective &operator=(const VolumeContaminatedWaterConsumedObjective &other)
  {
    if(this!=&other)
      {
	ObjectiveBase::operator=(other);
	myAggregateConsumption=other.myAggregateConsumption;
      }
    return *this;
  }

  ///
  virtual void init(const std::string &outputPrefix,PNetInfo net,PNodeInfo nodes,PLinkInfo links,double detectionDelay)
  {
    ObjectiveBase::init(outputPrefix,net,nodes,links,detectionDelay);
    myAggregateConsumption=0.0;
  }

  ///
  virtual void resetForScenario(PNetInfo net, PNodeInfo nodes, PLinkInfo links,
				PSource sources)
  {
    ObjectiveBase::resetForScenario(net,nodes,links,sources);
    myAggregateConsumption=0.0;
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
  virtual void updateImpactStatisticsDueToNode(int thisNodeIndex, double thisNodeQuality, PNodeInfo nodes, PLinkInfo links, int thisTimeStep, double reportStep)
  {
    double thisNodeDemand(nodes[thisNodeIndex].q[thisTimeStep]);
    if((thisNodeDemand>0.0)&&(nodes[thisNodeIndex].ntype!=tank))
      {
	// conversion is liters->gallons
	myAggregateConsumption+=(thisNodeDemand*reportStep);
      }
  }

  ///
  virtual void addNominalImpact(int thisNodeIndex,int thisTimeStep,double reportStep)
  {
    updateImpacts(thisNodeIndex,myAggregateConsumption,thisTimeStep*reportStep);
    updateSensorEvaluation(thisNodeIndex,myAggregateConsumption);
  }

  ///
  virtual void addUndetectedImpact(int maxTimeStep,double reportStep)
  {
    updateImpacts(UNDETECTED_NODE_INDEX,myAggregateConsumption,maxTimeStep*reportStep);
    updateSensorEvaluation(UNDETECTED_NODE_INDEX,myAggregateConsumption);
  }

  ///
  static std::string ourImpactOutputFileSuffix;

  ///
  static std::string ourEvaluationOutputFileSuffix;

private:

  /// units are in gallons. defaults to 0.0.
  double myAggregateConsumption; 

};

#endif
