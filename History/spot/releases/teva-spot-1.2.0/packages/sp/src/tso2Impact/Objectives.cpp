#include "MassConsumedObjective.h"
#include "PopulationExposedObjective.h"
#include "PopulationKilledObjective.h"
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

std::string PopulationExposedObjective::ourImpactOutputFileSuffix="pe.impact";
std::string PopulationExposedObjective::ourEvaluationOutputFileSuffix="pe.eval";

std::string PopulationKilledObjective::ourImpactOutputFileSuffix="pk.impact";
std::string PopulationKilledObjective::ourEvaluationOutputFileSuffix="pk.eval";

std::string TimeToDetectionObjective::ourImpactOutputFileSuffix="td.impact";
std::string TimeToDetectionObjective::ourEvaluationOutputFileSuffix="td.eval";

std::string VolumeContaminatedWaterConsumedObjective::ourImpactOutputFileSuffix="vc.impact";
std::string VolumeContaminatedWaterConsumedObjective::ourEvaluationOutputFileSuffix="vc.eval";
