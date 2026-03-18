
#include <stdlib.h>
#include <jni.h>
#include "ExternalAnalysis.h"
#include "HealthImpacts.h"

/*
 * Class:     Java_anl_teva_analysis_modules_HIA
 * Method:    getMethods
 * Signature: (V)I
 */
JNIEXPORT jint JNICALL Java_anl_teva_analysis_modules_HIAnalysis_getMethodPointers
  (JNIEnv *env, jobject obj)
{
	// DevPartner identifies this as a memory leak because the
	// variable f is going out of scope without being initialized.
	// this is only because the JVM is not instrumented and it does not
	// know it is being assigned upon the return from this method.
	// This is not a memory leak because the pointer value is held by the
	// HIAnalysis class and is the memory is freed up via a call to cleanup().

	FunctionsPtr f = (FunctionsPtr)calloc(1,sizeof(Functions));
	f->aggr_initialize   = HIAnalysis_aggr_initialize;
	f->aggr_newResults   = HIAnalysis_aggr_newResults;
	f->aggr_getResults   = HIAnalysis_aggr_getResults;
	f->aggr_writeResults = HIAnalysis_aggr_writeResults;
	f->aggr_shutdown     = HIAnalysis_aggr_shutdown;

	f->ia_initialize    = HIAnalysis_ia_initialize;
	f->ia_newSimResults = HIAnalysis_ia_newSimResults;
	f->ia_shutdown      = HIAnalysis_ia_shutdown;

	return (jint)f;
}
/*
 * Class:     Java_anl_teva_analysis_modules_HIA
 * Method:    cleanup
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_anl_teva_analysis_modules_HIAnalysis_cleanup
  (JNIEnv *env, jobject obj, jint functions)
{
	free((void *)functions);
}