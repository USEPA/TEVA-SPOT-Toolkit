#include <limits.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "tso2ImpactAnalysis.h"
#include <sp/impacts.h>

extern "C" {
#include "ExternalAnalysis.h"
#include "NamedData.h"


static PMem x_mem;
static int x_nscenarios;

static std::string outputFilePrefix;
static std::string tsoFileOrDirectoryName;
static int detectionDelay=0;
static double minQuality=0.0;
static bool mcSelected=false;
static bool vcSelected=false;
static bool nfdSelected=false;
static bool tdSelected=false;
static bool ecSelected=false;
static bool pkSelected=false;
static bool peSelected=false;
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

/* 
 * called once after the first simulaiton has been completed 
 * (so demands are there)
 */
int tso2ImpactAnalysis_ia_initialize(NamedDataRef *analysisOptionsRef, 
		 		     NamedDataRef *simResultsRef)
{
	// at this point, simResultsRef does not conatin any quality, 
	// but will contain demands 
	x_mem = loadOptions(analysisOptionsRef, simResultsRef,
		       IntermediateAnalysis,
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
	x_nscenarios=0;
	PLinkInfo dummy=NULL;	// no link info in simResultsRef yet
	std::string s = "";
        initTso2ImpactModuleData(
		analysisOptionsRef, simResultsRef,
                numNodes, start, stop,
                x_mem->node->info, dummy,
                x_mem->net->info, x_mem->tso, x_mem->sources,
                s,
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

	IngestionRate(x_mem->net,x_mem->node);
	dumpHIAInputParameters(x_mem);

	return NON_REMOTE;	// pass results back by value
}

/* 
 * called once after every simulation is done to perform the 
 * intermediate analysis.  The data placed in the intResultsRef will 
 * be passed along to the aggregation server
 */
void tso2ImpactAnalysis_ia_newSimResults(NamedDataRef *analysisOptionsRef,
				    NamedDataRef *simResultsRef, 
				    NamedDataRef *intResultsRef)
{
	int i,j, nsteps, nnodes;

	char *simID;

	x_nscenarios++;
	nsteps=x_mem->net->info->nsteps;
	nnodes=x_mem->net->info->nnodes;

	int *numNonzero= (int*)calloc(nnodes,sizeof(int));
	for(i=0;i<nnodes;i++) {
		numNonzero[i] = 0;
	}
	for(j=0;j<nnodes;j++) {
		x_mem->node[j].info->nz[0] = 0;
	}
	for(i=0;i<nsteps;i++) {
		float *tres;
		if(getFloatArrayAtIndex(simResultsRef,"quality",
					i,&tres)==ND_FAILURE) {
			return;
		}
		for(j=0;j<nnodes;j++) {
			float tc = tres[j];
			if(tc > 0) {
				x_mem->node[j].info->nz[0]++;
				numNonzero[j]++;
			}
			x_mem->node[j].info->c[0][i] = tc;
//			if(tc>0) printf("%d %d %f %f %f\n",i,j,tc,x_mem->node[j].info->c[0][i],x_mem->nodeinfo[j].c[0][i]);
		}
		nd_free(tres);
	}
//	for(j=0;j<nnodes;j++) {
//	  printf("%d\t",numNonzero[j]);
//	}
//      printf("\n");
	int numInjections = 0;
	if(getString(simResultsRef,"injectionID",&simID)==ND_FAILURE) return;
	if(getInt(simResultsRef,"numInjections",&numInjections)==ND_FAILURE) 
		return;
	int *sourceIndices;
	x_mem->net->info->nsources=numInjections;
	if(getIntArray(simResultsRef, "injectionIndices", 
			&sourceIndices)==ND_FAILURE) 
		return;

	for (int i=0; i<numInjections; i++) {
		x_mem->sources[i].SourceNodeIndex = sourceIndices[i];
	}

	// the second argument to processScenario is the simulation id,
	// an int in the standalone tso2Impact which doesn't exist in 
	// simResultsRef.  The analysis server will assign a unique 
	// identifier to the sim results when it gets them.  Pass a 0
	// to processScenario -- it doesn't need to know a sim id in the
	// module.
	processScenario(theObjectives, 0, detectionDelay, minQuality,
			x_mem->net->info, x_mem->nodeinfo, x_mem->linkinfo, 
			x_mem->sources, intResultsRef);
	// processScenario called ObjectiveBase::construct

	if(addVariable(intResultsRef,"injectionID",  STRING_TYPE )==ND_FAILURE) return;
	if(addVariable(intResultsRef,"numInjections",  INT_TYPE )==ND_FAILURE) return;
	if(addVariable(intResultsRef,"injectionIndices",  INT_ARRAY_TYPE )==ND_FAILURE) return;

	if(setString(intResultsRef,"injectionID",   simID)==ND_FAILURE) return;
	if(setInt(intResultsRef,   "numInjections", numInjections)==ND_FAILURE) return;
	if(setIntArray(intResultsRef,"injectionIndices",    sourceIndices,numInjections)==ND_FAILURE) return;

	nd_free(sourceIndices);
	nd_free(simID);
}

/* 
 * called once after the scenario has been completed
 */
void tso2ImpactAnalysis_ia_shutdown(NamedDataRef *analysisOptionsRef)
{
	finalizeTheObjectives(theObjectives);
	freeMemory(x_mem,IntermediateAnalysis);
}
};
