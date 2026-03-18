/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// parParams.cpp
//

// Defines parameters, etc, used for general mixed-integer linear programming
//

#include <acro_config.h>
#include <utilib/ValidateParameter.h>
#include <pico/parMilpParams.h>

using namespace std;

namespace pico{

using utilib::ParameterSet;

parMilpParams::parMilpParams()
{
pCostShareCutoff=2;
ParameterSet::create_categorized_parameter("pCostShareCutoff",pCostShareCutoff,"<int>","2",
"When a new pseudocost is generated, if this processor has seen no\n\t\
more than pCostShareCutoff pseudocosts for this variable in this\n\t\
direction, it shares the information with other processors.  Upper\n\t\
bound is somewhat arbitrary.  In general, there are diminishing\n\t\
returns after the first few shares of pseudocosts.",
	"parMIP",
utilib::ParameterNonnegative<int>());

pCostMinBcastSize=5;
ParameterSet::create_categorized_parameter("pCostMinBcastSize",pCostMinBcastSize,"<int>","5",
	"TODO",
	"parMIP",
	utilib::ParameterPositive<int>());

rampUpTableInitFrac=1.0;
ParameterSet::create_categorized_parameter("rampUpTableInitFrac",rampUpTableInitFrac,"<double>","1.0",
"Fraction of candidate pseudocost initialization we'll do during\n\t\
ramp up (when we are initializing in parallel)",
	"parMIP",
utilib::ParameterBounds<double>(0.0,1.0));

numPCInitSmoothFactor=0.25;
ParameterSet::create_categorized_parameter("numPCInitSmoothFactor",numPCInitSmoothFactor,
			       "<double>","0.25",
     "Exponential smoothing factor for estimating recent number of\n\t"
     "pseudo-cost initializations per subproblem during ramp-up",
	"parMIP",
     utilib::ParameterBounds<double>(0.0,1.0));

numPCCrossFac=2;
ParameterSet::create_categorized_parameter("numPCCrossFac",numPCCrossFac,"<int>","2",
    "Helps determine crossover from ramp-up to tree parallelism:\n\t"
    "Cross over if number of available subproblems > numPCCrossFac * \n\t\t"
    "(avg recent number of pseudocost initializations).\n\t"
    "See also the treeSizeCrossFac criterion",
	"parMIP",
utilib::ParameterNonnegative<int>());

pCostTreeRadix=2;
ParameterSet::create_categorized_parameter("pCostTreeRadix",pCostTreeRadix,"<int>","2",
	"Radix of the pseudo-cost tree",
	"parMIP",
	utilib::ParameterPositive<int>());

ofile = false;
create_categorized_parameter("ofile",ofile,
                "<bool>","false",
                "If true, create the output file parMIP-out",
	"parMIP");

}

}
