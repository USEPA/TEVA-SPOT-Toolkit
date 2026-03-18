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
#ifndef _LOGGINGUTILS_H_
#define _LOGGINGUTILS_H_

#include <jni.h>

typedef struct LoggingData {
	jclass loggerClass;  // java.logging.Logger
	jclass levelClass;   // java.logging.Level

	jmethodID loggerGetLogger;
	jmethodID loggerLogLevelString;

	jmethodID levelParse;
	jobject levelSEVERE;
	jobject levelWARNING;
	jobject levelCONFIG;
	jobject levelINFO;
	jobject levelFINE;
	jobject levelFINER;
	jobject levelFINEST;

} LoggingData, *LoggingDataPtr;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void ANL_UTIL_LogSevere(JNIEnv *env,const char *loggerName, const char *msg);
JNIEXPORT void ANL_UTIL_LogWarning(JNIEnv *env,const char *loggerName, const char *msg);
JNIEXPORT void ANL_UTIL_LogConfig(JNIEnv *env,const char *loggerName, const char *msg);
JNIEXPORT void ANL_UTIL_LogInfo(JNIEnv *env,const char *loggerName, const char *msg);
JNIEXPORT void ANL_UTIL_LogFine(JNIEnv *env,const char *loggerName, const char *msg);
JNIEXPORT void ANL_UTIL_LogFiner(JNIEnv *env,const char *loggerName, const char *msg);
JNIEXPORT void ANL_UTIL_LogFinest(JNIEnv *env,const char *loggerName, const char *msg);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _LOGGINGUTILS_H_ */
