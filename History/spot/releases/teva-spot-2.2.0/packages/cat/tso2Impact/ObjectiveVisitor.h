
#include <list>
#include "Impact.h"

class ObjectiveVisitor
{
public:

  std::string impactOutputFileName;

  virtual void outputScenarioImpactData(std::list<Impact>& myImpacts, int scenarioIndex)=0;

  virtual void finalize() = 0;

};

