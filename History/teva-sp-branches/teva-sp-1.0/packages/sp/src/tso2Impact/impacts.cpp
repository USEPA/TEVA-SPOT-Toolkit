/*!
 * \file impacts.cpp
 * \brief Routines to evaluate the impact of a contamination event, given
 * 	  sensors.
 */

#include "impacts.h"
#include <utilib/sort.h>
#include <utilib/comments.h>
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

///
class SPCompare
{
public:

  ///
  bool operator()(const pair<double,double>& a, 
				const pair<double,double>& b) const
  {
  if (a.first < b.first) return true;
  if (a.first > b.first) return false;
  return (a.second < b.second); 
  }

};

void read_probabilities(std::istream& istr, std::map<int,double>& probabilities)
{
//
// Read the (node,probability) pairs
//
double total=0.0;
int nodeid;
istr >> nodeid;
while (istr) {
  double probability;
  istr >> probability;
  total += probability;
  probabilities[nodeid] = probability;
  istr >> nodeid;
  }
//
// Double check that this is a valid probability distribution
//
if (fabs(total-1.0) > 1e-7)
   EXCEPTION_MNGR(runtime_error, "read_probabilities - Invalid probability distribution!  Total=" << total;)
}


void read_impacts(std::istream& istr, std::vector<std::map<int,double> >& impacts, int delay)
{
  int nnodes, numDelays;
  istr >> nnodes >> numDelays;
  #if 0
  utilib::IntVector delays(numDelays);
  for (int i=0; i < numDelays; i++) {
  	istr >> delays[i];
	if (i > 0)  assert(delays[i] >= delays[i-1]);  // force tso2Impact to
						       // put these in order
  }
  // now, search the delays to find the index of the delay specified by the user
  int delayIndex = -1;
  for (int i=0; i < numDelays; i++) {
	if (delays[i] == delay)
		delayIndex = i;
  }
  #endif
	
  //
  // Read delays
  //
  int delayIndex=-1;
  for (int i=0; i<numDelays; i++) {
    int tmp;
    istr >> tmp;
    if (tmp == delay) delayIndex = i;
    }
  if (delayIndex == -1) {
	EXCEPTION_MNGR(runtime_error, "delay " 
		<< delay << " not found in impact file" << endl);
  }
  //
  // Variables used to read in the impact file
  // NOTE: attack and impact IDs are locations that are 1-based indices.
  //
  int prevScenarioIndex=-1;		// ID of the previous attack scenario
  int scenarioIndex;			// Attack scenario ID
  //
  // Read in the impact file.
  //
  istr >> scenarioIndex;
  while(istr)
    {
      //
      // If we have a new scenario, then extent the impacts array to include a new 
      // hash table for the impacts that this new attack scenario has had.
      //
      if(scenarioIndex!=prevScenarioIndex)
	{
	  impacts.push_back(std::map<int,double>());
	  prevScenarioIndex=scenarioIndex;
	}
      //
      // Add the new impact that we have just read to the hash table for the
      // latest attack scenario
      //
      int hitNodeIndex;			// ID of an impacted node "downstream"
      int witnessTime;
      double impact(0.0);
      istr >> hitNodeIndex >> witnessTime;
      for (int i=0; i<numDelays; i++) {
        double tmp;
        istr >> tmp;
        if (i == delayIndex) impact = tmp;
        }
      impacts.back()[hitNodeIndex]=impact;
      #if 0
      int detectionTime;		// true detection, not considering delay
      int impactTime;   		// time at which delayed impact is 
					// measured (detect + delay)
      double impact=0.0;
      istr >> hitNodeIndex >> detectionTime;
      impactTime = detectionTime + delay;
      for (int i=0; i<numDelays; i++) {
		double nextImpact;
		istr >> nextImpact;
		if (i == delayIndex) {
			impact = nextImpact;
		}
      }
      impacts.back()[hitNodeIndex]=impact;
      #endif
      //
      //cerr << "DEBUG " << scenarioIndex << " " << hitNodeIndex << " " << impact << " " << delay << endl;
      //
      // Read in the next impact
      //
      istr >> scenarioIndex;
    }
}
 


void compute_delayed_impacts(utilib::BasicArray<std::map<int,std::pair<double,double> > >& raw_impacts,
                        double delay,
                        utilib::BasicArray<std::map<int,double> >& impacts)
{
impacts.resize(raw_impacts.size());
for (unsigned int i=0; i<raw_impacts.size(); i++) {
  utilib::BasicArray<pair<double,double> > values(raw_impacts[i].size());
  utilib::BasicArray<int> node(raw_impacts[i].size());
  utilib::BasicArray<int> ord;
  {
  std::map<int,std::pair<double,double> >::iterator curr = raw_impacts[i].begin();
  std::map<int,std::pair<double,double> >::iterator end  = raw_impacts[i].end();
  int k=0;
  while (curr != end) {
    values[k] = curr->second;
    node[k] = curr->first;
    k++;
    curr++;
    }
  order(ord, values, SPCompare());
  }
  //for (unsigned int j=0; j<ord.size(); j++)
    //cerr << j << " " << node[ord[j]] << " " << values[ord[j]].first << " " << values[ord[j]].second << endl;

  if (ord.size() == 0)
     EXCEPTION_MNGR(runtime_error, "Null impacts!?!?!")
  double last_impact;
  unsigned int ii=0, jj=1;
  last_impact = values[ord[jj-1]].first;
  while ((jj < ord.size()) &&
         (values[ord[jj]].second <= (delay + 1e-7 + (values[ord[ii]].second)))) {
    last_impact = values[ord[jj-1]].first;
    cerr << "jj: " << jj << " " << "last impact reset to: " << last_impact << endl;
    jj++;
    }
  last_impact = values[ord[jj-1]].first;
  while (ii < ord.size()) {
    impacts[i][node[ord[ii]]] = last_impact;
    ii++;
    while ((jj < ord.size()) &&
         (values[ord[jj]].second <= (delay + 1e-7 + values[ord[ii]].second))) {
      last_impact = values[ord[jj-1]].first;
      jj++;
      }
    last_impact = values[ord[jj-1]].first;
    }
  //for (unsigned int j=0; j<ord.size(); j++)
    //cerr << j << " " << node[ord[j]] << " " << impacts[i][node[ord[j]]] << endl;
  }
}


void read_sensor_placements(std::istream& istr, std::map<int, utilib::BasicArray<int> >& sensors)
{
istr >> utilib::comment_lines;
//
// Read in the sensors one at a time
//
while (istr) {
  int designID;
  istr >> designID;
  if (!istr) break;
  int len;
  istr >> len;
  utilib::BasicArray<int> array(len);
  for (int i=0; i<len; i++)
    istr >> array[i];
  sensors[designID] = array;
  istr >> utilib::comment_lines;
  }
}


void read_network_ids(std::istream& istr, utilib::BasicArray<int>& sensors)
{
  //
  // Read in the sensors one at a time
  //
  int newLocation;
  istr >> newLocation;
  while(!(!istr))
    {
      sensors.push_back(newLocation);
      istr >> newLocation;
    }
}


void compute_statistics( const utilib::BasicArray<int>& sensors,
			std::vector<std::map<int,double> >& impacts,
			ImpactStats& stats,
			std::map<int,double>& probabilities,
			utilib::BasicArray<int>& work,
			double gamma)
{
//
// Collect the impact values for each attack
//
utilib::BasicArray<double> values(impacts.size());
stats.witness.resize(impacts.size());
stats.impact.resize(impacts.size());
for (size_t i=0; i<impacts.size();i++) {
  std::map<int,double> &theseImpacts=impacts[i];

  int witnessOfMin=-1;
  double minImpact = theseImpacts[witnessOfMin];
  for (size_t j=0;j<sensors.size();j++) {
    if(theseImpacts.find(sensors[j])!=theseImpacts.end())
	    {
	      if (minImpact > theseImpacts[sensors[j]]) {
	         minImpact=theseImpacts[sensors[j]];
		 witnessOfMin = sensors[j];
                 }
	    }
    }
  stats.witness[i] = witnessOfMin;
  stats.impact[i] = values[i] = minImpact;
  }
//
// Setup the probability array
//
utilib::BasicArray<double> probs(values.size());
if (probabilities.size() > 0) {
   probs << 0.0;
   std::map<int,double>::iterator curr = probabilities.begin();
   std::map<int,double>::iterator end = probabilities.end();
   while (curr != end) {
     probs[curr->first] = curr->second;
     curr++;
     }
   }
else {
   probs << 1.0/values.size();
   }
//
// Setup data structures for weighted quantiles
//
// NOTE: this is probably not the most efficient way to do this
//
utilib::order(work,values);
utilib::BasicArray<double> cumval(values.size());
cumval[0] = probs[work[0]];
for (unsigned int i=1; i<values.size(); i++)
  cumval[i] = cumval[i-1] + probs[work[i]];
//
// Compute values for the statistics object
//
// nscenarios
stats.nscenarios = impacts.size();
// MIN
stats.minImpact = min(values);
// MAX
stats.maxImpact = max(values);
// MEAN
stats.meanImpact = 0.0;
for (unsigned int i=0; i<values.size(); i++)
  stats.meanImpact += probs[i]*values[i];
// Quartiles
stats.firstQImpact = -1.0;
stats.medianImpact = -1.0;
stats.thirdQImpact = -1.0;
stats.VaR = -1.0;
double alpha=1.0-gamma;
int ndx=-1;
for (unsigned int i=0; i<values.size(); i++) {
  if ((stats.firstQImpact == -1.0) && (cumval[i] >= 0.25 - 1e-7)) {
     stats.firstQImpact = values[work[i]];
     }
  else if ((stats.medianImpact == -1.0) && (cumval[i] >= 0.5 - 1e-7)) {
     stats.medianImpact = values[work[i]];
  }
  else if ((stats.thirdQImpact == -1.0) && (cumval[i] >= 0.75 - 1e-7)) {
     stats.thirdQImpact = values[work[i]];
  }
  if ((stats.VaR == -1.0) && (cumval[i] >= alpha - 1e-7)) {
     ndx = i;
     stats.VaR = values[work[i]];
  }
  }
// TCE
stats.TCE=0.0;
double tail_prob=0.0;
for (unsigned int i=ndx; i<values.size(); i++) {
  tail_prob += probs[work[i]];
  stats.TCE += values[work[i]]*probs[work[i]];
  }
stats.TCE /= tail_prob;
}


double compute_cost(const utilib::BasicArray<int>& sensors, std::map<std::string,double>& costs, std::map<int,std::string>& nodemap)
{
double total=0.0;
//
// Get the default cost
//
double dval = 0.0;
std::map<std::string,double>::iterator dcost = costs.find("__default__");
if (dcost != costs.end())
   dval = dcost->second;
//
// Loop through the sensors and use the cost value
//
utilib::BasicArray<int>::const_iterator scurr = sensors.begin();
utilib::BasicArray<int>::const_iterator send  = sensors.end();
while (scurr != send) {
  std::map<std::string,double>::iterator cost = costs.find(nodemap[*scurr]);
  if (cost != costs.end())
     total += cost->second;
  else
     total += dval;
  scurr++;
  }
return total;
}



void write_impact_statistics(int id, const utilib::BasicArray<int>& sensors,
			utilib::BasicArray<std::map<int,ImpactStats> >& stats, 
			utilib::BasicArray<std::vector<std::map<int,double> > >& impacts,
			const std::list<std::string>& files,
			const std::string& format, bool flag, bool debug,
			std::map<int,std::string>& nodemap,
			std::map<std::string,double>& costs,
			double gamma)
{
if (format == "cout") {
   if (flag)
      std::cout << "------------------------------------------------------------------------------" << endl;
   std::cout << "Sensor placement id:        " << id << std::endl;
   std::cout << "Number of sensors:          " << sensors.size() << std::endl;
   std::cout << "Total cost:                 " << compute_cost(sensors,costs,nodemap) << std::endl;
   std::cout << "Sensor node IDs:            ";
   utilib::BasicArray<int>::const_iterator scurr = sensors.begin();
   utilib::BasicArray<int>::const_iterator send  = sensors.end();
   while (scurr != send) {
     std::cout << *scurr << " ";
     scurr++;
     }
   std::cout << std::endl;
   if (nodemap.size() > 0) {
      std::cout << "Sensor junctions:           ";
      utilib::BasicArray<int>::const_iterator scurr = sensors.begin();
      utilib::BasicArray<int>::const_iterator send  = sensors.end();
      while (scurr != send) {
        std::cout << nodemap[*scurr] << " ";
        scurr++;
        }
      std::cout << std::endl;
      }

   utilib::BasicArray<std::vector<std::map<int,double> > >::iterator icurr = impacts.begin();
   std::list<std::string>::const_iterator fcurr = files.begin();
   std::list<std::string>::const_iterator fend = files.end();
   utilib::BasicArray<std::map<int,ImpactStats> >::iterator curr = stats.begin();
   int percent = static_cast<int>(floor(gamma*100));
   for (; fcurr != fend; fcurr++, curr++, icurr++) {
     ImpactStats& stat = (*curr)[id];
     std::cout << std::endl;
     std::cout << "Impact File:                " << *fcurr << std::endl;
     std::cout << "Number of attack scenarios: " << stat.nscenarios << std::endl;
     std::cout << "Min impact:                 " << stat.minImpact << std::endl;
     std::cout << "Mean impact:                " << stat.meanImpact << std::endl;
     std::cout << "Lower quartile impact:      " << stat.firstQImpact << std::endl;
     std::cout << "Median impact:              " << stat.medianImpact << std::endl;
     std::cout << "Upper quartile impact:      " << stat.thirdQImpact << std::endl;
     std::cout << "Value at Risk (VaR) (" << setw(3) << percent << "%): " << stat.VaR << std::endl;
     std::cout << "TCE                 (" << setw(3) << percent << "%): " << stat.TCE << std::endl;
     std::cout << "Max impact:                 " << stat.maxImpact << std::endl;
     if (debug) { 
        for (unsigned int i=0; i<stat.witness.size(); i++) {
          std::cout << i << " " << stat.witness[i] << " " << 
				(*icurr)[i][stat.witness[i]] << std::endl;
          }
        }
     }
   std::cout << "------------------------------------------------------------------------------" << endl;
   }

else if (format == "xls-sensors") {
   if (nodemap.size() > 0) {
      if (flag) {
         std::cout << "Sensor ID\tNum Sensors\tSensor Placements" << std::endl;
         }
      std::cout << id << "\t";
      std::cout << sensors.size() << "\t";
      utilib::BasicArray<int>::const_iterator scurr = sensors.begin();
      utilib::BasicArray<int>::const_iterator send  = sensors.end();
      while (scurr != send) {
        std::cout << nodemap[*scurr] << "\t";
        scurr++;
        }
      std::cout << std::endl;
      }
   }

else if (format == "xls-summary") {
   if (flag) {
      std::cout << "Sensor ID\tNum Sensors\tCost\tImpactFile\tNum Attacks\tMin\tMean\tLower Quartile\tMedian\tUpper Quartile\tVaR\tTCE\tMax" << endl;
      }
   std::list<std::string>::const_iterator fcurr = files.begin();
   std::list<std::string>::const_iterator fend = files.end();
   utilib::BasicArray<std::map<int,ImpactStats> >::iterator curr = stats.begin();
   for (; fcurr != fend; fcurr++, curr++) {
     ImpactStats& stat = (*curr)[id];
     std::cout << id << "\t" << sensors.size() << "\t" <<
		compute_cost(sensors,costs,nodemap) << "\t" <<
		*fcurr << "\t" <<
		stat.nscenarios << "\t" <<
		stat.minImpact << "\t" <<
		stat.meanImpact << "\t" <<
		stat.firstQImpact << "\t" <<
		stat.medianImpact << "\t" <<
		stat.thirdQImpact << "\t" <<
		stat.VaR << "\t" <<
		stat.TCE << "\t" <<
		stat.maxImpact << std::endl;
     }
   }

}

void write_impact_statistics(map<int, utilib::BasicArray<int> >& sensors,
			utilib::BasicArray<std::map<int,ImpactStats> >& stats, 
			utilib::BasicArray<std::vector<std::map<int,double> > >& impacts,
			const std::list<std::string>& files,
			const std::string& format, bool flag, bool debug,
			std::map<int,std::string>& nodemap,
			std::map<std::string,double>& costs,
			double gamma)
{
if (format == "xls-impact-distributions") {

unsigned int ndx=0;
unsigned int done=0;

while (done < sensors.size()*files.size()) {
  switch (ndx) {
     case 0:
     {
     map<int,utilib::BasicArray<int> >::iterator curr = sensors.begin();
     map<int,utilib::BasicArray<int> >::iterator end  = sensors.end();
     while (curr != end) {
       std::list<std::string>::const_iterator fcurr = files.begin();
       std::list<std::string>::const_iterator fend = files.end();
       while (fcurr != fend) {
         cout << curr->first << "\t";
         cout << curr->first << "\t";
         cout << curr->first << "\t";
         cout << curr->first << "\t";
         cout << "\t";
         fcurr++;
         }
       curr++;
       }
     }
     break;

     case 1:
     {
     map<int,utilib::BasicArray<int> >::iterator curr = sensors.begin();
     map<int,utilib::BasicArray<int> >::iterator end  = sensors.end();
     while (curr != end) {
       std::list<std::string>::const_iterator fcurr = files.begin();
       std::list<std::string>::const_iterator fend = files.end();
       while (fcurr != fend) {
         cout << *fcurr << "\t";
         cout << *fcurr << "\t";
         cout << *fcurr << "\t";
         cout << *fcurr << "\t";
         cout << "\t";
         fcurr++;
         }
       curr++;
       }
     }
     break;

     case 2:
     {
     map<int,utilib::BasicArray<int> >::iterator curr = sensors.begin();
     map<int,utilib::BasicArray<int> >::iterator end  = sensors.end();
     while (curr != end) {
       utilib::BasicArray<std::map<int,ImpactStats> >::iterator scurr = stats.begin();
       std::list<std::string>::const_iterator fcurr = files.begin();
       std::list<std::string>::const_iterator fend = files.end();
       while (fcurr != fend) {
         cout << (*scurr)[curr->first].nscenarios << "\t";
         cout << (*scurr)[curr->first].nscenarios << "\t";
         cout << (*scurr)[curr->first].nscenarios << "\t";
         cout << (*scurr)[curr->first].nscenarios << "\t";
         cout << "\t";
         scurr++;
         fcurr++;
         }
       curr++;
       }
     }
     break;

     case 3:
     {
     map<int,utilib::BasicArray<int> >::iterator curr = sensors.begin();
     map<int,utilib::BasicArray<int> >::iterator end  = sensors.end();
     while (curr != end) {
       utilib::BasicArray<std::map<int,ImpactStats> >::iterator scurr = stats.begin();
       std::list<std::string>::const_iterator fcurr = files.begin();
       std::list<std::string>::const_iterator fend = files.end();
       while (fcurr != fend) {
         cout << "Attack ID\t";
         cout << "Witness Location Index\t";
         cout << "Witness Location ID\t";
         cout << "Impact\t";
         cout << "\t";
         scurr++;
         fcurr++;
         }
       curr++;
       }
     }
     break;

     default:
     {
     map<int,utilib::BasicArray<int> >::iterator curr = sensors.begin();
     map<int,utilib::BasicArray<int> >::iterator end  = sensors.end();
     while (curr != end) {
       utilib::BasicArray<std::map<int,ImpactStats> >::iterator scurr = stats.begin();
       utilib::BasicArray<std::map<int,ImpactStats> >::iterator send  = stats.end();
       while (scurr != send) {
         if ((ndx-4) < (*scurr)[curr->first].witness.size()) {
            cout << (ndx-3) << "\t";
            cout << (*scurr)[curr->first].witness[ndx-4] << "\t";
            cout << nodemap[(*scurr)[curr->first].witness[ndx-4]] << "\t";
            cout << (*scurr)[curr->first].impact[ndx-4] << "\t";
            cout << "\t";
            }
         else {
            cout << "\t";
            cout << "\t";
            cout << "\t";
            cout << "\t";
            cout << "\t";
            if ((ndx-4) == (*scurr)[curr->first].witness.size()) done++;
            }
         scurr++;
         }
       curr++;
       }
     }

     }
  cout << endl;
  ndx++;
  }
}
}
