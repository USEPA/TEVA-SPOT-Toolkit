#include <cmath>
#include <fstream>
#include <sp/scenario.h>
#include "MassConsumedObjective.h"
#include "PopulationExposedObjective.h"
#include "PopulationKilledObjective.h"
#include "NumberFailedDetectionsObjective.h"
#include "TimeToDetectionObjective.h"
#include "VolumeContaminatedWaterConsumedObjective.h"
#include "ExtentOfContaminationObjective.h"

extern "C" {


}

const std::string NODE_MAP_FILE_SUFFIX="nodemap";
const std::string SCENARIO_MAP_FILE_SUFFIX="scenariomap";
char emsg[STRMAX];


void processScenario(std::vector<ObjectiveBase*> &theObjectives,
		     int thisScenarioIndex,
		     double detectionDelay,
		     double minimumQuality,
		     PNetInfo net, PNodeInfo nodes, PLinkInfo links, 
		     PSource sources, void *aggrInput)
{
  // detection delay units are assumed to be in minutes

  double reportStep(net->stepsize*60); // units are in minutes

  int detectionDelaySteps=int(ceil(double(detectionDelay)/reportStep));

  // has contaminant hit a node yet?
  std::vector<bool> nodeHitRecorded(net->nnodes,false); 

  std::map<int, int> trueNodeDetections;
  // what time-step does detection effectively occur at, including the user-specified
  // detection delay. first index is time-step number, second indicies are node indicies
  std::vector<std::vector<int> > effectiveNodeDetections(net->nsteps,std::vector<int>());

  // a sensor at the source index can always immediately detect the attack.
  // the '-1' factor is required because the source node indicies are already 1-based.

  for(int i=0;i<net->nsources;i++)
    {
      nodeHitRecorded[(sources[i].SourceNodeIndex)-1]=true;
      effectiveNodeDetections[std::min(0+detectionDelaySteps,(net->nsteps)-1)].push_back((sources[i].SourceNodeIndex)-1); 
    }

  // track this for verification purposes - we should always have at least 2:
  // one for the source node index and one for the dummy node .
  int numEntries(0);

  // initialize the objectives for this scenario computation.
  for(std::vector<ObjectiveBase*>::iterator iter=theObjectives.begin();iter!=theObjectives.end();iter++)
    {
      (*iter)->resetForScenario(net,nodes,links,sources);
    }

  // step through the contaminant concentration time-series, updating
  // the global/aggregate impact for each objective at each step.

  for(int i=0;i<net->nsteps;i++)
    {
      // skip the initial water quality assignments - the 0-impact cases for 
      // source nodes are handled in the immediately preceding case.

      if(i>0)
	{
	  // update aggregate node impacts incurred during the previous report step.
	  for(int j=0;j<net->nnodes;j++)
	    {
	      double thisQuality(nodes[j].c[0][i]);

	      if((thisQuality>=minimumQuality)&&
		 (thisQuality>0.0))
		{
		  for(std::vector<ObjectiveBase*>::iterator iter=theObjectives.begin();iter!=theObjectives.end();iter++)
		    {
		      (*iter)->updateImpactStatisticsDueToNode(j,thisQuality,nodes,links,i,reportStep);
		    }
		}
	    }

	  // update aggregate edge impacts incurred during the previous report step.
	  for(int j=0;j<net->nlinks;j++)
	    {
	      int fromNodeIndex=links[j].from-1;
	      int toNodeIndex=links[j].to-1;

	      double fromNodeQuality=nodes[fromNodeIndex].c[0][i];
	      double toNodeQuality=nodes[toNodeIndex].c[0][i];
	      
	      if(((fromNodeQuality>0.0)&&(links[j].flow[i]>0.0)&&(fromNodeQuality>=minimumQuality))||
		 ((toNodeQuality>0.0)&&(links[j].flow[i]<0.0)&&(toNodeQuality>=minimumQuality)))
		{
		  for(std::vector<ObjectiveBase*>::iterator iter=theObjectives.begin();iter!=theObjectives.end();iter++)
		    {
		      (*iter)->updateImpactStatisticsDueToLink(j,nodes,links,i,reportStep);
		    }
		}
	    }

	  // update the node hit time/statistics.
	  int j;
	  std::vector<bool>::iterator iter;
	  for(j=0,iter=nodeHitRecorded.begin();j<net->nnodes;j++,iter++)
	    {
	      double thisQuality(nodes[j].c[0][i]);

	      if(((*iter)==false)&&(thisQuality>=minimumQuality)&&(thisQuality>0.0))
		{
		  (*iter)=true;
		  trueNodeDetections[j] = i;
		  effectiveNodeDetections[std::min(i+detectionDelaySteps,(net->nsteps)-1)].push_back(j);
		}
	    }
	}

      std::vector<int> &newNodeArrivals=effectiveNodeDetections[i];

      std::sort(newNodeArrivals.begin(),newNodeArrivals.end());

      // add impact entries for each new arrival during this time-step
      for(std::vector<ObjectiveBase*>::iterator iter=theObjectives.begin();iter!=theObjectives.end();iter++)
	{
	  ObjectiveBase *thisObjective=(*iter);
	  
	  for(int j=0;j<int(newNodeArrivals.size());j++)
	    {
	      thisObjective->addNominalImpact(newNodeArrivals[j],trueNodeDetections[newNodeArrivals[j]],reportStep);
	    }
	}
      
      numEntries+=int(newNodeArrivals.size());
    }

  // add entries for the case where an attack is undetected
  for(std::vector<ObjectiveBase*>::iterator iter=theObjectives.begin();iter!=theObjectives.end();iter++)
    {
      // the 0th time-step is the set of initial conditions
      (*iter)->addUndetectedImpact(net->nsteps-1,reportStep);
    }

  numEntries++;

  if(numEntries<2)
    {
      std::cout << "***ERROR - Only one entry generated for attack scenario index=" << sources->SourceNodeIndex << std::endl;
    }

  // finalize the objectives for this scenario computation.
  for(size_t k=0;k<theObjectives.size();k++)
    {
      if (aggrInput) {
      	theObjectives[k]->constructAggregationServerData(aggrInput,
							 sources);
      } else {
      	theObjectives[k]->finalizeForScenario(thisScenarioIndex,sources);
      }
    }
}
