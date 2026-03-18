#ifdef HAVE_CONFIG_H
#include <teva_config.h>
#endif
#include "ObjectiveBase.h"
/*
#include <sp/scenario.h>
#include <utilib/exception_mngr.h>
#include "MassConsumedObjective.h"
#if TEVA_SPOT_LIMITED
#include "PopulationExposedObjective.h"
#include "PopulationKilledObjective.h"
#endif
#include "NumberFailedDetectionsObjective.h"
#include "TimeToDetectionObjective.h"
#include "VolumeContaminatedWaterConsumedObjective.h"
#include "ExtentOfContaminationObjective.h"
#include "impacts.h"
*/
void finalizeTheObjectives(std::vector<ObjectiveBase*>&theObjectives)
{
      for(size_t i=0;i<theObjectives.size();i++)
	{
	  theObjectives[i]->finalize();
	  delete theObjectives[i];
	}

      theObjectives.clear();
}
