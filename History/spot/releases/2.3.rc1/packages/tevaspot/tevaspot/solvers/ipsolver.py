
import tevaspot.util
from tevaspot.core import *
import coopr.opt
from pyutilib.plugin import *

class IPSolver(Solver):

    def __init__(self):
        Solver.__init__(self, "ip")
        self.ip_solver_priority=["cplex", "PICO", "cbc", "glpk"]

    def _create_parser_groups(self):
        Solver._create_parser_groups(self)

        self._parser_group["IP Solver Options"] = OptionGroup(self.parser, "IP Solver Options")

        self._parser_group["IP Solver Options"].add_option("--compute-bound",
            help="Only compute a bound on the value of the optimal solution.",
            action="store_true",
            dest="compute_bound",
            default=False)

        self._parser_group["IP Solver Options"].add_option("--ip-solver",
            help="Specify the type of IP solver used",
            action="store",
            dest="ip_solver",
            default=None)

    def _process_options(self):
        """
        Process problem options
        """
        Solver._process_options(self)
        #
        # Find setupIPData
        #
        self.setupIPData = pyutilib.registered_executable("setupIPData")
        if self.setupIPData is None:
            print "ERROR: the 'setupIPData' command is not available"
            sys.exit(1)
        #
        # Find an IP executable
        #
        solver = None
        if not self.options.ip_solver is None:
            solver = coopr.opt.SolverFactory(self.options.ip_solver)
            if solver is None:
                print "ERROR: IP solver '%s' found!" % self.options.ip_solver
                print "Available IP solvers:"
                print coopr.opt.SolverFactory()
                sys.exit(1)
        else:
            for name in self.ip_solver_priority:
                solver = coopr.opt.SolverFactory(name)
                if not solver is None:
                    break
            if solver is None:
                print "ERROR: no IP solver found!"
                print "Available IP solvers:"
                print coopr.opt.SolverFactory()
                sys.exit(1)
        self.ip_solver=solver

    def printParameters(self):
        """
        Print the parameters of this solver.
        """
        if self.options.debug == True:
            print "IP Solver:  ", self.ip_solver.name

    def solve(self, problem, results=None):
        if problem.ip_model is None:
            print "ERROR: no IP model is available for this problem"
            sys.exit(1)
        if not results is None:
            print "WARNING: ignoring results provided to IP solver"
        #
        # Setup problem
        #
        self.create_config_file(problem)
        ##problem.aggregate_impacts()
        self.ampl_data_file=self.run_setupIPData(problem)
        #
        # Setup command-line options
        #
        options = self.options.solver_options
        logFileName = problem.global_options.tmpname+"_ipsolver.log"
        ###
        ### Currently, we cannot apply valgrind and memmon to IP solvers, since they
        ### are being executed by Coopr.
        ###
        ###if ('all' in problem.global_options.memcheck) or ('ipsolver' in problem.global_options.memcheck):
        ###    valgrind=True
        ###    valgrind_log = self.global_options.tmpname+"memcheck.ipsolver.log"
        ###else:
        ###    valgrind=False
        ###    valgrind_log = ""
        #
        # Setup PICO solver options
        #
        if self.ip_solver.name is 'pico':
            options += " debug=1 lpType=clp "
            if self.options.compute_bound:
                options += " onlyRootLP=true "
            else:
                min_sensors_obj = False
                for goal in problem.goals:
                    if problem.objflag[goal] == True and goal == "ns":
                        min_sensors_obj = True
                if min_sensors_obj == True:
                    options += " RRTrialsPerCall=8 RRDepthThreshold=-1 feasibilityPump=false usingCuts=true absTolerance=.99999 "
                else:
                    options += " RRTrialsPerCall=8 RRDepthThreshold=-1 feasibilityPump=false usingCuts=true "
            if not self.options.timelimit is None:
                options += " maxWallMinutes=" + `self.options.timelimit`
        #
        # Setup GLPK solver options
        #
        elif self.ip_solver.name is 'glpk':
            if self.options.compute_bound:
                options += " nomip= "
            if not self.options.timelimit is None:
                options += " tmlim=" + `self.options.timelimit`
        #
        # Setup cplex solver options
        #
        elif self.ip_solver.name is 'cplex':
            if self.options.compute_bound:
                options += " relax_integrality= "
            if not self.options.timelimit is None:
                options += " timelimit=" + `int(self.options.timelimit*60.0)`
        #
        # Setup cbc solver options
        #
        elif self.ip_solver.name is 'cbc':
            if self.options.compute_bound:
                options += " nomip= "
            if not self.options.timelimit is None:
                options += " sec=" + `int(self.options.timelimit*60.0)`
        #
        # Unknown solver
        #
        else:
            print "WARNING: unknown solver '%s'" % self.ip_solver.name
        #
        # Execute IP Solver
        #
        print "Running IP solver",self.ip_solver.name
        results = self.ip_solver.solve(problem.ip_model, problem.data_dir + os.sep + problem.global_options.tmpname + ".dat", logfile=logFileName, options=options)
        #
        # Archive output and return
        #
        self.print_log(logFileName)
        return results

    def run_setupIPData(self, problem):
        """
        Execute the 'setupIPData' command.
        """
        if ('all' in problem.global_options.memcheck) or ('setupIPData' in problem.global_options.memcheck):
            valgrind=True
            valgrind_log = problem.global_options.tmpname+"memcheck.setupIPData.log"
        else:
            valgrind=False
            valgrind_log = ""
        outFileName = problem.global_options.tmpname+".dat"
        logFileName = problem.global_options.tmpname+"_setupIPData.log"
        if 'gamma' in dir(problem.options):
            gammastr = " --gamma="+`problem.options.gamma`
        else:
            gammastr = ""
        command = self.setupIPData.get_path()+gammastr+" --output="+outFileName+" "+problem.global_options.tmpname+".config "
        if problem.global_options.debug:
            print "Running command...",command
        pyutilib.run_command(command, outfile=logFileName, valgrind=valgrind, valgrind_log=valgrind_log, memmon=self.global_options.memmon)
        #
        # When we run with memmon, we need to strip this output out of the 
        # *.dat file.
        #
        if problem.global_options.memmon:
            tevaspot.util.print_memmon_log('setupIPData', logFileName)
        #
        # Return the *.dat filename
        #
        return outFileName

    
pyutilib.register_executable(name="setupIPData")
SolverRegistration("ip", IPSolver, description="Optimize with an integer programming solver")

