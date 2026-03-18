/*!
 * \file evalsensor.cpp
 * \brief Evaluate a sensor placement using impacts defined by a TSO file.
 *
 * \todo How should we do command-line processing?  Using UTILIB functionality?
 */

#include <fstream>
#include <sp/impacts.h>
#include <utilib/BasicArray.h>
#include <vector>


/// The main routine for the evalsensor executable.
int main(int argc, char **argv)
{
try {
  //
  // Check for errors in the command-line arguments
  //
  if(argc < 3)
    {
      std::cerr << endl;
      std::cerr << "\
evalsensor - An application that reads in one or more sensor placements and\n\
\tsummarized their performance according to various metrics.\n\
\n\
usage:\n\
\n\
   evalsensor [options...] <sensor-file> <impact-file1> [<impact-file2> ... ]\n\
\n\
options:\n\
\n\
   --debug\n\
	For each set of sensor locations, output information about each attack\n\
\n\
   --delay=<minutes>\n\
	Specifies the response delay used when evaluating sensor placements.\n\
\n\
   --alpha=<double>\n\
	Specifies the quantile value used to compute the VaR\n\
	and TCE performance measures. (The default value is 0.95.)\n\
\n\
   --format=(cout|xls|xml)\n\
	Controls the type of output that the evaluation will generate:\n\
	  cout  Generates output that is easily read. (default)\n\
	  xls   Generates output that is easily imported into a MS Excel\n\
		spreadsheet.\n\
	  xml   Generates an XML-formated output that is used to communicate\n\
		with the TEVA GUI.  (Not currently supported.)\n\
\n\
   --probabilities=<filename>\n\
	Specifies a file with the probability of the different contamination\n\
	scenarios.  (Not currently supported.)\n\
\n\
   --nodemap=<filename>\n\
	Specifies a file with the node map information, for translating node\n\
	IDs into junction labels.\n\
\n\
   --costs=<filename>\n\
	Specifies a file with the cost information for each location id.\n\
\n\
arguments:\n\
\n\
   <sensor-file>\n\
        A sensor placement file, which contains one or more sensor\n\
        placements that will be evaluated.\n\
\n\
   <impact-file>\n\
	An impact file, which contains the impact data concerning a\n\
	contamination event.  If one or more impact files are specified, then\n\
	evaluations are performed for each impact separately.\n\n";

      return 1;
    }
  int ndx=1;
  bool debug=false;
  if (strncmp(argv[ndx],"--debug",7) == 0) {
     debug=true;
     ndx++;
     }
  int delay=0;
  if (strncmp(argv[ndx],"--delay",7) == 0) {
     delay=atoi(&(argv[ndx][8]));
     ndx++;
     }
  double alpha=0.95;
  if (strncmp(argv[ndx],"--alpha",7) == 0) {
     alpha=atof(&(argv[ndx][8]));
     ndx++;
     }
  std::string format;
  if (strncmp(argv[ndx],"--format",8) == 0) {
     format=&(argv[ndx][9]);
     ndx++;
     }
  else
     format="cout";
  //
  // Read the attack probabilities
  //
  std::string probabilityFilename;
  if (strncmp(argv[ndx],"--probabilities=",16) == 0) {
     probabilityFilename=&(argv[ndx][17]);
     ndx++;
     }
  std::map<int,double> probabilities;
  if (probabilityFilename != "") {
     std::ifstream probabilityInputFile(probabilityFilename.c_str());
     read_probabilities(probabilityInputFile, probabilities);
     }
  //
  // Read the nodemap
  //
  map<int,std::string> nodemap;
  if (strncmp(argv[ndx],"--nodemap",9) == 0) {
     std::ifstream ifstr(&(argv[ndx][10]));
     read_nodemap(ifstr, nodemap);    
     ndx++;
     }
  //
  // Read the costs
  //
  map<std::string,double> costs;
  if (strncmp(argv[ndx],"--costs",7) == 0) {
     if (nodemap.size() == 0)
        EXCEPTION_MNGR(runtime_error, "Cannot specify costs without a nodemap file")
     std::ifstream ifstr(&(argv[ndx][8]));
     read_costs(ifstr, costs);
     ndx++;
     }

  //
  // Read sensor placement file
  //
  std::map<int, utilib::BasicArray<int> > sensors;
  {
  std::ifstream sensorInputFile(argv[ndx]);
  if(!sensorInputFile)
    {
      std::cerr << "***Failed to open sensor file=" << argv[ndx] << std::endl;
      return 0;
    }
  ndx++;
  read_sensor_placements(sensorInputFile, sensors);
  sensorInputFile.close();
  }
  //
  // Get impact filenames
  //
  std::list<std::string> files;
  while (ndx < argc) {
    files.push_back(argv[ndx]);
    ndx++;
    }
  //
  // Read impact files and compute impact information
  //
  utilib::BasicArray<std::map<int,ImpactStats> > statistics(files.size());
  utilib::BasicArray<std::vector<std::map<int,double> > > delayed_impacts(files.size());
  utilib::BasicArray<std::map<int,ImpactStats> >::iterator scurr = statistics.begin();
  std::list<std::string>::iterator fcurr = files.begin();
  std::list<std::string>::iterator fend = files.end();
  utilib::BasicArray<int> work;
  utilib::BasicArray<std::vector<std::map<int,double> > >::iterator dicurr = delayed_impacts.begin();
  for (; fcurr != fend; fcurr++, scurr++, dicurr++) {
    //
    // Read the data from the impact file
    //	impacts[i] is a hash table of (node-id,impact) pairs
    //
    std::ifstream impactInputFile(fcurr->c_str());
    if(!impactInputFile)
      {
        std::cerr << "***Failed to open impact file=" << *fcurr << std::endl;
        return 0;
      }
    //utilib::BasicArray<std::map<int,std::pair<double,double> > > impacts;
    //read_impacts(impactInputFile, impacts);
    //compute_delayed_impacts(impacts,delay,*dicurr);
    read_impacts(impactInputFile, *dicurr, delay);
    impactInputFile.close();
    //
    // Compute impact statistics
    //
    std::map<int,utilib::BasicArray<int> >::iterator mcurr = sensors.begin();
    std::map<int,utilib::BasicArray<int> >::iterator mend  = sensors.end();
    while (mcurr != mend) {
      ImpactStats tmp;
      compute_statistics(mcurr->second,
			*dicurr,tmp,probabilities,work,alpha);
      (*scurr)[mcurr->first] = tmp;
      mcurr++;
      }
    }
  //
  // Write out impact statistics
  //
  if (format == "cout") {
     std::map<int,utilib::BasicArray<int> >::iterator mcurr = sensors.begin();
     std::map<int,utilib::BasicArray<int> >::iterator mend  = sensors.end();
     bool flag=true;
     while (mcurr != mend) {
       write_impact_statistics(mcurr->first, mcurr->second,
				   statistics,delayed_impacts,
				   files,format,flag, debug, nodemap, costs);
       flag=false;
       mcurr++;
       }
     }

  else if (format == "xls") {
     std::map<int,utilib::BasicArray<int> >::iterator mcurr;
     std::map<int,utilib::BasicArray<int> >::iterator mend;
     bool flag;

     mcurr = sensors.begin();
     mend  = sensors.end();
     flag=true;
     while (mcurr != mend) {
       write_impact_statistics(mcurr->first, mcurr->second,
				   statistics,delayed_impacts,
				   files,"xls-sensors",flag, debug, nodemap, costs);
       flag=false;
       mcurr++;
       }

     mcurr = sensors.begin();
     mend  = sensors.end();
     flag=true;
     while (mcurr != mend) {
       write_impact_statistics(mcurr->first, mcurr->second,
				   statistics,delayed_impacts,
				   files,"xls-summary",flag, debug, nodemap, costs);
       flag=false;
       mcurr++;
       }

     write_impact_statistics(sensors, statistics,delayed_impacts,
				   files,"xls-impact-distributions",flag, debug, nodemap, costs);
     }
  }
STD_CATCH(;)

return 0;
}

