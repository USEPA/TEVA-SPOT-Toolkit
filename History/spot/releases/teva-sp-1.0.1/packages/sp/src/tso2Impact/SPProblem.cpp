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



void SPProblem::read_config_file(istream& istr)
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
  istr >> curr->goal >> curr->impact_file >> curr->threshold;
  if (curr->impact_file != "none") {
     ifstream ifstr(curr->impact_file.c_str());
     if (!ifstr)
        EXCEPTION_MNGR(runtime_error, "Missing impact file: " << curr->impact_file);
     read_impacts(ifstr, curr->impacts, delay);
     if (conserve_memory)
        curr->raw_impacts.clear();
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
  if (curr->objective_flag==false) {
     for (int j=0; j<tmp; j++) {
       double bound;
       istr >> bound;
       curr->bound.push_back(bound);
       }
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
for (unsigned int i=0; i<invalid_placements.size(); i++)
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
if ((goal == "ns") || (goal == "cost")) return;
//
// Resize the superlocation array
//
super_locs.resize(impacts.size());
sl_worst.resize(impacts.size());
//
// Find different super locations for each attack scenario
//
std::vector<map<int,double> >::iterator curr = impacts.begin();
std::vector<map<int,double> >::iterator end  = impacts.end();
utilib::BasicArray<list<int> >::iterator scurr = super_locs.begin();
utilib::BasicArray<map<int,int> >::iterator slcurr = sl_worst.begin();
while (curr != end) {
  //
  // Collect attack impacts and locations
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
  // Order the attack values and compute the super locations
  //
  utilib::BasicArray<int> ord;
  utilib::order(ord,values);
  int last=0;
  utilib::BasicArray<int> locs;
  i=0;
  while ((i<ord.size()) && (nodes[ord[i]] != -1) &&
         !feasible_location(nodes[ord[i]]-1)) {
    i++;
    }
  int prev_feasible=i;
  if (i == ord.size())
     EXCEPTION_MNGR(runtime_error, "SPGoalInfo::compute_superlocations - no feasible points!")
  locs.push_back(nodes[ord[i++]]);
  while (i <= ord.size()) {
    //
    // Ignore infeasible locations
    //
    if ((i < ord.size()) &&
        (nodes[ord[i]] != -1) && !feasible_location(nodes[ord[i]]-1)) {
       i++;
       continue;
       }
    //
    // Perform a simple pass through to find super locations.
    //
    if ((i<ord.size()) && (fabs(values[ord[last]]-values[ord[i]]) <= threshold)) {
       //
       // Add to the current super location set
       //
       locs.push_back(nodes[ord[i]]);
       prev_feasible = i;
       }
    else {
       //
       // Look for the super location set in the current set of membersets
       //
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
       locs.clear();
       while ((i<ord.size()) && (nodes[ord[i]] != -1) &&
              !feasible_location(nodes[ord[i]]-1)) {
         i++;
         }
       if (i == ord.size()) break;
       prev_feasible=i;
       locs.push_back(nodes[ord[i]]);
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



void SPProblem::write_ampl_data(ostream& os)
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
// Goal constraint information
//
os << "param: goalConstraints: GoalBound :=" << endl;
{
utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
while (curr != end) {
  if (curr->objective_flag) {
     //
     // Skip over objectives
     //
     curr++;
     continue;
     }
  list<double>::iterator bcurr = curr->bound.begin();
  list<string>::iterator mcurr = curr->measure.begin();
  list<string>::iterator mend  = curr->measure.end();
  while (mcurr != mend) {
    os << curr->goal << " " << *mcurr << " " << *bcurr << endl;
    bcurr++;
    mcurr++;
    }
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
// Attack information
//
os << endl;
{
os << "param numAttacks := " << endl;
utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
while (curr != end) {
  if (curr->num_attacks() > 0) 
     os << curr->goal << " " << curr->num_attacks() << endl;
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
   os << "param AttackWeight :=" << endl;
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
// Points where attacks can have an impact
//
{
os << endl;
utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
while (curr != end) {
  for (unsigned int i=0; i<curr->impacts.size(); i++) {
    os << "set AttackTouchedLocations [" << curr->goal << "," << (i+1) << "] := ";
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
// The AttackSuperLocation sets
//
{
os << endl;
utilib::BasicArray<SPGoalInfo>::iterator curr = goals.begin();
utilib::BasicArray<SPGoalInfo>::iterator end  = goals.end();
while (curr != end) {
  for (unsigned int i=0; i<curr->super_locs.size(); i++) {
    os << "set AttackSuperLocations [" << curr->goal << "," << (i+1) << "] := ";
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
// The ID of the worst super location member for a given goal and attack
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

os << endl;
os << "end;" << endl;

}
