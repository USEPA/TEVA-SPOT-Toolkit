#include <jni.h>
#include "IntAnalysisServer.h"
#include "ExternalAnalysis.h"
/*
 * Class:     anl_teva_analysis_module_ExternalIntermediateAnalysisServerImpl
 * Method:    init
 * Signature: (Lanl/teva/analysis/module/NamedDataServer;)V
 */
JNIEXPORT jboolean JNICALL Java_anl_teva_analysis_module_ExternalIntermediateAnalysisServerImpl_init
  (JNIEnv *env, jobject obj, jint functions, jobject jAnalysisOptions, jobject jSimResults)
{
	FunctionsPtr fp = (FunctionsPtr)functions;

	NamedDataRef analysisOptionsRef;
	NamedDataRef simResultsRef;

	analysisOptionsRef.env = env;
	analysisOptionsRef.obj = jAnalysisOptions;

	simResultsRef.env = env;
	simResultsRef.obj = jSimResults;

	return fp->ia_initialize(&analysisOptionsRef,&simResultsRef)==1;
}

/*
 * Class:     anl_teva_analysis_module_ExternalIntermediateAnalysisServerImpl
 * Method:    shutdown
 * Signature: (Lanl/teva/analysis/module/NamedDataServer;)V
 */
JNIEXPORT void JNICALL Java_anl_teva_analysis_module_ExternalIntermediateAnalysisServerImpl_shutdown
  (JNIEnv *env, jobject obj, jint functions, jobject jAnalysisOptions)
{
	FunctionsPtr fp = (FunctionsPtr)functions;

	NamedDataRef analysisOptionsRef;
	analysisOptionsRef.env = env;
	analysisOptionsRef.obj = jAnalysisOptions;

	fp->ia_shutdown(&analysisOptionsRef);
}

/*
 * Class:     anl_teva_analysis_module_ExternalIntermediateAnalysisServerImpl
 * Method:    newSimulationResults
 * Signature: (Lanl/teva/analysis/module/NamedDataServer;Lanl/teva/analysis/module/NamedDataServer;Lanl/teva/analysis/module/NamedData;)V
 */
JNIEXPORT void JNICALL Java_anl_teva_analysis_module_ExternalIntermediateAnalysisServerImpl_newSimulationResults
  (JNIEnv *env, jobject obj, jint functions, jobject jAnalysisOptions, jobject jSimResults, jobject jIntermediateResults)
{
	FunctionsPtr fp = (FunctionsPtr)functions;

	NamedDataRef analysisOptionsRef;
	NamedDataRef simResultsRef;
	NamedDataRef intResultsRef;

	analysisOptionsRef.env = env;
	analysisOptionsRef.obj = jAnalysisOptions;

	simResultsRef.env = env;
	simResultsRef.obj = jSimResults;

	intResultsRef.env = env;
	intResultsRef.obj = jIntermediateResults;

	fp->ia_newSimResults(&analysisOptionsRef,&simResultsRef, &intResultsRef);
}


#ifdef __cplusplus
}
#endif
