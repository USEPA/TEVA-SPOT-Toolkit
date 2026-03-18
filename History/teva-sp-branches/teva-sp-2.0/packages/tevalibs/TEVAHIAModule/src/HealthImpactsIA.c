#include <limits.h>
#include <memory.h>
#include <stdlib.h>

#include "HealthImpacts.h"
#include "ExternalAnalysis.h"
#include "NamedData.h"

static PMem x_mem;
static int x_nscenarios;
static int x_numToSave;
static XA x_na;

//#define WRITE_SIDF


void writeFileData(char *nodeID,FILE *fp,float *data, float pop,int totsteps);
int sortFloatDesc(const void *a, const void *b);



/* 
 * called once after the first simulaiton has been completed (so demands are there)
 */
int HIAnalysis_ia_initialize(NamedDataRef *analysisOptionsRef, NamedDataRef *simResultsRef)
{
	// at this point, simResultsRef does not conatin any quality, but will contain
	// demands however
	x_mem = loadHIAOptions(analysisOptionsRef, simResultsRef,IntermediateAnalysis);
	x_nscenarios=0;

	Population(x_mem->net,x_mem->dr,x_mem->node);

	IngestionRate(x_mem->net,x_mem->node);
	if(getInt(analysisOptionsRef, "NumWorstCaseToKeep", &x_numToSave)==ND_FAILURE) return 0;
	initXA(&x_na,x_mem->net->info->nnodes);
	return x_numToSave>0;
}
/* 
 * called once after every simulation is done to perform the intermediate analysis.  The
 * data placed in the intResultsRef will be passed along to the aggregation server
 */
void HIAnalysis_ia_newSimResults(NamedDataRef *analysisOptionsRef,NamedDataRef *simResultsRef, NamedDataRef *intResultsRef)
{
	int i,j,
		dtmin,
		dtime,
		nsteps,
		totsteps,
		nnodes;
	float maxConc=-1,maxDosage=-1,totInfected=0;
	int minNodesFor90pctFatalities=0;
	int numNodesWithFatalities=0;
	float *dos;

	HIST hist;
	char *simID;
//	double totalConcentration=0;
	float **conc;
	JNIEnv *env = analysisOptionsRef->env;
#ifdef WRITE_SIDF
	FILE *fp[6];
#endif /* WRITE_SIDF */
	char *dt[6] = {"Dosage","Response","S","I","D","F"};

	x_nscenarios++;
	memset(&hist,0,sizeof(HIST));
	nsteps=x_mem->net->info->nsteps;
	nnodes=x_mem->net->info->nnodes;

	if(addVariable(intResultsRef,"NodeConcentration",   FLOAT_ARRAY_2D_TYPE)==ND_FAILURE) return;
	if(addVariable(intResultsRef,"RankValue", DOUBLE_TYPE)==ND_FAILURE) return;
	totsteps = x_mem->dr->maxsteps;

	if(x_numToSave>0) {
		conc = (float**)calloc(nsteps,sizeof(float*));
		for(i=0;i<nsteps;i++) {
			conc[i] = (float*)calloc(nnodes,sizeof(float));
		}
	}
	for(j=0;j<nnodes;j++) {
		x_mem->node[j].info->nz[0] = 0;
	}
	for(i=0;i<nsteps;i++) {
		float *tres;
		/** IMPORTANT!!! Need to free these later via call to nd_free */
		if(getFloatArrayAtIndex(simResultsRef,"quality",i,&tres)==ND_FAILURE) {
			return;
		}
		for(j=0;j<nnodes;j++) {
			float tc = tres[j];
			if(tc > 0) x_mem->node[j].info->nz[0] = 1;
			x_mem->node[j].info->c[0][i] = tc;
			if(x_mem->node[j].info->c[0][i] > maxConc)
				maxConc = x_mem->node[j].info->c[0][i];
//			totalConcentration+=tres[j];
		}
		if(x_numToSave>0) {
			memcpy(conc[i],tres,nnodes*sizeof(float));
		}
		nd_free(tres);
	}

	/* Sampling network detection time */
	dtmin=INT_MAX;
    for(i=0;i<x_mem->net->info->nnodes;i++) {
        if((x_mem->node[i].sensor.type==existing || x_mem->node[i].sensor.type==selected) && x_mem->node[i].info->nz[0]) {
            if( (dtime=Detect(x_mem->net,&x_mem->node[i]))<dtmin ) dtmin=dtime;
        }
    }

#ifdef WRITE_SIDF
	if(getString(simResultsRef,"injectionID",&simID)==ND_FAILURE) return;
	for(i=0;i<6;i++) {
		char fn[256];
		sprintf(fn,"%s_%s.txt",simID,dt[i]);
		fp[i] = fopen(fn,"w");
	}
	nd_free(simID);
#endif /* WRITE_SIDF */

	resetXA(x_mem->xa,totsteps);
//	if(x_numToSave>0) {
		resetXA(&x_na,nnodes);
//	}
	/* Time-dependent cumulative expected dose and disease response */
	for(i=0;i<nnodes;i++) {
		Dose(dtmin,x_mem->net,x_mem->dr,x_mem->u,&x_mem->node[i]);
//		if(x_mem->dr->responseType==Probit) {
//			ProbitResponse(x_mem->net,x_mem->dr,x_mem->u);
//		} else {
			Response(x_mem->net,x_mem->dr,x_mem->u);
//		}
		DiseaseRM(x_mem->net,x_mem->dr,x_mem->u);
		Accumulate(i,x_mem->node[i].info->nz[0],x_mem->net,x_mem->dr,x_mem->node[i].pop,x_mem->u,x_mem->xa,x_mem->ta,&hist);
		// x_mem->u holds the s,i,d, f, dosage, and response values for this receptor for all time
//		if(x_numToSave>0) {
			x_na.dos[i] = x_mem->u->dos[totsteps-1];
			x_na.res[i] = x_mem->u->res[totsteps-1];
			x_na.s[i] = x_mem->u->s[totsteps-1];
			x_na.i[i] = x_mem->u->i[totsteps-1];
			x_na.d[i] = x_mem->u->d[totsteps-1];
			x_na.f[i] = x_mem->u->f[totsteps-1];
//		}
		dos = x_mem->u->dos;
		for(j=0;j<totsteps;j++) {
			if(dos[j] > maxDosage)
				maxDosage = dos[j];
		}
		totInfected+=x_mem->node[i].pop*x_mem->u->i[totsteps-1];
#ifdef WRITE_SIDF
		writeFileData(x_mem->node[i].id,fp[0],x_mem->u->dos,x_mem->node[i].pop,totsteps);
		writeFileData(x_mem->node[i].id,fp[1],x_mem->u->res,x_mem->node[i].pop,totsteps);
		writeFileData(x_mem->node[i].id,fp[2],x_mem->u->s,x_mem->node[i].pop,totsteps);
		writeFileData(x_mem->node[i].id,fp[3],x_mem->u->i,x_mem->node[i].pop,totsteps);
		writeFileData(x_mem->node[i].id,fp[4],x_mem->u->d,x_mem->node[i].pop,totsteps);
		writeFileData(x_mem->node[i].id,fp[5],x_mem->u->f,x_mem->node[i].pop,totsteps);
#endif /* WRITE_SIDF */
	}
#ifdef WRITE_SIDF
	for(i=0;i<6;i++) {
		fclose(fp[i]);
	}
#endif /* WRITE_SIDF */

	if(addVariable(intResultsRef,"Dose_t",     FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(intResultsRef,"Response_t", FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(intResultsRef,"S_t",        FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(intResultsRef,"I_t",        FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(intResultsRef,"D_t",        FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(intResultsRef,"F_t",        FLOAT_ARRAY_TYPE)==ND_FAILURE) return;

	if(setFloatArray(intResultsRef,"Dose_t",    x_mem->xa->dos,totsteps)==ND_FAILURE) return;
	if(setFloatArray(intResultsRef,"Response_t",x_mem->xa->res,totsteps)==ND_FAILURE) return;
	if(setFloatArray(intResultsRef,"S_t",       x_mem->xa->s,  totsteps)==ND_FAILURE) return;
	if(setFloatArray(intResultsRef,"I_t",       x_mem->xa->i,  totsteps)==ND_FAILURE) return;
	if(setFloatArray(intResultsRef,"D_t",       x_mem->xa->d,  totsteps)==ND_FAILURE) return;
	if(setFloatArray(intResultsRef,"F_t",       x_mem->xa->f,  totsteps)==ND_FAILURE) return;

//	if(x_numToSave>0) {
	if(addVariable(intResultsRef,"Dose_n",     FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(intResultsRef,"Response_n", FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(intResultsRef,"S_n",        FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(intResultsRef,"I_n",        FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(intResultsRef,"D_n",        FLOAT_ARRAY_TYPE)==ND_FAILURE) return;
	if(addVariable(intResultsRef,"F_n",        FLOAT_ARRAY_TYPE)==ND_FAILURE) return;

	if(setFloatArray(intResultsRef,"Dose_n",    x_na.dos,nnodes)==ND_FAILURE) return;
	if(setFloatArray(intResultsRef,"Response_n",x_na.res,nnodes)==ND_FAILURE) return;
	if(setFloatArray(intResultsRef,"S_n",       x_na.s,  nnodes)==ND_FAILURE) return;
	if(setFloatArray(intResultsRef,"I_n",       x_na.i,  nnodes)==ND_FAILURE) return;
	if(setFloatArray(intResultsRef,"D_n",       x_na.d,  nnodes)==ND_FAILURE) return;
	if(setFloatArray(intResultsRef,"F_n",       x_na.f,  nnodes)==ND_FAILURE) return;
//	}

	if(addVariable(intResultsRef,"Hist_testh", FLOAT_TYPE)==ND_FAILURE) return;
	if(addVariable(intResultsRef,"Hist_F",     FLOAT_TYPE)==ND_FAILURE) return;
	if(addVariable(intResultsRef,"Hist_Dos",   FLOAT_TYPE)==ND_FAILURE) return;
	if(addVariable(intResultsRef,"Hist_Resp",  FLOAT_TYPE)==ND_FAILURE) return;

	if(setFloat(intResultsRef,"Hist_testh", hist.testh)==ND_FAILURE) return;
	if(setFloat(intResultsRef,"Hist_F",     hist.fh   )==ND_FAILURE) return;
	if(setFloat(intResultsRef,"Hist_Dos",   hist.dosh )==ND_FAILURE) return;
	if(setFloat(intResultsRef,"Hist_Resp",  hist.resh )==ND_FAILURE) return;

	if(addVariable(intResultsRef,"Hist_SimID",  STRING_TYPE)==ND_FAILURE) return;
	if(getString(simResultsRef,"injectionID",&simID)==ND_FAILURE) return;
	if(setString(intResultsRef,"Hist_SimID", simID )==ND_FAILURE) return;
	nd_free(simID);

	if(setDouble(intResultsRef, "RankValue", hist.fh)==ND_FAILURE) return;
	if(x_numToSave>0) {
		if(set2DFloatArray(intResultsRef, "NodeConcentration", conc, nsteps, nnodes)==ND_FAILURE) return;
		for(i=0;i<nsteps;i++) {
			free(conc[i]);
		}
		free(conc);
	}
	// additions for TEVA Version 1 ...
	if(addVariable(intResultsRef,"MaxConcentration",  FLOAT_TYPE )==ND_FAILURE) return;
	if(addVariable(intResultsRef,"MaxDosage",         FLOAT_TYPE )==ND_FAILURE) return;
	if(addVariable(intResultsRef,"TotalInfected",     FLOAT_TYPE )==ND_FAILURE) return;
	if(addVariable(intResultsRef,"TotalFatalities",   FLOAT_TYPE )==ND_FAILURE) return;
	if(addVariable(intResultsRef,"HighFatalityNodes", INT_TYPE )==ND_FAILURE) return;
	if(addVariable(intResultsRef,"NumNodesWithFatalities", INT_TYPE )==ND_FAILURE) return;
	if(addVariable(intResultsRef,"SimulationID",      STRING_TYPE)==ND_FAILURE) return;

	if(setFloat(intResultsRef,"MaxConcentration", maxConc)==ND_FAILURE) return;
	if(setFloat(intResultsRef,"MaxDosage", maxDosage)==ND_FAILURE) return;
	if(setFloat(intResultsRef,"TotalInfected", hist.fh/x_mem->dr->frate)==ND_FAILURE) return;
	if(setFloat(intResultsRef,"TotalFatalities", hist.fh)==ND_FAILURE) return;

	
	{
		int k;
		float tf=0;
		float *fatalities = (float *)calloc(nnodes,sizeof(float));

		for(k=0;k<nnodes;k++) {
			fatalities[k]=x_na.f[k]*x_mem->node[k].pop;
			if(fatalities[k]>0)
				numNodesWithFatalities++;
			tf += fatalities[k];
		}
		qsort(fatalities,nnodes,sizeof(float),sortFloatDesc);
		tf*=0.9f;
		minNodesFor90pctFatalities=0;
		for(k=0;k<nnodes && tf > 0;k++) {
			minNodesFor90pctFatalities++;
			tf -= fatalities[k];
		}

		free(fatalities);
	}
	
	
	if(setInt(intResultsRef,"NumNodesWithFatalities", numNodesWithFatalities)==ND_FAILURE) return;
	if(setInt(intResultsRef,"HighFatalityNodes", minNodesFor90pctFatalities)==ND_FAILURE) return;

	if(getString(simResultsRef,"injectionID",&simID)==ND_FAILURE) return;
	if(setString(intResultsRef,"SimulationID", simID )==ND_FAILURE) return;
	nd_free(simID);


}

/* 
 * called once after the scenario has been completed
 */
void HIAnalysis_ia_shutdown(NamedDataRef *analysisOptionsRef)
{
	if(x_numToSave>0) {
		freeXA(&x_na,x_mem->net->info->nnodes);
	}
	freeHIAMemory(x_mem,IntermediateAnalysis);
}



void writeFileData(char *nodeID,FILE *fp,float *data, float pop,int totsteps)
{
	int i;
	fprintf(fp,"%s",nodeID);
	for(i=0;i<totsteps;i++) {
		fprintf(fp,"\t%f",data[i]*pop);
	}
	fprintf(fp,"\n");
}

int sortFloatDesc(const void *a, const void *b)
{
	float f=*((float *)b) - *((float *)a);
	return (f<0?-1:(f>0?1:0));
}