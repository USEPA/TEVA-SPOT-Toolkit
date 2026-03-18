/*
 * Copyright © 2008 UChicago Argonne, LLC
 * NOTICE: This computer software, TEVA-SPOT, was prepared for UChicago Argonne, LLC
 * as the operator of Argonne National Laboratory under Contract No. DE-AC02-06CH11357
 * with the Department of Energy (DOE). All rights in the computer software are reserved
 * by DOE on behalf of the United States Government and the Contractor as provided in
 * the Contract.
 * NEITHER THE GOVERNMENT NOR THE CONTRACTOR MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR
 * ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE.
 *
 * This software is distributed under the BSD License.
 */
#include <stdio.h>

#include "teva-assess.h"
#include "ExternalAnalysis.h"

/* Global Data Types */

typedef struct ConcentrationData {
	double rankValue;
	char *simID;
	char *injDef;
	int nnodes;
	int nsteps;
	float** conc;
	XA na;
	struct ConcentrationData *next;
} ConcentrationData, *ConcentrationDataPtr;

typedef struct CachedData {
    float val;
	char *simID;
	char *injDef;
	__file_pos_t fileOffset;
} CachedData, *CachedDataPtr;

typedef struct DiskCachedData {
	FILE *fp;
	char fn[256];
	int num;
	int size;
	CachedDataPtr *data;
}DiskCachedData, *DiskCachedDataPtr;

JNIEXPORT PMem loadHIAOptions(void *analysisOptionsRef, void *simResultsRef, ModuleType componentType);
JNIEXPORT void freeHIAMemory(PMem mem, ModuleType componentType);
JNIEXPORT PDoseOverThresh allocateDoseOverThresholdWithIDs(void *analysisOptionsRef, PMem mem);
JNIEXPORT PDoseOverThresh allocateDoseOverThreshold(void *analysisOptionsRef, PMem mem);
JNIEXPORT void freeDoseOverThresholdData(PMem mem);

JNIEXPORT int HIAnalysis_ia_initialize(NamedDataRef *analysisOptionsRef, NamedDataRef *simResultsRef);
JNIEXPORT void HIAnalysis_ia_newSimResults(NamedDataRef *analysisOptionsRef,NamedDataRef *simResultsRef, NamedDataRef *intResultsRef);
JNIEXPORT void HIAnalysis_ia_shutdown(NamedDataRef *analysisOptionsRef);

JNIEXPORT void HIAnalysis_aggr_initialize(NamedDataRef *analysisOptionsRef, NamedDataRef *simResultsRef);
JNIEXPORT void HIAnalysis_aggr_newResults(NamedDataRef *analysisOptionsRef,NamedDataRef *intResultsRef);
JNIEXPORT void HIAnalysis_aggr_getResults(NamedDataRef *analysisOptionsRef,NamedDataRef *resultsRef);
JNIEXPORT void HIAnalysis_aggr_writeResults(NamedDataRef *analysisOptionsRef,NamedDataRef *resultsRef, const char *dir, char *fn);
JNIEXPORT void HIAnalysis_aggr_shutdown(NamedDataRef *analysisOptionsRef);
