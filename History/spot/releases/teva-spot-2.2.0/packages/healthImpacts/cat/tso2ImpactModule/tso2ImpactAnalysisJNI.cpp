#ifdef HAVE_CONFIG_H
#include <teva_config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <jni.h>

#include "tso2ImpactAnalysis.h"
extern "C" {
#include "ExternalAnalysis.h"


/*
 * Class:     Java_anl_teva_analysis_modules_tso2ImpactA
 * Method:    getMethods
 * Signature: (V)I
 */
JNIEXPORT jint JNICALL Java_anl_teva_analysis_modules_tso2ImpactModule_getMethodPointers
  (JNIEnv *env, jobject obj)
{
	printf("getMethodPointers\n");
	fflush(stdout);
	FunctionsPtr f = (FunctionsPtr)calloc(1,sizeof(Functions));
	f->aggr_initialize   = tso2ImpactAnalysis_aggr_initialize;
	f->aggr_newResults   = tso2ImpactAnalysis_aggr_newResults;
	f->aggr_getResults   = tso2ImpactAnalysis_aggr_getResults;
	f->aggr_writeResults = tso2ImpactAnalysis_aggr_writeResults;
	f->aggr_shutdown     = tso2ImpactAnalysis_aggr_shutdown;

	f->ia_initialize    = tso2ImpactAnalysis_ia_initialize;
	f->ia_newSimResults = tso2ImpactAnalysis_ia_newSimResults;
	f->ia_shutdown      = tso2ImpactAnalysis_ia_shutdown;

	return (jint)f;
}
/*
 * Class:     Java_anl_teva_analysis_modules_tso2ImpactA
 * Method:    cleanup
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_anl_teva_analysis_modules_tso2ImpactModule_cleanup
  (JNIEnv *env, jobject obj, jint functions)
{
	free((void *)functions);
}

};
