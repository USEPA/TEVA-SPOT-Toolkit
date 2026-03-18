/****************************************************************
 *
 * program: grasp
 *          provides heuristic solutions to the pmedian problem
 * author: Renato Werneck (rwerneck@princeton.edu)
 * log: May 29, 2002 - file created
 *
 ****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "basics.h"
#include "bossa_timer.h"
#include "config.h"
#include "elite.h"
#include "instance.h"
#include "search.h"
#include "solution.h"
#include "bossa_random.h"
#include "bossa_heap.h"
#include "rfw_stats.h"
#include "search_tables.h"
#include "constructive.h"
#include "distance.h"
#include "matrix_instance.h"
#include "euclidean_instance.h"
#include "cover_instance.h"
#include "graph_instance.h"
#include "geo_instance.h"
#include "hybrid_instance.h"
#include "impact_instance.h"
#include "path_relink.h"
#include "recorder.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <list>

const int GRASP_VERSION = 20030721;

class Grasp {
private:
  void fatal (const char *func, const char *msg);

public:

  double grasp (
		PMSolution *s,   //output: will hold the best solution found
		PMConstructive *constructive, //input: used to run the appropriate constructive heuristic
		int it,          //input: number of iterations to be performed
		PMElite *elite,  //input/output: will hold best solutions found and be used for path-relinking (can be NULL)
		char *lsmethod,   //local search method to be used
		char *combmethod,
		bool stats,      //input: print statistics to stdout?
		bool partials    //input: print partial information to stdout?
		);

  PMInstance *readInstance (const char *filename, int p);
  void showUsage(char *argv[], bool full=true);
  void testConstructive (PMInstance *inst, PMConstructive *tc, PMConfig *config);
  int run(int argc, char *argv[]);
  void testLocalSearch (PMInstance *inst, PMConstructive *constr, char *method, double mt, bool output_density, bool profile);
  void testPathRelinking (PMInstance *inst, PMConstructive *c, PMElite *elite, const char *method);
};


/*-----------------------------------------------
  | fatal: prints error message and exits program
  *----------------------------------------------*/

void Grasp::fatal (const char *func, const char *msg) {
  fprintf (stdout, "%s: %s.\n", func, msg);
  exit (-1);
}


/*-----------------------------------------------------------
  | Given a filename, creates an instances of the appropriate
  | type (based on the extension), reads the instance, and
  | returns a pointer to the allocated instance; the instance
  | must be deallocated elsewhere.
  *----------------------------------------------------------*/

PMInstance *Grasp::readInstance (const char *filename, int p) {
  PMInstance *instance = NULL;
  enum {NONE, DIMACS, TSP, PMI, PMM, GEO, MSC, IMP, UFL, IMPACT} itype = NONE;

  if (strlen(filename)>=4) {
    if      (strcmp (&filename[strlen(filename)-4], ".tsp") == 0) itype = TSP;
    else if (strcmp (&filename[strlen(filename)-4], ".pmm") == 0) itype = PMM;
    else if (strcmp (&filename[strlen(filename)-4], ".pmi") == 0) itype = PMI;
    else if (strcmp (&filename[strlen(filename)-4], ".msc") == 0) itype = MSC; //set cover
    else if (strcmp (&filename[strlen(filename)-4], ".imp") == 0) itype = IMP; //implicit pmedian
    else if (strcmp (&filename[strlen(filename)-4], ".geo") == 0) itype = GEO;
    else if (strcmp (&filename[strlen(filename)-4], ".ufl") == 0) itype = UFL;
    else if (strcmp (&filename[strlen(filename)-7], ".dimacs") == 0) itype = DIMACS;
    else if (strcmp (&filename[strlen(filename)-7], ".impact") == 0) itype = IMPACT;
  }
	
  FILE *input = fopen (filename, "r");
  if (input == NULL) fatal ("readInstance", "could not open input file");

  switch (itype) {
  case UFL:
    instance = new PMMatrixInstance();
    ((PMMatrixInstance*)instance)->readUFL(input);
    break;
  case PMM:
    instance = new PMMatrixInstance();
    ((PMMatrixInstance*)instance)->readPMM (input, p);
    break;
  case TSP:
    instance = new PMEuclideanInstance();
    ((PMEuclideanInstance*)instance)->readTSP (input, p);
    break;
  case MSC:
    instance = new PMCoverInstance(true); //use the extra element
    ((PMCoverInstance*)instance)->readCover (input, p);
    break;
  case IMP:
    instance = new PMCoverInstance(false); //don't use the extra element
    ((PMCoverInstance*)instance)->readCover (input, p);
    break;
  case PMI:
    instance = new PMEuclideanInstance();
    ((PMEuclideanInstance*)instance)->readPMI (input, p);
    break;
  case GEO:
    instance = new PMGeoInstance();
    ((PMGeoInstance*)instance)->readGEO(input, p);
    break;
  case DIMACS:
    instance = new PMGraphInstance();
    ((PMGraphInstance*)instance)->readDimacs(input, p);
    break;
  case IMPACT:
    instance = new PMImpactInstance();
    ((PMImpactInstance*)instance)->readImpact(input, p);
    break;
  case NONE:
    fatal ("readInstance", "format not supported");
  }
  fclose(input);
  return instance;
}



/*--------------------------------------------------------------------
  |
  | GRASP: executes a GRASP with n interations and path-relinking (if 
  |   'nelite' is greater than 0). Uses fastInterchange if 'fi' is 
  |   true, otherwise uses localSearch. The best solution found after
  |   path-relinking is returned in s.
  |
  *--------------------------------------------------------------------*/
 
double Grasp::grasp(PMSolution *s,    //output: will hold the best solution found
		    PMConstructive *constructive, //input: used to run the appropriate constructive heuristic
		    int it,           //input: number of iterations to be performed
		    PMElite *elite,   //input/output: will hold best solutions found and be used for path-relinking (can be NULL)
		    char *lsmethod,   //local search method to be used
		    char *combmethod, //path-relinking mathod to be used
		    bool stats,       //input: print statistics to stdout?
		    bool partials     //input: print partial information to stdout?
		    ) 
{
  int verbose = stats ? 1 : 0;

  PMRecorder recorder;
  BossaTimer t(true);          //overall
  BossaTimer ls_timer(false);  //local search
  BossaTimer con_timer(false); //constructive
  BossaTimer comb_timer(false); //combination time

  PMInstance *inst = s->getInstance();
  PMElite *elite_best = new PMElite(inst,1); // this will store just the best solution
  PMSolution *r  = new PMSolution(inst);
  PMSolution *rs = new PMSolution(inst);      //relink solution
	
  double total_p = 0; //total number of facilities is all local optima

  if(verbose)
    { 
      fprintf (stdout, "Running grasp...\n");
    }

  for(int i=1; i<=it; i++) 
    {
      //-------------------------------
      // build a solution from scratch
      //-------------------------------
      con_timer.resume();
      s->reset();

      int p;
      if (inst->fixedP()) p = inst->getP();
      else {
	if (total_p == 0) {
	  p = (inst->getM()+1) / 2;
	} else {
	  p = (int) ((double) (total_p) / (double) (i - 1));
	}
      }

      constructive->run(s,p);
      double original = s->calcCost();
      con_timer.pause();	
		
      //----------------------
      // perform local search
      //----------------------
      ls_timer.resume();
      PMSearch::runLocalSearch(s,lsmethod);
      double result=s->calcCost();
      ls_timer.pause();

      if(verbose)
	{
	  fprintf(stdout, "%5d: [%.2f] %.2f -> %.2f", i,elite_best->getSolutionCost(1),original,result);
	}

      unsigned int seed = BossaRandom::getNonZero();

      total_p += s->getOpenCount();

      if (elite!=NULL) 
	{
	  comb_timer.resume();
	  int d = elite->countConsistent();
	  bool insert = true;

	  //if there are elite solutions, relink
	  if (d>=1) { 
	    int e = elite->getRandomDifferent (s, 1, d); //a probably different solution
				
	    if (e!=0) { //solution is not identical to some solution in the pool
	      double elite_cost = elite->getSolution (e, r); //r is the solution
	      double oldcost = (elite_cost < result) ? elite_cost : result; //get best of two original

	      if (!r->equals(s)) { //if they are not equal, try to relink them
		//--------------------------------
		// combine both solutions into rs
		//--------------------------------
		double newcost = PMRelink::combine (rs, s, r, combmethod); //s is the one produced by local search, r is the elite one
		if (verbose) fprintf (stdout, " x %.2f -> %.2f", elite_cost, newcost);
		if (rs->computeDifference(r)>1 && rs->computeDifference(s)>1) {
		  PMSearch::localSearch (rs, true, false); //try to improve by local search if necssary
		  newcost = rs->calcCost();
		  if (verbose) fprintf (stdout, " -> %.2f", newcost);
		}

		//-----------------------------------------------------------
		// if there is an improvement, try to save 'hybrid' solution
		//-----------------------------------------------------------
		if (newcost < oldcost - EPSILON) {
		  elite->add (rs, newcost);      //add to the set of elite solutions
		  elite_best->add (rs, newcost); //save it in the local list of elite solutions
		  if (recorder.report (i, t.getTime(), newcost)) {
		    if (partials) fprintf (stdout, "partial %.3f %.3f\n", recorder.best_time, recorder.best_value);
		  }
		}
	      }
	    } 
	    else 
	      {
		insert=false;
	      }
	  }
	  if (insert) elite->add (s, result);
	  comb_timer.pause();
	}

      if(verbose) 
	{
	  double mre=((result-elite_best->getSolutionCost(1))/elite_best->getSolutionCost(1))*100.0;
	  std::cout.setf(std::ios_base::fixed,std::ios_base::floatfield);	  
	  std::cout.precision(2);
	  std::cout << " MRE=" << mre << "% ";

	  std::list<int> open;
	  for(int k=1;k<=p;k++) 
	    {
	      open.push_back(s->getFacility(k));
	    }
	  open.sort();

	  std::cout << "Solution=[";
	  for(std::list<int>::const_iterator iter=open.begin();iter!=open.end();iter++) 
	    {
	      std::cout << (*iter) << " ";
	    }
	  std::cout << "]" << std::endl;
	  std::cout.flush();
	}

      //------------------------
      // save original solution
      //------------------------
      elite_best->add(s, result); 
      if (recorder.report (i, t.getTime(), result)) {
	if (partials) {fprintf (stdout, "partial %.3f %.3f\n", recorder.best_time, recorder.best_value);}
      }
		
      if (verbose) {
	if (recorder.best_iteration==i) fprintf (stdout, " <");
	fprintf (stdout, "\n");
      }
      BossaRandom::randomize(seed);		
    }
	
  //----------------------------------------------------
  // copy best solution to s (and its value to solcost)
  //----------------------------------------------------
  double solcost = elite_best->getSolution (1, s);
	
  //-----------------------
  // deallocate structures
  //-----------------------
  delete elite_best;
  delete r;
  delete rs;

  //--------------
  // print report
  //--------------
  if (stats) 
    {
      fprintf(stdout, "grasp %.3f\n", solcost);
      fprintf(stdout, "graspconst ");
      constructive->printMethod(stdout);
      fprintf(stdout, "\n");
      fprintf(stdout, "lsaveragep %.2f\n", total_p / (double)it);
      fprintf(stdout, "lsmethod %s\n", lsmethod);
      fprintf(stdout, "graspcomb %s\n", combmethod);
      fprintf(stdout, "grasplstimeavg %.2f\n", ls_timer.getTime()/(double)it);
      fprintf(stdout, "graspconsttimeavg %.2f\n", con_timer.getTime()/(double)it);
      fprintf(stdout, "grasplstime %.2f\n", ls_timer.getTime());
      fprintf(stdout, "graspconsttime %.2f\n", con_timer.getTime());
      fprintf(stdout, "grasptime %.2f\n", t.getTime());
      fprintf(stdout, "graspit %d\n", it);
      fprintf(stdout, "graspbestit %d\n", recorder.best_iteration);
      fprintf(stdout, "graspbesttime %.2f\n", recorder.best_time); 
    }

  return solcost;
}





/*-----------------------------------------------
  | Show usage: prints message explaining how the 
  | program should be used and exits
  *----------------------------------------------*/

void Grasp::showUsage(char *argv[], bool full) 
{
  fprintf (stdout, "\nUsage: %s <file> [options]\n", argv[0]);
	
  if(full) fprintf (stdout, "\n Main options:\n");
	
  fprintf (stdout, "    -graspit <n>           : number of grasp iterations [default=32]\n");
  fprintf (stdout, "    -elite <x>             : size of the pool of elite solutions [default=10]\n");
  fprintf (stdout, "    -p <x>                 : sets number of facilities (p); overrides value in the input file (if any)\n");
  fprintf (stdout, "    -rp <x>                : same as p, but as a fraction of m (0<p<=1)\n");
  fprintf (stdout, "    -output <x>            : output solution to file 'x'\n");
  fprintf (stdout, "    -outputFacilities <x>  : output facility locations to file 'x'\n");

  if(full) 
    {
      fprintf(stdout, "\n Additional options:\n");
      fprintf(stdout, "    -greedy      : execute greedy algorithm\n"); 
      fprintf(stdout, "    -grasp       : execute grasp\n");
      fprintf(stdout, "    -ls <met>    : use <met> as the local search method (default is m:-1)\n");
      fprintf(stdout, "    -ch <met>    : use <met> as the constructive heuristic (default is rpg:1)\n");
      fprintf(stdout, "    -cf <x>      : cache factor (will be x times m/p) (negative -> full size)\n");		
      fprintf(stdout, "    -seed <x>    : use <x> as the seed for the random number generator\n");
      //fprintf (stdout, "  -vnds        : run vnds\n");
    }
  fprintf (stdout, "\n");
  exit (-1);
}



/*------------------------------
  | test constructive heuristics
  *-----------------------------*/

void Grasp::testConstructive (PMInstance *inst, PMConstructive *tc, PMConfig *config) {
	
  int i,j,k,p;
  k = config->tc; //number or iterations
  double *v, *lit;

  p = inst->getP();

  PMSolution **s;
	
  s = new PMSolution* [k+1];
  v = new double [k+1];
  lit = new double [k+1];
  for (i=1; i<=k; i++) s[i] = new PMSolution (inst);

  fprintf (stdout, "tcmethod ");
  tc->printMethod(stdout);
  fprintf (stdout, "\n");


  bool run_ls = true;

  BossaTimer t;
  t.start();
  for (i=1; i<=k; i++) {
    BossaRandom::randomize(config->seed - 1 + i);
		
    int p;
    if (inst->fixedP()) {p = inst->getP();}
    else {p = (inst->getM()+1)/2;}
    tc->run(s[i], p);

    lit[i] = 0.0;
    if (run_ls) {
      int it;
      PMSearch::localSearch (s[i], true, false, &it);
      lit[i] = (double)it;
    }
    v[i] = s[i]->calcCost();
  }
  fprintf (stdout, "tctime %.2f\n", t.getTime() / (double)k);
  fprintf (stdout, "tccount %.2f\n", (double)k);
  fprintf (stdout, "tcmin %.2f\n", RFWStats::min(v,1,k));
  fprintf (stdout, "tcavg %.2f\n", RFWStats::average(v,1,k));
  fprintf (stdout, "tcstddev %.2f\n", RFWStats::stddev(v,1,k));
  fprintf (stdout, "tcmed %.2f\n", RFWStats::median(v,1,k));
  fprintf (stdout, "tcmax %.2f\n", RFWStats::max(v,1,k));

  fprintf (stdout, "tcminit %.2f\n", RFWStats::min(lit,1,k));
  fprintf (stdout, "tcavgit %.2f\n", RFWStats::average(lit,1,k));
  fprintf (stdout, "tcstddevit %.2f\n", RFWStats::stddev(lit,1,k));
  fprintf (stdout, "tcmedit %.2f\n", RFWStats::median(lit,1,k));
  fprintf (stdout, "tcmaxit %.2f\n", RFWStats::max(lit,1,k));

  int npairs = k * (k-1) / 2;
  double *diff = new double [npairs+1];

  int c = 0;
  for (i=1; i<k; i++) {
    for (j=i+1; j<=k; j++) {
      c++;
      diff[c] = (double) (s[i]->computeDifference(s[j]));
    }
  }

  fprintf (stdout, "tcmindiff %.3f\n", RFWStats::min(diff,1,npairs) / (double)p);
  fprintf (stdout, "tcavgdiff %.3f\n", RFWStats::average(diff,1,npairs)/ (double)p);
  fprintf (stdout, "tcmeddiff %.3f\n", RFWStats::median(diff,1,npairs)/ (double)p);
  fprintf (stdout, "tcmaxdiff %.3f\n", RFWStats::max(diff,1,npairs)/ (double)p);
	
  for (i=1; i<=k; i++) delete s[i];
  delete [] s;
  delete [] v;
}


/*---------------------
  | Test path-relinking
  *--------------------*/
void Grasp::testPathRelinking (PMInstance *inst, PMConstructive *c, PMElite *elite, const char *method) {
  if (elite==NULL) fatal ("testPathRelinking", "number of elite solutions cannot be zero");
  fprintf (stdout, "Resetting list of elite solutions...\n" );
  PMElite *single_elite = new PMElite (inst, 1);
	
  elite->reset();
  fprintf (stdout, "Creating new solution...\n");
  PMSolution *s1 = new PMSolution (inst);
  PMSolution *s2 = new PMSolution (inst);
  PMSolution *s  = new PMSolution (inst);
  PMSearchPathRelinking *sm = new PMSearchPathRelinking (inst);


  int i, j, t;
  int e = elite->getCapacity();
  int maxtries = 2 * e;

  //----------------------------------
  // fill the pool of elite solutions
  //----------------------------------
  fprintf (stdout, "Entering main loop...\n");
  for (t=1; t<=maxtries; t++) {
    fprintf (stdout, "%d ", t);
    s1->reset();   //start with empty solution
    c->run(s1,0);  //constructive algorithm on top of it
    PMSearch::localSearch(s1, true, false, false, false); //local search
    elite->add(s1, s1->calcCost()); //add to list
    fprintf (stdout, "%d/%d\n", t, elite->countConsistent());
    if (elite->countConsistent()>=e) break;
  }

  enum {DOWN, HOR, UP};

  int m = inst->getM(); //number of candidate facilities
	
	
  double *imp  [3];
  int    *count[3]; 
  double *sum  [3];

  for (i=0; i<3; i++) {
    count[i] = new int [m+1];
    imp  [i] = new double [m+1];
    sum  [i] = new double [m+1];
    for (j=0; j<=m; j++) {
      count[i][j] = 0;
      imp  [i][j] = 0;
      sum  [i][j] = 0;
    }
  }

  int ns = elite->countConsistent();

  int eliteplus = 10;

  int bestuniform = 0; //number of times uniform won
  int bestbiased  = 0; //number of times biased won
  int ties        = 0; //number of times there was a tie
  double sumuniform = 0;
  double sumbiased  = 0;

  for (int i2=eliteplus+1; i2<=ns; i2++) {
    int totalcount = 0;
    int rightcount = 0;
    int totaldiff = 0;
    int rightdiff = 0;
    for (int i1=1; i1<=eliteplus; i1++) {

      if (i1==i2) continue;
      double v1 = elite->getSolution (i1, s1);
      double v2 = elite->getSolution (i2, s2);
      int diff = s1->computeDifference(s2);

      int dir = HOR;
      if (v1<v2) dir = UP;
      else if (v1 > v2) dir = DOWN;

      PMRelink::combine (s, s1, s2, method);

      /*
	sm->setSolutions (s1, s2);           //define the extremes of the path
	single_elite->reset();               //the best solution in the path will be here
	s->copyFrom (s1);              
	search (s, true, sm, single_elite, true);  //excecute path-relinking
      */
      //double vsol = single_elite->getSolution (1, s);   //copy solution to s and get value
      double vsol = s->calcCost();
      if (s->computeDifference(s1)>1 && s->computeDifference(s2)>1) {
	PMSearch::localSearch (s, true, false);  //try to improve solution with local search
	vsol = s->calcCost();   //get the value
      }

      //calculate profit
      double profit;
      if (v1<v2) profit = v1 - vsol;
      else profit = v2 - vsol;
      if (profit<0) profit = 0;

      //bool success = (profit > 0);

      fprintf (stdout, "[%d:%d] %f x %f (%d,%d) -> %f (%f)\n", i1, i2, v1, v2, dir, diff, vsol, profit);

      count[dir][diff] ++;
      imp  [dir][diff] += profit;
      sum  [dir][diff] += v1+v2;

      totalcount ++;
      totaldiff  += diff;

      if (profit) {
	rightcount ++;
	rightdiff += diff;
      }

    }
    double probcount = (double)rightcount / (double)totalcount;
    double probdiff  = (double)rightdiff  / (double)totaldiff;

    double ratio;
    if (probcount>0) ratio = probdiff / probcount;
    else ratio = 1;
		
    fprintf (stdout, "tpd %d %f %f %f\n", i2, probdiff, probcount, ratio);
    fprintf (stdout, "tpd %d %f %f %f\n", i2, probdiff, probcount, ratio);

    double mindiff = 0.001;
    if (ratio > 1 + mindiff) {
      bestbiased ++;	
    } else if (ratio < 1 - mindiff) {
      bestuniform ++;
    } else {
      ties ++;
    }

    sumbiased += probdiff;
    sumuniform += probcount;
  }


  {
    int x = bestbiased + bestuniform + ties;
    fprintf (stdout, "trt %f\n", (double)ties/(double)x);
    fprintf (stdout, "tru %f\n", (double)bestuniform/(double)x);
    fprintf (stdout, "trb %f\n", (double)bestbiased/(double)x);
    fprintf (stdout, "trpu %f\n", (double)sumuniform/(double)x);
    fprintf (stdout, "trpb %f\n", (double)sumbiased/(double)x);
    fprintf (stdout, "trpr %f\n", (double)sumbiased/(double)sumuniform);


  }

  for (i=0; i<3; i++) {
    for (j=0; j<=m; j++) {
      if (count[i][j]==0) continue;
      fprintf (stdout, "tpr %d %d %d %f\n", i, j, count[i][j], imp[i][j]); 
    }
  }

  for (j=0; j<=m; j++) {
    int c = count[0][j] + count[1][j] + count[2][j];
    double v = imp[0][j] + imp[1][j] + imp[2][j];
    double a = (sum[0][j] + sum[1][j] + sum[2][j]) / (2.0 * (double)c);
    double im = v / (double)c;
    if (c>0) {
      fprintf (stdout, "tpa %d %d %f %f %f \n", j, c, a, im, a-im);
    }
  }

  delete single_elite;
  delete sm;
  delete s1;
  delete s2;
  delete s;
  for (i=0; i<3; i++) {
    delete [] count[i];
    delete [] imp[i];
    delete [] sum[i];
  }
}


/*---------------------------------------------------------------
  | procedure to test (time) local search methods. First, 
  | builds a solution using the method specified by constr. Then,
  | executes local search on the resulting solution; if the 
  | minimum time not reached (mt), restores the original solution 
  | and does it again --- until mt is reached. Prints the average 
  | time to run everything.
  |
  | If output_density is true, also outputs the fraction of the
  | entries in extra that are nonzero. This has an important
  | effect on running time, so use false when you are really 
  | interested in time.
  *--------------------------------------------------------------*/

void Grasp::testLocalSearch (PMInstance *inst, PMConstructive *constr, char *method, double mt, bool output_density, bool profile) {
  double times[4];
	
  BossaTimer timer(true);
  PMSolution *greedy = new PMSolution (inst);
  constr->run(greedy, 0);
  fprintf (stdout, "lsconsttime %.2f\n", timer.getTime());
  fprintf (stdout, "lsconstmethod ");
  constr->printMethod(stdout);
  fprintf (stdout, "\n");
  fprintf (stdout, "lstestmaxtime %f\n", mt);
  fprintf (stdout, "lstestmethod %s\n", method);
  fprintf (stdout, "lstestcf %f\n", PMDistanceOracle::cache_factor);
  fprintf (stdout, "lstestmethodcf %s:%.1f\n", method, PMDistanceOracle::cache_factor);

  PMSolution *temp = new PMSolution (inst);
	
  double t;
  for (int i=3; i>=0; i--) times[i] = 0.0;
  int runs = 0;
  timer.start();
		
  while (1) {
    temp->copyFrom(greedy);
    PMSearch::runLocalSearch(temp, method, output_density, profile ? times : NULL); 
    t = timer.getTime();
    runs++;
    if (t > mt) break;
  }

  if (profile) {
    double btime, ctime, rtime, utime, ttime;
    btime = times[0];
    ctime = times[1];
    rtime = times[2];
    utime = times[3];
    ttime = times[0]+times[1]+times[2]+times[3];
		
    fprintf (stdout, "lstestbtime %.4f\n", btime);
    fprintf (stdout, "lstestbtimef %.3f\n", btime / ttime);
    fprintf (stdout, "lstestbtimep %.3f\n", 100 * (btime / ttime));
    fprintf (stdout, "lstestctime %.4f\n", ctime);
    fprintf (stdout, "lstestctimef %.3f\n", ctime / ttime);
    fprintf (stdout, "lstestctimep %.3f\n", 100 * (ctime / ttime));
    fprintf (stdout, "lstestrtime %.4f\n", rtime);
    fprintf (stdout, "lstestrtimef %.3f\n", rtime / ttime);
    fprintf (stdout, "lstestrtimep %.3f\n", 100 * (rtime/ttime));
    fprintf (stdout, "lstestutime %.4f\n", utime);
    fprintf (stdout, "lstestutimef %.3f\n", utime / ttime);
    fprintf (stdout, "lstestutimep %.3f\n", 100 * (utime/ttime));
  }

  fprintf (stdout, "lstestconstr %.2f\n", greedy->calcCost());
  fprintf (stdout, "lstestvalue %.2f\n", temp->calcCost());
  fprintf (stdout, "lstestruns %d\n", runs);
  fprintf (stdout, "lstesttime %.6f\n", t / (double)runs);

  delete greedy;
  delete temp;
}

/*--------------
  | Main program
  *-------------*/

int Grasp::run(int argc, char *argv[]) 
{
  PMInstance *inst;
  BossaTimer t;
  PMConfig c;

  //-------------------------
  // read input parameters
  //-------------------------	
  if (argc<2) showUsage (argv);
  bool success = c.readParameters (argc, argv);
  if (!success) showUsage (argv);	

  PMDistanceOracle::cache_factor = c.cf;
  fprintf (stdout, "cf %.3f\n", PMDistanceOracle::cache_factor);

  //---------------
  // read instance
  //---------------
  t.start();

  inst = readInstance (argv[1], c.pvalue);
  if (c.pvalue>0 && c.pvalue<=inst->getM()) inst->setP (c.pvalue);
  if (c.rpvalue>0 && c.rpvalue<=1) inst->setP((int) ceil ((double)(inst->getM()) * c.rpvalue));
  if (inst->fixedP() && (inst->getP()<=0 || inst->getP()>inst->getM())) {
    fprintf (stdout, "Error: number of facilities to open (p) must be between 1 and %d (use the '-p' option).\n", inst->getM());
    exit (-1);
  }

  fprintf (stdout, "Fixed:%d p=%d\n", inst->fixedP(), inst->getP());

  fprintf (stdout, "version %d\n", GRASP_VERSION);
  fprintf (stdout, "inittime %.2f\n", t.getTime());
  fprintf (stdout, "oracletime %.3f\n", inst->getOracleTime());

  //---------------------------
  // print instance attributes
  //---------------------------
  BossaRandom::randomize(c.seed);

  char *filename = argv[1];
  char *instance = new char [strlen(filename)+1];
  strcpy (instance, filename);
  RFW::stripPath (instance);
  RFW::stripExtension (instance);

  fprintf (stdout, "file %s\n", filename);
  fprintf (stdout, "instance %s\n", instance);
  fprintf (stdout, "instancep %s-%d\n", instance, inst->getP());
  //fprintf (stdout, "vertices %d\n", inst->getN());
  //fprintf (stdout, "centers %d\n", inst->getP());
  //fprintf (stdout, "rcenters %0.4f\n", inst->getP() / (double)inst->getM());
  fprintf (stdout, "seed %d\n", c.seed);
  inst->outputParameters(stdout);


  //----------------------------------------
  // create a variable to hold the solution
  //----------------------------------------

  PMSolution *s = new PMSolution(inst);      //this variable will hold the solution
	
  if (strlen(c.inputsol)>0) {
    s->readSolution (c.inputsol);	
  } else {
    fprintf (stdout, "Creating random solution...");
    PMConstructive::addRandom (s, inst->getP());	//starts with a random solution
    fprintf (stdout, "done.\n");

  }

  //parameters for the constructive algorithm
  PMConstructive constructive;
  constructive.setMethod (c.ch);

  //create pool of elite solutions
  PMElite *elite = (c.elite>0) ? new PMElite (inst, c.elite) : NULL; 

  //start timing
  t.start(); 

  //run grasp, updating s and populating 'elite'
  if (c.run_grasp) grasp(s, &constructive, c.graspit, elite, c.ls, c.cmg, true, c.partials);
	
  //--------------------
  // run path-relinking
  //--------------------
  if (elite!=NULL && !c.test_pr && c.postopt) {
    PMRelink::pathRelinking (s, elite, c.cmp, true);
    fprintf (stdout, "postopt 1\n");
  } else fprintf (stdout, "postopt 0\n");

  fprintf (stdout, "usepr %d\n", (elite!=NULL) ? 1 : 0);

  //--------------------------
  // test individual elements
  //--------------------------
  if (c.tc>0) {testConstructive (inst, &constructive, &c);}
  if (c.tls) {testLocalSearch (inst, &constructive, c.ls, c.tlstime, c.output_density, c.profile);}
  if (c.test_pr) {testPathRelinking (inst, &constructive, elite, c.cmg);}

  double totaltime = t.getTime();
  fprintf (stdout, "totaltime %.2f\n", totaltime);
  totaltime += inst->getOracleTime();
  fprintf (stdout, "totaltimeo %.2f\n", totaltime);
  totaltime += inst->getFloydTime();
  fprintf (stdout, "totaltimeof %.2f\n", totaltime);
  fprintf (stdout, "cputime %.2f\n", totaltime);
  fprintf (stdout, "bestsol %.4f\n", s->calcCost());

  // output best solution to file
  if(c.output) s->printSolution (c.outname, instance);

  // output facility locations to file
  if(c.outputFacilities) 
    {
      int numFacs=s->getOpenCount();
      std::vector<int> locations;
      locations.reserve(numFacs);
      for(int i=1;i<=numFacs;i++)
	{
	  locations.push_back(s->getFacility(i));
	}
      std::sort(locations.begin(),locations.end());

      std::ofstream facilityOutputFile(c.outFacilityName);
      if(!facilityOutputFile)
	{
	  std::cerr << "***Failed to open facility output file=" << c.outFacilityName << std::endl;
	  return 0;
	}

      facilityOutputFile << "#" << std::endl;
      facilityOutputFile << "# Generated by GRASP solver interface" << std::endl;
      facilityOutputFile << "#" << std::endl;

      facilityOutputFile << rand() << " "; // the ID
      facilityOutputFile << numFacs << " "; // the number of sensors
      for(size_t i=0;i<locations.size();i++)
	{
	  facilityOutputFile << locations[i] << " ";	  
	}
      facilityOutputFile << std::endl;

      facilityOutputFile.close();
    }

  std::cout << "Number of attacks " << inst->getN() << std::endl;
  std::cout << "Number of potential sensor locations " << inst->getM() << std::endl;
  std::cout << "Normalized impact " << s->calcCost()/double(inst->getN()) << std::endl;

  //clean up
  delete elite;
  delete [] instance;
  delete s;	
  delete inst;

  return 0;
}

void outOfMemoryTrap(void)
{
  std::cout << "***OUT OF MEMORY***" << std::endl;
  exit(1);
}

/*--------------------------------
  | main function: just passes the 
  | input parameters to the solver
  *-------------------------------*/

int main (int argc, char *argv[]) 
{
//m  std::set_new_handler(outOfMemoryTrap);

  Grasp *grasp=new Grasp();

  grasp->run(argc,argv);

  delete grasp;

  return 0;
}
