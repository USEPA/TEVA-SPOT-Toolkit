/**
 * \file SPProblem.cpp
 * 
 * Methods for the SPProblem class.
 */

#include <sp/SPProblem.h>
#include <sp/impacts.h>
#include <fstream>
#include <algorithm>
#include <utilib/BasicArray.h>
#include <utilib/sort.h>

using namespace std;

const double SPGoalInfo::OBJECTIVE_GOAL_BOUND=-99999.0;
// TNT: Added flag to optionally load impact data.  It is not always needed
// to be loaded in this format.
void SPProblem::read_config_file(istream& istr, bool loadImpacts)
{
  int ngoals;
  string fname;
  istr >> num_nodes >> delay;
  feasible_location.resize(num_nodes);
  feasible_location.set();
  //
  // Read goals
  //
  {
    istr >> ngoals;
    goals.resize(ngoals);
    int i=0;
    utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
    utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
    while (curr != end) {
      //
      // Read impact info
      //
      istr >> curr->goal >> curr->impact_file >> curr->threshold >> curr->aggregation_percentage
	   >> curr->aggregation_ratio >> curr->distinguish_detection;
      if (curr->threshold > 0.0 && curr->aggregation_percentage > 0.0)
	// CAP: is there a standard way to issue warnings in teva-sp?
	cerr << "Warning: both threshold and percentage aggregation set for goal " << curr->goal << ". Using percentage aggregation.\n";
      if (curr->aggregation_percentage > 1.0) {
	cerr << "Warning: aggregation percentage greater than 1.0.  Using 1.0 = 100 percent\n";
        curr->aggregation_percentage=1.0;
        }
      if (curr->aggregation_percentage < 0.0) {
	cerr << "Warning: aggregation percentage less than 0.  Using 0.\n";
        curr->aggregation_percentage=1.0;
        }
      if (curr->aggregation_ratio > 1.0) {
	cerr << "Warning: aggregation ratio greater than 1.0.  Using 1.0.\n";
        curr->aggregation_ratio=1.0;
      }
      if (curr->aggregation_ratio < 0.0) {
	cerr << "Warning: aggregation ratio less than 0.0.  Using 0.0.\n";
        curr->aggregation_ratio=0.0;
      }
      if (curr->impact_file != "none") {
	ifstream ifstr(curr->impact_file.c_str());
	if (!ifstr)
	  EXCEPTION_MNGR(runtime_error, "Missing impact file: " << curr->impact_file);
	if(loadImpacts) {
        // TNT: removed tmpArrivalSequences - not used by anything that calls this method
		read_impacts(ifstr, curr->impacts, NULL, delay);
	}
	if (conserve_memory)
	  curr->raw_impacts.clear();

	// if there's a file of event probabilities corresponding to this
	// impact file, read it now.
	std::string probfname = curr->impact_file + ".prob";
	ifstream ifprobstr(probfname.c_str());
	if (ifprobstr) {
		std::map<int,double> probs; 
		read_probabilities(ifprobstr,probs);
		int num_events = curr->num_events();
		curr->probabilities.resize(num_events);
		for (int j=1; j<=num_events; j++) {
			curr->probabilities[j-1] = probs[j];
      		}
	}
      }
      //
      // Read measures
      //
      int tmp;
      istr >> tmp;
      for (int j=0; j<tmp; j++) {
	string measure;
	istr >> measure;
	curr->measure.push_back(measure);
      }
      //
      // Read objective flag
      //
      char c;
      istr >> c;
      if (c == 'o') {
	objectives.push_back(i);
	curr->objective_flag=true;
      }
      else
	curr->objective_flag=false;
      //
      // Read constraint bound information
      //
      for (int j=0; j<tmp; j++) {
	double bound;
	istr >> bound;
	curr->bound.push_back(bound);
      }
      i++;
      curr++;
    }
  }

  //
  // Read fixed placements
  //
  read_array(istr,fixed_placements);
  //
  // Read invalid placements
  //
  read_array(istr,invalid_placements);

  for(unsigned int i=0; i<invalid_placements.size(); i++)
    if((invalid_placements[i]>=1)&&(invalid_placements[i]<=num_nodes))
      feasible_location.reset(invalid_placements[i]-1);

  //cerr << feasible_location << endl;
  //
  // Read costs
  //
  read_id_costs(istr,costs);

}



void SPProblem::compute_superlocations()
{
  utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
  utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
  while (curr != end) {
    curr->compute_superlocations(membersets,feasible_location);
    curr++;
  }
}


void SPGoalInfo::compute_superlocations(map<utilib::BasicArray<int>,int>& membersets,
					utilib::BitArray& feasible_location)
{
  if ((goal == "ns") || (goal == "cost") || (goal=="awd")) return;
  //
  // The default aggregation ratio is 1.0
  //
  final_aggregation_ratio=1.0;
  //
  // Resize the superlocation array
  //
  super_locs.resize(impacts.size());
  sl_worst.resize(impacts.size());
  //
  // Find different super locations for each event
  //
  std::vector<map<int,double> >::iterator curr = impacts.begin();
  std::vector<map<int,double> >::iterator end  = impacts.end();

  // Compute the aggregation threshold if necessary.  We're saving space
  // instead of time, so there's some redundant pulling of impact information, etc.
  // If the threshold is also set, we reset using percentages.  We should have
  // already issued a warning in that case.  Recall: a threshold of -1 disables
  // aggregation.

  if (threshold >= 0 && aggregation_percentage > 0 && aggregation_percentage <= 1.0)
    {
      // Find biggest difference between (feasible) detection and no detection
      double max_difference = 0.0;
      double lowest_impact, no_detection_impact;
      bool lowest_impact_set;
      while (curr != end) {
	if (curr->size() == 1)
	  continue; // no differential to consider
	// Get impact information for this event.  Repeat of what's below, but changing
	// the index to better distinguish the sections.
	utilib::BasicArray<int> nodes(curr->size());
	utilib::BasicArray<double> values(curr->size());
	map<int,double>::iterator mcurr = curr->begin();
	map<int,double>::iterator mend  = curr->end();
	unsigned int j=0;
	while (mcurr != mend) {
	  nodes[j] = mcurr->first;
	  values[j] = mcurr->second;
	  j++;
	  mcurr++;
	}

	// We won't sort this, since only need max and min impact.

	// This is to avoid having to figure out what we're using for MAXDOUBLE.
	lowest_impact_set = false;
	for (j = 0; j < nodes.size(); j++)
	  {
	    if (nodes[j] == -1)
	      {
		no_detection_impact = values[j];
		continue;
	      }
	    // feasible_location is indexed by node ID - 1 (nodes are 1-based)
	    if (!feasible_location(nodes[j]-1))
	      continue;
	    if (values[j] < lowest_impact || !lowest_impact_set)
	      {
		lowest_impact = values[j];
		lowest_impact_set = true;
	      }
	  }
	if (lowest_impact_set && ((no_detection_impact - lowest_impact) > max_difference))
	  max_difference = no_detection_impact - lowest_impact;
	curr++;
      }
      threshold = aggregation_percentage * max_difference;

      // reset this for the next pass
      curr = impacts.begin();
    }

  utilib::BasicArray<list<int> >::iterator scurr = super_locs.begin();
  utilib::BasicArray<map<int,int> >::iterator slcurr = sl_worst.begin();
  while (curr != end) {
    double tmp_threshold=threshold;
    //
    // Collect event impacts and locations
    //
    utilib::BasicArray<int> nodes(curr->size());
    utilib::BasicArray<double> values(curr->size());
    map<int,double>::iterator mcurr = curr->begin();
    map<int,double>::iterator mend  = curr->end();
    unsigned int i=0;
    while (mcurr != mend) {
      nodes[i] = mcurr->first;
      values[i] = mcurr->second;
      i++;
      mcurr++;
    }
    //
    // Order the event values and compute the super locations
    //
    utilib::BasicArray<int> ord;
    utilib::stable_order(ord,values,std::greater<double>());
    utilib::BasicArray<int> locs;
    utilib::BasicArray<double> lvalues;
    double sl_worst, sl_best;
    //
    // Start by adding the 'nonwitnessed' location
    //
    i=0;
    //
    // Many nodes may have the worst case impact; not just the dummy.
    // We now have a sorted order in 'ord', but the index of the dummy node 
    // is not guaranteed to be in ord[0].  For example the impact fragment:
    // 3 2 400.5
    // 3 -1 400.5
    // contributes two "400.5"s to the values array.  The utilib stable_order
    // routine will rank these at the top, but it's a toss up whether witness
    // "2" or "-1" gets the zero'th spot.  If "2" got it, then the increment
    // of i below in the line commented "skip dummy" will fail in its purpose.
    // We have to do a final swap to make sure that the dummy node is indeed
    // ranked in ord[0].
    while ((i<ord.size()) && (nodes[ord[i]] != -1)) {
      i++;
    }
    int tmp = ord[i];
    ord[i] = ord[0];
    ord[0] = tmp;

    assert(nodes[ord[0]] == -1);
    sl_worst=sl_best = values[ord[0]];
    locs.push_back(nodes[ord[0]]);
    lvalues.push_back(values[ord[0]]);  

    // last marks the first element of the superlocation set under construction.
    // This is an index into ord[].  So it marks the rank.
    int last=0;
    // prev_feasible marks the last element added to the superlocation set
    // under construction (guaranteed within threshold of the first element).
    // Again this is an index into ord[] (hence a rank).
    int prev_feasible=0;
    //
    // Find next feasible location
    //
    i++;			// skip dummy
    while (i <= ord.size()) {
      //
      // Ignore infeasible locations
      //
      if ((i<ord.size()) && !feasible_location(nodes[ord[i]]-1)) {
	i++;
	continue;
      }
      //
      // Perform a simple pass through to find super locations.  Add current node to the
      // supernode we're growing as long as it meets the threshold conditions.  If this is
      // the last node, then act as if the threshold is exceeded.
      //
      if ((i<ord.size()) && (values[ord[last]] < values[ord[i]])) cerr << "ERROR: somehow our items are not sorted!";
#if 0
      if (i<ord.size()) cerr << "# Info:    " << i << " " << distinguish_detection << " " << (values[ord[last]]-values[ord[i]]) << " " << tmp_threshold << " " << aggregation_ratio << " " << (values[ord[i]]/values[ord[last]]) << " " << values[ord[i]] << " " << values[ord[last]] << endl;
      if (i<ord.size()) cerr << "# " << (i<ord.size())  << " " <<
           (!distinguish_detection || (distinguish_detection && ((last>0) || (values[ord[i]] == values[ord[0]]))))  << " " <<
           ((values[ord[last]]-values[ord[i]]) <= tmp_threshold) << " " <<
           ((values[ord[i]]/values[ord[last]]) >= aggregation_ratio) << endl;
#endif

      if ( (i<ord.size()) && tmp_threshold >= 0.0 &&
           (!distinguish_detection || (distinguish_detection && ((last>0) || (values[ord[i]] == values[ord[0]]))))  && 
           ((values[ord[last]]-values[ord[i]]) <= tmp_threshold) &&
           ((values[ord[i]]/values[ord[last]]) >= aggregation_ratio)
         )
	{
	  //
	  // Add to the current super location set
	  //
          if (values[ord[i]] > sl_worst)
             sl_worst=values[ord[i]];
          if (values[ord[i]] < sl_best)
             sl_best=values[ord[i]];
	  locs.push_back(nodes[ord[i]]);
	  lvalues.push_back(values[ord[i]]);
	  prev_feasible = i;
	}
      else {
	//
	// Look for the super location set in the current set of membersets
	//
	#if 0
        cerr << "# SuperLoc IDs:    " << locs << endl;
        cerr << "# SuperLoc Values: " << lvalues << endl;
        cerr << "# Ratio:           ";
        if  (max(lvalues) == 0.0) cerr << "1.0" << endl;
        else cerr << min(lvalues)/max(lvalues) << endl;
        cerr << "#" << endl;
	#endif
	sort(locs.begin(),locs.end());
	map<utilib::BasicArray<int>,int>::iterator lcurr = membersets.find(locs);
	if (lcurr == membersets.end()) {
          //
          // Add to the current super location set
          //
	  int ndx = membersets.size();
          scurr->push_back(ndx);
          (*slcurr)[ndx] = nodes[ord[prev_feasible]];
          membersets[locs] = ndx;
	}
	else {
          scurr->push_back(lcurr->second);
          (*slcurr)[lcurr->second] = nodes[ord[prev_feasible]];
	}
	last = i;
        if ((sl_worst != 0.0) && (final_aggregation_ratio > (sl_best/sl_worst)))
           final_aggregation_ratio = sl_best/sl_worst;
        /// NOTE: if this is uncommented, then only one superlocation is created with a nonzero threshold.
        //tmp_threshold=0.0;
	locs.clear();
	lvalues.clear();
	if (i == ord.size()) break;
	prev_feasible=i;
        sl_worst=sl_best = values[ord[i]];
	locs.push_back(nodes[ord[i]]);
	lvalues.push_back(values[ord[i]]);
      }
      i++;
    }
    //
    // Update indices
    //
    curr++;
    scurr++;
    slcurr++;
  }
}



void SPProblem::write_ampl_data(ostream& os, bool imperfect)
{
  os << "#" << endl;
  os << "# GeneralSP AMPL file" << endl;
  os << "#" << endl;

  //
  // For now, we only consider a single objective
  //
  os << endl;
  os << "param objectiveGoal := " << goals[*(objectives.begin())].goal << " ;" << endl;
  os << "param objectiveMeasure := " << *(goals[*(objectives.begin())].measure.begin()) << " ;" << endl;
  os << endl;
  //
  // Value used for CVaR and TCE calculations
  //
  os << "param gamma := " << gamma <<  " ;" << endl;
  //
  // Goal constraint information
  //
  os << "param: goalConstraints: GoalBound :=" << endl;
  {
    utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
    utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
    while (curr != end) {
      list<double>::iterator bcurr = curr->bound.begin();
      list<string>::iterator mcurr = curr->measure.begin();
      list<string>::iterator mend  = curr->measure.end();
      while (mcurr != mend) {
        double goalval = *bcurr;
	if (goalval != SPGoalInfo::OBJECTIVE_GOAL_BOUND)
		os << curr->goal << " " << *mcurr << " " << *bcurr << endl;
	bcurr++;
	mcurr++;
      }
      curr++;
    }
    os << ";" << endl;
  }
  //
  // Superlocation aggregation threshold information
  //
  os << endl;
  {
    os << "param slThreshold := " << endl;
    utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
    utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
    while (curr != end) {
      if (curr->num_events() > 0) 
	os << curr->goal << " " << curr->threshold << endl;
      curr++;
    }
    os << ";" << endl;
  }
  //
  // Superlocation aggregation ratio information
  //
  os << endl;
  {
    os << "param slAggregationRatio := " << endl;
    utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
    utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
    while (curr != end) {
      if (curr->num_events() > 0) 
	os << curr->goal << " " << curr->final_aggregation_ratio << endl;
      curr++;
    }
    os << ";" << endl;
  }

  ////
  //// Network location information
  ////
  os << endl;
  os << "param numNetworkLocations := " << num_nodes << " ;" << endl;
  //
  // Fixed locations
  //
  if (fixed_placements.size() > 0) {
    os << "set FixedSensorLocations :=" << endl;
    utilib::BasicArray<int>::iterator curr = fixed_placements.begin();
    utilib::BasicArray<int>::iterator end  = fixed_placements.end();
    while (curr != end) {
      os << *curr << endl;
      curr++;
    }
    os << ";" << endl;
  }
  //
  // Invalid locations
  //
  if (invalid_placements.size() > 0) {
    os << "set InfeasibleLocations :=" << endl;
    utilib::BasicArray<int>::iterator curr = invalid_placements.begin();
    utilib::BasicArray<int>::iterator end  = invalid_placements.end();
    while (curr != end) {
      os << *curr << endl;
      curr++;
    }
    os << ";" << endl;
  }

  //
  // Event information
  //
  os << endl;
  {
    os << "param numEvents := " << endl;
    utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
    utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
    while (curr != end) {
      if (curr->num_events() > 0) 
	os << curr->goal << " " << curr->num_events() << endl;
      curr++;
    }
    os << ";" << endl;
  }
  {
    int tmp=0;
    for (unsigned int i=0; i<goals.size(); i++)
      tmp +=goals[i].probabilities.size();
    if (tmp > 0) {
      //
      // At least one of the goals has specified probabilities
      //
      os << "param EventWeight :=" << endl;
      utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
      utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
      while (curr != end) {
	for (unsigned int i=0; i<curr->probabilities.size(); i++)
	  os << curr->goal << " " << (i+1) << " " << curr->probabilities[i] << endl;
	curr++;
      }
      os << ";" << endl;
    }
  }

  //
  // Points where events can have an impact
  //
  {
    os << endl;
    utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
    utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
    while (curr != end) {
      for (unsigned int i=0; i<curr->impacts.size(); i++) {
	os << "set EventTouchedLocations [" << curr->goal << "," << (i+1) << "] := ";
	map<int,double>::iterator mcurr = curr->impacts[i].begin();
	map<int,double>::iterator mend  = curr->impacts[i].end();
	while (mcurr != mend) {
	  os << " " << mcurr->first;
	  mcurr++;
	}
	os << ";" << endl;
      }
      curr++;
    }
  }

  //
  // SuperLocations
  //
  os << endl;
  {
    os << "set SuperLocations :=";
    map<utilib::BasicArray<int>,int>::iterator curr = membersets.begin();
    map<utilib::BasicArray<int>,int>::iterator end  = membersets.end();
    while (curr != end) {
      os << " " << curr->second;
      curr++;
    }
    os << ";" << endl;
  }

  //
  // SuperLocation Members
  //
  os << endl;
  {
    map<utilib::BasicArray<int>,int>::iterator curr = membersets.begin();
    map<utilib::BasicArray<int>,int>::iterator end  = membersets.end();
    while (curr != end) {
      os << "set SuperLocationMembers [" << curr->second << "] :=";
      utilib::BasicArray<int>::const_iterator lcurr = curr->first.begin();
      utilib::BasicArray<int>::const_iterator lend  = curr->first.end();
      while (lcurr != lend) {
	os << " " << *lcurr;
	lcurr++;
      }
      os << ";" << endl;
      curr++;
    }
  }

  //
  // The EventSuperLocation sets
  //
  {
    os << endl;
    utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
    utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
    while (curr != end) {
      for (unsigned int i=0; i<curr->super_locs.size(); i++) {
	os << "set EventSuperLocations [" << curr->goal << "," << (i+1) << "] := ";
	list<int>::iterator lcurr = curr->super_locs[i].begin();
	list<int>::iterator lend  = curr->super_locs[i].end();
	while (lcurr != lend) {
	  os << " " << *lcurr;
	  lcurr++;
	}
	os << ";" << endl;
      }
      curr++;
    }
  }


  //
  // Impact measuresments for each goal
  //
  {
    os << endl;
    os << "param GoalMeasurement :=" << endl;
    utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
    utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
    while (curr != end) {
      for (unsigned int i=0; i<curr->impacts.size(); i++) {
	map<int,double>::iterator mcurr = curr->impacts[i].begin();
	map<int,double>::iterator mend  = curr->impacts[i].end();
	while (mcurr != mend) {
	  os << curr->goal << " " << (i+1) << " " << mcurr->first << " " << mcurr->second << endl;
	  mcurr++;
	}
      }
      curr++;
    }
    os << ";" << endl;
  }

  //
  // Placement costs
  //
  if (costs.size() > 0) {
    os << "param PlacementCost :=" << endl;
    double dval;
    map<int,double>::iterator tmp = costs.find(-1);
    if (tmp == costs.end())
      dval = 0.0;
    else
      dval = tmp->second;
    for (unsigned int i=0; i<feasible_location.size(); i++) 
      if (feasible_location(i)) {
        map<int,double>::iterator tmp = costs.find(i+1);
        if (tmp == costs.end())
	  os << (i+1) << " " << dval << endl;
        else
	  os << (i+1) << " " << costs[i+1] << endl;
      }
    os << ";" << endl;
  }

  //
  // The ID of the worst super location member for a given goal and event
  //
  {
    os << endl;
    os << "param WorstSuperLocationMember :=" << endl;
    utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
    utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
    while (curr != end) {
      for (unsigned int i=0; i<curr->sl_worst.size(); i++) {
	map<int,int>::iterator mcurr = curr->sl_worst[i].begin();
	map<int,int>::iterator mend  = curr->sl_worst[i].end();
	while (mcurr != mend) {
	  os << curr->goal << " " << (i+1) << " " << mcurr->first << " " << mcurr->second << endl;
	  mcurr++;
	}
      }
      curr++;
    }
    os << ";" << endl;
  }

  if (imperfect) {
    os << "param numEnvironmentTypes := " << 3 << " ;" << endl;

    os << "param EnvironmentTypes := " << endl;
    for (int i=1; i<num_nodes/3; i++) {
      os << i << " 1" << endl;
    }
    for (int i=num_nodes/3; i<2*num_nodes/3; i++) {
      os << i << " 2" << endl;
    }
    for (int i=2*num_nodes/3; i<num_nodes; i++) {
      os << i << " 3" << endl;
    }
    os << ";" << endl << endl;

    os << "param FalsePositiveRate := " << endl;
    os << "1 0.00001" << endl;
    os << "2 0.00001" << endl;
    os << "3 0.00001" << endl;
    os << ";" << endl << endl;

    os << "param FalseNegativeProbability := " << endl;
    os << "1 0.125" << endl;
    os << "2 0.25" << endl;
    os << "3 0.5" << endl;
    os << ";" << endl;

  }

  os << endl;
  os << "end;" << endl;
}

void SPProblem::write_lagrangian_data(std::ostream& os, bool imperfect)
{
  //
  // Write the input file for the lagrangian relaxation code.
  //
  if (goals.size() != 2) {
        std::cerr << "lagrangean/subgradient method currently supports "
                << "a single objective only" << std::endl;
  }
  utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
  SPGoalInfo spg = *curr;
  int num_customers = curr->num_events();

  int tmp = 0;
  for (unsigned int i=0; i<goals.size(); i++)
      tmp +=goals[i].probabilities.size();
  if (tmp == 0) {
	double uniform_prob = 1.0/num_customers;
	goals[0].probabilities.resize(num_customers);
	for (int i=0; i<num_customers; i++) {
		goals[0].probabilities[i] = uniform_prob;
	}
  }

  //
  // Impact measuresments for each goal
  //
    double total_of_max_impacts = 0;
    int num_events=0;
    int num_impacts=0;
    std::list<int> theLocations;
    curr = goals.begin();
    utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
    int goalCount=0;
    while (curr != end && goalCount < 1) {
      goalCount++;
      num_events = curr->impacts.size();
      for (unsigned int i=0; i<curr->impacts.size(); i++) {
        map<int,double>::iterator mcurr = curr->impacts[i].begin();
        map<int,double>::iterator mend  = curr->impacts[i].end();
        double max_impact = 0;
        while (mcurr != mend) {
          if (mcurr->second > max_impact) {
                max_impact = mcurr->second;
          }
	  num_impacts++;
          theLocations.push_back(mcurr->first);
          mcurr++;
        }
        total_of_max_impacts += max_impact;
        theLocations.sort();    // is there a better way to get the
        theLocations.unique();  // node id's?
      }
      curr++;
    }
    int num_sensors=0;
    if (curr != end) {
      list<double>::iterator bcurr = curr->bound.begin();
      list<string>::iterator mcurr = curr->measure.begin();
      list<string>::iterator mend  = curr->measure.end();
      while (mcurr != mend) {
        double goalval = *bcurr;
        if (curr->goal == "ns" && *mcurr == "total")
                num_sensors = (int) *bcurr;
        bcurr++;
        mcurr++;
      }
    }
    int num_nodes_hit = theLocations.size();
    double cost_of_one_facility = total_of_max_impacts/num_nodes_hit;
    assert(theLocations.size() <= num_nodes);
    int *water2lr_map = new int[num_nodes];
    int *lr2water_map = new int[num_nodes_hit];
    int next_id=1;
    list<int>::iterator lit = theLocations.begin();
/*  This was for uflp (now deprecated)
    while (lit != theLocations.end()) {
        int loc = *lit;
        if (loc == -1) loc = num_nodes;
        os << cost_of_one_facility << std::endl;
        water2lr_map[loc] = next_id;
        lr2water_map[next_id] = loc;
        lit++;
        next_id++;
    }
*/
    // lagrangian solver wants a sensor to place on the dummy
    // JWB: but this is a problem if the dummy is never a witness.
    //      we used to send num_sensors+1, but in the no-dummy-witness
    //      case, that gave it too many sensors!  We need to modify the
    //      ufl code to lock down the dummy as a sensor.
    os << num_nodes << " " << num_events << " " << num_impacts << " "
	<< num_sensors+1 << endl;
    curr = goals.begin(); 
    end  = goals.end(); 
    while (curr != end) {
      for (unsigned int i=0; i<curr->impacts.size(); i++) {
        map<int,double>::iterator mcurr = curr->impacts[i].begin();
        map<int,double>::iterator mend  = curr->impacts[i].end();
	double prob = goals[0].probabilities[i];
        while (mcurr != mend) {
          int loc = mcurr->first;
          if (loc == -1) {
		loc = num_nodes;
	  } else {
		loc = loc - 1;
	  }
          os << loc << "\t" << i+1 << "\t" << prob * mcurr->second << std::endl;
          mcurr++;
        }
      }
      curr++;
  } 
  delete [] lr2water_map;
  delete [] water2lr_map;
}

