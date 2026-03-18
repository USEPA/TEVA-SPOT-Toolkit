#ifndef tevasp_ExtentOfContaminationObjective_h
#define tevasp_ExtentOfContaminationObjective_h

#include <map>
#include "ObjectiveBase.h"


/// Trait/Functional class for the extent-of-contamination objective //
class ExtentOfContaminationObjective : public ObjectiveBase
{
public:

  /// Constructor.
  ExtentOfContaminationObjective(void):
    myContaminatedPipeFeet(0.0)
  {
  }

  /// Copy constructor.
  ExtentOfContaminationObjective(const ExtentOfContaminationObjective &other):
    ObjectiveBase(other),
    myContaminatedPipeFeet(other.myContaminatedPipeFeet),
    myPipeLengths(other.myPipeLengths),
    myLinkHits(other.myLinkHits)
  {
  }


  /// Destructor.
  virtual ~ExtentOfContaminationObjective(void)
  {
    // no dynamic memory to worry about
  }

  /// Assignment operator.
  ExtentOfContaminationObjective &operator=(const ExtentOfContaminationObjective &other)
  {
    if(this!=&other)
      {
	ObjectiveBase::operator=(other);
	myContaminatedPipeFeet=other.myContaminatedPipeFeet;
	myPipeLengths=other.myPipeLengths;
	myLinkHits=other.myLinkHits;
      }
    return *this;
  }

  ///
  virtual void init(const std::string &outputPrefix,PNetInfo net,PNodeInfo nodes,PLinkInfo links,double detectionDelay)
  {
    ObjectiveBase::init(outputPrefix,net,nodes,links,detectionDelay);
    myContaminatedPipeFeet=0.0;
    myLinkHits=std::vector<bool>(net->nlinks,false);
  }

  ///
  void setPipeLengths(const std::map<std::pair<int,int>,double> &theLengths)
  {
    myPipeLengths=theLengths;
  }

  ///
  virtual void resetForScenario(void)
  {
    ObjectiveBase::resetForScenario();
    myContaminatedPipeFeet=0.0;
    for(size_t i=0;i<myLinkHits.size();i++)
      {
	myLinkHits[i]=false;
      }
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
  virtual void updateImpactStatisticsDueToLink(int thisLinkIndex,PNodeInfo nodes,PLinkInfo links,int thisTimeStep,double reportStep)
  {
    if(myLinkHits[thisLinkIndex]==false)
      {
	std::pair<int,int> key(std::make_pair(links[thisLinkIndex].from-1,links[thisLinkIndex].to-1));
	
	if(myPipeLengths.find(key)==myPipeLengths.end())
	  {
	    // skipping valve/pump/other dynamic link entries
	  }
	else
	  {
	    myContaminatedPipeFeet+=myPipeLengths[key];
	  }
	
	myLinkHits[thisLinkIndex]=true;
      }
  }

  ///
  virtual void addNominalImpact(int thisNodeIndex,int thisTimeStep,double reportStep)
  {
    updateImpacts(thisNodeIndex,myContaminatedPipeFeet,thisTimeStep*reportStep);

    updateSensorEvaluation(thisNodeIndex,myContaminatedPipeFeet);
  }

  ///
  virtual void addUndetectedImpact(int maxTimeStep,double reportStep)
  {
    updateImpacts(UNDETECTED_NODE_INDEX,myContaminatedPipeFeet,maxTimeStep*reportStep);

    updateSensorEvaluation(UNDETECTED_NODE_INDEX,myContaminatedPipeFeet);
  }

  ///
  static std::string ourImpactOutputFileSuffix;

  ///
  static std::string ourEvaluationOutputFileSuffix;

private:

  /// The aggregate length of pipe that is currently contaminated.
  /// Units are in feet. defaults to 0.0.
  double myContaminatedPipeFeet;

  /// Provided externally - maps pipe indicies in the form of from/to TEVA
  /// node index pairs to pipe lengths, in feet.
  std::map<std::pair<int,int>,double> myPipeLengths;

  /// A map to indicate whether or not links (0-based TEVA indicies) 
  /// have been "hit" during the current scenario.
  std::vector<bool> myLinkHits;

};

#endif
