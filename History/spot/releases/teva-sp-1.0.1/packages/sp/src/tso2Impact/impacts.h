/*!
 * \file impacts.h
 * \brief Routines to evaluate the impact of a contamination event, given
 * 	  sensors.
 */

#ifndef tevasp_impacts_h
#define tevasp_impacts_h

#include <iostream>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <utilib/BasicArray.h>
#include <utilib/_math.h>

//#define OLD_IMPACT_FORMAT

template <class T>
void read_array(std::istream& istr, utilib::BasicArray<T>& array)
{
int tmp;
istr >> tmp;
array.resize(tmp);
for (int i=0; i<tmp; i++)
  istr >> array[i];
}


/**
 * Read a node map file.
 * Format
 *    <node-id> <string>
 */
inline void read_nodemap(std::istream& istr, std::map<int,string>& nodemap)
{
int id;
istr >> id;
while (istr) {
  std::string tmp;
  istr >> tmp;
  nodemap[id] = tmp;
  istr >> id;
  }
}



/**
 * Read a file containing probabilities of attacks for different scenarios.
 * Format:
 *    <node-id> <probability>
 */
void read_probabilities(std::istream& istr, std::map<int,double>& probabilities);

inline void read_probabilities(std::istream& istr, utilib::BasicArray<double>& probabilities)
{
std::map<int,double> probs;
read_probabilities(istr, probs);
for (unsigned int i=0; i<probabilities.size(); i++)
  probabilities[i] = 0.0;
double total=0.0;
std::map<int,double>::iterator curr = probs.begin();
std::map<int,double>::iterator end  = probs.end();
while (curr != end) {
  probabilities[curr->first] = curr->second;
  total += curr->second;
  curr++;
  }
for (unsigned int i=0; i<probabilities.size(); i++)
  probabilities[i] /= total;
}


inline void read_costs(std::istream& istr, std::map<std::string,double>& costs)
{
string id;
istr >> id;
while (istr) {
  double cost;
  istr >> cost;
  costs[id] = cost;
  istr >> id;
  }
}


inline void read_id_costs(std::istream& istr, std::map<int,double>& costs)
{
int len=0;
istr >> len;
for (int i=0; i<len; i++) {
  int index;
  double val;
  istr >> index >> val;
  costs[index] = val;
  }
}


/**
 * Read an impact file.
 */
void read_impacts(std::istream& istr,
		std::vector<std::map<int,double> >& impacts,
		int delay);

/**
 * Take raw impact information and a delay, and compute new
 * impact values that take into account these delays.
 */
void compute_delayed_impacts(utilib::BasicArray<std::map<int,std::pair<double,double> > >& raw_impacts,
			double delay,
			utilib::BasicArray<std::map<int,double> >& impacts);


/**
 * Read sensor locations from a file.
 */
void read_sensor_placements(std::istream& istr, std::map<int, utilib::BasicArray<int> >& sensors);

/**
 * Read network ids from a file.
 * Format:
 *    <node-id>
 */
void read_network_ids(std::istream& istr, utilib::BasicArray<int>& sensors);


/// A class that contains summary statistics about the performance of a sensor placement.
class ImpactStats
{
public:

  ImpactStats()
	: goal(""), minImpact(-DBL_MAX), maxImpact(DBL_MIN), medianImpact(0.0), 
	  firstQImpact(0.0), thirdQImpact(0.0), meanImpact(0.0), VaR(0.0), TCE(0.0),
	  nscenarios(0)
	{}

  ///
  string goal;

  ///
  double minImpact;

  ///
  double maxImpact;

  ///
  double medianImpact;

  ///
  double firstQImpact;

  ///
  double thirdQImpact;

  ///
  double meanImpact;

  ///
  double VaR;

  ///
  double TCE;

  ///
  int nscenarios;

  ///
  utilib::BasicArray<int> witness;  // indexed by attack number

  ///
  utilib::BasicArray<double> impact;// indexed by attack number
};


/// Compute impact statistics
void compute_statistics(const utilib::BasicArray<int>& sensors, 
		std::vector<std::map<int,double> >& impacts, 
		ImpactStats& stats, std::map<int,double>& probabilities,utilib::BasicArray<int>& work, double alpha);


/// Write impact statistics

/// The "impacts" are stored as follows:
///    -  the top level vector is indexed by the id of the sensor placement
///    -  the nested vector is indexed by the impact type (mc, vc, nfd, etc)
///    -  the map maps attack numbers to impact values 
void write_impact_statistics(int id, const utilib::BasicArray<int>& sensors, 
		utilib::BasicArray<std::map<int,ImpactStats> >& stats,
		utilib::BasicArray<std::vector<std::map<int,double> > >& impacts, 
		const std::list<std::string>& files, const std::string& format,
		bool first_flag, bool debug, std::map<int,std::string>& nodemap,
		std::map<std::string,double>& costs);

void write_impact_statistics(std::map<int,utilib::BasicArray<int> >& sensors, 
		utilib::BasicArray<std::map<int,ImpactStats> >& stats,
		utilib::BasicArray<std::vector<std::map<int,double> > >& impacts, 
		const std::list<std::string>& files, const std::string& format,
		bool first_flag, bool debug, std::map<int,std::string>& nodemap,
		std::map<std::string,double>& costs);

#endif
