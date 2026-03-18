/**
 * \file SPProblem.h
 * 
 */

#ifndef tevasp_SPProblem_h
#define tevasp_SPProblem_h

#include <list>
#include <map>
#include <string>
#include <iostream>
#include <utilib/BitArray.h>
#include <utilib/BasicArray.h>

///
class SPGoalInfo
{
public:

  /// a magic number representing the "null" bound 
  /// for an objective goal/measure combination.
  static const double OBJECTIVE_GOAL_BOUND;

  /// Compute the superlocations for this impact.
  /// Note: superlocations are not meaningful for
  /// the "ns" and "cost" goals.
  void compute_superlocations(std::map<utilib::BasicArray<int>,int>& membersets,
			      utilib::BitArray& feasible_locations,
			      utilib::BitArray& fixed_locations,
                              bool output_impact);

  /// Access info in goal record
  int get_witness_time(int loc, std::map<int, int> &timemap) const;
  void make_witness_time_map(int event, std::map<int, int> &tmap) const;
  double get_impact(int event, int loc) const;

  /// The number of events
  int num_events() const
	{return impacts.size();}

  /// The name of the impact
  std::string goal;

  /// The file of impacts that are associated with this goal
  std::string impact_file;

  /// The impact values.
  /// impacts[i] is a map of the impacts for the i-th event
  utilib::BasicArray<std::map<int,std::pair<double,double> > > raw_impacts;

  /// The impact values.
  /// impacts[i] is a map of the impacts for the i-th event
  std::vector<std::map<int,double> > impacts;

  /// The witness time values.
  /// impacts[i] is a map of the witness time for the i-th event
  std::vector<std::list<std::pair<int,int> > > wtime;

  /// The probabilities of each event
  utilib::BasicArray<double> probabilities;

  /// The superlocations ids
  utilib::BasicArray<std::list<int> > super_locs;

  /// An absolute threshold used to compute super locations.
  double threshold;

  /// The ratio of the best super-location objective value to
  /// the value used by the super-location
  double aggregation_ratio;

  /// The final ratio of the best super-location objective value to
  /// the value used by the super-location observed after aggregation
  double final_aggregation_ratio;

  /// An alternative specification for aggregation (super locations).
  /// This sets the threshold for a specific goal as a percentage of
  /// the maximum impact difference (over all events) between
  /// detection and non-detection.  This takes priority over
  /// thresholds if both are specified and this is a valid percentage > 0.

  double aggregation_percentage;

  /// If we are aggregating (with percentage or absolute threshold),
  /// and this is true, then for an individual event, if the global
  /// threshold is large enough to bring all locations into a single
  /// superlocation, force the dummy into a second location.  This
  /// means that for "inconsequential" events, we still distinguish
  /// between detection at all (with a value equal to the worst for
  /// detection) and no detection.

  bool distinguish_detection;

  /// The measures that are associated with this goal
  std::list<std::string> measure;

  /// If true, then this goal is in the objective
  /// Note: only the first measure is currently used in the
  /// objective.
  bool objective_flag;

  /// The bounds on the goal measures. if the measure is
  /// the objective, the goal
  std::list<double> bound;

  /// The worst node in each superlocation set
  /// Note if output_impact is "true", we overload this array to
  /// to hold not the worst node, but the node with earliest
  /// impact time - so it's really "sl_first".
  utilib::BasicArray<std::map<int,int> > sl_worst;

  /// Each superlocation has an original location with earliest
  /// witness time for this event.  This map maps witness
  /// times to lists of superlocation/original-location ID pairs.
  /// 
  /// Only needed if we're writing an impact file of super locations
  /// (output_impact == true).

  utilib::BasicArray<std::map<int,std::list<std::pair<int, int> > > > sl_wtime;
};


///
class SPProblem
{
public:

  ///
  SPProblem() 
    : num_nodes(0), delay(0), num_superlocations(0),
      conserve_memory(true), gamma(0.05), output_impact(false) {}

  /// Read the SPProblem configuration file
// TNT: Added flag to optionally load impact data.  It is not always needed
// to be loaded in this format.
  void read_config_file(std::istream& istr, bool loadImpacts=true, bool loadWtime=false);

  /// Write an AMPL *.dat file used with GeneralSP.mod
  void write_ampl_data(std::ostream& os, bool imperfect);

  /// Write a data file used with GeneralSP.mod and ufl Lagrangian solver
  void write_lagrangian_data(bool imperfect);

  /// Write an impact file for a specific goal.
  void write_impact_file(std::ostream& os, int goal_index) const;

  /// Write an impact file with superlocations replacing original locations
  void write_aggregated_impacts(std::ostream& osIm, int goal_index) const;

  /// Write a new configuration file reflecting location aggregation
  void write_aggregated_config_file(std::ostream& ostr, std::vector<std::string> &impactFiles);

  /// Compute the super locations
  void compute_superlocations();

  /// The goal data
  utilib::BasicArray<SPGoalInfo> goals;

  /// The locations where sensors have been predeteremined
  utilib::BasicArray<int> fixed_placements;

  /// The locations where sensors cannot be placed
  utilib::BasicArray<int> invalid_placements;

  /// Feasible locations
  utilib::BitArray feasible_location;

  /// Fixed locations
  utilib::BitArray fixed_location;

  /// The costs of placement
  std::map<int,double> costs;

  /// The goals that are included in the objective.
  std::list<int> objectives;

  /// The superlocation sets used in this model
  std::map<utilib::BasicArray<int>,int> membersets;

  /// Maps from the 'internal' representation used by impact files to
  /// the reduced representation after invalid locations are
  /// eliminated.
  std::map<int,int> reduced_repn_map;

  /// The original node ID from the impact file, indexed by our
  /// contiguous node IDs after invalid locations are eliminated.
  std::vector<int> original_node_id;

  /// The number of nodes in the network
  int num_nodes;

  /// The number of superlocations in the network, not including
  /// the dummy location.  We use superlocation ID 0 for the dummy.
  int num_superlocations;

  ///
  int delay;

  ///
  bool conserve_memory;

  ///
  double gamma;

  /// Output an impact file containing superlocations
  bool output_impact;
};

#endif
