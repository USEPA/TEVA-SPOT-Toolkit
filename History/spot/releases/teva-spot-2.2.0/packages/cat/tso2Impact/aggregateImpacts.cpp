/**
  * \file aggregateImpacts.cpp
  * 
  * Read a configuration file and impact file(s).  Aggregate
  * locations into superlocations.  Write a new impact file
  * with superlocations instead of original locations.  Write
  * a new configuration file reflecting the superlocations.
  * Add to the end of the configuration file a mapping from
  * each superlocation to representative real locations.
  *
  * aggregateImpacts {configuration-file-name}
  *
  * Optional argument that follows configuration file name is
  * the maximum number of impacts to read at one time from an
  * impact file.  The default is to read all impacts in.
  *
  * aggregateImpacts test1_mc.impact 10000
  *
  * will read no more than 10000 impacts at a time (unless there
  * is an event with more than 10000 impacts, in which case it
  * reads all impacts in the event).
  */

#ifdef HAVE_CONFIG_H
#include <teva_config.h>
#endif

#include <utilib/ParameterSet.h>
#include <utilib/ParameterList.h>
#include <utilib/string_ops.h>
#include <sp/SPProblem.h>
#include <string>

using namespace std;


/// The main routine for setupIPData
int main(int argc, char **argv)
{
try {
  int impactReadLimit=0;

  if (argc == 1)
     EXCEPTION_MNGR(runtime_error,"Missing configuration file!");
  char *configFile = argv[1];
  ifstream ifstr(configFile);
  if (!ifstr)
     EXCEPTION_MNGR(runtime_error,"Bad filename " << configFile);

  if (argc > 2){
    impactReadLimit = atoi(argv[2]);
    if (impactReadLimit < 0){
      std::cout << "USAGE ERROR: optional second argument" << std::endl;
      std::cout << "aggregateImpacts test1_mc.impact 10000" << std::endl;
      std::cout << "will read no more than 10000 impacts at a time (unless there" << std::endl;
      std::cout << "is an event with more than 10000 impacts, in which case it" << std::endl;
      std::cout << "reads all impacts in the event)." << std::endl;
      exit(0);
    }
  }
  
  SPProblem info;
  info.output_impact = true;

  if (impactReadLimit > 0){
    info.SetMaximumNumberOfImpacts(impactReadLimit);
  }
  else{
    info.SetReadAllImpacts(true);
  }
  
  info.read_config_file(ifstr,true,false,true);
  
  ifstr.close();
  
  info.compute_superlocations();
  
  // Write new impact files.     prob_goal.impact -> prob_goal_agg.impact

  std::vector<std::string> newImpacts(info.goals.size(), "");
  
  for (int i=0; i<info.goals.size(); i++) {
    std::string newname = info.goals[i].impact_file;
    if (newname != "none"){
      int loc = newname.find_last_of(".");
      if (loc >= 0){
        newname.insert(loc, "_agg");
      }
      else{
        newname.append("_agg");
      }
   
      ofstream ofImpact(newname.c_str());    
      info.write_aggregated_impacts(ofImpact, i);
      ofImpact.close();
      std::cout << "New impact file: " << newname << std::endl;
    }
    newImpacts[i] = newname;
  }
  
  // Write a corresponding configuration file  prob.config -> prob_agg.config

  std::string newname(configFile);   // new config file
  std::string earliestname(configFile); // maps sl ID to earliest impact orig ID
  std::string mapname(configFile);   // maps sl ID to all orig IDs in sl

  int loc = newname.find_last_of(".");
  if (loc >= 0){
    newname.insert(loc, "_agg");
    earliestname.insert(loc, "_earliest");
    mapname.insert(loc, "_map");
  }
  else{
    newname.append("_agg");
    earliestname.append("_earliest");
    mapname.append("_map");
  }

  ofstream of(newname.c_str());    
  info.write_aggregated_config_file(of, newImpacts);
  of.close();

  // compute_superlocations wrote out a file with a full list of
  // real locations that are in each superlocation, and a file
  // with "earliest" ids for each event for each superlocation.

  rename("superlocations_first.txt", earliestname.c_str());
  rename("superlocations_all.txt", mapname.c_str());
 
  std::cout << "New configuration file: " << newname << std::endl;
  std::cout << "Worst impact original location IDs: " << earliestname << std::endl;
  std::cout << "Map SL IDs to original location IDs: " << mapname << std::endl;
}
STD_CATCH(;)
  
return 0;
}
