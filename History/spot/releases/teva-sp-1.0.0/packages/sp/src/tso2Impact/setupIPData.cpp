/**
  * \file setupIPData.cpp
  * 
  * Creates an input file for the GeneralSP.mod IP model.
  */

#include <utilib/ParameterSet.h>
#include <utilib/ParameterList.h>
#include <sp/SPProblem.h>

using namespace std;


/// The main routine for setupIPData
int main(int argc, char **argv)
{
try {

string amplOutputFile;
utilib::ParameterSet params;

params.create_parameter("dat",amplOutputFile, "<string>","<null>",
	"The output filename.  By default, output goes to stdout.");
/*
params.create_parameter("costs",costFile, "<string>","<null>",
	"The cost file.");
params.create_parameter("lock-sensors",fixedSensorFile, "<string>","<null>",
	"A file with the locations of pre-fixed sensor locations.");
params.create_parameter("infeasible",infeasibleLocationsFile, "<string>","<null>",
	"A file with the locations that are infeasible for sensors.");
params.create_parameter("",infeasibleLocationsFile, "<string>","<null>",
	"A file with the locations that are infeasible for sensors.");
*/

utilib::ParameterList plist;
plist.register_parameters(params);
plist.process_parameters(argc,argv,1);
params.set_parameters(plist,false);

if (argc == 1)
   EXCEPTION_MNGR(runtime_error,"Missing sensor placement filename!")
ifstream ifstr(argv[1]);
if (!ifstr)
   EXCEPTION_MNGR(runtime_error,"Bad filename " << argv[1]);

SPProblem info;
//
// Read in the problem data
//
info.read_config_file(ifstr);
//
// Compute super location information
//
info.compute_superlocations();
//
// Write the AMPL *.dat file
//
if (amplOutputFile != "") {
   ofstream ofstr(amplOutputFile.c_str());
   info.write_ampl_data(ofstr);
   }
else
   info.write_ampl_data(cout);
}
STD_CATCH(;)

return 0;
}
