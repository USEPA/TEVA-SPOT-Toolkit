#include <stdio.h>

extern "C" {

#include "ExternalAnalysis.h"
#include "tevautil.h"

/* Global Data Types */
typedef enum ModuleType 
{
	Aggregation,
	IntermediateAnalysis
} ModuleType;

typedef struct
{
	PTSO         tso;
	PNodeInfo    node;
	PSource      sources;
	PNetInfo     net;
	int 	     hello; // dummy GUI input
} Mem;
typedef Mem *PMem;


PMem loadOptions(NamedDataRef *analysisOptionsRef, NamedDataRef *simResultsRef, 
		 ModuleType componentType);
void freeMemory(PMem mem, ModuleType componentType);


int HelloAnalysis_ia_initialize(NamedDataRef *analysisOptionsRef, 
				NamedDataRef *simResultsRef);
void HelloAnalysis_ia_newSimResults(NamedDataRef *analysisOptionsRef,
				    NamedDataRef *simResultsRef, 
				    NamedDataRef *intResultsRef);
void HelloAnalysis_ia_shutdown(NamedDataRef *analysisOptionsRef);

void HelloAnalysis_aggr_initialize(NamedDataRef *analysisOptionsRef, 
				   NamedDataRef *simResultsRef);
void HelloAnalysis_aggr_newResults(NamedDataRef *analysisOptionsRef,
				   NamedDataRef *intResultsRef);
void HelloAnalysis_aggr_getResults(NamedDataRef *analysisOptionsRef,
				   NamedDataRef *resultsRef);
void HelloAnalysis_aggr_writeResults(NamedDataRef *analysisOptionsRef,
				     NamedDataRef *resultsRef, 
				     const char *dir, char *fn);
void HelloAnalysis_aggr_shutdown(NamedDataRef *analysisOptionsRef);

};
