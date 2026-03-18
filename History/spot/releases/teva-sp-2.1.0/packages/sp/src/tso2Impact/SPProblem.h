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
			      utilib::BitArray& feasible_locations);

  /// The number of attacks
  int num_attacks() {return impacts.size();}

  /// The name of the impact
  std::string goal;

  /// The file of impacts that are associated with this goal
  std::string impact_file;

  /// The impact values.
  /// impacts[i] is a map of the impacts for the i-th attack
  utilib::BasicArray<std::map<int,std::pair<double,double> > > raw_impacts;

  /// The impact values.
  /// impacts[i] is a map of the impacts for the i-th attack
  std::vector<std::map<int,double> > impacts;

  /// The probabilities of each attack
  utilib::BasicArray<double> probabilities;

  /// The superlocations ids
  utilib::BasicArray<std::list<int> > super_locs;

  /// An absolute threshold used to compute super locations.
  double threshold;

  /// An alternative specification for aggregation (super locations).
  /// This sets the threshold for a specific goal as a percentage of
  /// the maximum impact difference (over all attacks) between
  /// detection and non-detection.  This takes priority over
  /// thresholds if both are specified and this is a valid percentage > 0.

  double aggregation_percentage;

  /// If we are aggregating (with percentage or absolute threshold),
  /// and this is true, then for an individual attack, if the global
  /// threshold is large enough to bring all locations into a single
  /// superlocation, force the dummy into a second location.  This
  /// means that for "inconsequential" attacks, we still distinguish
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
  utilib::BasicArray<std::map<int,int> > sl_worst;
};


///
class SPProblem
{
public:

  ///
  SPProblem() 
    : num_nodes(0), delay(0),
      conserve_memory(true), gamma(0.05) {}

  /// Read the SPProblem configuration file
  void read_config_file(std::istream& istr);

  /// Write an AMPL *.dat file used with GeneralSP.mod
  void write_ampl_data(std::ostream& os, bool imperfect);

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

  /// The costs of placement
  std::map<int,double> costs;

  /// The goals that are included in the objective.
  std::list<int> objectives;

  /// The superlocation sets used in this model
  std::map<utilib::BasicArray<int>,int> membersets;

  /// The number of nodes in the network
  int num_nodes;

  ///
  int delay;

  ///
  bool conserve_memory;

  ///
  double gamma;
};

#endif
