#include <sp/scenario.h>
#include <utilib/exception_mngr.h>
#include "impacts.h"
#include "MassConsumedObjective.h"
#include "PopulationExposedObjective.h"
#include "PopulationKilledObjective.h"
#include "NumberFailedDetectionsObjective.h"
#include "TimeToDetectionObjective.h"
#include "VolumeContaminatedWaterConsumedObjective.h"
#include "ExtentOfContaminationObjective.h"

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
	PMem& assessMem,
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

      if(mcSelected==true)
	{
	  MassConsumedObjective *mcObj=new MassConsumedObjective;
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
      if(pkSelected==true || peSelected==true) 
	{
	  PInputFileData taiData = loadTAIFileData(taiInputFilename.c_str());
	  assessMem = new Mem;
	  assessMem->linkinfo = links;
	  assessMem->nodeinfo = nodes;
	  InitializeAssessMemory(assessMem, net);
	  processTAIData(taiData, assessMem);
	  freeInputFileData(&taiData);
	  Population(assessMem->net, assessMem->dr, assessMem->node);
	  IngestionRate(assessMem->net, assessMem->node);

      	}
      if(pkSelected==true)
	{
	  PopulationKilledObjective *pkObj=
			new PopulationKilledObjective(assessMem);
	  theObjectives.push_back(pkObj);
	}
      if(peSelected==true)
	{
	  PopulationExposedObjective *peObj=
				new PopulationExposedObjective(assessMem);
	  theObjectives.push_back(peObj);
	}

      for(size_t i=0;i<theObjectives.size();i++)
	{
	  theObjectives[i]->setSensorLocations(sensorLocations);
	  theObjectives[i]->init(outputFilePrefix,net,nodes,links,detectionDelay);
	}

}

int main(int argc, char **argv)
{
  try 
    {
      PNodeInfo nodes;
      PLinkInfo links;
      PNetInfo net;
      PTSO tso;
      PMem assessMem;
      PSource sources;
      sources=(PSource)calloc(MAXSOURCES,sizeof(Source));

      std::string outputFilePrefix;
      std::string tsoFileOrDirectoryName;
      int detectionDelay=0;
      double minQuality=0.0;
      bool mcSelected=false;
      bool vcSelected=false;
      bool nfdSelected=false;
      bool tdSelected=false;
      bool ecSelected=false;
      std::string epanetInputFilename;
      bool pkSelected=false;
      bool peSelected=false;
      std::string taiInputFilename;
      std::string tsoPattern;
      std::string sensorInputFilename;
      std::list<int> sensorLocations;
      int numNodes;
      std::string nodeMapFileName;
      std::string scenarioMapFileName;
      std::ofstream nodeMapFile;
      std::ofstream scenarioMapFile;
      std::vector<std::string> nodeIndexToIDMap;
      std::map<std::string,int> nodeIDToIndexMap; // maps node textual IDs to 0-based TEVA node indicies
      clock_t start, stop;
      std::vector<ObjectiveBase*> theObjectives;

      process_arguments(argc, argv, nodes, links, net, tso, sources,
			outputFilePrefix,tsoFileOrDirectoryName,
			detectionDelay, minQuality, mcSelected, vcSelected,
			nfdSelected, tdSelected, ecSelected,epanetInputFilename,
			pkSelected, peSelected, taiInputFilename, tsoPattern,
      			sensorInputFilename, sensorLocations);


      std::cout << "main: after process_arguments: net: " << net << std::endl;
      initStandaloneTso2Impact(numNodes, start, stop, nodes, links, net, 
			tso, sources, 
			outputFilePrefix, tsoFileOrDirectoryName,
			detectionDelay, minQuality, mcSelected, vcSelected,
			nfdSelected, tdSelected, ecSelected,epanetInputFilename,
			pkSelected, peSelected, taiInputFilename, assessMem,
			tsoPattern,
			sensorInputFilename, sensorLocations, nodeMapFileName,
			scenarioMapFileName, nodeMapFile, scenarioMapFile,
			theObjectives, nodeIndexToIDMap, nodeIDToIndexMap);

      //////////////////////////////////////////////////////////////
      int numScenarios = 0;
      start = clock();
      while(TSO_ReadSimulationResults(tso,net,nodes,sources)>0) 
	{
	  numScenarios++; 
	
	  std::cout << "Processing simulation results for scenario=" << numScenarios << std::endl;

          #ifdef OUTPUT_TSO_DETAIL

	  std::cout << "Number of sources=" << net->nsources << std::endl;
	  std::cout << "Number of species=" << net->nspecies << std::endl;
	  std::cout << std::endl;

	  for(int i=0;i<net->nsources;i++)
	    {
	      // recall this index is 1-based
	      std::cout << "Source node index=" << sources[i].SourceNodeIndex << ", ID=" << sources[i].SourceNodeID << std::endl;
	      std::cout << "Source node type=" << sources[i].SourceType << std::endl;
	      std::cout << "Source node start=" << sources[i].SourceStart << std::endl;
	      std::cout << "Source node stop=" << sources[i].SourceStop << std::endl;
	      std::cout << "Source node strength=" << sources[i].SourceStrength << std::endl;
	      std::cout << std::endl;
	    }

	  for(int i=0;i<net->nnodes;i++)
	    {
	      std::cout << "Node index=" << i+1 << ", ID=" << nodes[i].id << " qualities=";
	      for(int j=0;j<net->nsteps;j++)
		{
		  std::cout << nodes[i].c[0][j] << " ";
		}
	      std::cout << std::endl;
	    }

          #endif

	  processScenario(theObjectives,numScenarios,
			  detectionDelay,minQuality,
			  net,nodes,links,sources, NULL);
	  for(int i=0;i<net->nsources;i++)
            scenarioMapFile << sources[i].SourceNodeIndex << " " << 
			sources[i].SourceNodeID << " " <<
			sources[i].SourceType << " " <<
			sources[i].SourceStart << " " << 
			sources[i].SourceStop << " " << 
			sources[i].SourceStrength << std::endl;
	}

      ////////////////////////////////////////
      // perform cleanup for each objective //
      ////////////////////////////////////////

      finalizeTheObjectives(theObjectives);

      ////////////////////////////////
      // clean up the TSO interface //
      ////////////////////////////////

      TSO_Close(&tso);
      TSO_ReleaseNetworkData(&nodes,&links,net);
      TSO_ReleaseNetworkInfo(&net);
      free(sources);
      stop=clock();
      printf("Processed input in %7.3f seconds\n", (float)(stop-start)/(float)CLOCKS_PER_SEC);

      ///////////////////////////
      // dump final stats/info //
      ///////////////////////////

      std::cout << std::endl;
      std::cout << "Total number of scenarios=" << numScenarios << std::endl;
      std::cout << std::endl;

      std::cout << "Node map file written to file=" << nodeMapFileName << std::endl;
      std::cout << std::endl;

      if(mcSelected==true)
	{
	  std::cout << "Mass consumed impacts written to file=" << outputFilePrefix << "_" << MassConsumedObjective::ourImpactOutputFileSuffix << std::endl;
	  //std::cout << "Mass consumed sensor evaluation written to file=" << outputFilePrefix << "_" << MassConsumedObjective::ourEvaluationOutputFileSuffix << std::endl;    
	}
      if(vcSelected==true)
	{
	  std::cout << "Volume of contaminated water consumed impacts written to file=" << outputFilePrefix << "_" << VolumeContaminatedWaterConsumedObjective::ourImpactOutputFileSuffix << std::endl;
	  //std::cout << "Volume of contaminated water consumed sensor evaluation written to file=" << outputFilePrefix << "_" << VolumeContaminatedWaterConsumedObjective::ourEvaluationOutputFileSuffix << std::endl;
	}
      if(nfdSelected==true)
	{
	  std::cout << "Number failed detections impacts written to file=" << outputFilePrefix << "_" << NumberFailedDetectionsObjective::ourImpactOutputFileSuffix << std::endl;
	  //std::cout << "Number failed detections sensor evaluation written to file=" << outputFilePrefix << "_" << NumberFailedDetectionsObjective::ourEvaluationOutputFileSuffix << std::endl;
	}
      if(tdSelected==true)
	{
	  std::cout << "Time to detection impacts written to file=" << outputFilePrefix << "_" << TimeToDetectionObjective::ourImpactOutputFileSuffix << std::endl;
	  //std::cout << "Time to detection sensor evaluation written to file=" << outputFilePrefix << "_" << TimeToDetectionObjective::ourEvaluationOutputFileSuffix << std::endl;
	}
      if(ecSelected==true)
	{
	  std::cout << "Extent of contamination impacts written to file=" << outputFilePrefix << "_" << ExtentOfContaminationObjective::ourImpactOutputFileSuffix << std::endl;
	  //std::cout << "Extent of contamination sensor evaluation written to file=" << outputFilePrefix << "_" << ExtentOfContaminationObjective::ourEvaluationOutputFileSuffix << std::endl;
	}
      if(pkSelected==true)
	{
	  std::cout << "Population killed impacts written to file=" << outputFilePrefix << "_" << PopulationKilledObjective::ourImpactOutputFileSuffix << std::endl;
	  //std::cout << "Population killed sensor evaluation written to file=" << outputFilePrefix << "_" << PopulationKilledObjective::ourEvaluationOutputFileSuffix << std::endl;
	}
      if(peSelected==true)
	{
	  std::cout << "Population exposed impacts written to file=" << outputFilePrefix << "_" << PopulationExposedObjective::ourImpactOutputFileSuffix << std::endl;
	  //std::cout << "Population exposed sensor evaluation written to file=" << outputFilePrefix << "_" << PopulationExposedObjective::ourEvaluationOutputFileSuffix << std::endl;
	}
      if(peSelected==true || pkSelected==true) 
	{
	  //FreeAssessMemory(assessMem);
	}
    }

  STD_CATCH(;)

  return 1;
}
