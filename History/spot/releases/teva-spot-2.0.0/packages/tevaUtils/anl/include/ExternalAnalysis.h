#ifndef EXTERNAL_ANALYSIS_H
#define EXTERNAL_ANALYSIS_H

#include "NamedData.h"

typedef struct Functions {
	void (*aggr_initialize)(NamedDataRef *, NamedDataRef *);
	void (*aggr_newResults)(NamedDataRef *, NamedDataRef *);
	void (*aggr_getResults)(NamedDataRef *, NamedDataRef *);
	void (*aggr_writeResults)(NamedDataRef *, NamedDataRef *, const char *, char *);
	void (*aggr_shutdown)(NamedDataRef *);

	int  (*ia_initialize)(NamedDataRef *, NamedDataRef *);
	void (*ia_newSimResults)(NamedDataRef *,NamedDataRef *, NamedDataRef *);
	void (*ia_shutdown)(NamedDataRef *);

} Functions, *FunctionsPtr;

typedef enum ModuleType
{
        Aggregation,
        IntermediateAnalysis
} ModuleType;



/* return values from ia_initialize method */
#define NON_REMOTE 0
#define REMOTE     1

#endif
