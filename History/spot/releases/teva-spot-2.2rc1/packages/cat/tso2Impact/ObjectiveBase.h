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
#include <map>
#include <assert.h>
#include <assert.h>

#include <utilib/Any.h>

int read_impact_preamble(std::istream& istr,int& nnodes,int delay);

int read_next_impact(std::istream& istr, int eventIndex,
		  std::map<int,double>& next_event_impacts, 
		  std::vector<std::pair<int,int> > &next_event_sequence,
		  int delayIndex);

extern "C"
{
#include "toolkit.h"
#include "tevautil.h"

#if TEVA_SPOT_LIMITED
// required for the health impacts objective
#include "teva-assess.h"
#include "ta_input.h"
#include "tai.h"
#include "sss.h"
#include "ta_utils.h"
//#include "NamedData.h"	// for NamedDataRef (constructAggregationServerData())
//#include "ExternalAnalysis.h" // for setIntArray, setDoubleArray 
#endif
}

#include "ObjectiveVisitor.h"


/// The common virtual base class for all objectives.
class ObjectiveBase
{
 public:

  /// Constructor.
  ObjectiveBase(void):
    myScenarioIDFile(0),
    penalize_detection_failures(true),
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
    penalize_detection_failures(true),
    mySensorLocations(other.mySensorLocations),
    myScenarioImpactOutputFile(0),
    //myScenarioEvaluationOutputFile(0),
    myScenarioCount(other.myScenarioCount),
    myImpacts(other.myImpacts),
    myImpactAtDetection(other.myImpactAtDetection),
    myAggregateImpactsAtDetection(other.myAggregateImpactsAtDetection),
    scenario_aggr(other.scenario_aggr)
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
      if(myScenarioIDFile)
	{
	  myScenarioIDFile->close();
	  delete myScenarioIDFile;
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

  virtual void update(utilib::Any) {}

  // Set the sensor locations for this objective.
  void setSensorLocations(const std::list<int> &theLocations)
    {
      mySensorLocations=theLocations;
    }

  /// Initialize the impact structures, open the output file, etc.
  /// This method is called once, before individual scenario impacts 
  /// are computed.
  // TNT: modified to write out the number of scenarios in the file (will not write it out if the value is -1).
  // Currently if an impact file is generated while running the simulations in TEVA, the number of simulations
  // will NOT be written to the file.  It will be written via tso2Impact and from TEVA if the tso2Impact
  // is run after the ensemble is run (as in sensor placement)
  virtual void init(const std::string &outputPrefix,PNetInfo net,PNodeInfo nodes,PLinkInfo links,double detectionDelay, int numScenarios=-1)
    {
	  nnodes = net->nnodes;
	  if(outputPrefix != "") {
		impactOutputFileName = outputPrefix+"_"+impactFilenameSuffix();
     	myScenarioImpactOutputFile=new std::ofstream(impactOutputFileName.c_str());
        if(!(*myScenarioImpactOutputFile))
	  {
    	  std::cerr << "***Failed to open output file=" << impactOutputFileName << std::endl;
	     exit(1);
	  }
      else 
    	{
			(*myScenarioImpactOutputFile) << net->nnodes
						      << std::endl;
			/*  TNT ins, JWB del -- see comment in tso2Impact.cpp
			if(numScenarios == -1) {
				(*myScenarioImpactOutputFile) << std::endl;
			} else {
				(*myScenarioImpactOutputFile) << " " << numScenarios << std::endl;
			}
			*/
			(*myScenarioImpactOutputFile) << 1 << " " << int(detectionDelay) << std::endl;
		}

	      std::string scenarioIDFileName(outputPrefix+"_"+impactFilenameSuffix()+".id");
	      myScenarioIDFile=new std::ofstream(scenarioIDFileName.c_str());
	      if(!(*myScenarioIDFile))
		{
		  std::cerr << "***Failed to open output file=" << scenarioIDFileName << std::endl;
		  exit(1);
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
	}
      myScenarioCount=0;
      //myImpacts.reserve(net->nnodes);
      myImpactAtDetection=-1.0;
      myAggregateImpactsAtDetection=0.0;

      std::list<ObjectiveVisitor*>::iterator vcurr = visitors.begin();
      std::list<ObjectiveVisitor*>::iterator vend  = visitors.end();
      while (vcurr != vend) {
          (*vcurr)->impactOutputFileName = impactOutputFileName;
          vcurr++;
      }
    }
  

  virtual void resetForScenario()
    {
      myScenarioCount++;
      myImpactAtDetection=-1.0;
      myImpacts.clear();
    }  
  /// Reset the local variables for impact compuations for a new event 
  /// scenario.  The arguments are used by some objectives and not others.
  virtual void resetForScenario(PNetInfo net, PNodeInfo nodes, PLinkInfo links,
				PSource sources)
    {
      this->resetForScenario();
    }  

  /// Finalize all data (e.g., output) for the current event.
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
         if(sources != NULL)
             (*myScenarioIDFile) << scenarioIndex << " " << sources[0].SourceNodeIndex << " " << sources[0].SourceStart << std::endl;
      }

      std::list<ObjectiveVisitor*>::iterator vcurr = visitors.begin();
      std::list<ObjectiveVisitor*>::iterator vend  = visitors.end();
      while (vcurr != vend) {
          (*vcurr)->outputScenarioImpactData(myImpacts, scenarioIndex);
          vcurr++;
      }

      if(myScenarioImpactOutputFile) {
      
	std::list<Impact>::iterator curr = myImpacts.begin();
	std::list<Impact>::iterator end  = myImpacts.end();
	while (curr != end) {
	      // NOTE: the current .impact file format doesn't handle multiple 
	      // source node indicies the scenario indicies are 1-based (along 
	      // with the node indicies, enforced upon impact entry creation)
	     (*myScenarioImpactOutputFile) << scenarioIndex << " " 
					   << curr->nodeID 
			<< " " << curr->timeOfDetection << " " 
			<< curr->impact << std::endl;
	    curr++;
	    }
	  }
    }

  virtual void constructAggregationServerData(void *aggrData, PSource sources)
    {
    }

/*
  void compileAggregationServerData(NamedDataRef *aggrData)
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
*/

  #if 0
  /// Output the sensor evaluation data for the current scenario.
  virtual void outputScenarioEvaluationData(int scenarioIndex,PSource sources)
    {
      assert(myScenarioEvaluationOutputFile!=0);
    
      (*myScenarioEvaluationOutputFile) << scenarioIndex << " " << myImpactAtDetection << std::endl;
    }
  #endif

  /// Return the impact filename suffix for this objective.
  virtual const std::string &impactFilenameSuffix(void)
  {
    return ourImpactOutputFileSuffix;
  }

  /// Return the evaluation filename suffix for this objective.
  virtual const std::string &evaluationFilenameSuffix(void)
  {
    return ourEvaluationOutputFileSuffix;
  }

  /// Update impact statistics to reflect the indicated water quality at 
  /// the indicated node.
  virtual void updateImpactStatisticsDueToNode(int thisNodeIndex, double thisNodeQuality, PNodeInfo nodes, PLinkInfo links, int thisTimeStep, double reportStep)
    {
      // default behavior is a no-op; not all objectives track node-related 
      // impacts.    
    }

  /// Update impact statistics to reflect the water quality measures at the 
  /// link end-points.
  virtual void updateImpactStatisticsDueToLink(int thisLinkIndex,double thisLinkQuality, PNodeInfo nodes,PLinkInfo links,int thisTimeStep,double reportStep)
    {
      // default behavior is a no-op; not all objectives track link-related 
      // impacts.
    }

  /// Add an impact entry for the input node given the current scenario impact.
  virtual void addNominalImpact(int thisNodeIndex,int thisTimeStep,double reportStep)=0;

  /// Add an impact entry for the case of an undetected event.
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
      if(myScenarioIDFile)
	{
	  myScenarioIDFile->close();
	  delete myScenarioIDFile;
	  myScenarioIDFile=0;
	}
    std::list<ObjectiveVisitor*>::iterator vcurr = visitors.begin();
    std::list<ObjectiveVisitor*>::iterator vend  = visitors.end();
    while (vcurr != vend) {
        (*vcurr)->finalize();
        vcurr++;
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

#if 0
    void write_aggregated_impact_file(std::vector<scenario_group>& groups,
          			      std::map<int, int>& group_map)
    {
	std::ifstream imp_f(impactOutputFileName.c_str());
	
	int eventIndex;
	int nnodes;
	int num_events      = group_map.size();
	int num_aggr_events = groups.size();
	int delayIndex = read_impact_preamble(imp_f, nnodes, 0); 
        imp_f >> eventIndex;
	for (int i=0; i<num_events; i++) {
		std::map<int,double> next_event_impacts;
		std::vector<std::pair<int,int> > next_event_sequence;
		int cur_event = eventIndex;
		eventIndex = read_next_impact(imp_f, eventIndex, 
					      next_event_impacts, 
		  		 	      next_event_sequence, delayIndex);
		int num_impacts = next_event_impacts.size();
		int group_num = group_map[cur_event];
		for (int j=0; j<groups[group_num].max_length; j++) {
			int id = -1;
			if (j < next_event_impacts.size()) {
				id = next_event_sequence[j].first;
			}
			groups[group_num].impacts[j] += 
				next_event_impacts[id];
		}
		if (next_event_impacts.size() == groups[group_num].max_length) {
			for (int j=0; j<groups[group_num].max_length; j++) {
				groups[group_num].hit_sequence[j] = 
					next_event_sequence[j].first; 
				groups[group_num].hit_times[j] = 
					next_event_sequence[j].second; 
			}
		}
	}
	std::string agImpactOutputFileName = "aggr" + impactOutputFileName;
	std::ofstream a_imp_f(agImpactOutputFileName.c_str());
	std::string agProbOutputFileName = impactOutputFileName + ".prob";
	std::ofstream a_prob_f(agProbOutputFileName.c_str());

	a_imp_f << nnodes << endl;
	a_imp_f << "1 0" << endl;	// only support delay of 0 in current
					// impact format; delays implemented in
					// setupIPData, randomsample
	for (int i=0; i<num_aggr_events; i++) {
		// assuming uniform original event probabilities for now
		groups[i].event_prob=groups[i].num_events/ (double)num_events;
		for (int j=0; j<groups[i].max_length; j++) {
			groups[i].impacts[j] /= groups[i].num_events;
		}
           	for(int j=0;j<groups[i].max_length; j++) {
			int hitnode = groups[i].hit_sequence[j];
			int hittime = groups[i].hit_times[j];
	     		a_imp_f << i+1 << " " << hitnode
			<< " " << hittime
			<< " " << groups[i].impacts[j] << std::endl;
	  	}
		a_prob_f << i+1 << " " << groups[i].event_prob << endl;
	}
	a_imp_f.close();
	a_prob_f.close();
   }
#endif
	
	

  ///
  enum {UNDETECTED_NODE_INDEX=-1};

  /// The output file to which the scenario information is
  /// written.
  std::ofstream *myScenarioIDFile;

  /// Updates the scenario impact entries.
  /// Node indicies are 0-based, translated into 1-based once impact 
  /// entries are created.  This routine was originally protected, but
  /// the analysis module already knows the impact values and is not 
  /// compiling aggregate statistics.  Therefore, it needs to call this
  /// routine directly rather than via addNominalImpact.
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

  ///
  void no_failure_penalty()
	{
	penalize_detection_failures=0;
        ourImpactOutputFileSuffix = "d" + ourImpactOutputFileSuffix;
        ourEvaluationOutputFileSuffix = "d" + ourEvaluationOutputFileSuffix;
	}

  ///
  bool penalize_detection_failures;

 protected:

  //// Locations of sensors in the network. 
  std::list<int> mySensorLocations;

  /// Returns true if there is a sensor at the indicated node?
  bool sensorAt(int theNodeIndex)const
    {
      std::list<int>::const_iterator iter=std::find(mySensorLocations.begin(),mySensorLocations.end(),theNodeIndex);
      return iter!=mySensorLocations.end();
    }


  /// Update the sensor evaluation statistics.
  /// A node index equal to UNDETECTED_NODE_INDEX indicates that 
  /// the event was not detected. Node indicies are assumed
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

   std::vector<std::vector<int> > computeScenarioAggregation()
   {
	std::vector<std::vector<int> > result;
	return result;
   }


 protected:

  /// The output file to which the impacts are written.
  /// This defaults to 0, and we assume that this class
  /// can delete this object.
  std::ofstream *myScenarioImpactOutputFile;
  std::string impactOutputFileName;

  /// The output file to which the impacts at the time
  /// of detection (if detected) or maximal impacts (if
  /// not detected) are written.
  //std::ofstream *myScenarioEvaluationOutputFile;

  /// The number of scenarios processed thus far.
  /// This defaults to 0, indicating unassigned.
  int myScenarioCount;

  /// A list of impacts for an individual event.
  std::list<Impact> myImpacts;

  /// Network-wide impact at point of detection by
  /// first contaminated node with a sensor.
  /// This defaults to -1.0, indicating unassigned.
  /// This value is relative to the current scenario.
  double myImpactAtDetection;

  /// Aggregate of myImpactAtDetection over all of
  /// the scenarios encountered thus far.
  /// Thisdefaults to 0.0, indicating unassigned.
  double myAggregateImpactsAtDetection;

  ///
  int nnodes;

  ///
  bool scenario_aggr;

  ///
  std::string ourImpactOutputFileSuffix;

  ///
  std::string ourEvaluationOutputFileSuffix;

  ///
  std::list<ObjectiveVisitor*> visitors;

};

#endif
