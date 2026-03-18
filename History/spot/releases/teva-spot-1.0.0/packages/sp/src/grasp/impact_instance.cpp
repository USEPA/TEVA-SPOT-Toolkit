/**************************************************************
 * class PMImpactInstance: 
 **************************************************************/

#include "instance.h"
#include "impact_instance.h"
#include "bossa_timer.h"
#include <string.h>

#include <iostream>
#include <iomanip>
#include <algorithm>

#include <float.h>

/*------------------------------------------
  | constructor: produces an empty instance,
  | with p = 0
  *-----------------------------------------*/ 

PMImpactInstance::PMImpactInstance(void) 
{
  reset();
}

void PMImpactInstance::reset(void) 
{
  myOracleTime=0.0;
  myOracle=0;   

  myImpactRepresentation=0;
  myStoreArrivalSequences=false;

  myUndetectedImpacts.clear();

  myN=myM=myP=0;  // customers, potential facilites, actual facilities: all zero
}


/*-----------------------------------------------------------
  | constructor: produces a restricted version of an existing
  | instance, with only a subset of the facilities and of
  | the customers
  *----------------------------------------------------------*/

PMImpactInstance::PMImpactInstance(PMInstance *original, int *of, int *oc) 
{
  std::cout << "***** WARNING - SHOULDN'T BE CALLING THIS!!" << std::endl;
  //  reset();

  //   n = oc[0]; //number of customers in this instance
  //   m = of[0]; //number of facilities in this instance
  //   p = 0;     //just guessing some value
  //   int i;
	
  //   d = new double * [n+1];
  //   for (i=0; i<=n; i++) {
  //     d[i] = new double [m+1];
  //     d[i][0] = POPSTAR_INFINITY;
  //   }
  //   for (i=1; i<=m; i++) d[0][i] = POPSTAR_INFINITY; 
	
  //   //---------------
  //   // set distances
  //   //---------------
  //   int c0, f0, c, f;
  //   for (c=1; c<=n; c++) {
  //     c0 = oc[c]; 
  //     for (f=1; f<=m; f++) {
  //       f0 = of[f];
  //       d[c][f] = original->getDist(c0,f0);
  //     }
  //   }

  //   initOracle();
}

/*------------------------------------------
  | printMatrix: outputs the distance matrix
  | (even if there is no such thing)
  *-----------------------------------------*/

void PMImpactInstance::printMatrix(FILE *file) 
{
  std::cout << "***** WARNING - SHOULDN'T BE CALLING THIS!!" << std::endl;
  // TBD - REDO!
  //  int c, f;
  //  for (c=1; c<=n; c++) {
  //    for (f=1; f<=m; f++) {
  //      double d = getDist(c,f);			
  //      if (d==POPSTAR_INFINITY) fprintf (file, "--- ");
  //      else fprintf (file, "%f ", d);
  //    }
  //    fprintf (file, "\n");
  //  }
}


/*-----------------------------------------------
  | fatal: prints error message and exits program
  *----------------------------------------------*/

void PMImpactInstance::fatal(const char *func, const char *msg) 
{
  fprintf (stderr, "PMImpactInstance::%s: %s.\n", func, msg);
  exit(-1);
}


/*-------------------------------------------------
  | resetDistances: sets all distances to infinity
  |
  | except those from a city to itself (set to 0);
  | this routine assumes each city is a potential
  | facility
  *------------------------------------------------*/

void PMImpactInstance::resetDistances(void) 
{	
  //  std::cout << "PMImpactInstance::resetDistances() - begin" << std::endl;

  //   int f, u;

  //   //reset distances
  //   d[0][0] = POPSTAR_INFINITY;
  //   for (f=1; f<=m; f++) d[0][f] = POPSTAR_INFINITY * POPSTAR_INFINITY;
	
  //   for (u=1; u<=n; u++) {
  //     d[u][0] = POPSTAR_INFINITY * POPSTAR_INFINITY;
  //     for (f=1; f<=m; f++) d[u][f] = POPSTAR_INFINITY;	
  //   }

  //   for (f=1; f<=m; f++) d[f][f] = 0; //distance to itself is zero
	
  //   //note that we can use the fact that d[0][x] = d[x][0] = POPSTAR_INFINITY
  //   //except for x = 0
  //  std::cout << "PMImpactInstance::resetDistances() - end" << std::endl;
}


/*******************************************************
 * 
 * read file in impact format, a partially specified distance matrix
 *
 *******************************************************/

void PMImpactInstance::readImpact(FILE *input, int _p) 
{
  //  std::cout << "PMImpactInstance::readImpact() - begin" << std::endl;

  const int LINESIZE(256); 
  char buffer[LINESIZE+1];

  myN=0;
  myM=0;
  myP=_p;

  resetDistances();

  // working variables for the input - not needed once the
  // sparse or dense matrix forms are compiled.
  std::map<int,std::map<int,double> > inputDetectedImpacts; 

  // grab the total number of candidate facilities
  // from the first line in the impact file. you 
  // don't want to compute it from the highest index
  // you see in the impact file, as some nodes aren't
  // represented there (e.g., those with zero demand
  // or those that can't be reached by an injection).
  fgets(buffer,LINESIZE,input);
  if(sscanf(buffer,"%d",&myM)!=1)
    {
      fatal("readImpact", "Incorrectly formatted input file");
    }

  // skip the second line, which details the delay 
  // information and the number of impacts per scenario
  fgets(buffer,LINESIZE,input);

  int prevScenarioIndex(-1);

  // create the empty 0th-entry of the arrival sequence
  myArrivalSequence.push_back(std::vector<std::pair<int,double> >());

  while(fgets(buffer, LINESIZE, input)) 
    {
      //      std::cout << "Input line=" << buffer << std::endl;

      int scenarioIndex;
      int nodeIndex;
      int detectionTime;
      double thisImpact;
      if(sscanf(buffer,"%d %d %d %lf",&scenarioIndex,&nodeIndex,&detectionTime,&thisImpact)==4)
	{
	  if(scenarioIndex>myN)
	    {
	      myN=scenarioIndex;
	    }

	  //	  std::cout << "Read line - scenario index=" << scenarioIndex << " node index=" << nodeIndex << " impact=" << thisImpact << " detection time=" << detectionTime << std::endl;
	  if(nodeIndex==-1)
	    {
	      if(myUndetectedImpacts.find(scenarioIndex)!=myUndetectedImpacts.end())
		{
		  std::cerr << "***Undetected impact for scenario index=" << scenarioIndex << " already defined" << std::endl;
		  exit(1);
		}

	      myUndetectedImpacts[scenarioIndex]=thisImpact;
	    }
	  else
	    {
	      if(inputDetectedImpacts.find(scenarioIndex)==inputDetectedImpacts.end())
		{
		  inputDetectedImpacts[scenarioIndex]=std::map<int,double>();
		}
	      
	      std::map<int,double> &thisSubmap=inputDetectedImpacts[scenarioIndex];
	      
	      if(thisSubmap.find(nodeIndex)!=thisSubmap.end())
		{
		  std::cerr << "***Detected impact for scenario index=" << scenarioIndex << ", node index=" << nodeIndex << " already defined" << std::endl;
		  exit(1);
		}

	      thisSubmap[nodeIndex]=thisImpact;
	    }

	  // update the arrival sequences
	  if(scenarioIndex!=prevScenarioIndex)
	    {
	      myArrivalSequence.push_back(std::vector<std::pair<int,double> >());
	    }
	  prevScenarioIndex=scenarioIndex;
	  myArrivalSequence[scenarioIndex].push_back(std::make_pair(nodeIndex,detectionTime));
	}
      else
	{
	  fatal("readImpact", "Incorrectly formatted input file");
	}
    }	

  // create sparse or dense versions for faster lookup.

  if(myImpactRepresentation==0)
    {
      myDetectedImpactsSparse=std::vector<std::map<int,double> >(myN+1,std::map<int,double>());
      myUndetectedImpactsSparse=std::vector<double>(myN+1,0.0);
  
      for(std::map<int,double>::iterator iter=myUndetectedImpacts.begin();iter!=myUndetectedImpacts.end();iter++)
	{
	  int thisScenarioIndex=(*iter).first;
	  double thisImpact=(*iter).second;
	  myUndetectedImpactsSparse[thisScenarioIndex]=thisImpact;
	}

      for(std::map<int,std::map<int,double> >::iterator iter=inputDetectedImpacts.begin();iter!=inputDetectedImpacts.end();iter++)
	{
	  int thisScenarioIndex=(*iter).first;
	  const std::map<int,double> &thisImpactMap=(*iter).second;
	  myDetectedImpactsSparse[thisScenarioIndex]=thisImpactMap;
	}
    }
  else
    {
      myDetectedImpactsDense=std::vector<std::vector<double> >(myN+1,std::vector<double>(myM+1,0.0));
      
      for(int i=1;i<=myN;i++)
	{
	  const std::map<int,double> &detectedImpacts=inputDetectedImpacts[i];
	  double undetectedImpact=myUndetectedImpacts[i];
	  for(int j=1;j<=myM;j++)
	    {
	      std::map<int,double>::const_iterator iter=detectedImpacts.find(j);
	      if(iter==detectedImpacts.end())
		{
		  myDetectedImpactsDense[i][j]=undetectedImpact;
		}
	      else
		{
		  myDetectedImpactsDense[i][j]=(*iter).second;
		}
	    }
	}

      myUndetectedImpactsDense=std::vector<double>(myN+1,0.0);
      for(int i=1;i<=myN;i++)
	{
	  myUndetectedImpactsDense[i]=myUndetectedImpacts[i];
	}
    }

  if(myStoreArrivalSequences==true)
    {
      myArrivalTimesCompiled=std::vector<std::vector<double> >(myN+1,std::vector<double>(myM+1,DBL_MAX));

      for(int i=1;i<=myN;i++)
	{
	  const std::vector<std::pair<int,double> > &thisArrivalSequence=myArrivalSequence[i];
	  for(size_t j=0;j<thisArrivalSequence.size();j++)
	    {
	      int thisNode(thisArrivalSequence[j].first);
	      // dummy witness arrival times aren't currently stored - see note in header
	      if(thisNode!=-1) 
		{
		  double thisArrivalTime(thisArrivalSequence[j].second);
		  myArrivalTimesCompiled[i][thisNode]=thisArrivalTime;
		}
	    }
	}
    }

  initOracle();
}


/*******************************************
 *
 * destructor: deallocates distance matrix,
 * gets rid of distance oracle
 *
 *******************************************/

PMImpactInstance::~PMImpactInstance(void) 
{
  delete myOracle;
  myUndetectedImpacts.clear();
}
