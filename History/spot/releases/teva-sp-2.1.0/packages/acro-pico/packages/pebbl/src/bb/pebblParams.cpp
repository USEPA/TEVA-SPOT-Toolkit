/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// pebblParams.cpp
//

#include <acro_config.h>
#include <pebbl/pebblParams.h>
#include <pebbl/pebblBase.h>

namespace pebbl {

using utilib::ParameterLowerBound;
using utilib::ParameterBounds;
using utilib::ParameterNonnegative;

extern int randomSeed;

pebblParams::pebblParams()
  : statusPrintCount(100000),
    statusPrintSeconds(10.0),
    depthFirst(false),
    breadthFirst(false),
    initialDive(false),
    integralityDive(true),
    lazyBounding(false),
    eagerBounding(false),
    relTolerance(1e-7),
    absTolerance(0.0),
    earlyOutputMinutes(0.0),
    startIncumbent(0.0),
    validateLog(false),    
    heurLog(false),    
    maxSPBounds(0),
    maxCPUMinutes(0.0),
    maxWallMinutes(0.0),
    printAbortMessage(true),
    printIntMeasure(false),    
    printDepth(false),
    debugPrecision(0),
    suppressWarnings(false),
    loadMeasureDegree(1),
    EnumerationRelTolerance(-1.0),
    EnumerationAbsTolerance(-1.0),
    EnumerationCount(-1),
    debug_solver_params(false),
#ifdef ACRO_VALIDATING
    use_abort(true)
#else
    use_abort(false)
#endif
{
/// GENERAL

  create_categorized_parameter("loadMeasureDegree",loadMeasureDegree, 
		"<int>","1",
		"Measure 'weight' of a subproblem by its difference\n\t"
                "from the incumbent to this power",
		"Search",
		utilib::ParameterBounds<int>(0,pebblBase::maxLoadDegree));

  randomSeed=1;
  create_categorized_parameter("randomSeed",randomSeed,
		"<int>","1",
		"Global seed for random number generation.",
		"General",
		utilib::ParameterNonnegative<int>());

/// ACRO_VALIDATING

  version_flag=false;
  create_categorized_parameter("version",version_flag,
                "<bool>","false",
                "If true, print version info (must be first parameter)",
		"General");

  create_categorized_parameter("debug-solver-params",debug_solver_params,
                "<bool>","false",
                "If true, print solver parameters",
		"Debugging");

  create_categorized_parameter("useAbort",use_abort,
                "<bool>",
                #ifdef ACRO_VALIDATING
                "true",
                #else
                "false",
                #endif
                "If true, then force an abort when an error occurs.",
                "Termination");

  create_categorized_parameter("printIntMeasure",printIntMeasure,
	      	"<bool>","false",
	      	"Include subproblem integrality measures in debug output",
		"Debugging");

  create_categorized_parameter("printDepth",printDepth,
		"<bool>", "false",
		"Include subproblem depth in debug output",
		"Debugging");

  create_categorized_parameter("debugPrecision",debugPrecision,
		"<int>","0",
		"Debug output objective value precision",
		"Debugging",
		ParameterBounds<int>(0,20));

  create_categorized_parameter("suppressWarnings",suppressWarnings,
		"<bool>","false",
		"Suppress warning messages.",
		"Output");

  create_categorized_parameter("debug",debug,
		"<int>","0",
		"Debugging output level",
		"Debugging",
		ParameterNonnegative<int>());

  create_categorized_parameter("statusPrintCount",statusPrintCount,
		"<int>","100000",
		"Number of subproblems bounded between status printouts",
		"Output",
		ParameterNonnegative<int>());

  create_categorized_parameter("statusPrintSeconds",statusPrintSeconds,
		"<double>","10.0",
		"Seconds elapsing between status printouts",
		"Output",
		ParameterNonnegative<double>());

  create_categorized_parameter("earlyOutputMinutes",earlyOutputMinutes,
		"<double>","0",
		"If this much time elapses, make sure current incumbent\n\t"
		"is output to a file in case of crash etc.",
		"Output",
		ParameterNonnegative<double>());

  create_categorized_parameter("validateLog",validateLog,
		"<bool>","false",
		"Output validationion log files val*.log for logAnalyze",
		"Debugging");

/// SEARCH

  create_categorized_parameter("depthFirst",depthFirst,
		"<bool>","false",
		"Use depth-first search",
		"Search");

  create_categorized_parameter("breadthFirst",breadthFirst,
		"<bool>","false",
		"Use breadth-first search",
		"Search");

  create_categorized_parameter("initialDive",initialDive,
		"<bool>","false",
		"Use depth-first-like 'dive' until first incumbent found",
		"Search");

  create_categorized_parameter("integralityDive",integralityDive,
		"<bool>","true",
		"Use subproblem integrality measure to guide initial\n\t"
                "diving; otherwise, just use depth",
		"Search");

  create_categorized_parameter("lazyBounding",lazyBounding,
		"<bool>","false",
		"Delay bounding subproblems as long as possible",
		"Search");

  create_categorized_parameter("eagerBounding",eagerBounding,
		"<bool>","false",
		"Bound problems as soon as possible",
		"Search");

/// TERMINATION

  create_categorized_parameter("relTolerance",relTolerance,
		"<double>","1e-7",
		"Relative tolerance for optimal objective value",
		"Termination",
		ParameterNonnegative<double>());

  create_categorized_parameter("absTolerance",absTolerance,
		"<double>","0.0",
		"Absolute tolerance for optimal objective value",
		"Termination",
		ParameterNonnegative<double>());

  create_categorized_parameter("integerTolerance",pebblBase::integerTolerance,
		"<double>","1e-5",
		"Tolerance to determine whether values are integral",
		"Termination",
		ParameterBounds<double>(0.0,1.0));

  create_categorized_parameter("maxSPBounds",maxSPBounds,
		"<int>","0",
		"Maximum subproblem bounds computed before abort\n\t"
		"(0 means no limit)",
		"Termination",
		ParameterNonnegative<int>());

  create_categorized_parameter("maxCPUMinutes",maxCPUMinutes,
		"<double>","0.0",
		"CPU minutes (per processor) allowed before abort\n\t"
		"(0 means no limit)",
		"Termination",
		ParameterNonnegative<double>());

  create_categorized_parameter("maxWallMinutes",maxWallMinutes,
		"<double>","0.0",
		"Wall clock minutes allowed before abort (0 means no limit)",
		"Termination",
		ParameterNonnegative<double>());

  create_categorized_parameter("printAbortMessage",printAbortMessage,
		"<bool>","true",
		"Print an alert message and statistics when aborting",
		"Termination");

/// INCUMBENT MANAGEMENT

  create_categorized_parameter("startIncumbent",startIncumbent,
		"<double>","0.0",
		"Value of some known feasible solution",
		"Incumbent Management");

  create_categorized_parameter("heurLog",heurLog,
		"<bool>","false",
		"Output log for debugging incumbent heuristics",
		"Incumbent Management");

/// ENUMERATION

  create_categorized_parameter("enumRelTolerance",
		EnumerationRelTolerance, "<double>","-1.0",
                "Relative tolerance for enumeration.  Find solutions\n\t"
                "that are within this multiplicative factor of being\n\t"
                "optimal.  For example, a value of 0.1 requests solutions\n\t"
                "within 10% of optimality.",
		"Enumeration",
		utilib::ParameterLowerBound<double>(-1.0));

  create_categorized_parameter("enumAbsTolerance",
                EnumerationAbsTolerance, "<double>","-1.0",
		"Absolute tolerance for enumeration.  Find solutions\n\t"
                "that are within this additive distance of optimality.",
		"Enumeration",
		utilib::ParameterLowerBound<double>(-1.0));

  create_categorized_parameter("enumCount",EnumerationCount, "<int>","0",
            "If positive, the limit on the number of enumerated solutions.\n\t"
            "If enumRelTolerance or enumAbsTolerance are set, return an\n\t"
            "arbitrary set of up to enumCount solutions meeting the\n\t"
            "tolerance criteria.  If neither enumeration tolerance\n\t"
            "is set, return a set of enumCount solutions with the\n\t"
	    "best acheivable objective values.",
	    "Enumeration",
	     utilib::ParameterNonnegative<int>());


  printFullSolution=false;
  create_categorized_parameter("printFullSolution",printFullSolution,
		  "<bool>","false",
		  "Print full solution to standard output as well\n\t"
		  "as writing it to a file","Output");



}

} // namespace pebbl
