#ifndef tevasp_MassConsumedObjective_h
#define tevasp_MassConsumedObjective_h

#include <string>
#include "ObjectiveBase.h"

const double LITERS_PER_GAL=3.7854;


/// Trait/Functional class for the mass consumed objective
class MassConsumedObjective : public ObjectiveBase
{
public:

  /// Constructor.
  MassConsumedObjective(void):
    myAggregateConsumption(0.0)
  {
  }

  /// Copy constructor.
  MassConsumedObjective(const MassConsumedObjective &other):
    ObjectiveBase(other),
    myAggregateConsumption(other.myAggregateConsumption)
  {
  }

  /// Assignment operator.
  MassConsumedObjective &operator=(const MassConsumedObjective &other)
  {
    if(this!=&other)
      {
	ObjectiveBase::operator=(other);
	myAggregateConsumption=other.myAggregateConsumption;
      }
    return *this;
  }

  /// Destructor.
  virtual ~MassConsumedObjective(void)
  {
    // no dynamic memory to worry about
  }

  ///
  virtual void init(const std::string &outputPrefix,PNetInfo net,PNodeInfo nodes,PLinkInfo links,double detectionDelay)
  {
    ObjectiveBase::init(outputPrefix,net,nodes,links,detectionDelay);
    myAggregateConsumption=0.0;
  }

  ///
  virtual void resetForScenario(void)
  {
    ObjectiveBase::resetForScenario();
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
	myAggregateConsumption+=(thisNodeQuality*LITERS_PER_GAL*thisNodeDemand*reportStep);
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

  /// Units are in gallons. defaults to 0.0.
  double myAggregateConsumption; 

};

#endif
