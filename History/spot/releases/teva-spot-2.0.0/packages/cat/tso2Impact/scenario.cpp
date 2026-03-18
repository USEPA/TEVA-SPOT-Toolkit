#ifdef HAVE_CONFIG_H
#include <teva_config.h>
#endif

#include <cmath>
#include <fstream>
#include <sp/scenario.h>
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
#include "Detection.h"

extern "C"
  {


  }

const std::string NODE_MAP_FILE_SUFFIX="nodemap";
const std::string SCENARIO_MAP_FILE_SUFFIX="scenariomap";
char emsg[STRMAX];


void processScenario(std::vector<ObjectiveBase*> &theObjectives,
                     int thisScenarioIndex,
                     double detectionDelay,
                     int detectionConfidence,
                     double minimumQuality,
                     PNetInfo net, PNodeInfo nodes, PLinkInfo links,
                     PSource sources, void *aggrInput,
                     bool scenario_aggr)
{
  Detection *det = NULL;

  if (detectionConfidence > 1){
    // detection occurs at a node only after N nodes (including the source
    // node) have detected the contaminant
    det = new DetectionNWay(detectionConfidence);
  }
  else{
    // detection occurs at a node when water quality exceeds minimumQuality
    det = new DetectionMinQuality(minimumQuality);
  }

  det->SetSources(sources);
  det->SetNodes(nodes);
  det->SetNetwork(net);
  det->Calculate();

  // detection delay units are assumed to be in minutes

  double reportStep((int)((net->stepsize)*60)); // units are in minutes

  int detectionDelaySteps=int(ceil(double(detectionDelay)/reportStep));

  // track this for verification purposes - we should always have at least 2:
  // one for the source node index and one for the dummy node .
  int numEntries(0);

  // initialize the objectives for this scenario computation.
  for (std::vector<ObjectiveBase*>::iterator iter=theObjectives.begin();iter!=theObjectives.end();iter++)
    {
      (*iter)->resetForScenario(net,nodes,links,sources);
    }

  // step through the contaminant concentration time-series, updating
  // the global/aggregate impact for each objective at each step.

  for (int i=0;i<net->nsteps;i++)
    {
      // skip the initial water quality assignments - the 0-impact cases for
      // source nodes are handled by the detection object and will given to
      // addNominalImpact

      if (i>0)
        {
          // update aggregate node impacts incurred during the previous report step.
          for (int j=0;j<net->nnodes;j++)
            {
              double thisQuality(nodes[j].c[0][i]);

              if ((thisQuality>=minimumQuality)&&
                  (thisQuality>0.0))
                {
                  for (std::vector<ObjectiveBase*>::iterator iter=theObjectives.begin();iter!=theObjectives.end();iter++)
                    {
                      (*iter)->updateImpactStatisticsDueToNode(j,thisQuality,nodes,links,i,reportStep);
                    }
                }
            }

          // update aggregate edge impacts incurred during the previous report step.
          for (int j=0;j<net->nlinks;j++)
            {
              int fromNodeIndex=links[j].from-1;
              int toNodeIndex=links[j].to-1;

              double fromNodeQuality=nodes[fromNodeIndex].c[0][i];
              double toNodeQuality=nodes[toNodeIndex].c[0][i];

              if (((fromNodeQuality>0.0)&&(links[j].flow[i]>0.0)&&(fromNodeQuality>=minimumQuality))||
                  ((toNodeQuality>0.0)&&(links[j].flow[i]<0.0)&&(toNodeQuality>=minimumQuality)))
                {
                  for (std::vector<ObjectiveBase*>::iterator iter=theObjectives.begin();iter!=theObjectives.end();iter++)
                    {
                      (*iter)->updateImpactStatisticsDueToLink(j,nodes,links,i,reportStep);
                    }
                }
            }
        }

      // Get list of nodes that detected impact at timestep i - detectionDelaySteps
      // and note their impact due to possibly delayed response

      int trueDetectTime = i - detectionDelaySteps;
      int numSteps= int(det->detectionNodeList.size());

      if ((trueDetectTime >= 0) && (numSteps >= trueDetectTime-1)){

        std::vector<int> detNodes = det->detectionNodeList[trueDetectTime];

        if (detNodes.size() > 0){
          std::sort(detNodes.begin(), detNodes.end());
          for (std::vector<ObjectiveBase*>::iterator iter=theObjectives.begin();iter!=theObjectives.end();iter++)
            {
              ObjectiveBase *thisObjective=(*iter);
              for (int j=0;j<int(detNodes.size());j++)
                {
                  thisObjective->addNominalImpact(detNodes[j],trueDetectTime,reportStep);
                }
            }
            numEntries+=int(detNodes.size());
          }
      }
    }

    // if detection time plus response delay is greater than nsteps, include that now

    for (int trueDetectTime = net->nsteps - detectionDelaySteps;
         trueDetectTime < net->nsteps; trueDetectTime++){
      std::vector<int> detNodes = det->detectionNodeList[trueDetectTime];
      if (detNodes.size() > 0){
        std::sort(detNodes.begin(), detNodes.end());
        for (std::vector<ObjectiveBase*>::iterator iter=theObjectives.begin();iter!=theObjectives.end();iter++)
          {
            ObjectiveBase *thisObjective=(*iter);
 
            for (int j=0;j<int(detNodes.size());j++)
              {
                thisObjective->addNominalImpact(detNodes[j],trueDetectTime,reportStep);
              }
          }
        }
      }

  delete det;

  // add entries for the case where an event is undetected
  for (std::vector<ObjectiveBase*>::iterator iter=theObjectives.begin();iter!=theObjectives.end();iter++)
    {
      // the 0th time-step is the set of initial conditions
      if ((*iter)->penalize_detection_failures)
         (*iter)->addUndetectedImpact(net->nsteps-1,reportStep);
      else
         (*iter)->addUndetectedImpact(net->nsteps-1,-reportStep);
    }

  numEntries++;

  if ((numEntries<2) && (detectionConfidence < 2))
    {
      std::cout << "***ERROR - Only one entry generated for event scenario index=" << sources->SourceNodeIndex << std::endl;
    }

  // finalize the objectives for this scenario computation.
  for (size_t k=0;k<theObjectives.size();k++)
    {
      if (aggrInput)
        {
          theObjectives[k]->constructAggregationServerData(aggrInput,
              sources);
        }
      else
        {
          theObjectives[k]->finalizeForScenario(thisScenarioIndex,sources,
                                                scenario_aggr);
        }
    }
}
