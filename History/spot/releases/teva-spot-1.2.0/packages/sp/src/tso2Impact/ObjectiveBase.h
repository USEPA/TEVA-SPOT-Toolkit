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

// required for the health impacts objective
#include "teva-assess.h"
#include "ta_input.h"
#include "tai.h"
#include "sss.h"
#include "ta_utils.h"
//#include "NamedData.h"	// for NamedDataRef (constructAggregationServerData())
//#include "ExternalAnalysis.h" // for setIntArray, setDoubleArray 
}

#include "Impact.h"
#include "VecTrie.h"

class scenario_group {
public:
	// no need to store event indices explicitly;
	// these objects will be stored in a vector, and the index will be
	// the group id.  Events will map to group id's.

	// we need to know the maximum witness list length for any event
	// in this group
	scenario_group() : max_length(0), num_events(0), event_prob(0.0),
			   hit_sequence(), hit_times(), impacts() {}
	scenario_group(const scenario_group& sg) : 
			max_length(sg.max_length),
			num_events(sg.num_events),
			event_prob(sg.event_prob),
			hit_sequence(sg.hit_sequence),
			hit_times(sg.hit_times),
			impacts(sg.impacts) {}
	int max_length;	
	int num_events;
	double event_prob;	
	std::vector<int> hit_sequence;
	std::vector<int> hit_times;	// correspond to those of a max length
					// event in this aggregated event
	std::vector<double> impacts;
};

class ScenarioAggrVisitor {
public:
	ScenarioAggrVisitor(vector<int>& k, vector<int>& sp, 
		 	    std::vector<scenario_group>& grps,
			    std::map<int, int>& group_mp) 
		: spine_scenarios(sp), groups(grps),
		  group_map(group_mp) {}

	void discover(VecTrieNode<int,vector<int> >* n, int nextchar) 
	{
		key.push_back(nextchar);
		if (n->hasdata()) {
			vector<int>& v = n->getdata();
			for (int i=0; i<v.size(); i++) {
				spine_scenarios.push_back(v[i]);
			}
		}
	}
	void finish(VecTrieNode<int,vector<int> >* n)
	{
		if (n->hasdata() && (spine_scenarios.size() > 0)) {
			outputGroup();
		}
		key.pop_back();
	}

	void outputGroup() { 
		scenario_group sg;
		int group_index = groups.size();
		sg.max_length = key.size() + 1;
		cout << "outputGroup: setting ml[" << group_index << "] to " << sg.max_length << endl;
		sg.num_events = spine_scenarios.size();
		sg.hit_sequence.reserve(sg.max_length);
		sg.hit_times.reserve(sg.max_length);
		sg.impacts.reserve(sg.max_length);
		for (int i=0; i<sg.max_length; i++) {
			sg.hit_sequence.push_back(0);
			sg.hit_times.push_back(0);
			sg.impacts.push_back(0.0);
		}
		std::cout << "TrieNode: ";
		for (int i=0; i<key.size(); i++) {
			std::cout << key[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "Aggregated Scenario Indices: " 
			  << std::endl;
		for (int j=0; j<spine_scenarios.size(); j++) {
			int event_ind = spine_scenarios[j];
			group_map[event_ind] = group_index;
			std::cout << spine_scenarios[j] << " ";
		}
		std::cout << std::endl;
		groups.push_back(sg);
		spine_scenarios.clear();
	}

private:
	vector<int> key;
	vector<int>& spine_scenarios;
	std::vector<scenario_group>& groups;
	std::map<int, int>& group_map; 
};

class DepthFindingScenarioAggrVisitor {
public:
	DepthFindingScenarioAggrVisitor(
		std::map<VecTrieNode<int,vector<int> >*, int>& m) 
		: next(NULL), depthmap(m), maxDepthToLeaf(0) {}
	DepthFindingScenarioAggrVisitor(const DepthFindingScenarioAggrVisitor&v)
		: depthmap(v.depthmap), 
		  next( (DepthFindingScenarioAggrVisitor*) &v),
		  maxDepthToLeaf(0) { }

	void discover(VecTrieNode<int,vector<int> >* n, int nextchar) 
	{
		if (n->numlinks()==0) {
			DepthFindingScenarioAggrVisitor *tmp = next;
			int curdepth=1;
			while (tmp && curdepth > tmp->maxDepthToLeaf) {
				tmp->maxDepthToLeaf = curdepth;
				curdepth++;
				tmp = tmp->next;
			}
		}
	}
	void finish(VecTrieNode<int,vector<int> >* n) 
	{
		depthmap[n] = maxDepthToLeaf;
	}
private:
	int maxDepthToLeaf;
	DepthFindingScenarioAggrVisitor *next;
	std::map<VecTrieNode<int, vector<int> >*, int>& depthmap;
};

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
    myAggregateImpactsAtDetection(0.0),
    theScenarioAggrTrie(NULL),
    scenario_aggr(false)
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

  virtual void update(utilib::AnyValue) {}

   void setScenarioAggr(bool sa) { scenario_aggr = sa; }

  // Set the sensor locations for this objective.
  void setSensorLocations(const std::list<int> &theLocations)
    {
      mySensorLocations=theLocations;
    }

  void setScenarioAggrTrie(VecTrie<int, std::vector<int> >*theTrie)
    {
      theScenarioAggrTrie=theTrie;
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
      myImpacts.reserve(net->nnodes);
      myImpactAtDetection=-1.0;
      myAggregateImpactsAtDetection=0.0;
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
  virtual void finalizeForScenario(int scenarioIndex,PSource sources, 
				   bool scenario_aggr=false)
    {
      outputScenarioImpactData(scenarioIndex,sources,scenario_aggr);
      //outputScenarioEvaluationData(scenarioIndex,sources);
      myAggregateImpactsAtDetection+=myImpactAtDetection;
    }  

  /// Output the impacts for the current scenario.
  virtual void outputScenarioImpactData(int scenarioIndex,PSource sources,
				 	bool scenario_aggr=false)
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
      if (scenario_aggr) {
	  vector<int> hit_list;
	  for(int i=0;i<int(myImpacts.size()-1);i++) {  // don't include dummy
	  	hit_list.push_back(myImpacts[i].nodeID);
	  }
	  VecTrieKey<int> key(hit_list);
	  VecTrieNode<int,vector<int> >* n = theScenarioAggrTrie->getnode(key);
	  if (!n) {
		std::vector<int> scenario_list; 
		theScenarioAggrTrie->insert(key, scenario_list);
	  } else if (!n->hasdata()) {
		std::vector<int> scenario_list; 
		n->setdata(scenario_list);
	  }
	  std::vector<int>& scenario_list = theScenarioAggrTrie->getdata(key);
	  scenario_list.push_back(scenarioIndex);
      } 

      if(myScenarioImpactOutputFile) {
      
        for(int i=0;i<int(myImpacts.size());i++) {
	      // NOTE: the current .impact file format doesn't handle multiple 
	      // source node indicies the scenario indicies are 1-based (along 
	      // with the node indicies, enforced upon impact entry creation)
	     (*myScenarioImpactOutputFile) << scenarioIndex << " " 
					   << myImpacts[i].nodeID 
			<< " " << myImpacts[i].timeOfDetection << " " 
			<< myImpacts[i].impact << std::endl;
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
      if (scenario_aggr) 
	{
          vector<int> key;
          vector<int> spine_scenarios;
          int num_groups = 0;
          std::vector<scenario_group> groups;
          std::map<int, int> group_map;
          ScenarioAggrVisitor vis(key, spine_scenarios, groups, group_map);
          theScenarioAggrTrie->dfs(vis);
          write_aggregated_impact_file(groups, group_map);
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

  VecTrie<int,std::vector<int> >* theScenarioAggrTrie;

  int nnodes;
  bool scenario_aggr;

};

#endif
