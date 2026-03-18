
#include <stdlib.h>
#include <jni.h>

#include "HelloAnalysis.h"
extern "C" {
#include "ExternalAnalysis.h"


/*
 * Class:     Java_anl_teva_analysis_modules_HelloA
 * Method:    getMethods
 * Signature: (V)I
 */
JNIEXPORT jint JNICALL Java_anl_teva_analysis_modules_HelloAnalysis_getMethodPointers
  (JNIEnv *env, jobject obj)
{
	FunctionsPtr f = (FunctionsPtr)calloc(1,sizeof(Functions));
	f->aggr_initialize   = HelloAnalysis_aggr_initialize;
	f->aggr_newResults   = HelloAnalysis_aggr_newResults;
	f->aggr_getResults   = HelloAnalysis_aggr_getResults;
	f->aggr_writeResults = HelloAnalysis_aggr_writeResults;
	f->aggr_shutdown     = HelloAnalysis_aggr_shutdown;

	f->ia_initialize    = HelloAnalysis_ia_initialize;
	f->ia_newSimResults = HelloAnalysis_ia_newSimResults;
	f->ia_shutdown      = HelloAnalysis_ia_shutdown;

	return (jint)f;
}
/*
 * Class:     Java_anl_teva_analysis_modules_HelloA
 * Method:    cleanup
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_anl_teva_analysis_modules_HelloAnalysis_cleanup
  (JNIEnv *env, jobject obj, jint functions)
{
	free((void *)functions);
}

};
