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

  /// The threshold used to compute super locations
  double threshold;

  /// The measures that are associated with this goal
  std::list<std::string> measure;

  /// If true, then this goal is in the objective
  /// Note: only the first measure is currently used in the
  /// objective.
  bool objective_flag;

  /// The bounds on the goal measures, when used in a constraint
  std::list<double> bound;

  /// The worst node in each superlocation set
  utilib::BasicArray<std::map<int,int> > sl_worst;
};


///
class SPProblem
{
public:

  /// Read the SPProblem configuration file
  void read_config_file(std::istream& istr);

  /// Write an AMPL *.dat file used with GeneralSP.mod
  void write_ampl_data(std::ostream& os);

  /// Compute the super locations
  void compute_superlocations();

  /// The number of nodes in the network
  int num_nodes;

  ///
  int delay;

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

  ///
  bool conserve_memory;
};

#endif
