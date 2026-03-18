/*
 * Copyright � 2008 UChicago Argonne, LLC
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
#ifdef __LINUX__
#include <errno.h>
#endif
#include <stdlib.h>
#include <memory.h>

#include "anl_teva_db_TSOReader.h"
#include "jniUtils.h"
#include "loggingUtils.h"
#include "tevautil.h"
#include "tsodata.h"

// local data definitions
static TSODataPtr x_tsoData=NULL;

// private method definitions
static jclass x_findClass(JNIEnv *env, char *className, char *dottedClassName);
static jboolean x_initTSOData(JNIEnv *env);
static void x_ensureDataInitialized(JNIEnv *env);
static jobject x_CreateNetInfoObject(JNIEnv *env, PNetInfo pNetInfo) ;
static jobject x_CreateNodeInfoObject(JNIEnv *env,PNodeInfo pNodeInfo, PNetInfo pNetInfo, int nodeidx, int nspecies, int nsteps);
static jobjectArray x_CreateNodeInfoArray(JNIEnv *env,PNetInfo pNetInfo,PNodeInfo pNodeInfo);
static jobjectArray x_CreateLinkInfoArray(JNIEnv *env,PNetInfo pNetInfo,PLinkInfo pLinkInfo);
static jobject x_CreateLinkInfoObject(JNIEnv *env,PLinkInfo pLinkInfo, PNetInfo pNetInfo, int linkidx, int nsteps);
static jobject x_createSourcesArray(JNIEnv *env, int nsources, PSource pSsource);
static void x_populateNodeInfoAndSources(JNIEnv *env, jobject reader, PNetInfo pNetInfo, PNodeInfo pNodeInfo, PSourceData pSource, jobjectArray jNodeInfo);


/*
 * Class:     anl_msv_dpf_erd_db_TSOReader
 * Method:    open
 * Signature: (Ljava/lang/String;Ljava/lang/String;CC)I
 */
JNIEXPORT jlong JNICALL Java_anl_msv_dpf_erd_db_TSOReader_open
  (JNIEnv *env, jobject reader, jstring jDirOrFilename, jstring jPatternOrNull, jchar fileVersion, jchar storageMethod)
{
	char *dirOrFilename;
	char *patternOrNull=NULL;
	PTSO ptso;
	char msg[1024];

	dirOrFilename=ANL_UTIL_javaStringToC(env,jDirOrFilename);
	if(jPatternOrNull != NULL)
		patternOrNull = ANL_UTIL_javaStringToC(env,jPatternOrNull);
	sprintf(msg,"Opening TSO file: %s  pattern: %s",dirOrFilename,patternOrNull);
	ANL_UTIL_LogInfo(env,"teva",msg);
	ptso = TSO_Open(dirOrFilename,patternOrNull);
	ANL_UTIL_free(dirOrFilename);
	ANL_UTIL_free(patternOrNull);
	return PTRTOJLONG(ptso);
}

/*
 * Class:     anl_msv_dpf_erd_db_TSOReader
 * Method:    positionAfterPrologue
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_anl_msv_dpf_erd_db_TSOReader_positionAfterPrologue
  (JNIEnv *env, jobject reader, jlong p_tso)
{
	TSO_PositionAfterPrologue(PTRCAST(PTSO)p_tso);
}

/*
 * Class:     anl_msv_dpf_erd_db_TSOReader
 * Method:    readPrologue
 * Signature: (I[Ljava/lang/Object;[I)V
 */
JNIEXPORT void JNICALL Java_anl_msv_dpf_erd_db_TSOReader_readPrologue
  (JNIEnv *env, jobject reader, jlong ptso, jint flags, jobjectArray objects)
{
	PNetInfo pNetInfo=NULL;
	PNodeInfo pNodeInfo;
	PLinkInfo pLinkInfo;
	PSource pSources;
	jobject netInfo;
	jobjectArray nodeInfoArray;
	jobjectArray linkInfoArray;

	TSO_ReadPrologue(PTRCAST(PTSO)ptso,&pNetInfo,&pNodeInfo,&pLinkInfo,flags);
	pSources = (PSource)calloc(MAXSOURCES,sizeof(Source));
	ANL_UTIL_setLongInstVar(env,reader,"pNodeInfo",PTRTOJLONG(pNodeInfo));
	ANL_UTIL_setLongInstVar(env,reader,"pLinkInfo",PTRTOJLONG(pLinkInfo));
	ANL_UTIL_setLongInstVar(env,reader,"pSource",PTRTOJLONG(pSources));

	netInfo = x_CreateNetInfoObject(env,pNetInfo);
	(*env)->SetObjectArrayElement(env,objects,0,netInfo);
	nodeInfoArray = x_CreateNodeInfoArray(env,pNetInfo,pNodeInfo);
	(*env)->SetObjectArrayElement(env,objects,1,nodeInfoArray);
	linkInfoArray = x_CreateLinkInfoArray(env,pNetInfo,pLinkInfo);
	(*env)->SetObjectArrayElement(env,objects,2,linkInfoArray);

}

/*
 * Class:     anl_msv_dpf_erd_db_TSOReader
 * Method:    closeAndFreeMemory
 * Signature: (IIIII)V
 */
JNIEXPORT void JNICALL Java_anl_msv_dpf_erd_db_TSOReader_closeAndFreeMemory
  (JNIEnv *env, jobject reader, jlong ptso, jlong pNetInfo, jlong pNodeInfo, jlong pLinkInfo, jlong pSource)
{
	PTSO tso = PTRCAST(PTSO)ptso;
	PNodeInfo nodeInfo = PTRCAST(PNodeInfo)pNodeInfo;
	PLinkInfo linkInfo = PTRCAST(PLinkInfo)pLinkInfo;
	PNetInfo netInfo = PTRCAST(PNetInfo)pNetInfo;

	TSO_Close(&tso);
	TSO_ReleaseNetworkData(&nodeInfo,&linkInfo,PTRCAST(PNetInfo)pNetInfo);
	TSO_ReleaseNetworkInfo(&netInfo);

}


/*
 * Class:     anl_msv_dpf_erd_db_TSOReader
 * Method:    readSimulationResults
 * Signature: (IIII[[F)V
 */
JNIEXPORT jboolean JNICALL Java_anl_msv_dpf_erd_db_TSOReader_readSimulationResults
  (JNIEnv *env, jobject reader, jlong ptso, jlong pNetInfo, jlong pNodeInfo, jlong pSource, jobjectArray jNodeInfo)
{
  jboolean hasData=0;
  int err;
  err = TSO_ReadSimulationResults(PTRCAST(PTSO)ptso,PTRCAST(PNetInfo)pNetInfo,
		  	  	  	  	  	  	  PTRCAST(PNodeInfo)pNodeInfo);
  if(err < 0) {
    char msg[512];
    sprintf(msg,"Error retrieving sim results in readSimulationResults. TSO_ERROR=%d",err);
    ANL_UTIL_LogSevere(env,"teva",msg);
    sprintf(msg," err=%s",strerror(errno));
    ANL_UTIL_LogSevere(env,"teva",msg);
    return 0;
  } else {
    hasData=err==1;
    if(hasData) {
      PTSO tso=PTRCAST(PTSO)ptso;
      x_populateNodeInfoAndSources(env,reader,PTRCAST(PNetInfo)pNetInfo,PTRCAST(PNodeInfo)pNodeInfo,tso->index[tso->curIndex]->sourceData, jNodeInfo);
    }
    return hasData;
  }
}

/*
 * Class:     anl_msv_dpf_erd_db_TSOReader
 * Method:    getSimulationResults
 * Signature: (IIII[[F)V
 */
JNIEXPORT jboolean JNICALL Java_anl_msv_dpf_erd_db_TSOReader_getSimulationResults
  (JNIEnv *env, jobject reader, jint id, jlong ptso, jlong pNetInfo, jlong pNodeInfo, jlong pSource, jobjectArray jNodeInfo)
{
	PTSO tso = PTRCAST(PTSO)ptso;
	jboolean hasData;
	int err = TSO_GetSimulationResults(id,tso,PTRCAST(PNetInfo)pNetInfo,PTRCAST(PNodeInfo)pNodeInfo);
	if(err < 0) {
	  char msg[512];
	  sprintf(msg,"Error retrieving sim results for id=%d. TSO_ERROR=%d",id,err);
	  ANL_UTIL_LogSevere(env,"teva",msg);
	  sprintf(msg," err=%s",strerror(errno));
	  ANL_UTIL_LogSevere(env,"teva",msg);
	} else {
	  hasData=err==1;
	  if(hasData) {
	    x_populateNodeInfoAndSources(env,reader,PTRCAST(PNetInfo)pNetInfo,PTRCAST(PNodeInfo)pNodeInfo,tso->index[tso->curIndex]->sourceData, jNodeInfo);
	  }
	}
	return hasData;
}

/*
 * Class:     anl_msv_dpf_erd_db_TSOReader
 * Method:    getRawSimulationResults
 * Signature: (IIII[[F)V
 */
JNIEXPORT jboolean JNICALL Java_anl_msv_dpf_erd_db_TSOReader_getRawSimulationResults
  (JNIEnv *env, jobject reader, jint id, jlong ptso, jlong pNetInfo, jlong pNodeInfo, jlong junkSource, jobjectArray jNodeInfo, jbyteArray jbuf)
{
	PTSO tso = PTRCAST(PTSO)ptso;
	jboolean hasData;
	jbyte* rawData;
	jboolean isCopy;
	int bLen;
	int err;
	char msg[512];

	bLen = (*env)->GetArrayLength(env,jbuf);
	rawData = (*env)->GetPrimitiveArrayCritical(env,jbuf,&isCopy);
	err = TSO_GetRawSimulationResults(id,tso,PTRCAST(PNetInfo)pNetInfo,PTRCAST(PNodeInfo)pNodeInfo,rawData,bLen);
	(*env)->ReleasePrimitiveArrayCritical(env,jbuf,rawData,0);
	if(err < 0) {
	  sprintf(msg,"Error retrieving raw sim results for id=%d. TSO_ERROR=%d",id,err);
	  ANL_UTIL_LogSevere(env,"teva",msg);
	  if(errno) {
	    sprintf(msg," err=%s",strerror(errno));
	    ANL_UTIL_LogSevere(env,"teva",msg);
	  }
	} else {
	  hasData=err==1;
	  if(hasData) {
		PSourceData pSource=tso->index[tso->curIndex]->sourceData;
	    jobjectArray sources = x_createSourcesArray(env, pSource->nsources, pSource->source);
	    (*env)->SetObjectField(env,reader,x_tsoData->readerSources,sources);
	  }
	}
	return hasData;
}

/*
 * Class:     anl_msv_dpf_erd_db_TSOReader
 * Method:    getIndex
 * Signature: (JJ)[Lanl/msv/dpf/erd/db/DataIndex
 */
JNIEXPORT jobjectArray JNICALL Java_anl_msv_dpf_erd_db_TSOReader_getIndex(
         JNIEnv *env, jobject reader, jlong ptso, jlong pNodeInfo)
{
	jobjectArray index;
	int i;
	PTSO tso = PTRCAST(PTSO)ptso;
	x_ensureDataInitialized(env);

	index = (*env)->NewObjectArray(env,tso->count,x_tsoData->dataIndexClass,0);

	for(i=0;i<tso->count;i++) {
		PDataIndex di = tso->index[i];
		jobjectArray sources;
		jobject dataIndex;

		sources = x_createSourcesArray(env, di->sourceData->nsources, di->sourceData->source);
		dataIndex = (*env)->NewObject(env,x_tsoData->dataIndexClass,x_tsoData->dataIndexConstructor,
			di->id,di->fileIndex,di->fileOffset,di->dataLength,sources);
		(*env)->SetObjectArrayElement(env,index,i,dataIndex);
	}
	return index;
}

//  private methods...
static void  x_ensureDataInitialized(JNIEnv *env)
{
	if(x_tsoData == NULL)
		x_initTSOData(env);
}
static jclass x_findClass(JNIEnv *env, char *className, char *dottedClassName)
{
	jclass tclass;
	tclass = (*env)->FindClass(env,className);
	if(ANL_UTIL_CheckForException(env, "unable to find class %s\n",dottedClassName))
		return NULL;
	return (*env)->NewGlobalRef(env,tclass);
}
static jfieldID x_findField(JNIEnv *env,jclass cls, char *fieldName, char *sig, char *className)
{
	jfieldID fldID = (*env)->GetFieldID(env,cls,fieldName,sig);
	if(ANL_UTIL_CheckForException(env, "unable to find field %s with signature %s in class %s\n",fieldName,sig,className))
		return 0;
	return fldID;
}

static jboolean x_initTSOData(JNIEnv *env)
{
	char *args;
	if(x_tsoData != NULL)
		return TRUE;
	x_tsoData = (TSODataPtr)calloc(1,sizeof(TSOData));
	if((x_tsoData->readerClass = x_findClass(env,"anl/msv/dpf/erd/db/TSOReader","anl.msv.dpf.erd.db.TSOReader"))==NULL)
		return FALSE;
	if((x_tsoData->netInfoClass = x_findClass(env,"anl/msv/dpf/erd/db/TSONetInfo","anl.msv.dpf.erd.db.TSONetInfo"))==NULL)
		return FALSE;
	if((x_tsoData->nodeInfoClass = x_findClass(env,"anl/msv/dpf/erd/db/TSONodeInfo","anl.msv.dpf.erd.db.TSONodeInfo"))==NULL)
		return FALSE;
	if((x_tsoData->linkInfoClass = x_findClass(env,"anl/msv/dpf/erd/db/TSOLinkInfo","anl.msv.dpf.erd.db.TSOLinkInfo"))==NULL)
		return FALSE;
	if((x_tsoData->sourceClass = x_findClass(env,"anl/msv/dpf/erd/db/TSOSource","anl.msv.dpf.erd.db.TSOSource"))==NULL)
		return FALSE;
	if((x_tsoData->dataIndexClass = x_findClass(env,"anl/msv/dpf/erd/db/TSODataIndex","anl.msv.dpf.erd.db.TSODataIndex"))==NULL)
		return FALSE;
	if((x_tsoData->compressedTSOClass = x_findClass(env,"anl/msv/dpf/erd/db/CompressedTSOData","anl.msv.dpf.erd.db.CompressedTSOData"))==NULL)
		return FALSE;
	if((x_tsoData->stringClass = x_findClass(env,"java/lang/String","java.lang.String"))==NULL)
		return FALSE;
	if((x_tsoData->floatArrayClass = x_findClass(env,"[F","float[]"))==NULL)
		return FALSE;



	args = "(IIIIIIII[Ljava/lang/String;FIIIIF)V";
	x_tsoData->netInfoConstructor = (*env)->GetMethodID(env,x_tsoData->netInfoClass,"<init>",args);
	if(ANL_UTIL_CheckForException(env, "unable to find constructor for TSONetInfo\n"))
		return FALSE;

	args = "(IFF[F[F[[F[ILjava/lang/String;)V";
	x_tsoData->nodeInfoConstructor = (*env)->GetMethodID(env,x_tsoData->nodeInfoClass,"<init>",args);
	if(ANL_UTIL_CheckForException(env, "unable to find constructor for TSONodeInfo\n"))
		return FALSE;

	args = "(IIF[F[FLjava/lang/String;I[F[F)V";
	x_tsoData->linkInfoConstructor = (*env)->GetMethodID(env,x_tsoData->linkInfoClass,"<init>",args);
	if(ANL_UTIL_CheckForException(env, "unable to find constructor for TSOLinkInfo\n"))
		return FALSE;

	args = "(Ljava/lang/String;IIIIF)V";
	x_tsoData->sourceConstructor = (*env)->GetMethodID(env,x_tsoData->sourceClass,"<init>",args);
	if(ANL_UTIL_CheckForException(env, "unable to find constructor for TSOSource\n"))
		return FALSE;

	args = "(IIJI[Lanl/msv/dpf/erd/db/TSOSource;)V";
	x_tsoData->dataIndexConstructor = (*env)->GetMethodID(env,x_tsoData->dataIndexClass,"<init>",args);
	if(ANL_UTIL_CheckForException(env, "unable to find constructor for TSODataIndex\n"))
		return FALSE;

	args = "(I[BII)V";
	x_tsoData->compressedTSOConstructor= (*env)->GetMethodID(env,x_tsoData->compressedTSOClass,"<init>",args);
	if(ANL_UTIL_CheckForException(env, "unable to find constructor for TSODataIndex\n"))
		return FALSE;

	if((x_tsoData->readerSources = x_findField(env,x_tsoData->readerClass,"sources","[Lanl/msv/dpf/erd/db/TSOSource;","anl.msv.dpf.erd.db.TSOReader"))==NULL)
		return FALSE;

	if((x_tsoData->nodeInfoQ = x_findField(env,x_tsoData->nodeInfoClass,"q","[F","anl.msv.dpf.erd.db.TSONodeInfo"))==NULL)
		return FALSE;
	if((x_tsoData->nodeInfoC = x_findField(env,x_tsoData->nodeInfoClass,"c","[[F","anl.msv.dpf.erd.db.TSONodeInfo"))==NULL)
		return FALSE;
	if((x_tsoData->nodeInfoNZ = x_findField(env,x_tsoData->nodeInfoClass,"nz","[I","anl.msv.dpf.erd.db.TSONodeInfo"))==NULL)
		return FALSE;

	if((x_tsoData->linkInfoVelocity = x_findField(env,x_tsoData->linkInfoClass,"velocity","[F","anl.msv.dpf.erd.db.TSOLinkeInfo"))==NULL)
		return FALSE;
	if((x_tsoData->linkInfoFlow = x_findField(env,x_tsoData->linkInfoClass,"flow","[F","anl.msv.dpf.erd.db.TSOLinkeInfo"))==NULL)
		return FALSE;
	if((x_tsoData->linkInfoVX = x_findField(env,x_tsoData->linkInfoClass,"vx","[F","anl.msv.dpf.erd.db.TSOLinkeInfo"))==NULL)
		return FALSE;
	if((x_tsoData->linkInfoVY = x_findField(env,x_tsoData->linkInfoClass,"vy","[F","anl.msv.dpf.erd.db.TSOLinkeInfo"))==NULL)
		return FALSE;

	return TRUE;
}

static jobject x_createSourcesArray(JNIEnv *env, int nsources, PSource pSource)
{
	jobjectArray sources;
	int s;
	sources = (*env)->NewObjectArray(env,nsources,x_tsoData->sourceClass,0);
	for(s=0;s<nsources;s++) {
		PSource srcp = &pSource[s];
		jstring nodeID = (*env)->NewStringUTF(env,srcp->sourceNodeID);
		jobject source = (*env)->NewObject(env,x_tsoData->sourceClass,x_tsoData->sourceConstructor,
			nodeID, srcp->sourceNodeIndex, srcp->sourceType,
			srcp->sourceStart, srcp->sourceStop, srcp->sourceStrength);
		(*env)->SetObjectArrayElement(env,sources,s,source);
	}
	return sources;
}
static jobject x_CreateNetInfoObject(JNIEnv *env, PNetInfo pNetInfo) 
{
	jobject rv;
	jobjectArray species=NULL;
	int i;

	x_ensureDataInitialized(env);

	species = (*env)->NewObjectArray(env,pNetInfo->numSpecies,x_tsoData->stringClass,0);

	for(i=0;i<pNetInfo->numSpecies;i++) {
		const char *str = pNetInfo->species[i]->id;
		jobject jstr = (*env)->NewStringUTF(env,str);
		(*env)->SetObjectArrayElement(env,species,0,jstr);
	}
	rv = (*env)->NewObject(env,x_tsoData->netInfoClass,x_tsoData->netInfoConstructor,
		(jint)pNetInfo,pNetInfo->numNodes, pNetInfo->numLinks, pNetInfo->numTanks,
		pNetInfo->numJunctions,
		pNetInfo->numSteps, pNetInfo->qualCode, pNetInfo->numSpecies,
		species, pNetInfo->stepSize, pNetInfo->simDuration,
			       pNetInfo->reportStart, pNetInfo->reportStep, pNetInfo->simStart, pNetInfo->fltMax);
	return rv;
}
static jobjectArray x_CreateNodeInfoArray(JNIEnv *env,PNetInfo pNetInfo,PNodeInfo pNodeInfo)
{
	jobjectArray rv;
	int i,nspecies,nsteps,nnodes;
	x_ensureDataInitialized(env);
	nspecies = pNetInfo->numSpecies;
	nnodes = pNetInfo->numNodes;
	nsteps = pNetInfo->numSteps;
	rv = (*env)->NewObjectArray(env,pNetInfo->numNodes,x_tsoData->nodeInfoClass,NULL);
	for(i=0;i<nnodes;i++) {
		jobject nodeInfo=NULL;
		nodeInfo = x_CreateNodeInfoObject(env,&pNodeInfo[i],pNetInfo, i, nspecies,nsteps);
		(*env)->SetObjectArrayElement(env,rv,i,nodeInfo);
	}
	return rv;
}

static jobject x_CreateNodeInfoObject(JNIEnv *env,PNodeInfo pNodeInfo, PNetInfo pNetInfo, int nodeidx, int nspecies, int nsteps)
{
	jfloatArray q=NULL;
	jfloatArray dp=NULL;
	jintArray nz=NULL;
	jobjectArray c=NULL;
	jstring id=NULL;
	int i;
	float *qelems=NULL;
	float *dpelems=NULL;

	// this will be null if the READ_DEMANDS flag wasn't set.
	if(pNetInfo->hydResults->demand != NULL) {
		float **dem=pNetInfo->hydResults->demand;
		q = (*env)->NewFloatArray(env,nsteps);
		qelems = (*env)->GetFloatArrayElements(env,q,NULL);
		for(i=0;i<nsteps;i++) {
			qelems[i]=dem[i][nodeidx];
		}
		(*env)->ReleaseFloatArrayElements(env,q,qelems,0);
	}
	dp = (*env)->NewFloatArray(env,pNodeInfo->demandProfileLength);
	dpelems = (*env)->GetFloatArrayElements(env,dp,NULL);
	memcpy(dpelems,pNodeInfo->demandProfile,sizeof(float)*pNodeInfo->demandProfileLength);
	(*env)->ReleaseFloatArrayElements(env,dp,dpelems,0);

	// this will be null if the READ_QUALITY flag wasn't set.
	if(pNetInfo->qualResults->nodeC != NULL) {
		c = (*env)->NewObjectArray(env,nspecies,x_tsoData->floatArrayClass,NULL);
		for(i=0;i<nspecies;i++) {
			jfloatArray cd = (*env)->NewFloatArray(env,nsteps);
			(*env)->SetObjectArrayElement(env,c,i,cd);
		}
	}
	nz = (*env)->NewIntArray(env,nspecies);
	id = (*env)->NewStringUTF(env,pNodeInfo->id);
	return (*env)->NewObject(env,x_tsoData->nodeInfoClass,x_tsoData->nodeInfoConstructor,
		pNodeInfo->type,pNodeInfo->x,pNodeInfo->y,dp,q,c,nz,id);
}

static jobjectArray x_CreateLinkInfoArray(JNIEnv *env,PNetInfo pNetInfo,PLinkInfo pLinkInfo)
{
	jobjectArray rv;
	int i,nsteps,nlinks;
	x_ensureDataInitialized(env);
	nlinks = pNetInfo->numLinks;
	nsteps = pNetInfo->numSteps;
	rv = (*env)->NewObjectArray(env,pNetInfo->numLinks,x_tsoData->linkInfoClass,NULL);
	for(i=0;i<nlinks;i++) {
		jobject linkInfo = x_CreateLinkInfoObject(env,&pLinkInfo[i], pNetInfo, i,nsteps);
		(*env)->SetObjectArrayElement(env,rv,i,linkInfo);
	}
	return rv;
}
static jobject x_CreateLinkInfoObject(JNIEnv *env,PLinkInfo pLinkInfo, PNetInfo pNetInfo, int linkidx, int nsteps)
{
	jfloatArray v=NULL,
			f=NULL,vx,vy;
	float *fElems=NULL;
	jstring id;
	int i;

	// this will be null if the READ_LINKVEL flag wasn't set.
	if(pNetInfo->hydResults->velocity != NULL) {
		float **vel=pNetInfo->hydResults->velocity;
		v = (*env)->NewFloatArray(env,nsteps);
		fElems = (*env)->GetFloatArrayElements(env,v,NULL);
		for(i=0;i<nsteps;i++) {
			fElems[i]=vel[i][linkidx];
		}
		(*env)->ReleaseFloatArrayElements(env,v,fElems,0);
	}

	// this will be null if the READ_LINKFLOW flag wasn't set.
	if(pNetInfo->hydResults->flow != NULL) {
		float **flow=pNetInfo->hydResults->flow;
		f = (*env)->NewFloatArray(env,nsteps);
		fElems = (*env)->GetFloatArrayElements(env,f,NULL);
		for(i=0;i<nsteps;i++) {
			fElems[i]=flow[i][linkidx];
		}
		(*env)->ReleaseFloatArrayElements(env,f,fElems,0);
	}
	vx = (*env)->NewFloatArray(env,pLinkInfo->nv);
	fElems = (*env)->GetFloatArrayElements(env,vx,NULL);
	memcpy(fElems,pLinkInfo->vx,sizeof(float)*pLinkInfo->nv);
	(*env)->ReleaseFloatArrayElements(env,vx,fElems,0);

	vy = (*env)->NewFloatArray(env,pLinkInfo->nv);
	fElems = (*env)->GetFloatArrayElements(env,vy,NULL);
	memcpy(fElems,pLinkInfo->vy,sizeof(float)*pLinkInfo->nv);
	(*env)->ReleaseFloatArrayElements(env,vy,fElems,0);

	id = (*env)->NewStringUTF(env,pLinkInfo->id);
	
	return (*env)->NewObject(env,x_tsoData->linkInfoClass,x_tsoData->linkInfoConstructor,
		pLinkInfo->from, pLinkInfo->to, pLinkInfo->length, v, f, id, pLinkInfo->nv, vx, vy);
}

static void x_populateNodeInfoAndSources(JNIEnv *env, jobject reader,
										 PNetInfo pNetInfo, PNodeInfo pNodeInfo,
										 PSourceData pSource, jobjectArray jNodeInfo)
{
	int s,n,nsteps;
	jobjectArray sources;
	
	sources = x_createSourcesArray(env, pSource->nsources, pSource->source);
	(*env)->SetObjectField(env,reader,x_tsoData->readerSources,sources);

	if(pNetInfo->qualResults->nodeC==NULL)
		return;
	nsteps = pNetInfo->numSteps;
	for(n=0;n<pNetInfo->numNodes;n++) {
		float **concData;  // node, time
		jobject nodeInfo;
		jobjectArray cc;

		nodeInfo = (*env)->GetObjectArrayElement(env,jNodeInfo,n);
		cc = (*env)->GetObjectField(env,nodeInfo,x_tsoData->nodeInfoC);
		for(s=0;s<pNetInfo->numSpecies;s++) {
			jfloatArray c;
			float *fElems;

			concData=pNetInfo->qualResults->nodeC[s];
			c = (*env)->GetObjectArrayElement(env,cc,s);
			fElems = (*env)->GetFloatArrayElements(env,c,NULL);
			memcpy(fElems,concData[n],sizeof(float)*nsteps);
			(*env)->ReleaseFloatArrayElements(env,c,fElems,0);
		}
	}
}
