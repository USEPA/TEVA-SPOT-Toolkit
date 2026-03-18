#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <list>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sp/impacts.h>
#include "tso2ImpactAnalysis.h"

extern "C" {
#include "loggingUtils.h"
#include "NamedData.h"
#include "ExternalAnalysis.h"

#ifndef HAVE_REALPATH
extern char *realpath(const char *pathname, char resolved_path[PATH_MAX]);
#endif

static PMem x_mem;


static JNIEnv *x_env;

static int x_nSims;

static std::string outputFilePrefix;  // not used
static int detectionDelay=0;
static double minQuality=0.0;
static bool mcSelected=false;
static bool vcSelected=false;
static bool nfdSelected=false;
static bool tdSelected=false;
static bool ecSelected=false;
static bool pkSelected=false;
static bool peSelected=false;
static std::string sensorInputFilename;
static std::list<int> sensorLocations;
static int numNodes;
static std::string nodeMapFileName;
static std::string scenarioMapFileName;
static std::ofstream nodeMapFile;
static std::ofstream scenarioMapFile;
static std::vector<std::string> nodeIndexToIDMap;
static std::map<std::string,int> nodeIDToIndexMap; // maps node textual IDs to 0-based TEVA node indicies
static clock_t start, stop;
static std::vector<ObjectiveBase*> theObjectives;
		int numImpacts;


/* 
 * called once after the first simulaiton has been completed (so demands are there)
 */
void tso2ImpactAnalysis_aggr_initialize(NamedDataRef *analysisOptionsRef, 
				   NamedDataRef *simResultsRef)
{
	// at this point, simResultsRef does not conatin any quality, but will contain
	// demands however
	int nnodes;
	int nsteps;
	x_nSims = 0;
	x_mem = loadOptions(analysisOptionsRef, simResultsRef,Aggregation,
      		       outputFilePrefix,
      		       detectionDelay,
      		       minQuality,
      		       mcSelected,
      		       vcSelected,
      		       nfdSelected,
      		       tdSelected,
      		       ecSelected,
      		       pkSelected,
      		       peSelected);
	// x_mem->net,x_mem->dr,x_mem->node
	PLinkInfo dummy=NULL;	// no link info in simResultsRef yet
        initTso2ImpactModuleData(
		analysisOptionsRef, simResultsRef,
                numNodes, start, stop,
                x_mem->node->info, dummy,
                x_mem->net->info, x_mem->tso, x_mem->sources,
                outputFilePrefix,
                detectionDelay,
                minQuality,
                mcSelected,
                vcSelected,
                nfdSelected,
                tdSelected,
                ecSelected,
                pkSelected,
                peSelected,
		x_mem,
                sensorLocations,
                nodeMapFileName,
                scenarioMapFileName,
                nodeMapFile,
                scenarioMapFile,
                theObjectives,
                nodeIndexToIDMap,
                nodeIDToIndexMap);
	Population(x_mem->net,x_mem->dr,x_mem->node);

	dumpHIAInputParameters(x_mem);

	nnodes = x_mem->net->info->nnodes;
	nsteps = x_mem->net->info->nsteps;
}

/* 
 * called once after every simulation with the results of the intermediate analysis
 */
void tso2ImpactAnalysis_aggr_newResults(NamedDataRef *analysisOptionsRef,NamedDataRef *intResultsRef)
{
	JNIEnv *env = analysisOptionsRef->env;
	char msg[256];
	x_env=env;

	x_nSims++;
	if (0) ANL_UTIL_LogSevere(x_env,"anl.teva", "test error!");

	sprintf(msg,"Processed simulation results for scenario=%d", x_nSims);
	ANL_UTIL_LogFine(env,"teva.analysis.server",msg);
	sprintf(msg,"Aggregation: newResults:");
	ANL_UTIL_LogFine(env,"teva.analysis.server",msg);

	int numInjections = 0;
//	if(getString(intResultsRef,"injectionID",&simID)==ND_FAILURE) return;
	if(getInt(intResultsRef,"numInjections",&numInjections)==ND_FAILURE) 
		return;
	int *sourceIndices;
	x_mem->net->info->nsources=numInjections;
	if(getIntArray(intResultsRef, "injectionIndices", 
			&sourceIndices)==ND_FAILURE) 
		return;

	for (int i=0; i<numInjections; i++) {
		x_mem->sources[i].SourceNodeIndex = sourceIndices[i]+1;
	}

	std::vector<ObjectiveBase*>::iterator it;
	for (it=theObjectives.begin(); it!=theObjectives.end();it++) {
		ObjectiveBase *thisObjective=(*it);
		const char *str = thisObjective->impactFilenameSuffix().c_str();
		int *witness;
		int *td;
		double *impact;
		char numImpactsStr[256];
		char witnessStr[256];
		char tdStr[256];
		char impactStr[256];
		sprintf(numImpactsStr,"%s_numImpacts", str);
		sprintf(witnessStr,"%s_witnessID", str);
		sprintf(tdStr,"%s_timeToDetection", str);
		sprintf(impactStr,"%s_impact", str);
		if(getInt(intResultsRef,(char*)numImpactsStr, &numImpacts)
						==ND_FAILURE) {
			sprintf(msg,"Unable to retrieve %s",numImpactsStr);
			ANL_UTIL_LogSevere(env,"teva.analysis.server",msg);
			return;
		}
		if(getIntArray(intResultsRef,(char*)witnessStr, &witness)
						==ND_FAILURE) {
			sprintf(msg,"Unable to retrieve %s",witnessStr);
			ANL_UTIL_LogSevere(env,"teva.analysis.server",msg);
			return;
		}
		if(getIntArray(intResultsRef,(char*)tdStr, &td)
						==ND_FAILURE) {
			sprintf(msg,"Unable to retrieve %s",tdStr);
			ANL_UTIL_LogSevere(env,"teva.analysis.server",msg);
			return;
		}
		if(getDoubleArray(intResultsRef,(char*)impactStr, &impact)
						==ND_FAILURE) {
			sprintf(msg,"Unable to retrieve %s",impactStr);
			ANL_UTIL_LogSevere(env,"teva.analysis.server",msg);
			return;
		}
		for (int j=0; j<numImpacts; j++) {
		    thisObjective->updateImpacts(witness[j]-1,impact[j],td[j]);
		}
		nd_free(witness);
		nd_free(td);
		nd_free(impact);
		thisObjective->outputScenarioImpactData(x_nSims,x_mem->sources);
		thisObjective->resetForScenario();
	}
}


/* 
 * called usually only once after the scenario is done to obtain the completed analysis results
 */
void tso2ImpactAnalysis_aggr_getResults(NamedDataRef *analysisOptionsRef,NamedDataRef *resultsRef)
{
	// This is a curious routine.  It is supposed to load a results
	// NamedDataRef with the results of the analysis.  These results
	// seem to be used in two ways: (1) to enable TEVA to write a binary
	// file containing the information, and (2) for the _writeResults
	// function below to read.
	//
	// Since we don't want any binary files, the _writeResults function
	// has access to the same static data that all routines in this file
	// do, and since there's only one aggregation server, there's no
	// need for this last communication between TEVA and this module.
}

/* 
 * called usually only once after the scenario is done to obtain the completed analysis results
 */
void tso2ImpactAnalysis_aggr_writeResults(NamedDataRef *analysisOptionsRef,NamedDataRef *resultsRef, const char *dir, char *fn) {
	char absoluteOutputDir[1024],absoluteSourceDir[1024];
	JNIEnv *env = analysisOptionsRef->env;
	char msg[256];
	char tfn[256];
	char *prefix;
	FILE *fp;
	int i;
	x_env=env;

	//
	// ignore the empty parameters and use the data we can already
	// access.
	//
	
	sprintf(msg,"in aggr_writeResults(): dir=%s",dir);
	ANL_UTIL_LogFine(env,"teva.analysis.server",msg);
	realpath(dir,absoluteOutputDir);
	sprintf(msg,"in aggr_writeResults(): absoluteOutputDir=%s",absoluteOutputDir);
	ANL_UTIL_LogFine(env,"teva.analysis.server",msg);
	realpath(".",absoluteSourceDir);
	sprintf(msg,"in aggr_writeResults(): absoluteSourceDir=%s",absoluteSourceDir);
	ANL_UTIL_LogFine(env,"teva.analysis.server",msg);

	if(getString(analysisOptionsRef,"outputFileRoot",&prefix)==ND_FAILURE) return;

	sprintf(tfn,"%s.nodemap",prefix);
	strcat(fn,tfn);
	fp=fopen(tfn,"w");
	for(i=0;i<x_mem->net->info->nnodes;i++) {
	  fprintf(fp,"%d %s\n",i+1,x_mem->nodeinfo[i].id);
	}
	fclose(fp);
	std::vector<ObjectiveBase*>::iterator it;
	for (it=theObjectives.begin(); it!=theObjectives.end();it++) {
		ObjectiveBase *thisObjective=(*it);
		const char *sfx = thisObjective->impactFilenameSuffix().c_str();
		char baseFn[256];
		sprintf(baseFn,"%s_%s",prefix,sfx);
		if(strlen(fn) > 0) strcat(fn,",");
		strcat(fn,baseFn);
		if(strlen(dir)>0)
		  sprintf(tfn,"%s/%s",dir,baseFn);
		else
		  sprintf(tfn,"%s",baseFn);
		// if the destination directory is the same as the current working 
		// directory (where all the impact files are written), we don't need to do
		// anything else here
		if(strcmp(absoluteOutputDir,absoluteSourceDir)!=0) {
		  sprintf(msg,"in aggr_writeResults(): moving file %s to %s",baseFn,tfn);
		  ANL_UTIL_LogFine(env,"teva.analysis.server",msg);
		  rename(baseFn,tfn);
		}

		sprintf(baseFn,"%s_%s.id",prefix,sfx);
		if(strlen(fn) > 0) strcat(fn,",");
		strcat(fn,baseFn);
		if(strlen(dir)>0)
		  sprintf(tfn,"%s/%s",dir,baseFn);
		else
		  sprintf(tfn,"%s",baseFn);
		// if the destination directory is the same as the current working 
		// directory (where all the impact files are written), we don't need to do
		// anything else here
		if(strcmp(absoluteOutputDir,absoluteSourceDir)!=0) {
		  sprintf(msg,"in aggr_writeResults(): moving file %s to %s",baseFn,tfn);
		  ANL_UTIL_LogFine(env,"teva.analysis.server",msg);
		  rename(baseFn,tfn);
		}

	}
	nd_free(prefix);
}

/* 
 * called once after the scenario has been completed
 */
void tso2ImpactAnalysis_aggr_shutdown(NamedDataRef *analysisOptionsRef)
{
	char msg[256];

	JNIEnv *env = analysisOptionsRef->env;

	sprintf(msg,"There were %d simulations", x_nSims);
	ANL_UTIL_LogInfo(env,"teva.analysis.server",msg);

	finalizeTheObjectives(theObjectives);


	// should be last in case any others need data from x_mem
	freeMemory(x_mem,Aggregation);
/*
	free(x_aveNonzero);
*/
}
};
