#ifndef TSO2IMPACTANALYSIS_H
#define TSO2IMPACTANALYSIS_H

#include <stdio.h>
#include <iostream>
#include <string>


extern "C" {

#include "ExternalAnalysis.h"
#include "tevautil.h"
#include "teva-assess.h"

/* Global Data Types */

/*
typedef struct
{
	PTSO         tso;
	PNodeInfo    node;
	PSource      sources;
	PNetInfo     net;
} tso2ImpactMem;
typedef tso2ImpactMem *Ptso2ImpactMem;
*/

PMem loadOptions(NamedDataRef *analysisOptionsRef, 
		 NamedDataRef *simResultsRef, ModuleType componentType,
      		 std::string&outputFilePrefix,
      		 std::string&tsoFileOrDirectoryName,
      		 int&detectionDelay,
      		 double&minQuality,
      		 bool&mcSelected,
      		 bool&vcSelected,
      		 bool&nfdSelected,
      		 bool&tdSelected,
      		 bool&ecSelected,
      		 std::string&epanetInputFilename,
      		 bool&pkSelected,
      		 bool&peSelected,
      		 std::string&taiInputFilename,
      		 std::string&tsoPattern);

void freeMemory(PMem mem, ModuleType componentType);


int tso2ImpactAnalysis_ia_initialize(NamedDataRef *analysisOptionsRef, 
				NamedDataRef *simResultsRef);
void tso2ImpactAnalysis_ia_newSimResults(NamedDataRef *analysisOptionsRef,
				    NamedDataRef *simResultsRef, 
				    NamedDataRef *intResultsRef);
void tso2ImpactAnalysis_ia_shutdown(NamedDataRef *analysisOptionsRef);

void tso2ImpactAnalysis_aggr_initialize(NamedDataRef *analysisOptionsRef, 
				   NamedDataRef *simResultsRef);
void tso2ImpactAnalysis_aggr_newResults(NamedDataRef *analysisOptionsRef,
				   NamedDataRef *intResultsRef);
void tso2ImpactAnalysis_aggr_getResults(NamedDataRef *analysisOptionsRef,
				   NamedDataRef *resultsRef);
void tso2ImpactAnalysis_aggr_writeResults(NamedDataRef *analysisOptionsRef,
				     NamedDataRef *resultsRef, 
				     const char *dir, char *fn);
void tso2ImpactAnalysis_aggr_shutdown(NamedDataRef *analysisOptionsRef);

};

#endif
