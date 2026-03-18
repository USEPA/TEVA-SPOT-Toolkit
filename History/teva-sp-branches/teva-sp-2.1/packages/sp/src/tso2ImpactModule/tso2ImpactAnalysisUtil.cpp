
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#include <sp/impacts.h>

#include <sp/ObjectiveBase.h>
#include <sp/PopulationExposedObjective.h>
#include <sp/PopulationKilledObjective.h>
#include <sp/NumberFailedDetectionsObjective.h>
#include <sp/TimeToDetectionObjective.h>
#include <sp/VolumeContaminatedWaterConsumedObjective.h>
#include <sp/ExtentOfContaminationObjective.h>

#include "tso2ImpactAnalysis.h"
#include "ModuleMassConsumedObjective.h"
#include "ModulePopulationExposedObjective.h"
#include "ModuleExtentOfContaminationObjective.h"
#include "ModuleVolumeContaminatedWaterConsumedObjective.h"
#include "ModuleTimeToDetectionObjective.h"
#include "ModulePopulationKilledObjective.h"
#include "ModuleNumberFailedDetectionsObjective.h"
extern "C" {

#include "loggingUtils.h"
#include "NamedData.h"
#include "HealthImpacts.h"

bool get_analysisOptions(NamedDataRef *analysisOptionsRef,
			 NamedDataRef *simResultsRef,
      		       std::string&outputFilePrefix,
      		       int&detectionDelay,
      		       double&minQuality,
      		       bool&mcSelected,
      		       bool&vcSelected,
      		       bool&nfdSelected,
      		       bool&tdSelected,
      		       bool&ecSelected,
      		       bool&pkSelected,
      		       bool&peSelected,
		       PMem& mem,
		       ModuleType componentType)
{
	char     *tstr;
	char msg[256];
	JNIEnv *env = analysisOptionsRef->env;

	ANL_UTIL_LogFiner(env,"anl.teva","Unable to retrieve numTimeSteps");
	ANL_UTIL_LogFiner(env,"anl.teva","*****************get_analysisOptions");
	fflush(stdout);

	if(getString(analysisOptionsRef, "outputFileRoot", &tstr)==ND_FAILURE) {
		ANL_UTIL_LogSevere(env,"anl.teva","*****************failed to get outputFileRoot\n");
		return false;
	}
	ANL_UTIL_LogFiner(env,"anl.teva","*****************got file root");
	outputFilePrefix = tstr;
	ANL_UTIL_LogFiner(env,"anl.teva","*****************set file prefix");
	if(getInt(analysisOptionsRef, "detectionDelay", 
		  &detectionDelay)==ND_FAILURE)  {
		ANL_UTIL_LogSevere(env,"anl.teva","*****************failed to get detectionDelay");
		return false;
	}
	ANL_UTIL_LogFiner(env,"anl.teva","*****************get_analysisOptions");
	fflush(stdout);

	if(getInt(analysisOptionsRef, "detectionDelay", 
		  &detectionDelay)==ND_FAILURE)  {
		ANL_UTIL_LogSevere(env,"anl.teva","*****************failed to get detectionDelay");
		return false;
	}
	sprintf(msg,"*****************got detectionDelay: %d", detectionDelay);
	ANL_UTIL_LogFiner(env,"anl.teva",msg);

	if(getDouble(analysisOptionsRef, "minQuality", 
		  &minQuality)==ND_FAILURE)  {
		ANL_UTIL_LogSevere(env,"anl.teva","*****************failed to get minQuality");
		return false;
	}
	sprintf(msg,"*****************got minQuality: %f", minQuality);
	ANL_UTIL_LogFiner(env,"anl.teva",msg);

	int mcs;
	if(getInt(analysisOptionsRef, "mcSelected", 
		  &mcs)==ND_FAILURE)  {
		ANL_UTIL_LogSevere(env,"anl.teva","*****************failed to get mcSelected");
		return false;
	}
	mcSelected = (mcs != 0);
	sprintf(msg,"*****************got mcSelected: %d", mcs);
	ANL_UTIL_LogFiner(env,"anl.teva",msg);

	int vcs;
	if(getInt(analysisOptionsRef, "vcSelected", 
		  &vcs)==ND_FAILURE)  {
		ANL_UTIL_LogSevere(env,"anl.teva","*****************failed to get vcSelected");
		return false;
	}
	vcSelected = (vcs != 0);
	sprintf(msg,"*****************got vcSelected: %d", vcs);
	ANL_UTIL_LogFiner(env,"anl.teva",msg);

	int nfds;
	if(getInt(analysisOptionsRef, "nfdSelected", 
		  &nfds)==ND_FAILURE) {
		ANL_UTIL_LogSevere(env,"anl.teva","*****************failed to get nfdSelected");
		return false;
	}
	nfdSelected = (nfds != 0);
	sprintf(msg,"*****************got nfdSelected: %d", nfds);
	ANL_UTIL_LogFiner(env,"anl.teva",msg);

	int ecs;
	if(getInt(analysisOptionsRef, "ecSelected", 
		  &ecs)==ND_FAILURE) {
		ANL_UTIL_LogSevere(env,"anl.teva","*****************failed to get ecSelected");
		return false;
	}
	ecSelected = (ecs != 0);
	sprintf(msg,"*****************got ecSelected: %d", ecs);
	ANL_UTIL_LogFiner(env,"anl.teva",msg);

	int pks;
	if(getInt(analysisOptionsRef, "pkSelected", 
		  &pks)==ND_FAILURE) {
		ANL_UTIL_LogSevere(env,"anl.teva","*****************failed to get pkSelected");
		return false;
	}
	pkSelected = (pks != 0);
	sprintf(msg,"*****************got pkSelected: %d", pks);
	ANL_UTIL_LogFiner(env,"anl.teva",msg);

	int pes;
	if(getInt(analysisOptionsRef, "peSelected", 
		  &pes)==ND_FAILURE) {
		ANL_UTIL_LogSevere(env,"anl.teva","*****************failed to get peSelected");
		return false;
	}
	peSelected = (pes != 0);
	sprintf(msg,"*****************got peSelected: %d", pes);
	ANL_UTIL_LogFiner(env,"anl.teva",msg);

	NamedDataRef *hiaData;
	if(getNamedData(analysisOptionsRef, "HIAParameters", 
		  &hiaData)==ND_FAILURE) {
		ANL_UTIL_LogSevere(env,"anl.teva","*****************failed to HIAParameters named data");
		return false;
	}

	mem=loadHIAOptions(hiaData,simResultsRef,componentType);

	ANL_UTIL_LogFiner(env,"anl.teva","get_analysisOptions done");
	return true;
}

PMem loadOptions(NamedDataRef *analysisOptionsRef, 
		 NamedDataRef *simResultsRef, ModuleType componentType,
      		 std::string&outputFilePrefix,
      		 int&detectionDelay,
      		 double&minQuality,
      		 bool&mcSelected,
      		 bool&vcSelected,
      		 bool&nfdSelected,
      		 bool&tdSelected,
      		 bool&ecSelected,
      		 bool&pkSelected,
      		 bool&peSelected)
{
	JNIEnv *env = analysisOptionsRef->env;

	printf("*****************loadOptions\n");

	PMem       mem;

	if (!get_analysisOptions(analysisOptionsRef,
				 simResultsRef,
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
		       mem,
		       componentType))
		return NULL;

	ANL_UTIL_LogFiner(env,"anl.teva","*****************loadOptions: done\n");
	return mem;
}

void freeMemory(PMem mem, ModuleType componentType) {

	freeHIAMemory(mem,componentType);
}

}; // extern "C"

// **********************************************************************
// **initTso2ImpactAfterTsoRead() ***************************************
// **********************************************************************
// **
// **      This function had to be duplicated in the tso2ImpactModule
// **      world in order to compile the standalone tso2Impact without 
// **      libANLUtils.so dependency.  In other words, this function
// **      can't be included in libsp.a because it would conflict with
// **      the version in libTso2ImpactModule.so. 
// **********************************************************************
void initTso2ImpactModuleData(
	NamedDataRef *analysisOptionsRef, 
	NamedDataRef *simResultsRef, 
	int& numNodes, clock_t&start, clock_t& stop, 
	PNodeInfo&nodes, PLinkInfo&links,
	PNetInfo&net, PTSO&tso, PSource&sources,
	std::string&outputFilePrefix,
	int&detectionDelay,
	double&minQuality,
	bool&mcSelected,
	bool&vcSelected,
	bool&nfdSelected,
	bool&tdSelected,
	bool&ecSelected,
	bool&pkSelected,
	bool&peSelected,
	PMem&assessMem,
	std::list<int>&sensorLocations,
	std::string&nodeMapFileName,
	std::string&scenarioMapFileName,
	std::ofstream&nodeMapFile,
	std::ofstream&scenarioMapFile,
	std::vector<ObjectiveBase*>&theObjectives,
        std::vector<std::string>&nodeIndexToIDMap,
        std::map<std::string,int>&nodeIDToIndexMap)

{
      ///////////////////////////////////////
      // initialize the various objectives //
      ///////////////////////////////////////
      std::cout << "initTso2ImpactAfterTsoRead" << std::endl;
//      std::cerr << net->nnodes << std::endl;
//      for (int i=0; i<net->nnodes; i++) {
//	std::cerr << i << " " << nodes[i].id << " " << nodes[i].q[0] << " " 
//	     << nodes[i].ntype << std::endl;
//      }

      if(mcSelected==true)
	{
      	  std::cout << "initTso2ImpactAfterTsoRead: mc" << std::endl;
	  ModuleMassConsumedObjective *mcObj=new ModuleMassConsumedObjective;
	  theObjectives.push_back(mcObj);
	}
      if(vcSelected==true)
	{
	  ModuleVolumeContaminatedWaterConsumedObjective *vcObj=new ModuleVolumeContaminatedWaterConsumedObjective;
	  theObjectives.push_back(vcObj);
	}
      if(nfdSelected==true)
	{
	  ModuleNumberFailedDetectionsObjective *nfdObj=new ModuleNumberFailedDetectionsObjective;
	  theObjectives.push_back(nfdObj);
	}
      if(tdSelected==true)
	{
	  ModuleTimeToDetectionObjective *tdObj=new ModuleTimeToDetectionObjective;
	  theObjectives.push_back(tdObj);
	}
      if(ecSelected==true)
	{
	  ModuleExtentOfContaminationObjective *ecObj=new ModuleExtentOfContaminationObjective;
	  
	  std::map<std::pair<int,int>,double> pipeLengthsIndexBased;
	  
	  int *linkStartIndices;
	  int *linkEndIndices;
	  float *linkLengths;

	  if(getIntArray(simResultsRef, "linkStartNodeIdx", &linkStartIndices)==ND_FAILURE) 
		return;
	  if(getIntArray(simResultsRef, "linkEndNodeIdx", &linkEndIndices)==ND_FAILURE) 
		return;
	  if(getFloatArray(simResultsRef, "linkLengths", &linkLengths)==ND_FAILURE) 
		return;

	  for(int i=0;i<net->nlinks;i++) {
	    // by convention, valves and pumps have lengths equal to 0 in the TSO prologue
	    if(linkLengths[i]>0)
	      {
		pipeLengthsIndexBased[std::make_pair(int(linkStartIndices[i]-1),int(linkEndIndices[i]-1))]=linkLengths[i];
	      }
	  }
	  nd_free(linkStartIndices);
	  nd_free(linkEndIndices);
	  nd_free(linkLengths);

	  ecObj->setPipeLengths(pipeLengthsIndexBased);
	  
	  theObjectives.push_back(ecObj);
	}
      if(pkSelected==true)
	{
	  ModulePopulationKilledObjective *pkObj=
			new ModulePopulationKilledObjective(assessMem);
	  theObjectives.push_back(pkObj);
	}
      if(peSelected==true)
	{
          std::cout << "initTso2ImpactAfterTsoRead: pe" << std::endl;
	  ModulePopulationExposedObjective*peObj = 
			new ModulePopulationExposedObjective(assessMem);
	  theObjectives.push_back(peObj);
	}

      for(size_t i=0;i<theObjectives.size();i++)
	{
	  theObjectives[i]->setSensorLocations(sensorLocations);
	  theObjectives[i]->init(outputFilePrefix,net,nodes,links,detectionDelay);
	}
      std::cout << "initTso2ImpactAfterTsoRead: done" << std::endl;

}

