#if 0
/// WEH 0 this file does not appear to be used, so I'm ifdef'ing it out.

#ifdef HAVE_CONFIG_H
#include <teva_config.h>
#endif

#include <sp/scenario.h>
#include <utilib/exception_mngr.h>
#include "impacts.h"
#include "MassConsumedObjective.h"
#if TEVA_SPOT_LIMITED
#include "PopulationExposedObjective.h"
#include "PopulationKilledObjective.h"
#include "PopulationDosedObjective.h"
#endif
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
	bool&pdSelected,
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
#if TEVA_SPOT_LIMITED
      if(pkSelected==true || peSelected==true || pdSelected == true)
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
      if(pdSelected==true)
	{
	  PopulationDosedObjective *pdObj=
			new PopulationDosedObjective(assessMem);
	  theObjectives.push_back(pdObj);
	}
      if(peSelected==true)
	{
	  PopulationExposedObjective *peObj=
				new PopulationExposedObjective(assessMem);
	  theObjectives.push_back(peObj);
	}
#endif

      for(size_t i=0;i<theObjectives.size();i++)
	{
	  theObjectives[i]->setSensorLocations(sensorLocations);
	  theObjectives[i]->init(outputFilePrefix,net,nodes,links,detectionDelay);
	}
      std::cout << "at end of initTso2ImpactAfterTsoRead\n";
}

/// Argument processing for the standalone version
void process_arguments(int argc, char **argv, PNodeInfo&nodes, PLinkInfo&links,
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
      		       bool&pdSelected,
      		       bool&peSelected,
      		       std::string&taiInputFilename,
      		       std::string&tsoPattern,
      		       std::string&sensorInputFilename,
      		       std::list<int>&sensorLocations)
{
      if(argc<3) {
	std::cerr << "\
\n\
tso2Impact - An application that reads a TSO file and creates one or more\n\
\timpact files that are used to formulate sensor placement problems.\n\
\n\
usage:\n\
\n\
   tso2Impact <output-prefix> <tso-directory-or-file> [options...]\n\
\n\
options:\n\
\n\
   -detectionDelay <minutes>\n\
	This parameter indicates that a delayed detection impact will be\n\
	computed for the specified delay.\n\
\n\
   -minQuality <threshold>\n\
	This parameter indicates the minimum threshold needed to perform\n\
	detection with a sensor.\n\
\n\
   -mc\n\
	If this flag is specified, an impact file wll be generated for the\n\
	'mass consumed' measure.\n\
\n\
   -vc\n\
	If this flag is specified, an impact file wll be generated for the\n\
	'volume consumed' measure.\n\
\n\
   -td\n\
	If this flag is specified, an impact file wll be generated for the\n\
	'time-to-detection' measure.\n\
\n\
   -nfd\n\
	If this flag is specified, an impact file wll be generated for the\n\
	'number-of-failed detections' measure.\n\
\n\
   -ec\n\
	If this flag is specified, an impact file wll be generated for the\n\
	'extent of contamination' measure.\n\
\n\
#if TEVA_SPOT_LIMITED
   -pk <tai-input-file>\n\
	If this parameter is specified, an impact file wll be generated for the\n\
	'population killed' measure.  The TAI input file is a threat_assess\n\
	input that specifies parameters like dosage, response, lethality, etc\n\
	Note: it is an intensive measure to compute.\n\
\n\
   -pd <tai-input-file>\n\
	If this parameter is specified, an impact file wll be generated for the\n\
	'population dosed' measure.  The TAI input file is a threat_assess\n\
	input that specifies parameters like dosage, response, lethality, etc\n\
	Note: it is an intensive measure to compute.\n\
\n\
   -pe <tai-input-file>\n\
	If this parameter is specified, an impact file wll be generated for the\n\
	'population exposed' measure.  The TAI input file is a threat_assess\n\
	input that specifies parameters like dosage, response, lethality, etc\n\
	This measure concerns the number of people 'sickened' by exposure.\n\
\n\
#endif
   -epanetin <file-name>\n\
	This is used for TSO file versions less than 6.0, when computation\n\
	of the 'ec' objective is specified.  Pipelengths are extracted from\n\
	the EPANET input file.\n\
\n\
   -tsoPattern <string>\n\
	This string specifies a regular expression for all input TSO files\n\
	when the files are stored in a directory (e.g. when generated by TEVA).\n\
\n\
arguments:\n\
\n\
   <output-prefix>\n\
	The prefix used for all files generated by tso2Impact.\n\
\n\
   <tso-director-or-file>\n\
	This arguement indicates either a TSO file (generated by tevasim) or\n\
	a directory name for TSO files (generated by TEVA).  If the later, then\n\
	the filenames must be specified with the -tsoPattern option.\n\
" << std::endl;
	  exit(1);
	}

      ////////////////////////////////////////////////////////
      // parse/proceess any optional command-line arguments //
      ////////////////////////////////////////////////////////

      outputFilePrefix = argv[1];

      tsoFileOrDirectoryName = argv[2];

      int currentArgIndex(3);
      while(currentArgIndex<argc)
	{
	  if(strcmp(argv[currentArgIndex],"-mc")==0) 
	    {
	      mcSelected=true;
	      std::cout << "Generating mass consumed impact file" << std::endl;
	    } 
	  else if(strcmp(argv[currentArgIndex],"-vc")==0) 
	    {
	      vcSelected=true;
	      std::cout << "Generating volume of contaminated water consumed impact file" << std::endl;
	    } 
	  else if(strcmp(argv[currentArgIndex],"-nfd")==0) 
	    {
	      nfdSelected=true;
	      std::cout << "Generating number of failed detections impact file" << std::endl;
	    } 
	  else if(strcmp(argv[currentArgIndex],"-td")==0) 
	    {
	      tdSelected=true;
	      std::cout << "Generating time to detection impact file" << std::endl;
	    } 
	  else if(strcmp(argv[currentArgIndex],"-ec")==0) 
	    {
	      ecSelected=true;
	      std::cout << "Generating extent of contamination impact file" << std::endl;
	    } 
#if TEVA_SPOT_LIMITED
	  else if(strcmp(argv[currentArgIndex],"-pk")==0) 
	    {
	      pkSelected=true;
	      if(currentArgIndex==(argc-1))
		{
		  std::cerr << "***TAI input file not supplied - required for computation of population fatalities objective" << std::endl;
		  exit(1);
		}
	      taiInputFilename=argv[++currentArgIndex];
	      std::cout << "Generating population killed impact file" << std::endl;
	    } 
	  else if(strcmp(argv[currentArgIndex],"-pd")==0) 
	    {
	      pdSelected=true;
	      if(currentArgIndex==(argc-1))
		{
		  std::cerr << "***TAI input file not supplied - required for computation of population dosed objective" << std::endl;
		  exit(1);
		}
	      taiInputFilename=argv[++currentArgIndex];
	      std::cout << "Generating population dosed impact file" << std::endl;
	    } 
	  else if(strcmp(argv[currentArgIndex],"-pe")==0) 
	    {
	      peSelected=true;
	      if(currentArgIndex==(argc-1))
		{
		  std::cerr << "***TAI input file not supplied - required for computation of population exposed objective" << std::endl;
		  exit(1);
		}
	      taiInputFilename=argv[++currentArgIndex];
	      std::cout << "Generating population exposed impact file" << std::endl;
	    } 
#endif
	  else if(strcmp(argv[currentArgIndex],"-tsoPattern")==0) 
	    {
	      if(currentArgIndex==(argc-1))
		{
		  std::cerr << "***TSO file pattern not supplied" << std::endl;
		  exit(1);
		}
	      tsoPattern=argv[++currentArgIndex];
	    } 
	  else if(strcmp(argv[currentArgIndex],"-detectionDelay")==0) 
	    {
	      detectionDelay=atoi(argv[++currentArgIndex]);
	      if(detectionDelay<0)
		{
		  std::cerr << "***The detection delay must be non-negative" << std::endl;
		  exit(1);
		}
	      std::cout << "Detection delay (in minutes)=" << detectionDelay << std::endl;
	    } 
	  else if(strcmp(argv[currentArgIndex],"-minQuality")==0) 
	    {
	      minQuality=atof(argv[++currentArgIndex]);
	      if(minQuality<0.0)
		{
		  std::cerr << "***The minimum water quality must be non-negative" << std::endl;
		  exit(1);
		}
	      std::cout << "Water quality threshold=" << minQuality << std::endl;
	    } 
#if 0
	  else if(strcmp(argv[currentArgIndex],"-sensorFile")==0) 
	    {
	      if(currentArgIndex==(argc-1))
		{
		  std::cerr << "***Sensor input file not supplied" << std::endl;
		  exit(1);
		}
	      sensorInputFilename=argv[++currentArgIndex];
	      std::cout << "Sensor input file=" << sensorInputFilename << std::endl;
	    } 
#endif
	  else if(strcmp(argv[currentArgIndex],"-tsoPattern")==0) 
	    {
	      tsoPattern=argv[currentArgIndex];
	      std::cout << "TSO filename pattern=" << tsoPattern << std::endl;
	    } 
	  else if(strcmp(argv[currentArgIndex],"-epanetin")==0)
	    {
	      if(currentArgIndex==(argc-1))
		{
		  std::cerr << "***EPANET input file not supplied" << std::endl;
		  exit(1);
		}
	      epanetInputFilename=argv[++currentArgIndex];
	    }
	  else 
	    {
	      std::cerr << "***Unknown argument=" << argv[currentArgIndex] << std::endl;
	      exit(1);
	    }

	  currentArgIndex++;
	}

      ////////////////////////////////////////////////////////

      //////////////////////////////////////////////////////////////
      // read in the sensor locations, if any have been specified //
      // NOTE: the indicies in the input file are 1-based, and    //
      //       are converted to 0-based indicies for internal use //
      // NOTE: the program assumes the indices in the input file  //
      //       are identical to those found in the tso2Impact     //
      //       nodemap output file.                               //
      //////////////////////////////////////////////////////////////

      if(sensorInputFilename!="")
	{
	  std::ifstream sensorInputFile(sensorInputFilename.c_str());
	  if(!sensorInputFile)
	    {
	      std::cerr << "***Failed to open sensor input file=" << sensorInputFilename << std::endl;
	      exit(1);
	    }
      
	  for(;;)
	    {
	      int thisSensorIndex;
	      sensorInputFile >> thisSensorIndex;
	      if(!sensorInputFile)
		{
		  break;
		}
	      sensorLocations.push_back(thisSensorIndex-1);
	    }

	  std::cout << "Number of sensor locations=" << sensorLocations.size() << std::endl;
	  sensorInputFile.close();
	}

      //////////////////////////////////////////////////////////////
}

void readTsoInfo(int& numNodes,
	PNodeInfo&nodes, PLinkInfo&links,
	PNetInfo&net, PTSO&tso, PSource&sources,
	std::string&outputFilePrefix,
	std::string&tsoFileOrDirectoryName,
	std::string&tsoPattern,
	std::string&nodeMapFileName,
	std::string&scenarioMapFileName,
	std::ofstream&nodeMapFile,
	std::ofstream&scenarioMapFile,
        std::vector<std::string>&nodeIndexToIDMap,
        std::map<std::string,int>&nodeIDToIndexMap)
{
      nodeMapFileName = outputFilePrefix+"."+NODE_MAP_FILE_SUFFIX;
      scenarioMapFileName = outputFilePrefix+"."+SCENARIO_MAP_FILE_SUFFIX;
      nodeMapFile.open(nodeMapFileName.c_str()); 
      scenarioMapFile.open(scenarioMapFileName.c_str()); 
      nodeMapFileName = outputFilePrefix+"."+NODE_MAP_FILE_SUFFIX;
      std::cout << std::endl << "Loading TSO File..." << std::endl;
      if(tsoPattern=="")
	{
	  tso=TSO_Open((char*)tsoFileOrDirectoryName.c_str(),0);
	}
      else 
	{
	  tso=TSO_Open((char*)tsoFileOrDirectoryName.c_str(),(char*)tsoPattern.c_str());
	}
      std::cout << "Opened tso file" << std::endl;

      TSO_ReadPrologue(tso,&net,&nodes,&links);
      numNodes = net->nnodes;
      nodeIndexToIDMap= std::vector<std::string>(numNodes);
  
      std::cout << "Current file=" << tso->curFile << std::endl;
      std::cout << "Storage method=" << tso->storageMethod << std::endl;
      std::cout << "File version=" << tso->fileVersion << std::endl;

      std::cout << std::endl;
      std::cout << "Number of nodes=" << net->nnodes << std::endl;
      std::cout << "Number of links=" << net->nlinks << std::endl;
      std::cout << "Number of tanks=" << net->ntanks << std::endl;
      std::cout << "Number of junctions=" << net->njunctions << std::endl;
      std::cout << "Number of steps=" << net->nsteps << std::endl;
      std::cout << "Number of species=" << net->nspecies << std::endl;
      for(int i=0;i<net->nspecies;i++)
	{
	  std::cout << "Species=" << net->species[i] << std::endl;
	}
      std::cout << "Step size (hours)=" << net->stepsize << std::endl; // units appear to be in hours!!!
      std::cout << "Floatmax=" << net->fltmax << std::endl;
      std::cout << std::endl;

      #ifdef OUTPUT_TSO_DETAIL

      std::cout << std::endl;
      std::cout << "Node hydraulic information" << std::endl << std::endl;
      for(int i=0;i<net->nnodes;i++)
	{
	  std::cout << "Node (index=" << i+1 << ",ID=" << nodes[i].id << ") ";
	  std::cout << "Demands= ";
	  for(int j=0;j<net->nsteps;j++)
	    {
	      std::cout << nodes[i].q[j] << " ";
	    }
	  std::cout << std::endl;
	}
      std::cout << std::endl;

      #endif

      // NOTE: internal to tso2Impact.cc, the node indicies are 0-based, in
      //       correspondence with the TEVA node/link arrays. More annoyingly,
      //       the indicies maintained in the TEVA data structures are 1-based,
      //       requiring corrections when accessing the data structures.
      //       ALL external writes perform a 1-based translation.

      // a simple file to assist in translation between (1-based)
      // auxilliary node indicies and the real node names.


      for(int i=0;i<net->nnodes;i++)
	{
	  nodeMapFile << i+1 << " " << nodes[i].id << std::endl;

	  nodeIndexToIDMap[i]=nodes[i].id;
	  nodeIDToIndexMap[nodes[i].id]=i;
	}

      nodeMapFile.close();
  
      #ifdef OUTPUT_TSO_DETAIL

      std::cout << std::endl;
      std::cout << "Link hydraulic information" << std::endl << std::endl;
      for(int i=0;i<net->nlinks;i++)
	{
	  std::cout << "Link (Index=" << links[i].from << ",ID=" << nodes[(links[i].from)-1].id << " => Index=" << links[i].to << ",ID=" << nodes[(links[i].to)-1].id << ")" << std::endl;
	  std::cout << "Velocities= ";
	  for(int j=0;j<net->nsteps;j++)
	    {
	      std::cout << links[i].velocity[j] << " ";
	    }
	  std::cout << std::endl;
	  std::cout << "Flows= ";
	  for(int j=0;j<net->nsteps;j++)
	    {
	      std::cout << links[i].flow[j] << " ";
	    }
	  std::cout << std::endl;
	}
      std::cout << std::endl;

      #endif
}

void initStandaloneTso2Impact(
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
	bool&pdSelected,
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
	readTsoInfo(numNodes,
		nodes, links,
		net, tso, sources,
		outputFilePrefix,
		tsoFileOrDirectoryName,
		tsoPattern,
		nodeMapFileName,
		scenarioMapFileName,
		nodeMapFile,
		scenarioMapFile,
        	nodeIndexToIDMap,
        	nodeIDToIndexMap);
	initTso2ImpactAfterTsoRead(
		numNodes, start, stop, 
		nodes, links,
		net, tso, sources,
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
		pdSelected,
		peSelected,
		taiInputFilename,
		assessMem,
		tsoPattern,
		sensorInputFilename,
		sensorLocations,
		nodeMapFileName,
		scenarioMapFileName,
		nodeMapFile,
		scenarioMapFile,
		theObjectives,
		nodeIndexToIDMap,
		nodeIDToIndexMap);
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
      bool pdSelected=false;
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
			pkSelected, pdSelected,
			peSelected, taiInputFilename, tsoPattern,
      			sensorInputFilename, sensorLocations);


      std::cout << "main: after process_arguments: net: " << net << std::endl;
      initStandaloneTso2Impact(numNodes, start, stop, nodes, links, net, 
			tso, sources, 
			outputFilePrefix, tsoFileOrDirectoryName,
			detectionDelay, minQuality, mcSelected, vcSelected,
			nfdSelected, tdSelected, ecSelected,epanetInputFilename,
			pkSelected, pdSelected,
			peSelected, taiInputFilename, assessMem,
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
#if TEVA_SPOT_LIMITED
      if(pkSelected==true)
	{
	  std::cout << "Population killed impacts written to file=" << outputFilePrefix << "_" << PopulationKilledObjective::ourImpactOutputFileSuffix << std::endl;
	  //std::cout << "Population killed sensor evaluation written to file=" << outputFilePrefix << "_" << PopulationKilledObjective::ourEvaluationOutputFileSuffix << std::endl;
	}
      if(pdSelected==true)
	{
	  std::cout << "Population dosed impacts written to file=" << outputFilePrefix << "_" << PopulationDosedObjective::ourImpactOutputFileSuffix << std::endl;
	  //std::cout << "Population killed sensor evaluation written to file=" << outputFilePrefix << "_" << PopulationDosedObjective::ourEvaluationOutputFileSuffix << std::endl;
	}
      if(peSelected==true)
	{
	  std::cout << "Population exposed impacts written to file=" << outputFilePrefix << "_" << PopulationExposedObjective::ourImpactOutputFileSuffix << std::endl;
	  //std::cout << "Population exposed sensor evaluation written to file=" << outputFilePrefix << "_" << PopulationExposedObjective::ourEvaluationOutputFileSuffix << std::endl;
	}
      if(peSelected==true || pkSelected==true || pdSelected==true)
	{
	  //FreeAssessMemory(assessMem);
	}
#endif
    }

  STD_CATCH(;)

  return 1;
}

#endif
