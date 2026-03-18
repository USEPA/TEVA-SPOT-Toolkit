#if 0
#ifdef HAVE_CONFIG_H
#include <teva_config.h>
#endif
#include "MassConsumedObjective.h"
#if TEVA_SPOT_LIMITED
#include "PopulationExposedObjective.h"
#include "PopulationKilledObjective.h"
#include "PopulationDosedObjective.h"
#endif
#include "NumberFailedDetectionsObjective.h"
#include "TimeToDetectionObjective.h"
#include "VolumeContaminatedWaterConsumedObjective.h"
#include "ExtentOfContaminationObjective.h"

std::string ExtentOfContaminationObjective::ourImpactOutputFileSuffix="ec.impact";
std::string ExtentOfContaminationObjective::ourEvaluationOutputFileSuffix="ec.eval";

std::string MassConsumedObjective::ourImpactOutputFileSuffix="mc.impact";
std::string MassConsumedObjective::ourEvaluationOutputFileSuffix="mc.eval";

std::string NumberFailedDetectionsObjective::ourImpactOutputFileSuffix="nfd.impact";
std::string NumberFailedDetectionsObjective::ourEvaluationOutputFileSuffix="nfd.eval";

std::string TimeToDetectionObjective::ourImpactOutputFileSuffix="td.impact";
std::string TimeToDetectionObjective::ourEvaluationOutputFileSuffix="td.eval";

std::string VolumeContaminatedWaterConsumedObjective::ourImpactOutputFileSuffix="vc.impact";
std::string VolumeContaminatedWaterConsumedObjective::ourEvaluationOutputFileSuffix="vc.eval";
#endif
