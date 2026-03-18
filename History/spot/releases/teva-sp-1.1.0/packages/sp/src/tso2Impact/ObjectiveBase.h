#ifndef tevasp_ObjectiveBase_h
#define tevasp_ObjectiveBase_h

#ifdef _MSC_VER
#include <algorithm>
#endif

#include <cassert>
#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

extern "C"
{
#include "toolkit.h"
#include "tevautil.h"

// required for the health impacts objective
#include "teva-assess.h"
#include "ta_input.h"
#include "tai.h"
#include "sss.h"
#include "ta_utils.h"
}

#include "Impact.h"

/// The common virtual base class for all objectives.
class ObjectiveBase
{
 public:

  /// Constructor.
  ObjectiveBase(void):
    myScenarioIDFile(0),
    myScenarioImpactOutputFile(0),
    //myScenarioEvaluationOutputFile(0),
    myScenarioCount(0),
    myImpactAtDetection(0.0),
    myAggregateImpactsAtDetection(0.0)
    {
    }

  /// Copy construtor.
  ObjectiveBase(const ObjectiveBase &other):
    myScenarioIDFile(0),
    mySensorLocations(other.mySensorLocations),
    myScenarioImpactOutputFile(0),
    //myScenarioEvaluationOutputFile(0),
    myScenarioCount(other.myScenarioCount),
    myImpacts(other.myImpacts),
    myImpactAtDetection(other.myImpactAtDetection),
    myAggregateImpactsAtDetection(other.myAggregateImpactsAtDetection)
    {
    }

  /// Destructor.
  virtual ~ObjectiveBase(void)
    {
      if(myScenarioImpactOutputFile!=0)
	{
	  myScenarioImpactOutputFile->close();
	  delete myScenarioImpactOutputFile;
	}

      #if 0
      if(myScenarioEvaluationOutputFile!=0)
	{
	  myScenarioEvaluationOutputFile->close();
	  delete myScenarioEvaluationOutputFile;
	}
      #endif

      myImpacts.clear();
    }

  /// Assignment operator.
  ObjectiveBase &operator=(const ObjectiveBase &other)
    {
      if(this!=&other)
	{
	  myScenarioImpactOutputFile=0;
	  //myScenarioEvaluationOutputFile=0;
	  myScenarioCount=other.myScenarioCount;
	  myImpacts=other.myImpacts;
	  myImpactAtDetection=other.myImpactAtDetection;
	  myAggregateImpactsAtDetection=other.myAggregateImpactsAtDetection;
	  mySensorLocations=other.mySensorLocations;
	}
      return *this;
    }

  // Set the sensor locations for this objective.
  void setSensorLocations(const std::list<int> &theLocations)
    {
      mySensorLocations=theLocations;
    }

  /// Initialize the impact structures, open the output file, etc.
  /// This method is called once, before individual scenario impacts 
  /// are computed.
  virtual void init(const std::string &outputPrefix,PNetInfo net,PNodeInfo nodes,PLinkInfo links,double detectionDelay)
    {
      std::string impactOutputFileName(outputPrefix+"_"+impactFilenameSuffix());
      myScenarioImpactOutputFile=new std::ofstream(impactOutputFileName.c_str());
      if(!(*myScenarioImpactOutputFile))
	{
	  std::cerr << "***Failed to open output file=" << impactOutputFileName << std::endl;
	  exit(1);
	}
      else 
	{
	  (*myScenarioImpactOutputFile) << net->nnodes << std::endl;
	  (*myScenarioImpactOutputFile) << 1 << " " << int(detectionDelay) << std::endl;
	}

      #if 0
      std::string sensorEvaluationFileName(outputPrefix+"_"+evaluationFilenameSuffix());
      myScenarioEvaluationOutputFile=new std::ofstream(sensorEvaluationFileName.c_str());
      if(!(*myScenarioEvaluationOutputFile))
	{
	  std::cerr << "***Failed to open output file=" << sensorEvaluationFileName << std::endl;
	  exit(1);
	}
      #endif

      myScenarioCount=0;
      myImpacts.reserve(net->nnodes);
      myImpactAtDetection=-1.0;
      myAggregateImpactsAtDetection=0.0;
    }
  
  /// Reset the local variables for impact compuations for a new attack 
  /// scenario.
  virtual void resetForScenario(void)
    {
      myScenarioCount++;
      myImpactAtDetection=-1.0;
      myImpacts.clear();
    }  

  /// Finalize all data (e.g., output) for the current attack scenario.
  virtual void finalizeForScenario(int scenarioIndex,PSource sources)
    {
      outputScenarioImpactData(scenarioIndex,sources);
      //outputScenarioEvaluationData(scenarioIndex,sources);
      myAggregateImpactsAtDetection+=myImpactAtDetection;
    }  

  /// Output the impacts for the current scenario.
  virtual void outputScenarioImpactData(int scenarioIndex,PSource sources)
    {
      if (myScenarioIDFile) {
         //
         // the internal node output order is sorted first on time-hit, 
         // and then on node ID. the global order is based on impact
         // first, then node ID; UNDETECTED_NODE_INDEX node entries always appear 
         // last.
         //
         #ifdef IMPOSE_IMPACT_NODE_ORDER
         std::sort(myImpacts.begin(),myImpacts.end(),std::less<Impact>());
         #endif

         (*myScenarioIDFile) << scenarioIndex << " " << sources[0].SourceNodeIndex << " " << sources[0].SourceStart << std::endl;
         }
      for(int i=0;i<int(myImpacts.size());i++) {
	     // NOTE: the current .impact file format doesn't handle multiple 
	     // source node indicies the scenario indicies are 1-based (along 
	     // with the node indicies, enforced upon impact entry creation)
	     (*myScenarioImpactOutputFile) << scenarioIndex << " " 
					   << myImpacts[i].nodeID << " " << myImpacts[i].timeOfDetection << " " << myImpacts[i].impact << std::endl;
	 }
    }

  #if 0
  /// Output the sensor evaluation data for the current scenario.
  virtual void outputScenarioEvaluationData(int scenarioIndex,PSource sources)
    {
      assert(myScenarioEvaluationOutputFile!=0);
    
      (*myScenarioEvaluationOutputFile) << scenarioIndex << " " << myImpactAtDetection << std::endl;
    }
  #endif

  /// Return the impact filename suffix for this objective.
  virtual const std::string &impactFilenameSuffix(void)=0;

  /// Return the evaluation filename suffix for this objective.
  virtual const std::string &evaluationFilenameSuffix(void)=0;

  /// Update impact statistics to reflect the indicated water quality at 
  /// the indicated node.
  virtual void updateImpactStatisticsDueToNode(int thisNodeIndex, double thisNodeQuality, PNodeInfo nodes, PLinkInfo links, int thisTimeStep, double reportStep)
    {
      // default behavior is a no-op; not all objectives track node-related 
      // impacts.    
    }

  /// Update impact statistics to reflect the water quality measures at the 
  /// link end-points.
  virtual void updateImpactStatisticsDueToLink(int thisLinkIndex,PNodeInfo nodes,PLinkInfo links,int thisTimeStep,double reportStep)
    {
      // default behavior is a no-op; not all objectives track link-related 
      // impacts.
    }

  /// Add an impact entry for the input node given the current scenario impact.
  virtual void addNominalImpact(int thisNodeIndex,int thisTimeStep,double reportStep)=0;

  /// Add an impact entry for the case of an undetected attack.
  virtual void addUndetectedImpact(int maxTimeStep,double reportStep)=0;

  /// Geneate/write any cross-scenario statistics, close the output file, 
  /// and perform any cleanup of memory.
  virtual void finalize(void)
    {
      if(myScenarioImpactOutputFile)
	{
	  myScenarioImpactOutputFile->close();
	  delete myScenarioImpactOutputFile;
	  myScenarioImpactOutputFile=0;
	}

      #if 0
      (*myScenarioEvaluationOutputFile) << "Mean impact at detection=" 
					<< myAggregateImpactsAtDetection/double(myScenarioCount) << std::endl;

      if(myScenarioEvaluationOutputFile)
	{
	  myScenarioEvaluationOutputFile->close();
	  delete myScenarioEvaluationOutputFile;
	  myScenarioEvaluationOutputFile=0;
	}
      #endif

      myImpacts.clear();
    }

  ///
  enum {UNDETECTED_NODE_INDEX=-1};

  /// The output file to which the scenario information is
  /// written.
  std::ofstream *myScenarioIDFile;

 protected:

  //// Locations of sensors in the network. 
  std::list<int> mySensorLocations;

  /// Returns true if there is a sensor at the indicated node?
  bool sensorAt(int theNodeIndex)const
    {
      std::list<int>::const_iterator iter=std::find(mySensorLocations.begin(),mySensorLocations.end(),theNodeIndex);
      return iter!=mySensorLocations.end();
    }

  /// Updates the scenario impact entries.
  /// Node indicies are 0-based, translated into 1-based once impact 
  /// entries are created.
  virtual void updateImpacts(int thisNodeIndex,double thisImpact, double thisTime)
    {
      Impact newImpact;
      if(thisNodeIndex==UNDETECTED_NODE_INDEX)
	{
	  newImpact.nodeID=UNDETECTED_NODE_INDEX;
	}
      else
	{
	  newImpact.nodeID=thisNodeIndex+1;
	}
      newImpact.impact=thisImpact;
      newImpact.timeOfDetection=thisTime;
      myImpacts.push_back(newImpact);
    }

  /// Update the sensor evaluation statistics.
  /// A node index equal to UNDETECTED_NODE_INDEX indicates that 
  /// the attack was not detected. Node indicies are assumed
  /// to be 0-based.
  virtual void updateSensorEvaluation(int thisNodeIndex, double thisImpact)
    {
      if(myImpactAtDetection==-1.0)
	{
	  // this scenario hasn't been detected yet 
	  if((sensorAt(thisNodeIndex)==true)||
	     (thisNodeIndex==UNDETECTED_NODE_INDEX))
	    {
	      myImpactAtDetection=thisImpact;
	    }
	}
    }

 private:

  /// The output file to which the impacts are written.
  /// This defaults to 0, and we assume that this class
  /// can delete this object.
  std::ofstream *myScenarioImpactOutputFile;

  /// The output file to which the impacts at the time
  /// of detection (if detected) or maximal impacts (if
  /// not detected) are written.
  //std::ofstream *myScenarioEvaluationOutputFile;

  /// The number of scenarios processed thus far.
  /// This defaults to 0, indicating unassigned.
  int myScenarioCount;

  /// A list of impacts for an individual attack scenario.
  std::vector<Impact> myImpacts;

  /// Network-wide impact at point of detection by
  /// first contaminated node with a sensor.
  /// This defaults to -1.0, indicating unassigned.
  /// This value is relative to the current scenario.
  double myImpactAtDetection;

  /// Aggregate of myImpactAtDetection over all of
  /// the scenarios encountered thus far.
  /// Thisdefaults to 0.0, indicating unassigned.
  double myAggregateImpactsAtDetection;

};

#endif
