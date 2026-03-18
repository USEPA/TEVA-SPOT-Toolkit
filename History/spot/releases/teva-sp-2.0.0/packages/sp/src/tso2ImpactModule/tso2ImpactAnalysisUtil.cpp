
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

extern "C" {

#include "loggingUtils.h"
#include "NamedData.h"
#include "HealthImpacts.h"

bool get_analysisOptions(NamedDataRef *analysisOptionsRef,
			 NamedDataRef *simResultsRef,
      		       std::string&outputFilePrefix,
      		       std::string&tsoFileOrDirectoryName,
      		       int&detectionDelay,
      		       double&minQuality,
      		       bool&mcSelected,
      		       bool&vcSelected,
      		       bool&nfdSelected,
      		       bool&tdSelected,
      		       bool&ecSelected,
      		       std::string&epanetInputFilename,
      		       bool&pkSelected,
      		       bool&peSelected,
      		       std::string&taiInputFilename,
      		       std::string&tsoPattern,
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

	if (ecSelected == true) {
		JNIEnv *env = analysisOptionsRef->env;
		ANL_UTIL_LogSevere(env,"anl.teva", "analysis module "
			" cannot yet support extent of"
			" contamination objective");
		return false;
	}

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
      		 std::string&tsoFileOrDirectoryName,
      		 int&detectionDelay,
      		 double&minQuality,
      		 bool&mcSelected,
      		 bool&vcSelected,
      		 bool&nfdSelected,
      		 bool&tdSelected,
      		 bool&ecSelected,
      		 std::string&epanetInputFilename,
      		 bool&pkSelected,
      		 bool&peSelected,
      		 std::string&taiInputFilename,
      		 std::string&tsoPattern)
{
	JNIEnv *env = analysisOptionsRef->env;

	printf("*****************loadOptions\n");

	PMem       mem;

	if (!get_analysisOptions(analysisOptionsRef,
				 simResultsRef,
      		       outputFilePrefix,
      		       tsoFileOrDirectoryName,
      		       detectionDelay,
      		       minQuality,
      		       mcSelected,
      		       vcSelected,
      		       nfdSelected,
      		       tdSelected,
      		       ecSelected,
      		       epanetInputFilename,
      		       pkSelected,
      		       peSelected,
      		       taiInputFilename,
      		       tsoPattern,
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
void initTso2ImpactAfterTsoRead(
	int& numNodes, clock_t&start, clock_t& stop, 
	PNodeInfo&nodes, PLinkInfo&links,
	PNetInfo&net, PTSO&tso, PSource&sources,
	std::string&outputFilePrefix,
	std::string&tsoFileOrDirectoryName,
	int&detectionDelay,
	double&minQuality,
	bool&mcSelected,
	bool&vcSelected,
	bool&nfdSelected,
	bool&tdSelected,
	bool&ecSelected,
	std::string&epanetInputFilename,
	bool&pkSelected,
	bool&peSelected,
	std::string&taiInputFilename,
	PMem&assessMem,
	std::string&tsoPattern,
	std::string&sensorInputFilename,
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
	  VolumeContaminatedWaterConsumedObjective *vcObj=new VolumeContaminatedWaterConsumedObjective;
	  theObjectives.push_back(vcObj);
	}
      if(nfdSelected==true)
	{
	  NumberFailedDetectionsObjective *nfdObj=new NumberFailedDetectionsObjective;
	  theObjectives.push_back(nfdObj);
	}
      if(tdSelected==true)
	{
	  TimeToDetectionObjective *tdObj=new TimeToDetectionObjective;
	  theObjectives.push_back(tdObj);
	}
      if(ecSelected==true)
	{
	  ExtentOfContaminationObjective *ecObj=new ExtentOfContaminationObjective;

	  std::map<std::pair<int,int>,double> pipeLengthsIndexBased;

	  if(tso->fileVersion>=6)
	    {
	      std::cout << "Extracting pipe lengths from TSO prologoue" << std::endl << std::endl;

	      for(int i=0;i<net->nlinks;i++)
		{
		  // by convention, valves and pumps have lengths equal to 0 in the TSO prologue
		  if(links[i].length>0)
		    {
		      pipeLengthsIndexBased[std::make_pair(int(links[i].from-1),int(links[i].to-1))]=links[i].length;
		    }
		}
	    }
	  else
	    {
	      if(epanetInputFilename.size()==0)
		{
		  std::cout << "***TSO file does not embed pipe lengths and no EPANET input file was specified; the latter is required for compuation of the ec objective" << std::endl;
		  exit(1);
		}
	    
	      std::cout << "Reading EPANET input file to retrieve pipe lengths" << std::endl;
  
	      ENCHECK(ENopen((char*)epanetInputFilename.c_str(),"rpt","junk.out"));
	      std::cout << std::endl << std::endl; // to flush the EPANET line, which isn't new-line terminated

	      // maps to/from pairs - in string format - to pipe lengths.
	      // only contains link pipe types (=1) that are open. these by definition
	      // are always open, as they don't have check-valves.

	      int nlinks;
	      ENCHECK(ENgetcount(EN_LINKCOUNT,&nlinks));

	      std::map<std::pair<std::string,std::string>,double> pipeLengthsIDBased;

	      // the EPANET link indicies are 1-based
	      for(int i=1;i<=nlinks;i++)
		{
		  char id[1000]; // currently not used
		  int fromNodeIndex;
		  int toNodeIndex;
		  int type; // 1 is a pipe, the only type we care about (this is a static pipe - a pipe with a check valve is 0, but I don't think we need to worry about those)
		  float length; // in feet
		  float status; // 0=open, 1=closed (contrary to the EPANET toolkit documentation)
		  char fromNodeID[1000];
		  char toNodeID[1000];

		  ENCHECK(ENgetlinkid(i,id));
		  ENCHECK(ENgetlinknodes(i,&fromNodeIndex,&toNodeIndex));
		  ENCHECK(ENgetlinktype(i,&type));
		  ENCHECK(ENgetlinkvalue(i,EN_LENGTH,&length));
		  ENCHECK(ENgetlinkvalue(i,EN_STATUS,&status));      
		  ENCHECK(ENgetnodeid(fromNodeIndex,fromNodeID));
		  ENCHECK(ENgetnodeid(toNodeIndex,toNodeID));

		  // if this is a pure pipe w/o a check-valve, then add it to the pipe length map
		  // NOTE: need to revisit the ability/requirement to handle check-valved pipes, or other types of links that are more dynamic.
		  if((type==1)&&(status==0))
		    {
		      pipeLengthsIDBased[std::make_pair(std::string(fromNodeID),std::string(toNodeID))]=length;
		    }
		}

	      ENCHECK(ENclose());

	      // given the node ID -> TEVA index map, convert the pipe lengths map to TEVA indicies //

	      for(std::map<std::pair<std::string,std::string>,double>::iterator iter=pipeLengthsIDBased.begin();iter!=pipeLengthsIDBased.end();iter++)
		{
		  std::string fromNodeID=(*iter).first.first;
		  std::string toNodeID=(*iter).first.second;

		  if(nodeIDToIndexMap.find(fromNodeID)==nodeIDToIndexMap.end())
		    {
		      std::cerr << "***No TEVA node index for node ID=" << fromNodeID << std::endl;
		      exit(1);
		    }

		  if(nodeIDToIndexMap.find(toNodeID)==nodeIDToIndexMap.end())
		    {
		      std::cerr << "***No TEVA node index for node ID=" << toNodeID << std::endl;
		      exit(1);
		    }
      
		  int fromNodeIndex=nodeIDToIndexMap[fromNodeID];
		  int toNodeIndex=nodeIDToIndexMap[toNodeID];

		  pipeLengthsIndexBased[std::make_pair(fromNodeIndex,toNodeIndex)]=(*iter).second;
		}
	    }

	  ecObj->setPipeLengths(pipeLengthsIndexBased);

	  theObjectives.push_back(ecObj);
	}
      if(pkSelected==true)
	{
	  PopulationKilledObjective *pkObj=
			new PopulationKilledObjective(assessMem);
	  theObjectives.push_back(pkObj);
	}
      if(peSelected==true)
	{
          std::cout << "initTso2ImpactAfterTsoRead: pe" << std::endl;
	  PopulationExposedObjective*peObj = 
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

