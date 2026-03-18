#include <stdio.h>

#include "teva-assess.h"
#include "ExternalAnalysis.h"

/* Global Data Types */

typedef struct ConcentrationData {
	double rankValue;
	char *simID;
	int nnodes;
	int nsteps;
	float** conc;
	XA na;
	struct ConcentrationData *next;
} ConcentrationData, *ConcentrationDataPtr;

typedef struct CachedData {
    float val;
	char *simID;
	long fileOffset;
} CachedData, *CachedDataPtr;

typedef struct DiskCachedData {
	FILE *fp;
	char fn[256];
	int num;
	size_t size;
	CachedDataPtr *data;
}DiskCachedData, *DiskCachedDataPtr;

PMem loadHIAOptions(void *analysisOptionsRef, void *simResultsRef, ModuleType componentType);
void freeHIAMemory(PMem mem, ModuleType componentType);

int HIAnalysis_ia_initialize(NamedDataRef *analysisOptionsRef, NamedDataRef *simResultsRef);
void HIAnalysis_ia_newSimResults(NamedDataRef *analysisOptionsRef,NamedDataRef *simResultsRef, NamedDataRef *intResultsRef);
void HIAnalysis_ia_shutdown(NamedDataRef *analysisOptionsRef);

void HIAnalysis_aggr_initialize(NamedDataRef *analysisOptionsRef, NamedDataRef *simResultsRef);
void HIAnalysis_aggr_newResults(NamedDataRef *analysisOptionsRef,NamedDataRef *intResultsRef);
void HIAnalysis_aggr_getResults(NamedDataRef *analysisOptionsRef,NamedDataRef *resultsRef);
void HIAnalysis_aggr_writeResults(NamedDataRef *analysisOptionsRef,NamedDataRef *resultsRef, const char *dir, char *fn);
void HIAnalysis_aggr_shutdown(NamedDataRef *analysisOptionsRef);
