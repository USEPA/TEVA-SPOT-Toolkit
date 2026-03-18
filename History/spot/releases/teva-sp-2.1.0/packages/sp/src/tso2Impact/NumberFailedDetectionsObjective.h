#ifndef tevasp_NumberFailedDetectsionObjective_h
#define tevasp_NumberFailedDetectsionObjective_h

#include "ObjectiveBase.h"


/// Trait/Functional class for the number of failed detections objective //
class NumberFailedDetectionsObjective : public ObjectiveBase
{
public:

  /// Constructor.
  NumberFailedDetectionsObjective(void)
  {
  }

  /// Copy constructor.
  NumberFailedDetectionsObjective(const NumberFailedDetectionsObjective &other):
    ObjectiveBase(other)
  {
  }

  /// Destructor.
  virtual ~NumberFailedDetectionsObjective(void)
  {
    // no dynamic memory to worry about
  }

  /// Assignment operator.
  NumberFailedDetectionsObjective &operator=(const NumberFailedDetectionsObjective &other)
  {
    if(this!=&other)
      {
	ObjectiveBase::operator=(other);
      }
    return *this;
  }

  ///
  virtual void init(const std::string &outputPrefix,PNetInfo net,PNodeInfo nodes,PLinkInfo links,double detectionDelay)
  {
    ObjectiveBase::init(outputPrefix,net,nodes,links,detectionDelay);
  }

  ///
  virtual void resetForScenario(PNetInfo net, PNodeInfo nodes, PLinkInfo links,
				PSource sources)
  {
    ObjectiveBase::resetForScenario(net,nodes,links,sources);
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
    // this is a no-op - if a node is hit, then the attack is detected - 
    // no compuation is required.
  }

  ///
  virtual void addNominalImpact(int thisNodeIndex,int thisTimeStep,double reportStep)
  {
    updateImpacts(thisNodeIndex,0.0,thisTimeStep*reportStep); // 0.0 = false
    updateSensorEvaluation(thisNodeIndex,0.0);
  }

  ///
  virtual void addUndetectedImpact(int maxTimeStep,double reportStep)
  {
    updateImpacts(UNDETECTED_NODE_INDEX,1.0,maxTimeStep*reportStep); // 1.0 = true
    updateSensorEvaluation(UNDETECTED_NODE_INDEX,1.0);
  }

  ///
  static std::string ourImpactOutputFileSuffix;

  ///
  static std::string ourEvaluationOutputFileSuffix;

};

#endif
