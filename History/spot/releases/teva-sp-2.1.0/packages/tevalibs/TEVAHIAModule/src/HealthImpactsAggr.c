#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <float.h>
#include <sys/stat.h>
#include "loggingUtils.h"
#include "HealthImpacts.h"
#include "ExternalAnalysis.h"
#include "NamedData.h"


static PMem x_mem;
static int x_scenarioCount;
static PXA x_xa;
static PHIST x_prevHist;
static ConcentrationDataPtr x_worstScenarios;
static double x_nthWorstScenario;
static double x_worstScenario;

static int x_NumWorstToKeep;
static int x_numWorst;

static PXA x_worstIDF;
static float *maxConcentration;
static float *maxDosage;
static float *totalInfected;
static float *totalFatalities;
static int *highFatalityNodes;
static int *numNodesWithFatalities;
static int x_hasMultipleInjections;

static DiskCachedDataPtr x_Fatalities;
static DiskCachedDataPtr x_sidfData;

static JNIEnv *x_env;


static int x_nCalls;
static int x_nRetrievals;

int sortFloatAsc(const void *a, const void *b);
int sortIntAsc(const void *a, const void *b);
int sortCachedDataAsc(const void *a, const void *b);

void computeSummary(float *scenData, int numScen, int *percentiles, float *rv, int numPercentiles);
void savePercentiles(NamedDataRef *resultsRef, DiskCachedDataPtr fatalities, DiskCachedDataPtr sidf, int* percentiles, char *percentileDescs[], int numPercentiles);

static int x_initFloatArray(void *ref, char *varName, float *data, int len);
static int x_initIntArray(void *ref, char *varName, int *data, int len);
float **getNodeConcentrations(void *ref, char *name,int nsteps,int nnodes);
void freeConcentrationData(float **conc, int nsteps);

void freeDiskCachedData(JNIEnv *env,DiskCachedDataPtr *dcd);
DiskCachedDataPtr initDiskCachedData(char *fn, int nelems);
void cacheData(JNIEnv *env,DiskCachedDataPtr dcd, float val, char *simID, void *data, int size);
void getCachedData(DiskCachedDataPtr dcd, int i,void *data, int size);

static void x_xaToFloatArray(PXA xa,int nsteps,float *xa_data);
static void x_floatArrayToXA(PXA xa,int nsteps,float *xa_data);


void writeOKCFile(NamedDataRef *analysisOptionsRef,NamedDataRef *resultsRef, const char *dir, char *fn);
void writeAllIDFData(const char *dir, char *fn, DiskCachedDataPtr sidf,int nsteps) ;

/* 
 * called once after the first simulaiton has been completed (so demands are there)
 */
void HIAnalysis_aggr_initialize(NamedDataRef *analysisOptionsRef, NamedDataRef *simResultsRef)
{
	// at this point, simResultsRef does not conatin any quality, but will contain
	// demands however
	int nnodes;
	int nsteps;
	x_mem = loadHIAOptions(analysisOptionsRef, simResultsRef,Aggregation);
	Population(x_mem->net,x_mem->dr,x_mem->node);

	dumpHIAInputParameters(x_mem);
	if(getInt(analysisOptionsRef, "NumWorstCaseToKeep", &x_NumWorstToKeep)==ND_FAILURE) return;
	nnodes = x_mem->net->info->nnodes;
	nsteps = x_mem->net->info->nsteps+x_mem->dr->nL+x_mem->dr->nF;
	x_scenarioCount=0;
	x_xa = (PXA)calloc(1,sizeof(XA));
	x_worstIDF = (PXA)calloc(1,sizeof(XA));
	x_prevHist = NULL;
	x_worstScenarios=NULL;
	x_nthWorstScenario=-1;
	x_worstScenario=-1;
	x_numWorst=0;
	initXA(x_xa,nsteps);
	initXA(x_worstIDF,nsteps);

	maxConcentration=NULL;
	maxDosage=NULL;
	totalInfected=NULL;
	totalFatalities=NULL;
	highFatalityNodes=NULL;
	numNodesWithFatalities=NULL;

	x_nCalls=0;
	x_nRetrievals=0;

	x_Fatalities = initDiskCachedData("fatalities.cache",x_mem->net->info->nnodes);
	x_sidfData = initDiskCachedData("sidf.cache",nsteps);

}

DiskCachedDataPtr initDiskCachedData(char *fn, int nelems)
{
	DiskCachedDataPtr p = (DiskCachedDataPtr)calloc(1,sizeof(DiskCachedData));
	strcpy(p->fn,fn);
	p->fp=fopen(fn,"w+b");
	p->num=0;
	p->size=nelems;
	return p;
}

void freeDiskCachedData(JNIEnv *env,DiskCachedDataPtr *dcd)
{
	char logMsg[1024];
	struct stat stats;
	int i;

	fstat(fileno((*dcd)->fp),&stats);
	fclose((*dcd)->fp);

	sprintf(logMsg,"Cache file: %s size: %ld\n",(*dcd)->fn,stats.st_size);
	ANL_UTIL_LogFine(env,"teva.analysis.server",logMsg);
	unlink((*dcd)->fn);
	for(i=0;i<(*dcd)->num;i++) {
		free((*dcd)->data[i]);
	}
	free((*dcd)->data);
	free(*dcd);
	*dcd=NULL;
}

void cacheData(JNIEnv *env, DiskCachedDataPtr dcd, float val, char *simID, void *data, int size)
{
	char logMsg[1024];
	dcd->data=(CachedDataPtr*)realloc(dcd->data,(dcd->num+1)*sizeof(CachedDataPtr));
	dcd->data[dcd->num] = (CachedDataPtr)calloc(1,sizeof(CachedData));
	dcd->data[dcd->num]->val = val;
	dcd->data[dcd->num]->simID = simID;
	dcd->data[dcd->num]->fileOffset = ftell(dcd->fp);
	dcd->num++;
	fwrite(data,size,1,dcd->fp);
	fflush(dcd->fp);

	sprintf(logMsg,"wrote %ld bytes so far to the cache file",ftell(dcd->fp));
	ANL_UTIL_LogFine(env,"teva.analysis.server",logMsg);
}

void getCachedData(DiskCachedDataPtr dcd, int i,void *data, int size)
{
	long curFilePos = ftell(dcd->fp);
	fseek(dcd->fp,dcd->data[i]->fileOffset,SEEK_SET);
	fread(data,size,1,dcd->fp);
	fseek(dcd->fp,curFilePos,SEEK_SET);
}

void cdpFree(const void *data)
{
	int i;
	ConcentrationDataPtr cdp = (ConcentrationDataPtr)data;
	for(i=0;i<cdp->nsteps;i++) {
		free(cdp->conc[i]);
	}
	free(cdp->simID);
	free(cdp->conc);
	freeXA(&cdp->na,x_mem->net->info->nnodes);
	free(cdp);
}

void dumpList(ConcentrationDataPtr head) {
	char msg[256];
	ConcentrationDataPtr t;
	for(t=head;t != NULL;t=t->next) {
		sprintf(msg,"%08x: %25.8lf",t,t->rankValue);
		ANL_UTIL_LogSevere(x_env,"anl.teva",msg);
	}
}
ConcentrationDataPtr removeLastElement(ConcentrationDataPtr* head) {
	ConcentrationDataPtr t=*head,p=NULL;
	while(t->next != NULL) { p = t; t=t->next; }
	if(p!=NULL) {
		p->next = NULL;
	} else {
		*head = NULL;
	}
	return t;
}
ConcentrationDataPtr getLastElement(ConcentrationDataPtr head) {
	ConcentrationDataPtr t=head,p=NULL;
	while(t->next != NULL) t=t->next;
	return t;
}
void freeCDPData(ConcentrationDataPtr *head) {
	ConcentrationDataPtr t=*head;
	while(t != NULL) {
		ConcentrationDataPtr p=t;
		t=t->next;
		cdpFree(p);
	}
}
void insertList(ConcentrationDataPtr data, ConcentrationDataPtr *head) {
	ConcentrationDataPtr t,p;
	t=*head;
	p=NULL;
	while(t != NULL && data->rankValue < t->rankValue) {
		p=t;
		t=t->next;
	}
	if(p!=NULL) {
		p->next = data;
	} else {
		*head = data;
	}
	data->next = t;
}
void setResults(ConcentrationDataPtr head, void *resultsRef) 
{
	ConcentrationDataPtr cdp;
	int i;
	char name[256];

	for(cdp=head,i=0;cdp != NULL; cdp=cdp->next,i++) {

		sprintf(name,"SimID[%02d]",i);
		if(addVariable(resultsRef,name, STRING_TYPE) ==ND_FAILURE) return;
		if(setString(resultsRef,name, cdp->simID) ==ND_FAILURE) return;

		sprintf(name,"RankValue[%02d]",i);
		if(addVariable(resultsRef,name, DOUBLE_TYPE) ==ND_FAILURE) return;
		if(setDouble(resultsRef,name, cdp->rankValue) ==ND_FAILURE) return;

		sprintf(name,"NodeConcentration[%02d]",i);
		if(addVariable(resultsRef,name,     FLOAT_ARRAY_2D_TYPE) ==ND_FAILURE) return;
		if(set2DFloatArray(resultsRef,name, cdp->conc,cdp->nsteps, cdp->nnodes) ==ND_FAILURE) return;

		sprintf(name,"ReceptorDosage[%02d]",i);
		if(addVariable(resultsRef,name,     FLOAT_ARRAY_TYPE) ==ND_FAILURE) return;
		if(setFloatArray(resultsRef,name, cdp->na.dos, cdp->nnodes) ==ND_FAILURE) return;
	
		sprintf(name,"ReceptorResponse[%02d]",i);
		if(addVariable(resultsRef,name,     FLOAT_ARRAY_TYPE) ==ND_FAILURE) return;
		if(setFloatArray(resultsRef,name, cdp->na.res, cdp->nnodes) ==ND_FAILURE) return;
		
		sprintf(name,"ReceptorS[%02d]",i);
		if(addVariable(resultsRef,name,     FLOAT_ARRAY_TYPE) ==ND_FAILURE) return;
		if(setFloatArray(resultsRef,name, cdp->na.s, cdp->nnodes) ==ND_FAILURE) return;
		
		sprintf(name,"ReceptorI[%02d]",i);
		if(addVariable(resultsRef,name,     FLOAT_ARRAY_TYPE) ==ND_FAILURE) return;
		if(setFloatArray(resultsRef,name, cdp->na.i, cdp->nnodes) ==ND_FAILURE) return;
		
		sprintf(name,"ReceptorD[%02d]",i);
		if(addVariable(resultsRef,name,     FLOAT_ARRAY_TYPE) ==ND_FAILURE) return;
		if(setFloatArray(resultsRef,name, cdp->na.d, cdp->nnodes) ==ND_FAILURE) return;
		
		sprintf(name,"ReceptorF[%02d]",i);
		if(addVariable(resultsRef,name,     FLOAT_ARRAY_TYPE) ==ND_FAILURE) return;
		if(setFloatArray(resultsRef,name, cdp->na.f, cdp->nnodes) ==ND_FAILURE) return;
	
	}
}
/* 
 * called once after every simulation with the results of the intermediate analysis
 */
void HIAnalysis_aggr_newResults(NamedDataRef *analysisOptionsRef,NamedDataRef *intResultsRef)
{
	int i;
	int nnodes = x_mem->net->info->nnodes;
	int nsteps = x_mem->dr->maxsteps;
	int numModelTimeSteps = x_mem->net->info->nsteps;
	char *tstr;
	double rank;
	float *tdata;
	PHIST hist = (PHIST)calloc(1,sizeof(HIST));
	JNIEnv *env = analysisOptionsRef->env;
	int numInj;
	x_env=env;

	if(getInt(intResultsRef,"numInjections", &numInj)==ND_FAILURE) return;
	if(numInj>1) x_hasMultipleInjections=1;

	if(getDouble(intResultsRef,"RankValue", &rank)==ND_FAILURE) return;

	if(x_NumWorstToKeep>0) {
		char msg[256];
		x_nCalls++;
		sprintf(msg,"Rank: %lf nthWorstRank: %lf",rank,x_nthWorstScenario);
		ANL_UTIL_LogInfo(env,"teva.analysis.server",msg);
		if(rank>x_nthWorstScenario) {
			ConcentrationDataPtr cdp = (ConcentrationDataPtr)calloc(1,sizeof(ConcentrationData));

			x_nRetrievals++;
			sprintf(msg,"Retrieving concentration data: %d (total calls: %d)",x_nRetrievals,x_nCalls);
			ANL_UTIL_LogInfo(env,"teva.analysis.server",msg);

			cdp->rankValue=rank;
			cdp->nsteps = numModelTimeSteps;
			cdp->nnodes = nnodes;
			if(getString(intResultsRef,"Hist_SimID", &(tstr))==ND_FAILURE) return;
			cdp->simID=(char*)calloc(strlen(tstr)+1,sizeof(char));
			strcpy(cdp->simID,tstr);
			nd_free(tstr);
			cdp->conc = getNodeConcentrations(intResultsRef,"NodeConcentration",numModelTimeSteps,nnodes);
			initXA(&cdp->na,nnodes);
			if(getFloatArray(intResultsRef,"Dose_n",&tdata)==ND_FAILURE) return;
			memcpy(cdp->na.dos,tdata,nnodes*sizeof(float));
			nd_free(tdata);
			if(getFloatArray(intResultsRef,"Response_n",&tdata)==ND_FAILURE) return;
			memcpy(cdp->na.res,tdata,nnodes*sizeof(float));
			nd_free(tdata);
			if(getFloatArray(intResultsRef,"S_n",&tdata)==ND_FAILURE) return;
			memcpy(cdp->na.s,tdata,nnodes*sizeof(float));
			nd_free(tdata);
			if(getFloatArray(intResultsRef,"I_n",&tdata)==ND_FAILURE) return;
			memcpy(cdp->na.i,tdata,nnodes*sizeof(float));
			nd_free(tdata);
			if(getFloatArray(intResultsRef,"D_n",&tdata)==ND_FAILURE) return;
			memcpy(cdp->na.d,tdata,nnodes*sizeof(float));
			nd_free(tdata);
			if(getFloatArray(intResultsRef,"F_n",&tdata)==ND_FAILURE) return;
			memcpy(cdp->na.f,tdata,nnodes*sizeof(float));
			nd_free(tdata);
			insertList(cdp,&x_worstScenarios);
			x_numWorst++;
		}
		if(x_numWorst > x_NumWorstToKeep) {
			ConcentrationDataPtr lastCDP=NULL;
			lastCDP = removeLastElement(&x_worstScenarios);
			cdpFree(lastCDP);
			lastCDP = getLastElement(x_worstScenarios);
			x_nthWorstScenario=lastCDP->rankValue;
			x_numWorst--;
		}
	}

	if(x_prevHist == NULL)
		x_mem->hist = hist;
	else
		x_prevHist->next = hist;
	x_prevHist = hist;

	if(getFloat(intResultsRef,"Hist_testh", &(hist->testh))==ND_FAILURE) return;
	if(getFloat(intResultsRef,"Hist_F",     &(hist->fh   ))==ND_FAILURE) return;
	if(getFloat(intResultsRef,"Hist_Dos",   &(hist->dosh ))==ND_FAILURE) return;
	if(getFloat(intResultsRef,"Hist_Resp",  &(hist->resh ))==ND_FAILURE) return;
	if(getString(intResultsRef,"Hist_SimID", &(tstr))==ND_FAILURE) return;
	hist->simID=(char*)calloc(strlen(tstr)+1,sizeof(char));
	strcpy(hist->simID,tstr);
	nd_free(tstr);

	x_scenarioCount++;

	if(getFloatArray(intResultsRef,"F_n",&tdata)==ND_FAILURE) return;
	cacheData(intResultsRef->env,x_Fatalities,hist->fh,hist->simID,tdata,nnodes*sizeof(float));
	nd_free(tdata);

	x_initFloatArray(intResultsRef,"Dose_t",    x_xa->dos,nsteps);
	x_initFloatArray(intResultsRef,"Response_t",x_xa->res,nsteps);
	x_initFloatArray(intResultsRef,"S_t",       x_xa->s,  nsteps);
	x_initFloatArray(intResultsRef,"I_t",       x_xa->i,  nsteps);
	x_initFloatArray(intResultsRef,"D_t",       x_xa->d,  nsteps);
	x_initFloatArray(intResultsRef,"F_t",       x_xa->f,  nsteps);
	{
		float *xa_data=(float*)calloc(6*nsteps,sizeof(float));
		x_xaToFloatArray(x_xa,nsteps,xa_data);
		cacheData(intResultsRef->env,x_sidfData,hist->fh,hist->simID,xa_data,6*nsteps*sizeof(float));
		free(xa_data);
	}

	/* Sum state variables over all nodes and all scenarios */

	for(i=0;i<nsteps;i++) {
		x_mem->xa->dos[i] += x_xa->dos[i];
		x_mem->xa->res[i] += x_xa->res[i];
		x_mem->xa->s[i]   += x_xa->s[i];
		x_mem->xa->i[i]   += x_xa->i[i];
		x_mem->xa->d[i]   += x_xa->d[i];
		x_mem->xa->f[i]   += x_xa->f[i];
	}

	if(rank > x_worstScenario) {
		memcpy(x_worstIDF->dos,x_xa->dos,nsteps*sizeof(float));
		memcpy(x_worstIDF->res,x_xa->res,nsteps*sizeof(float));
		memcpy(x_worstIDF->s,x_xa->s,nsteps*sizeof(float));
		memcpy(x_worstIDF->i,x_xa->i,nsteps*sizeof(float));
		memcpy(x_worstIDF->d,x_xa->d,nsteps*sizeof(float));
		memcpy(x_worstIDF->f,x_xa->f,nsteps*sizeof(float));
		x_worstScenario=rank;
	}
	// resize all 'histogram' data arrays
	maxConcentration=(float *)realloc(maxConcentration,x_scenarioCount*sizeof(float));
	if(getFloat(intResultsRef,"MaxConcentration", &(maxConcentration[x_scenarioCount-1]))==ND_FAILURE) return;

	maxDosage=(float *)realloc(maxDosage,x_scenarioCount*sizeof(float));
	if(getFloat(intResultsRef,"MaxDosage", &(maxDosage[x_scenarioCount-1]))==ND_FAILURE) return;

	totalInfected=(float *)realloc(totalInfected,x_scenarioCount*sizeof(float));
	if(getFloat(intResultsRef,"TotalInfected", &(totalInfected[x_scenarioCount-1]))==ND_FAILURE) return;

	totalFatalities=(float *)realloc(totalFatalities,x_scenarioCount*sizeof(float));
	if(getFloat(intResultsRef,"TotalFatalities", &(totalFatalities[x_scenarioCount-1]))==ND_FAILURE) return;

	highFatalityNodes=(int *)realloc(highFatalityNodes,x_scenarioCount*sizeof(int));
	if(getInt(intResultsRef,"HighFatalityNodes", &(highFatalityNodes[x_scenarioCount-1]))==ND_FAILURE) return;

	numNodesWithFatalities=(int *)realloc(numNodesWithFatalities,x_scenarioCount*sizeof(int));
	if(getInt(intResultsRef,"NumNodesWithFatalities", &(numNodesWithFatalities[x_scenarioCount-1]))==ND_FAILURE) return;

	x_env=NULL;
}


/* 
 * called usually only once after the scenario is done to obtain the completed analysis results
 */
void HIAnalysis_aggr_getResults(NamedDataRef *analysisOptionsRef,NamedDataRef *resultsRef)
{
    char *sensortypes[]={"Ignored","Existing","Potential","Selected"};
	int i;
	int nnodes = x_mem->net->info->nnodes;
	int nsteps = x_mem->dr->maxsteps;
	PHIST tHist;
	float *hData,*time,*f,*d,*r,*pop;
	int percentiles[]={10, 25, 50, 75, 90, 100};
	char *rowNames[]={"10th percentile", "25th percentile", "50th percentile", "75th percentile", "90th percentile", "100th percentile", "Mean"};
	int numPercentiles=6;
	char *nodeDisp;
	char **sData,**ids,**sensorTypes;

	if(addVariable(resultsRef,"Length_t",   INT_TYPE)==ND_FAILURE) return;
	if(addVariable(resultsRef,"Length_n",   INT_TYPE)==ND_FAILURE) return;
	if(addVariable(resultsRef,"Dose_t",     FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(resultsRef,"Response_t", FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(resultsRef,"S_t",        FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(resultsRef,"I_t",        FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(resultsRef,"D_t",        FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(resultsRef,"F_t",        FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(resultsRef,"time",       FLOAT_ARRAY_TYPE)==ND_FAILURE) return;

	if(addVariable(resultsRef,"NSteps",       INT_TYPE)==ND_FAILURE) return;
	if(addVariable(resultsRef,"NNodes",       INT_TYPE)==ND_FAILURE) return;
	if(addVariable(resultsRef,"NodeIDs",      STRING_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(resultsRef,"NumScenarios", INT_TYPE)==ND_FAILURE) return;

	if(addVariable(resultsRef,"Hist_testh",     FLOAT_ARRAY_TYPE) ==ND_FAILURE) return;
	if(addVariable(resultsRef,"Hist_F",         FLOAT_ARRAY_TYPE) ==ND_FAILURE) return;
	if(addVariable(resultsRef,"Hist_Dos",       FLOAT_ARRAY_TYPE) ==ND_FAILURE) return;
	if(addVariable(resultsRef,"Hist_Resp",      FLOAT_ARRAY_TYPE) ==ND_FAILURE) return;
	if(addVariable(resultsRef,"Hist_SimID",     STRING_ARRAY_TYPE)==ND_FAILURE) return;

	if(addVariable(resultsRef,"InjNodeDisplay",  BYTE_ARRAY_TYPE)  ==ND_FAILURE) return;
	if(addVariable(resultsRef,"Map_NodeIDs",     STRING_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(resultsRef,"Map_SensorTypes", STRING_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(resultsRef,"Map_F",           FLOAT_ARRAY_TYPE) ==ND_FAILURE) return;
	if(addVariable(resultsRef,"Map_Dos",         FLOAT_ARRAY_TYPE) ==ND_FAILURE) return;
	if(addVariable(resultsRef,"Map_Resp",        FLOAT_ARRAY_TYPE) ==ND_FAILURE) return;

	if(addVariable(resultsRef,"Population",     FLOAT_ARRAY_TYPE) ==ND_FAILURE) return;

	if(addVariable(resultsRef,"MaxConcentration",               FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"MaxDosage",                      FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"TotalInfected",                  FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"TotalFatalities",                FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"HighFatalityNodes",              INT_ARRAY_TYPE   )==ND_FAILURE) return;
	if(addVariable(resultsRef,"NumNodesWithFatalities",         INT_ARRAY_TYPE   )==ND_FAILURE) return;

	if(addVariable(resultsRef,"MaxConcentrationBySource",       FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"MaxDosageBySource",              FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"TotalInfectedBySource",          FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"TotalFatalitiesBySource",        FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"HighFatalityNodesBySource",      INT_ARRAY_TYPE   )==ND_FAILURE) return;
	if(addVariable(resultsRef,"NumNodesWithFatalitiesBySource", INT_ARRAY_TYPE   )==ND_FAILURE) return;
	if(addVariable(resultsRef,"DummyXVals",                     INT_ARRAY_TYPE   )==ND_FAILURE) return;

	if(addVariable(resultsRef,"WorstScenario_Dosage",   FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"WorstScenario_Response", FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"WorstScenario_S",        FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"WorstScenario_I",        FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"WorstScenario_D",        FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"WorstScenario_F",        FLOAT_ARRAY_TYPE )==ND_FAILURE) return;

	if(addVariable(resultsRef,"RowNames",                STRING_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(resultsRef,"MaxConcentrationSummary", FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"MaxDosageSummary",        FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"TotalInfectedSummary",    FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
	if(addVariable(resultsRef,"TotalFatalitiesSummary",  FLOAT_ARRAY_TYPE )==ND_FAILURE) return;

	pop = (float*)calloc(nnodes,sizeof(float));
	for(i=0;i<nnodes;i++) {
		pop[i] = x_mem->node[i].pop;
	}
	if(setFloatArray(resultsRef,"Population",    pop,nnodes)==ND_FAILURE) return;
	free(pop);

	time = (float*)calloc(nsteps,sizeof(float));
	for(i=0;i<nsteps;i++) {
		time[i]=(float)i*x_mem->net->info->stepsize;
		x_xa->dos[i] = x_mem->xa->dos[i] / x_scenarioCount;
		x_xa->res[i] = x_mem->xa->res[i] / x_scenarioCount;
		x_xa->s[i]   = x_mem->xa->s[i]   / x_scenarioCount;
		x_xa->i[i]   = x_mem->xa->i[i]   / x_scenarioCount;
		x_xa->d[i]   = x_mem->xa->d[i]   / x_scenarioCount;
		x_xa->f[i]   = x_mem->xa->f[i]   / x_scenarioCount;
	}

	if(setInt(resultsRef,"Length_n",    nnodes)==ND_FAILURE) return;
	if(setInt(resultsRef,"Length_t",    nsteps)==ND_FAILURE) return;
	if(setFloatArray(resultsRef,"Dose_t",    x_xa->dos,nsteps)==ND_FAILURE) return;
	if(setFloatArray(resultsRef,"Response_t",x_xa->res,nsteps)==ND_FAILURE) return;
	if(setFloatArray(resultsRef,"S_t",       x_xa->s,  nsteps)==ND_FAILURE) return;
	if(setFloatArray(resultsRef,"I_t",       x_xa->i,  nsteps)==ND_FAILURE) return;
	if(setFloatArray(resultsRef,"D_t",       x_xa->d,  nsteps)==ND_FAILURE) return;
	if(setFloatArray(resultsRef,"F_t",       x_xa->f,  nsteps)==ND_FAILURE) return;
	if(setFloatArray(resultsRef,"time",      time,     nsteps)==ND_FAILURE) return;

	free(time);
	if(setInt(resultsRef,"NSteps",      x_mem->net->info->nsteps)==ND_FAILURE) return;
	if(setInt(resultsRef,"NNodes",      x_mem->net->info->nnodes)==ND_FAILURE) return;
	if(setInt(resultsRef,"NumScenarios",x_scenarioCount)==ND_FAILURE) return;

	nodeDisp = (char*)calloc(x_mem->net->info->nnodes,sizeof(char));
	f = (float*)calloc(x_mem->net->info->nnodes,sizeof(float));
	d = (float*)calloc(x_mem->net->info->nnodes,sizeof(float));
	r = (float*)calloc(x_mem->net->info->nnodes,sizeof(float));
	ids = (char **)calloc(x_mem->net->info->nnodes,sizeof(char*));
	sensorTypes = (char **)calloc(nnodes,sizeof(char *));
	for(i=0;i<nnodes;i++) {
		char *st = sensortypes[x_mem->node[i].sensor.type];
//		printf("%d\n",i);
//		if (i>=200)
//			printf("");
		ids[i]=x_mem->node[i].info->id;
		sensorTypes[i] = st;
	}
	if(setStringArray(resultsRef,"NodeIDs",ids,nnodes)==ND_FAILURE) return;
	if(setStringArray(resultsRef,"Map_SensorTypes",sensorTypes,nnodes)==ND_FAILURE) return;
	// do nt free individual elements because they were never allocate - they just point to the constant
	free(sensorTypes);

	for(tHist=x_mem->hist;tHist != NULL;tHist=tHist->next) {
		int idx = getnodeindex(x_mem->net,x_mem->node,tHist->simID);
		if(idx > -1) {
			nodeDisp[idx] = 1;
			f[idx]=tHist->fh;
			d[idx]=tHist->dosh;
			r[idx]=tHist->resh;
		}
	}
	if(setStringArray(resultsRef,"Map_NodeIDs",ids,  x_mem->net->info->nnodes)==ND_FAILURE) return;
	free(ids);
	if(!x_hasMultipleInjections) {
		if(setByteArray(resultsRef,"InjNodeDisplay", nodeDisp,  x_mem->net->info->nnodes)==ND_FAILURE) return;
	}
	free(nodeDisp);
	if(setFloatArray(resultsRef,"Map_F",     f,  x_mem->net->info->nnodes)==ND_FAILURE) return;
	free(f);
	if(setFloatArray(resultsRef,"Map_Dos",   d,  x_mem->net->info->nnodes)==ND_FAILURE) return;
	free(d);
	if(setFloatArray(resultsRef,"Map_Resp",  r,  x_mem->net->info->nnodes)==ND_FAILURE) return;
	free(r);

	hData = (float*)calloc(x_scenarioCount,sizeof(float));
	for(i=0,tHist=x_mem->hist;tHist != NULL;i++,tHist=tHist->next) hData[i] = tHist->fh;
	if(setFloatArray(resultsRef,"Hist_F",     hData,  x_scenarioCount)==ND_FAILURE) return;
	for(i=0,tHist=x_mem->hist;tHist != NULL;i++,tHist=tHist->next) hData[i] = tHist->dosh;
	if(setFloatArray(resultsRef,"Hist_Dos",   hData,  x_scenarioCount)==ND_FAILURE) return;
	for(i=0,tHist=x_mem->hist;tHist != NULL;i++,tHist=tHist->next) hData[i] = tHist->resh;
	if(setFloatArray(resultsRef,"Hist_Resp",     hData,  x_scenarioCount)==ND_FAILURE) return;
	for(i=0,tHist=x_mem->hist;tHist != NULL;i++,tHist=tHist->next) hData[i] = tHist->testh;
	if(setFloatArray(resultsRef,"Hist_testh", hData,  x_scenarioCount)==ND_FAILURE) return;
	free(hData);

	sData = (char **)calloc(x_scenarioCount,sizeof(char *));
	for(i=0,tHist=x_mem->hist;tHist != NULL;i++,tHist=tHist->next) sData[i] = tHist->simID;
	if(setStringArray(resultsRef,"Hist_SimID", sData,  x_scenarioCount)==ND_FAILURE) return;
	free(sData);


	if(x_NumWorstToKeep > 0) {
		if(addVariable(resultsRef,"NumWorstScenarios", INT_TYPE) ==ND_FAILURE) return;
		if(setInt(resultsRef,"NumWorstScenarios", x_NumWorstToKeep) ==ND_FAILURE) return;
		setResults(x_worstScenarios, resultsRef);

	}
	if(setFloatArray(resultsRef,"MaxConcentrationBySource", maxConcentration,  x_scenarioCount)==ND_FAILURE) return;
	if(setFloatArray(resultsRef,"MaxDosageBySource", maxDosage,  x_scenarioCount)==ND_FAILURE) return;
	if(setFloatArray(resultsRef,"TotalInfectedBySource", totalInfected,  x_scenarioCount)==ND_FAILURE) return;
	if(setFloatArray(resultsRef,"TotalFatalitiesBySource", totalFatalities,  x_scenarioCount)==ND_FAILURE) return;
	if(setIntArray(resultsRef,"HighFatalityNodesBySource", highFatalityNodes,  x_scenarioCount)==ND_FAILURE) return;
	if(setIntArray(resultsRef,"NumNodesWithFatalitiesBySource", numNodesWithFatalities,  x_scenarioCount)==ND_FAILURE) return;


	qsort(maxConcentration,x_scenarioCount,sizeof(float),sortFloatAsc);
	if(setFloatArray(resultsRef,"MaxConcentration", maxConcentration,  x_scenarioCount)==ND_FAILURE) return;

	qsort(maxDosage,x_scenarioCount,sizeof(float),sortFloatAsc);
	if(setFloatArray(resultsRef,"MaxDosage", maxDosage,  x_scenarioCount)==ND_FAILURE) return;
	
	qsort(totalInfected,x_scenarioCount,sizeof(float),sortFloatAsc);
	if(setFloatArray(resultsRef,"TotalInfected", totalInfected,  x_scenarioCount)==ND_FAILURE) return;
	
	qsort(totalFatalities,x_scenarioCount,sizeof(float),sortFloatAsc);
	if(setFloatArray(resultsRef,"TotalFatalities", totalFatalities,  x_scenarioCount)==ND_FAILURE) return;

	qsort(highFatalityNodes,x_scenarioCount,sizeof(int),sortFloatAsc);
	if(setIntArray(resultsRef,"HighFatalityNodes", highFatalityNodes,  x_scenarioCount)==ND_FAILURE) return;
	
	qsort(numNodesWithFatalities,x_scenarioCount,sizeof(int),sortFloatAsc);
	if(setIntArray(resultsRef,"NumNodesWithFatalities", numNodesWithFatalities,  x_scenarioCount)==ND_FAILURE) return;

	{
		int k;
		int *xvals=(int *)calloc(x_scenarioCount,sizeof(int));
		for(k=0;k<x_scenarioCount;k++) xvals[k]=k+1;
		if(setIntArray(resultsRef,"DummyXVals", xvals,  x_scenarioCount)==ND_FAILURE) return;
		free(xvals);
	}
	if(setFloatArray(resultsRef,"WorstScenario_Dosage",   x_worstIDF->dos,nsteps)==ND_FAILURE) return;
	if(setFloatArray(resultsRef,"WorstScenario_Response", x_worstIDF->res,nsteps)==ND_FAILURE) return;
	if(setFloatArray(resultsRef,"WorstScenario_S",        x_worstIDF->s,  nsteps)==ND_FAILURE) return;
	if(setFloatArray(resultsRef,"WorstScenario_I",        x_worstIDF->i,  nsteps)==ND_FAILURE) return;
	if(setFloatArray(resultsRef,"WorstScenario_D",        x_worstIDF->d,  nsteps)==ND_FAILURE) return;
	if(setFloatArray(resultsRef,"WorstScenario_F",        x_worstIDF->f,  nsteps)==ND_FAILURE) return;

//	if(getInt(analysisOptionsRef, "NumPercentiles", &numPercentiles)==ND_FAILURE) return;
//	if(getIntArray(analysisOptionsRef, "Percentiles", &percentiles)==ND_FAILURE) return;

	{
		JNIEnv *env = resultsRef->env;
		float *tval = (float*)calloc(numPercentiles+1,sizeof(float));
		if(setStringArray(resultsRef,"RowNames",             rowNames,numPercentiles+1)==ND_FAILURE) return;

		computeSummary(maxConcentration,x_scenarioCount,percentiles,tval,numPercentiles);
		if(setFloatArray(resultsRef,"MaxConcentrationSummary", tval,numPercentiles+1)==ND_FAILURE) return;

		computeSummary(maxDosage,x_scenarioCount,percentiles,tval,numPercentiles);
		if(setFloatArray(resultsRef,"MaxDosageSummary", tval,numPercentiles+1)==ND_FAILURE) return;

		computeSummary(totalInfected,x_scenarioCount,percentiles,tval,numPercentiles);
		if(setFloatArray(resultsRef,"TotalInfectedSummary", tval,numPercentiles+1)==ND_FAILURE) return;

		computeSummary(totalFatalities,x_scenarioCount,percentiles,tval,numPercentiles);
		if(setFloatArray(resultsRef,"TotalFatalitiesSummary", tval,numPercentiles+1)==ND_FAILURE) return;

		free(tval);

		ANL_UTIL_LogFiner(env,"teva.analysis.server","Before qsort:");
		for(i=0;i<x_Fatalities->num;i++) {
			char str[512];
			sprintf(str,"%4d:  %10.2f   %12ld   %12ld",i+1,x_Fatalities->data[i]->val,x_Fatalities->data[i]->fileOffset,x_sidfData->data[i]->fileOffset);
			ANL_UTIL_LogFiner(env,"teva.analysis.server",str);
		}
		qsort(x_Fatalities->data,x_Fatalities->num,sizeof(DiskCachedDataPtr),sortCachedDataAsc);
		qsort(x_sidfData->data,x_sidfData->num,sizeof(DiskCachedDataPtr),sortCachedDataAsc);
		ANL_UTIL_LogFiner(env,"teva.analysis.server","After qsort:");
		for(i=0;i<x_Fatalities->num;i++) {
			char str[512];
			sprintf(str,"%4d:  %10.2f   %12ld   %12ld",i+1,x_Fatalities->data[i]->val,x_Fatalities->data[i]->fileOffset,x_sidfData->data[i]->fileOffset);
			ANL_UTIL_LogFiner(env,"teva.analysis.server",str);
		}
		savePercentiles(resultsRef,x_Fatalities,x_sidfData,percentiles,rowNames,numPercentiles);

	}
//	nd_free(percentiles);

}
void computeSummary(float *scenData, int numScen, int* percentiles, float *rv, int numPercentiles)
{
	float tot=0;
	int i;
	for(i=0;i<numScen;i++) tot += scenData[i];
	for(i=0;i<numPercentiles;i++) {
		int idx = (int)ceil(percentiles[i]/100.0*numScen)-1;
		rv[i] = scenData[idx];
	}
	rv[numPercentiles] = tot/numScen;
}
void savePercentiles(NamedDataRef *resultsRef, DiskCachedDataPtr fatalitiesBySrc, DiskCachedDataPtr sidf, int* percentiles, char *percentileDescs[], int numPercentiles)
{
	int i;
	int num = fatalitiesBySrc->num;
	float *data = (float*)calloc(fatalitiesBySrc->size,sizeof(float));
	float *xa_data = (float*)calloc(6*sidf->size,sizeof(float));

	PXA txa = (PXA)calloc(1,sizeof(XA));
	initXA(txa,sidf->size);
	for(i=0;i<numPercentiles;i++) {
		int n;
		float totFatalities=0;
		char varName[1024];
		int idx = (int)ceil(percentiles[i]/100.0*num)-1;
		sprintf(varName,"TotalFatalitiesByReceptor-%s",percentileDescs[i]);
		getCachedData(fatalitiesBySrc,idx,data,fatalitiesBySrc->size*sizeof(float));
		for(n=0;n<fatalitiesBySrc->size;n++) {
			data[n] *= x_mem->node[n].pop;
			totFatalities += data[n];
		}
		if(addVariable(resultsRef,varName, FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
		if(setFloatArray(resultsRef,varName, data,fatalitiesBySrc->size)==ND_FAILURE) return;

		sprintf(varName,"SourceNodeIDs-%s",percentileDescs[i]);
		if(addVariable(resultsRef,varName, STRING_TYPE )==ND_FAILURE) return;
		if(setString(resultsRef,varName, fatalitiesBySrc->data[idx]->simID)==ND_FAILURE) return;
		
		sprintf(varName,"%s Injection",percentileDescs[i]);
		if(addVariable(resultsRef,varName, STRING_TYPE )==ND_FAILURE) return;
		if(setString(resultsRef,varName, fatalitiesBySrc->data[idx]->simID)==ND_FAILURE) return;
		
		sprintf(varName,"%s Fatalities",percentileDescs[i]);
		if(addVariable(resultsRef,varName, FLOAT_TYPE )==ND_FAILURE) return;
		if(setFloat(resultsRef,varName, totFatalities)==ND_FAILURE) return;

		getCachedData(sidf,idx,xa_data,sidf->size*6*sizeof(float));
		x_floatArrayToXA(txa,sidf->size,xa_data);

		sprintf(varName,"%s_Dosage",percentileDescs[i]);
		if(addVariable(resultsRef,varName, FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
		if(setFloatArray(resultsRef,varName, txa->dos,sidf->size)==ND_FAILURE) return;

		sprintf(varName,"%s_Response",percentileDescs[i]);
		if(addVariable(resultsRef,varName, FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
		if(setFloatArray(resultsRef,varName, txa->res,sidf->size)==ND_FAILURE) return;

		sprintf(varName,"%s_S",percentileDescs[i]);
		if(addVariable(resultsRef,varName, FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
		if(setFloatArray(resultsRef,varName, txa->s,sidf->size)==ND_FAILURE) return;

		sprintf(varName,"%s_I",percentileDescs[i]);
		if(addVariable(resultsRef,varName, FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
		if(setFloatArray(resultsRef,varName, txa->i,sidf->size)==ND_FAILURE) return;

		sprintf(varName,"%s_D",percentileDescs[i]);
		if(addVariable(resultsRef,varName, FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
		if(setFloatArray(resultsRef,varName, txa->d,sidf->size)==ND_FAILURE) return;
		
		sprintf(varName,"%s_F",percentileDescs[i]);
		if(addVariable(resultsRef,varName, FLOAT_ARRAY_TYPE )==ND_FAILURE) return;
		if(setFloatArray(resultsRef,varName, txa->f,sidf->size)==ND_FAILURE) return;

	}
	free(xa_data);
	freeXA(txa,sidf->size);
	free(txa);
	free(data);
}
/* 
 * called usually only once after the scenario is done to obtain the completed analysis results
 */
void HIAnalysis_aggr_writeResults(NamedDataRef *analysisOptionsRef,NamedDataRef *resultsRef, const char *dir, char *fn) {
	int nt,
		i,
		nnodes,
		nscenarios,
		nsteps,
		numWorstToKeep;
	float *tfa,*tda,*tra,*ttha;
	float *maxConc, *maxDose, *totInf, *totFatal;
    int *nFatalNodes;

	FILE *fp;
	XA xa;
	char fname[256];
	char **simids,**nodeIDs;

	if(getInt(resultsRef,"Length_t",&nsteps)==ND_FAILURE) return;
	if(getInt(resultsRef,"Length_n",&nnodes)==ND_FAILURE) return;
	if(getInt(resultsRef,"NSteps",&nt)==ND_FAILURE) return;
	if(getInt(resultsRef,"NumScenarios",&nscenarios)==ND_FAILURE) return;

	initXA(&xa,nsteps);

	x_initFloatArray(resultsRef,"Dose_t",    xa.dos,nsteps);
	x_initFloatArray(resultsRef,"Response_t",xa.res,nsteps);
	x_initFloatArray(resultsRef,"S_t",       xa.s,  nsteps);
	x_initFloatArray(resultsRef,"I_t",       xa.i,  nsteps);
	x_initFloatArray(resultsRef,"D_t",       xa.d,  nsteps);
	x_initFloatArray(resultsRef,"F_t",       xa.f,  nsteps);


	sprintf(fn,"%s/HIA.txt",dir);
	fp = fopen(fn,"w");
	sprintf(fn,"HIA.txt");

	fprintf(fp,"%d\n",nnodes);
	fprintf(fp,"%d\n",nsteps);
	fprintf(fp,"%d\n",nscenarios);
	for(i=0;i<nsteps;i++) fprintf(fp,"%f ",xa.s[i]);
	fprintf(fp,"\n");
	for(i=0;i<nsteps;i++) fprintf(fp,"%f ",xa.i[i]);
	fprintf(fp,"\n");
	for(i=0;i<nsteps;i++) fprintf(fp,"%f ",xa.d[i]);
	fprintf(fp,"\n");
	for(i=0;i<nsteps;i++) fprintf(fp,"%f ",xa.f[i]);
	fprintf(fp,"\n");
	for(i=0;i<nsteps;i++) fprintf(fp,"%f ",xa.dos[i]);
	fprintf(fp,"\n");
	for(i=0;i<nsteps;i++) fprintf(fp,"%f ",xa.res[i]);
	fprintf(fp,"\n");

	freeXA(&xa,nsteps);

	tfa = (float *)calloc(nscenarios,sizeof(float));
	tda = (float *)calloc(nscenarios,sizeof(float));
	tra = (float *)calloc(nscenarios,sizeof(float));
	ttha = (float *)calloc(nscenarios,sizeof(float));

	
	x_initFloatArray(resultsRef,"Hist_F",     tfa,  nscenarios);
	for(i=0;i<nscenarios;i++) fprintf(fp,"%f ",tfa[i]);
	fprintf(fp,"\n");
	x_initFloatArray(resultsRef,"Hist_Dos",   tda,  nscenarios);
	for(i=0;i<nscenarios;i++) fprintf(fp,"%f ",tda[i]);
	fprintf(fp,"\n");
	x_initFloatArray(resultsRef,"Hist_Resp",   tra,  nscenarios);
	for(i=0;i<nscenarios;i++) fprintf(fp,"%f ",tra[i]);
	fprintf(fp,"\n");
	x_initFloatArray(resultsRef,"Hist_testh", ttha,  nscenarios);
	for(i=0;i<nscenarios;i++) fprintf(fp,"%f ",ttha[i]);
	fprintf(fp,"\n");

	if(getStringArray(resultsRef,"Hist_SimID",&simids)==ND_FAILURE) return;
	for(i=0;i<nscenarios;i++) fprintf(fp,"%s ",simids[i]);
	fprintf(fp,"\n");
	fclose(fp);  // close standard output file

	sprintf(fname,"%s/HIA_data.txt",dir);
	fp = fopen(fname,"w");
	strcat(fn,","); strcat(fn,"HIA_data.txt");

	maxConc     = (float *)calloc(nscenarios,sizeof(float));
	maxDose     = (float *)calloc(nscenarios,sizeof(float));
	totInf      = (float *)calloc(nscenarios,sizeof(float));
	totFatal    = (float *)calloc(nscenarios,sizeof(float));
	nFatalNodes = (int   *)calloc(nscenarios,sizeof(int  ));

	x_initFloatArray(resultsRef,"MaxConcentrationBySource",      maxConc,    nscenarios);
	x_initFloatArray(resultsRef,"MaxDosageBySource",             maxDose,    nscenarios);
	x_initFloatArray(resultsRef,"TotalInfectedBySource",         totInf,     nscenarios);
	x_initFloatArray(resultsRef,"TotalFatalitiesBySource",       totFatal,   nscenarios);
	x_initIntArray(  resultsRef,"NumNodesWithFatalitiesBySource",nFatalNodes,nscenarios);


	fprintf(fp,"ID\tMax Concentration\tMax Dosage\tTotal Infected\tFatalities\tNodes for 90%% Fatalities\tTotal Dosage\tTotal Response\n");
	for(i=0;i<nscenarios;i++)
		fprintf(fp,"%s\t%f\t%f\t%f\t%f\t%d\t%f\t%f\n",
				simids[i],maxConc[i],maxDose[i],totInf[i],
				totFatal[i],nFatalNodes[i],tda[i],tra[i]);

	free(maxConc    ); maxConc    =NULL;
	free(maxDose    ); maxDose    =NULL;
	free(totInf     ); totInf     =NULL;
	free(totFatal   ); totFatal   =NULL;
	free(nFatalNodes); nFatalNodes=NULL;

	nd_freeStringArray(simids,nscenarios);

	free(tfa);
	free(tda);
	free(tra);
	free(ttha);
	fclose(fp);

	if(getInt(analysisOptionsRef, "NumWorstCaseToKeep", &numWorstToKeep)==ND_FAILURE) return;

	if(numWorstToKeep>0) {
		float *pop;
		// write out individual files for worst scenarios
		if(getInt(resultsRef,"NumWorstScenarios",&nscenarios)==ND_FAILURE) return;
		if(getInt(resultsRef,"NNodes",&nnodes)==ND_FAILURE) return;
		if(getStringArray(resultsRef,"NodeIDs",&nodeIDs)==ND_FAILURE) return;
		if(getFloatArray(resultsRef,"Population",&pop)==ND_FAILURE) return;
		for(i=0;i<nscenarios;i++) {
			char name[256],*simID;
			char fname[256];
			double totalConcentration;
			float **conc,*dos,*res,*s,*ii,*d,*f;
			int n,t;

			sprintf(fname,"%s/HIA_conc_%03d.txt",dir,i+1);
			fp=fopen(fname,"w");

			sprintf(fname,"HIA_conc_%03d.txt",i+1);
			strcat(fn,","); strcat(fn,fname);

			sprintf(name,"SimID[%02d]",i);
			if(getString(resultsRef,name, &simID) ==ND_FAILURE) return;
			fprintf(fp,"Simulation ID\t%s\n",simID);
			nd_free(simID);

			sprintf(name,"RankValue[%02d]",i);
			if(getDouble(resultsRef,name, &totalConcentration) ==ND_FAILURE) return;
			fprintf(fp,"Total Fatalities\t%lf\n",totalConcentration);

			sprintf(name,"ReceptorDosage[%02d]",i);
			if(getFloatArray(resultsRef,name, &dos) ==ND_FAILURE) return;
			sprintf(name,"ReceptorResponse[%02d]",i);
			if(getFloatArray(resultsRef,name, &res) ==ND_FAILURE) return;
			sprintf(name,"ReceptorS[%02d]",i);
			if(getFloatArray(resultsRef,name, &s) ==ND_FAILURE) return;
			sprintf(name,"ReceptorI[%02d]",i);
			if(getFloatArray(resultsRef,name, &ii) ==ND_FAILURE) return;
			sprintf(name,"ReceptorD[%02d]",i);
			if(getFloatArray(resultsRef,name, &d) ==ND_FAILURE) return;
			sprintf(name,"ReceptorF[%02d]",i);
			if(getFloatArray(resultsRef,name, &f) ==ND_FAILURE) return;

			sprintf(name,"NodeConcentration[%02d]",i);
			conc = getNodeConcentrations(resultsRef,name,nt,nnodes);
			fprintf(fp,"receptor\tpop\tdosage\tresponse\ts\ti\td\tf");
			for(t=0;t<nt;t++) { fprintf(fp,"\t%d",t);}
			fprintf(fp,"\n");
			for(n=0;n<nnodes;n++) {
				fprintf(fp,"%s\t%f",nodeIDs[n],pop[n]);
				fprintf(fp,"\t%f\t%f\t%f\t%f\t%f\t%f",dos[n],res[n],s[n],ii[n],d[n],f[n]);
				for(t=0;t<nt;t++) {
					fprintf(fp,"\t%f",conc[t][n]);
				}
				fprintf(fp,"\n");
			}
			nd_free(dos);
			nd_free(res);
			nd_free(s);
			nd_free(ii);
			nd_free(d);
			nd_free(f);
			freeConcentrationData(conc,nt);
			fclose(fp);
		}
		nd_freeStringArray(nodeIDs,nnodes);
		nd_free(pop);
	}
	writeOKCFile(analysisOptionsRef,resultsRef,dir,fn);
	writeAllIDFData(dir,fn,x_sidfData,nsteps);
}
/*
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
*/
void writeAllIDFData(const char *dir, char *fn, DiskCachedDataPtr sidf,int nsteps) 
{
	char filename[256];
	int i,j;
	FILE *fp;
	float *xa_data = (float*)calloc(6*sidf->size,sizeof(float));
	PXA txa = (PXA)calloc(1,sizeof(XA));
	initXA(txa,sidf->size);

	
	sprintf(filename,"%s/IDF.txt",dir);
	fp = fopen(filename,"w");
	strcat(fn,",IDF.txt");
	
	for(i=0;i<sidf->num;i++) {
		CachedDataPtr cdp = sidf->data[i];
		fprintf(fp,"Injection: %s\n",cdp->simID);
		getCachedData(sidf,i,xa_data,sidf->size*6*sizeof(float));
		x_floatArrayToXA(txa,sidf->size,xa_data);
		fprintf(fp,"I"); for(j=0;j<nsteps;j++) { fprintf(fp,"\t%f",txa->i[j]); } fprintf(fp,"\n");
		fprintf(fp,"D"); for(j=0;j<nsteps;j++) { fprintf(fp,"\t%f",txa->d[j]); } fprintf(fp,"\n");
		fprintf(fp,"F"); for(j=0;j<nsteps;j++) { fprintf(fp,"\t%f",txa->f[j]); } fprintf(fp,"\n");
	}
	fclose(fp);
	freeXA(txa,sidf->size);
	free(xa_data);
}
void writeOKCFile(NamedDataRef *analysisOptionsRef,NamedDataRef *resultsRef, const char *dir, char *fn)
{
	int i,
		nnodes,
		nscenarios;
	float *tc,*td,*ti,*tf;
	int *tn;
	float minC,minD,minI,minF,minN;
	float maxC,maxD,maxI,maxF,maxN;
	FILE *fp;
	char fname[256];
	char **simids;

	if(getInt(resultsRef,"Length_n",&nnodes)==ND_FAILURE) return;
	if(getInt(resultsRef,"NumScenarios",&nscenarios)==ND_FAILURE) return;

	tc = (float *)calloc(nscenarios,sizeof(float));
	td = (float *)calloc(nscenarios,sizeof(float));
	ti = (float *)calloc(nscenarios,sizeof(float));
	tf = (float *)calloc(nscenarios,sizeof(float));
	tn = (int *)calloc(nscenarios,sizeof(int));

	x_initFloatArray(resultsRef,"MaxConcentrationBySource",       tc,  nscenarios);
	x_initFloatArray(resultsRef,"MaxDosageBySource",              td,  nscenarios);
	x_initFloatArray(resultsRef,"TotalInfectedBySource",          ti,  nscenarios);
	x_initFloatArray(resultsRef,"TotalFatalitiesBySource",        tf,  nscenarios);
	x_initIntArray(  resultsRef,"NumNodesWithFatalitiesBySource", tn,  nscenarios);
	if(getStringArray(resultsRef,"Hist_SimID",&simids)==ND_FAILURE) return;

	sprintf(fname,"%s/HIA.okc",dir);
	fp = fopen(fname,"w");
	strcat(fn,","); strcat(fn,"HIA.okc");

	fprintf(fp,"5 %d\n",nscenarios);
	fprintf(fp,"MaxConcentration\n");
	fprintf(fp,"MaxDosage\n");
	fprintf(fp,"Infected\n");
	fprintf(fp,"Fatalities\n");
	fprintf(fp,"FatalNodes\n");

	minC=minD=minI=minF=minN=FLT_MAX;
	maxC=maxD=maxI=maxF=maxN=-FLT_MAX;
	for(i=0;i<nscenarios;i++) {
		if(tc[i]<minC) minC=tc[i];
		if(td[i]<minD) minD=td[i];
		if(ti[i]<minI) minI=ti[i];
		if(tf[i]<minF) minF=tf[i];
		if(tn[i]<minN) minN=(float)tn[i];

		if(tc[i]>maxC) maxC=tc[i];
		if(td[i]>maxD) maxD=td[i];
		if(ti[i]>maxI) maxI=ti[i];
		if(tf[i]>maxF) maxF=tf[i];
		if(tn[i]>maxN) maxN=(float)tn[i];
	}
	fprintf(fp," %f %f 1",minC*0.9,maxC*1.1);
	fprintf(fp," %f %f 1",minD*0.9,maxD*1.1);
	fprintf(fp," %f %f 1",minI*0.9,maxI*1.1);
	fprintf(fp," %f %f 1",minF*0.9,maxF*1.1);
	fprintf(fp," %f %f 1\n",minN*0.9,maxN*1.1);

	for(i=0;i<nscenarios;i++)
		fprintf(fp,"%f %f %f %f %f\n",tc[i],td[i],ti[i],tf[i],tn);

	nd_freeStringArray(simids,nscenarios);

	free(tc);
	free(td);
	free(ti);
	free(tf);
	free(tn);
	fclose(fp);
}
float **getNodeConcentrations(void *ref,char *name,int nsteps,int nnodes) {
	int i;
	float **conc = (float**)calloc(nsteps,sizeof(float*));
	for(i=0;i<nsteps;i++) {
		float *tres;
		conc[i] = (float*)calloc(nnodes,sizeof(float));
		/** IMPORTANT!!! Need to free these later via call to nd_free */
		if(getFloatArrayAtIndex(ref,name,i,&tres)==ND_FAILURE) {
			return NULL;
		}
		memcpy(conc[i],tres,nnodes*sizeof(float));
		nd_free(tres);
	}
	return conc;
}
void freeConcentrationData(float **conc, int nsteps) {
	int i;
	for(i=0;i<nsteps;i++) {
		free(conc[i]);
	}
	free(conc);
}
/* 
 * called once after the scenario has been completed
 */
void HIAnalysis_aggr_shutdown(NamedDataRef *analysisOptionsRef)
{
	int nnodes = x_mem->net->info->nnodes;
	int nsteps = x_mem->dr->maxsteps;
	char msg[256];

	JNIEnv *env = analysisOptionsRef->env;

	sprintf(msg,"Retrieved concentration data in %d of %d simulations",x_nRetrievals,x_nCalls);
	ANL_UTIL_LogInfo(env,"teva.analysis.server",msg);

	freeHistogramData(x_mem->hist);
	freeXA(x_xa,nsteps);
	free(x_xa);
	freeXA(x_worstIDF,nsteps);
	free(x_worstIDF);

	freeCDPData(&x_worstScenarios);

	// should be last in case any others need data from x_mem
	freeHIAMemory(x_mem,Aggregation);
	freeDiskCachedData(analysisOptionsRef->env,&x_Fatalities);
	freeDiskCachedData(analysisOptionsRef->env,&x_sidfData);

	free(maxConcentration);
	free(maxDosage);
	free(totalInfected);
	free(totalFatalities);
	free(highFatalityNodes);
	free(numNodesWithFatalities);

//	btFree(&x_worstScenarios,cdpFree);
}

static int x_initFloatArray(void *ref, char *varName, float *data, int len) {
	float *tres;
	if(getFloatArray(ref,varName,&tres)==ND_FAILURE) return ND_FAILURE;
	memcpy(data,tres,sizeof(float)*len);
	nd_free(tres);
	return ND_SUCCESS;
}


static int x_initIntArray(void *ref, char *varName, int *data, int len) {
	int *tres;
	if(getIntArray(ref,varName,&tres)==ND_FAILURE) return ND_FAILURE;
	memcpy(data,tres,sizeof(int)*len);
	nd_free(tres);
	return ND_SUCCESS;
}

int sortFloatAsc(const void *a, const void *b)
{
	float f=*((float *)a) - *((float *)b);
	return (f<0?-1:(f>0?1:0));
}
int sortCachedDataAsc(const void *a, const void *b)
{
	float f=(*((CachedDataPtr*)a))->val - (*((CachedDataPtr*)b))->val;
	return (f<0?-1:(f>0?1:0));
}
int sortIntAsc(const void *a, const void *b)
{
	return *((int*)a) - *((int*)b);
}

static void x_xaToFloatArray(PXA xa,int nsteps,float *xa_data)
{
	memcpy(&xa_data[0*nsteps],xa->dos,nsteps*sizeof(float));
	memcpy(&xa_data[1*nsteps],xa->res,nsteps*sizeof(float));
	memcpy(&xa_data[2*nsteps],xa->s  ,nsteps*sizeof(float));
	memcpy(&xa_data[3*nsteps],xa->i  ,nsteps*sizeof(float));
	memcpy(&xa_data[4*nsteps],xa->d  ,nsteps*sizeof(float));
	memcpy(&xa_data[5*nsteps],xa->f  ,nsteps*sizeof(float));
}
// xa is assumed to be properly allocated
static void x_floatArrayToXA(PXA xa,int nsteps,float *xa_data)
{
	memcpy(xa->dos,&xa_data[0*nsteps],nsteps*sizeof(float));
	memcpy(xa->res,&xa_data[1*nsteps],nsteps*sizeof(float));
	memcpy(xa->s  ,&xa_data[2*nsteps],nsteps*sizeof(float));
	memcpy(xa->i  ,&xa_data[3*nsteps],nsteps*sizeof(float));
	memcpy(xa->d  ,&xa_data[4*nsteps],nsteps*sizeof(float));
	memcpy(xa->f  ,&xa_data[5*nsteps],nsteps*sizeof(float));
}
