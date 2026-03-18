#include "constructive.h"
#include "bossa_stack.h"
#include "bossa_timer.h"
#include "bossa_random.h"
#include "rfw_stats.h"
#include "bossa_heap.h"
#include <stdlib.h>
#include <string.h>

/*---------------
  | error message
  *--------------*/

void PMConstructive::fatal (const char *func, const char *msg) {
  fprintf (stderr, "PMConstructive::%s: %s.\n", func, msg);
  exit(-1);
}


/*-----------------------------------------
  | print name of current message to 'file'
  *----------------------------------------*/

void PMConstructive::printMethod (FILE *file) {
  fprintf (file, method_name);
}


/*-----------------------------------------------------------------------
  | Parser for string that specifies a method; returns true if the 
  | string is valid (in which case, relevant local variables are updated)
  *----------------------------------------------------------------------*/

bool PMConstructive::tryMethod (char *m, const char *name, int code, int np) {
  if (np == 0) {
    if (strcmp (m, name) == 0) {
      sprintf (method_name, m);
      method = code;
      return true;		
    }
  } else if (np == 1) {
    char format[256], buffer[256];
    sprintf (format, "%s:%%s", name);
    if (sscanf (m, format, buffer) == 1) {
      sprintf (method_name, m);
      method = code;
      param[method] = atof (buffer);
      return true;
    }
  }
  return false;
}


/*-------------------------------------------------------------------
  | Define method to be executed by 'run'. Input is a string with the
  | name of the methods, each followed by its parameters (if any); 
  | different methods are separated by a colon (:).
  *------------------------------------------------------------------*/

void PMConstructive::setMethod (char *m) {
  if (tryMethod (m, "rgreedy", RGREEDY, 1)) return;
  if (tryMethod (m, "random", RANDOM, 1)) return;
  if (tryMethod (m, "rpg", RPG, 1)) return;
  if (tryMethod (m, "pgreedy", PGREEDY, 1)) return;
  if (tryMethod (m, "pworst",  PWORST, 1)) return;
  if (tryMethod (m, "preverse", PREVERSE, 0)) return;
  if (tryMethod (m, "mix", MIX, 0)) return;
  if (tryMethod (m, "sample", SAMPLE, 1)) return;
  //if (tryMethod (m, "mst", MST, 0)) return;

  fprintf (stderr, "WARNING: '%s' is not a valid constructive heuristic.\n", m);
}


/*-------------------------------------------
  | run 'active' method, defined by setMethod
  *------------------------------------------*/

void PMConstructive::run (PMSolution *s, int k) {
  int m;
  if (method != MIX) {m = method;}
  else {m = BossaRandom::getInteger(0,MIX-1);}

  double v = param[m];

  if (k<=0) {
    if (s->getInstance()->fixedP()) k = s->getInstance()->getP();
    else k = (s->getInstance()->getM()+1)/2;
  }

  switch (m) {
    //case MST     : mst      (s, k, false); break;
  case RPG     : rpg      (s, k, v, false); break;
  case RGREEDY : greedy   (s, k, false, v, NULL); break;
  case PGREEDY : greedy   (s, k, true, v, NULL); break;
  case PWORST  : pworst   (s, k, NULL, v, false); break;
  case PREVERSE: preverse (s, k, false); break;
  case SAMPLE  : sample   (s, k, v, false); break;
  case RANDOM  : random   (s, v, false); break;
  default: fatal ("run", "invalid constructive heuristic");
  }
}


/*-------------------------------------
  | Add k random elements to solution s
  *------------------------------------*/

void PMConstructive::addRandom (PMSolution *s, int k) {
  int i;
  int p = s->getOpenCount(); //number of facilities in the solution
  int m = s->getM();         //potential facilities
  int count = m-p;           //number of candidates
	
  if (k>count) {
    fprintf (stderr, "(p=%d, m=%d, k=%d)\n", p, m, k);
    fatal ("addRandom", "too many facilities");
  }

  int *v = new int[m+1];                                  
  for (i=1; i<=count; i++) v[i]=s->getFacility(i+p); //list of candidates

  partialShuffle (v, 1, count, k);                   //pick first k at random
	
  for (i=1; i<=k; i++) s->add(v[i],true);            //add them to the solution
  delete [] v;
}


/*------------------------------------------------------
  | Builds a completely random solution: each facility
  | has probability 'prob' of being open. At least one
  | facility will be opened. For facility location only.
  *-----------------------------------------------------*/

void PMConstructive::random (PMSolution *s, double prob, bool stats) {
  checkNotFixed (s, "random"); //for facility location only


  fprintf (stderr, "Running random...\n");
	
  BossaTimer timer (true);
  int m = s->getM();
  s->reset(); //ensure that the solution is empty

  //if probability not within range, guess something
  if (prob<0 || prob>1) {prob = BossaRandom::getDouble();}

  //open all facilities
  for (int u=m; u>0; u--) {
    if (BossaRandom::getDouble()<=prob) {s->add(u, false);}
  }

  //make sure at least one facility is opened...
  if (s->getOpenCount()==0) {s->add(BossaRandom::getInteger(1,m), false);}
	
  //compute closest, second closest
  s->ensureConsistency();

  //print statistics, if necessary
  if (stats) {
    double result = s->calcCost();
    fprintf (stdout, "random %.2f\n", result);
    fprintf (stdout, "randomtime %.3f\n", timer.getTime());
    fprintf (stdout, "randomprop %.3f\n", prob);
  }
}


/*--------------------------------------------------------
  | random+greedy: adds a fraction rfrac of the facilities 
  | at random, then comples with the greedy algorithm. If
  | rfrac is not between 0 and 1, it assumes a random
  | value in the range. For p-median only.
  *-------------------------------------------------------*/

void PMConstructive::rpg (PMSolution *s, int k, double rfrac, bool stats) {
  //checkFixed (s, "rpg"); //for p-median only
	
	
  if (k<=0 || k>s->getM()) fatal ("rpg", "number of facilities to insert out of range");
	
  //assert (k>0 && k<=s->getInstance()->getM());

  BossaTimer t(true);


  //if random fraction not within range, guess something
  if (rfrac<0 || rfrac>1) rfrac = BossaRandom::getDouble();

  //compute the actual number of random facilities
  //int r = (int) ceil (rfrac*(double)s->getInstance()->getP());
  int r = (int) ceil (rfrac * (double)k);

  //compute the solution
  s->reset();             //start with an empty solution
  addRandom (s, r);       //add r elements at random
  greedy (s, k, false, 0, NULL); //complete greedily

  //print statistics if necessary
  if (stats) {
    fprintf (stdout, "rpg %.2f\n", s->calcCost());
    fprintf (stdout, "rpgtime %.2f\n", t.getTime());
  }
}


/*------------------------------------------------------------------
  | Greedy: adds facilities to s one at a time, until p facilities
  |   are inserted. The running time is O(nk), where k is the 
  |   number of times cities 'change hands' during the execution of
  |   the algorithm. One can find trivial upper and lower bounds for
  |   k: n <= k <= np (n is a lower bound if we start from an empty 
  |   solution; if we start with something else we may have k<n).
  |
  | The input solution need not be empty; all facilities already
  | there will be preserved.
  *-----------------------------------------------------------------*/

void PMConstructive::greedy (PMSolution *s, int k, bool proportional, double alpha, int *initlist) {
  checkFixed (s, "greedy");
  int i, f;
  int verbose = 0;
  int *candlist;
  double *savelist = NULL;

  //assert (k>0 && k<=s->getInstance()->getM());

  if (k<=0 || k>s->getM()) fatal ("greedy", "invalid number of facilities to insert");


  //if alpha is not within range, we will use a random value
  bool random_alpha = (alpha<0 || alpha>1);

  //---
  // get basic information
  //---
  PMInstance *inst = s->getInstance(); //instance
  int n = inst->getN();                //number of customers
  int m = inst->getM();                //number of potential facilities
  //int p = inst->getP();                //number of facilities in the solution

  if (s->getOpenCount()>=k) return;    //maybe we're done already

  //if (s->getP() == p) return;          //maybe we're done already

  //---
  // check the list of candidates
  //---
  bool list_provided = (initlist!=NULL);
  if (!list_provided) {
    candlist = new int [m+1];
    candlist[0] = 0;             //position 0 contains the size of the array
    for (i=s->getOpenCount()+1; i<=m; i++) {
      candlist[0]++;
      candlist[candlist[0]] = s->getFacility(i);
    }
  } else candlist = initlist;

  //---
  // determine closeness information
  //---
  double *save = new double [m+1]; //save[i] = how much we save if we add i to the solution
  savelist = new double [m+1];

  save[0] = -EPSILON; //doing nothing saves us zero (essentially)
  for (i=m; i>0; i--) save[i] = 0; //WARNING: assumes cost is zero
  s->ensureConsistency(); //make sure closeness information makes sense

  //---
  // calculate 'save' for all facilities [O(nm) time]
  //---
  for (i=n; i>0; i--) { //for every city i
    double d = s->getDistClosest(i); //distance to the closest facility
    for (int j=s->getOpenCount()+1; j<=m; j++) { //consider all facilities outside the solution
      f = s->getFacility(j);
      double profit = d - inst->getDist(i,f);
      if (profit>0) save[f] += profit;
    }
  }

  if (verbose>1) fprintf (stderr, "Initial amounts computed.\n");

  //----------------------------------
  // we now add facilities one by one
  //----------------------------------
  while (s->getOpenCount() < k) {
    if (verbose>1) fprintf (stderr, "Adding facility %d.\n", s->getOpenCount()+1);

    //---
    // determine best facility to add
    //---
    int ncand = candlist[0];
    int bestj, bestf;
	
    //---
    // proportional method: facilities that save
    // the most have higher probabilitis
    //---
    for (int j=1; j<=ncand; j++) {savelist[j]=save[candlist[j]];} //list of candidates
		
    if (proportional) {
      double factor = alpha;
      if (factor == 0) {
	factor = 2 * BossaRandom::getDouble();
      }
      double min = RFWStats::min (savelist, 1, ncand);
      double max = RFWStats::max (savelist, 1, ncand);
      if (min == max) {bestj = BossaRandom::getInteger (1, ncand);}
      else {
	double x = BossaRandom::getDouble();
	for (int j=1; j<=ncand; j++) {savelist[j]-=min;}
	bestj = RFW::getWeightedIndex (savelist, 1, ncand, x, factor);
      }
    } 
		
    //---
    // standard method: select at random among the best candidates (fraction alpha)
    //---
    else {
      if (random_alpha) alpha = BossaRandom::getDouble();
      int c = (int) ceil (alpha*candlist[0]);                  //number of candidates to consider
      RFW::bound (c, 1, candlist[0]);                          //avoiding rounding errors
      bestj = RFW::randomLargest(savelist, 1, candlist[0], c); //get one of those c elements
    }
    bestf = candlist[bestj];

    //remove best from the list
    candlist[bestj] = candlist[candlist[0]];
    candlist[0] --;

    //finally, add facility
    s->add (bestf, true); //automatically updates closeness info

    //----------------------------------------------------------------------
    // make all vertices 'change hands' for the next iteration
    // (if there is a next iteration); runs in O(mn) time in the worst case
    //----------------------------------------------------------------------
    if (s->getOpenCount() < k) {
      for (int i=1; i<=n; i++) {
	if (s->getClosest(i)==bestf) {         
	  double d1 = s->getDistClosest(i);  //current closest
	  double d2 = s->getDistClosest2(i); //previous closest (has to be)
	  for (int j=s->getOpenCount()+1; j<=m; j++) { //update 'save' for facilities not in solution
	    int f = s->getFacility(j);
	    double dif = inst->getDist(i,f);
	    if (dif<d2) {
	      save[f] -= (d2-dif);             //we won't save w.r.t. previous closest
	      if (dif<d1) save[f] += (d1-dif); //but we may still save w.r.t. current closest
	    }
	  }
	}
      }
    }
  }

  if (verbose>1) fprintf (stderr, "All iterations executed.\n");

  //----------
  // clean up
  //----------
  if (!list_provided) {
    delete [] candlist;
    candlist = NULL;
  }
  delete [] save;
  if (savelist!=NULL) delete [] savelist;

  if (verbose>1) fprintf (stderr, "Greedy algorithm executed successfully.\n\n");
}


/*****************************************************************
 *
 * Proportional worst: 
 * - starts with a random facility
 * - in each further iteration, chooses costumer with probability
 *   proportional to how much thed'd save if chosen; adds closest
 *   facility to customer
 *
 * s (output): solution (input/output);
 * candlist (input): list of allowed facilities (if NULL, all)
 *                   (candlist[0] must be the length of the list)
 * stats (input): print statistics?
 *
 *****************************************************************/

void PMConstructive::pworst (PMSolution *s, int k, int *candlist, double exponent, bool stats) {
  checkFixed (s, "pworst");
	
  if (k<=0 || k>s->getM()) fatal ("pworst", "number of facilities to insert out of range");

  BossaTimer st (true);

  int bestf, f, i, j;
  PMInstance *inst = s->getInstance();
  //int p = inst->getOpenCount();
  int n = inst->getN();
  int m = inst->getM();

  //if (p==0) fatal ("pworst", "number of facilities cannot be zero");

  double *w  = new double [n+1]; //weights used to compute probabilities
  double *dc = new double [n+1]; //distance to closest candidate

  bool list_given = (candlist!=NULL); //list of candidates may be given as input
  if (!list_given) {                  //if it isn't, assume every facility is a candidate
    candlist = new int [m+1];  
    candlist[0] = m;
    for (i=1; i<=m; i++) candlist[i] = i;
  }

  if (candlist[0]<k) fatal ("pworst", "not enough candidates");
	
  // find distance from each user to the closest facility
  // THIS IS NOT ALWAYS RELEVANT --- IN MOST CASES, IT'S JUST ZERO
  if (n==m) {
    for (i=1; i<=n; i++) dc[i] = inst->getDist(i,i); //just an acceleration
  } else {
    for (i=1; i<=n; i++) dc[i] = POPSTAR_INFINITY; //this is the general case
  }

  for (i=1; i<=n; i++) {
    if (dc[i]==0) continue;
    for (j=1; j<=candlist[0]; j++) {
      f = candlist[j];
      double d = inst->getDist(i,f); //WARNING: THIS CAN BE GREATLY IMPROVED
      if (d<dc[i]) dc[i] = d;
    }
  }

  s->reset(); //start with an empty solution
  bestf = candlist[BossaRandom::getInteger(1,candlist[0])];
  s->add (bestf, true); //add a random facility
	
  //--------------
  // initialize w
  //--------------
  for (i=1; i<=n; i++) {
    w[i] = inst->getDist(i,bestf) - dc[i]; //WARNING: RANDOM ACCESS
  }

  if (exponent == 0) exponent = 2 * BossaRandom::getDouble();
  fprintf (stderr, "<%.02f> ", exponent);

  //----------------------------------
  // main loop: add facilities
  //----------------------------------
  while (s->getOpenCount() < k) {
    //select a user at random with relative probabilities given by w
    int u = RFW::getWeightedIndex (w, 1, n, BossaRandom::getDouble(), exponent);

    //find closest facility not in the solution
    bestf = 0;
    for (j=1; j<=candlist[0]; j++) {
      f = candlist[j];
      if (s->contains(f)) continue;
      if (inst->getDist(u,f) < inst->getDist(u,bestf)) bestf = f; //WARNING: CAN BE ACCELERATED
    }
		
    //add facility to the solution
    s->add(bestf, true);

    //update weights
    for (u=1; u<=n; u++) {
      w[u] = inst->getDist(u,s->getClosest(u)) - dc[u]; //WARNING: COULD BE ACCELERATED
    }
  }

  delete [] w;
  delete [] dc;
  if (!list_given) delete [] candlist;

  if (stats) {
    double result = s->calcCost();
    fprintf (stdout, "srandom %.2f\n", result);
    fprintf (stdout, "srandomtime %.3f\n", st.getTime());
  }
}


/*----------------------------------------------------
  | proportional reverse: starts with a full solution,
  | removes facilities one by one until only k are left. 
  | Probability of removal grows when loss decreases
  *---------------------------------------------------*/
 
void PMConstructive::preverse (PMSolution *s, int k, bool stats) {
  //checkFixed (s, "preverse");

	
  if (k<=0 || k>s->getM()) fatal ("preverse", "number of facilities to insert out of range");

  BossaTimer t (true);

  PMInstance *inst = s->getInstance();
  //int p = inst->getP(); //number of facilities that must remain open
  int n = inst->getN(); //total number of users
  int m = inst->getM();
  int i, j, f;
  double *w    = new double [m+1];
  double *loss = new double [m+1];

  //--------------------------------
  // add all facilities to solution
  //--------------------------------
  for (f=1; f<=m; f++) {s->add(f,false);}
  s->ensureConsistency();

  //----------------------------------
  // now remove facilities one by one
  //----------------------------------
  while (s->getOpenCount() > k) {
    int cp = s->getOpenCount();

    for (f=1; f<=m; f++) loss[f] = 0;
    for (i=1; i<=n; i++) {
      int c1 = s->getClosest(i);
      int c2 = s->getClosest2(i);
      double diff = inst->getDist(i,c2) - inst->getDist(i,c1);
      loss[s->getPosition(c1)] += diff;
    }

    double max = RFWStats::max (loss, 1, cp); //largest loss
    double min = RFWStats::min (loss, 1, cp); //smallest lost

    int best;
    if (max == min) { //same weight for all -> pick a random facility
      best = BossaRandom::getInteger (1, cp);
    } else { //else pick with weighted probabilities
      for (j=1; j<=cp; j++) {w[j] = max - loss[j];}
      best = RFW::getWeightedIndex (w, 1, cp, BossaRandom::getDouble());
    }
    int bestf = s->getFacility(best);

    s->remove(bestf, true);
  }


  s->ensureConsistency();
	
  if (stats) {
    fprintf (stdout, "preverse %.2f\n", s->calcCost());
    fprintf (stdout, "preversetime %.3f\n", t.getTime());
  }
	
  delete [] loss;
  delete [] w;
}


/*----------------------------------------------------------------
  | sample: greedy, but sampling only a fraction of the candidates
  | in each iteration.
  *---------------------------------------------------------------*/

void PMConstructive::sample (PMSolution *s, int k, double fraction, bool stats) {
  //checkFixed (s, "sample");

  if (k<=0 || k>s->getM()) fatal ("sample", "number of facilities to insert out of range");

  BossaTimer t;
  t.start();
  int m, n, verbose, i;
	
  PMInstance *inst = s->getInstance();	
  verbose = 0;
  n = inst->getN(); //number of cities
  m = inst->getM(); //number of potential facilities
  //p = inst->getP(); //number of facilities that must be in the solution

  int sample_size;
  if (fraction<0) fraction = BossaRandom::getDouble();
  if (fraction>1) {
    sample_size = (int)ceil ((log ((double)m/(double)k))/LN2);
  } else {
    sample_size = (int)ceil (fraction * (double)m);
  }
  if (sample_size <= 0) sample_size = 1;
  if (verbose > 1) {fprintf (stderr, "Fraction:%f, absolute %d.\n", fraction, sample_size);}

  //--------------------------
  // build list of candidates
  //--------------------------
  int *candlist = new int [m+1];
  candlist[0] = 0;             
  for (i=s->getOpenCount()+1; i<=m; i++) {
    candlist[0]++;
    candlist[candlist[0]] = s->getFacility(i);
  }

  //---------------------------------
  // Running time: O(p m fraction n) <---- WHY? THIS DOESN'T MAKE SENSE
  //---------------------------------
  s->reset(); //start with an empty solution
  int count = 0;
  while (s->getOpenCount() < k) { //O(pqn)
    count++;
    if (sample_size > candlist[0]) sample_size = candlist[0];

    //RFW::shuffle(candlist, candlist[0]);
    partialShuffle (candlist, 1, candlist[0], sample_size);

    //find best option in the sample
    int bestc = -1; /* WARNING, THIS HAS CHANGED */
    if (sample_size==1) bestc = 1;
    else {
      double bestsave = -1;
      for (int c=1; c<=sample_size; c++) { //O(qn)
	int f = candlist[c];

	//how much do we save by adding f?
	double save = 0; //s->calcSave (f); //0;
	for (int i=1; i<=n; i++) { //O(n)
	  double diff = s->getDistClosest(i) - inst->getDist(i,f); //WARNING: THIS COULD BE ACCELERATED
	  if (diff > 0) save += diff;	
	}	
	if (save > bestsave) {bestsave = save; bestc = c;}
      }
    }
    int bestf = candlist[bestc];
    s->add (bestf, true);        //O(n)

    //remove facility from list of candidates
    candlist[bestc] = candlist[candlist[0]];
    candlist[0]--;
  }

  s->ensureConsistency();
		
  delete [] candlist;

  if (stats) {
    fprintf (stdout, "sample %.2f\n", s->calcCost());
    fprintf (stdout, "samplef %.3f\n", fraction);
    fprintf (stdout, "sampletime %.3f\n", t.getTime());
  }
  if (verbose>0) fprintf (stderr, "(%.2f)", t.getTime());
}


/*---------------------------------------------------------------------
  | make sure the instance requires a solution of fixed size (p-median)
  *--------------------------------------------------------------------*/

void PMConstructive::checkFixed (PMSolution *s, const char *method) {
  if (!s->getInstance()->fixedP()) {
    fprintf (stderr, "ERROR: method \"%s\" can only be applied to p-median instances.\n", method);
    exit(1);
  }
}


/*---------------------------------------------------------------------
  | make sure the instance does not require a solution of fixed size
  | (facility location)
  *--------------------------------------------------------------------*/

void PMConstructive::checkNotFixed (PMSolution *s, const char *method) {
  if (s->getInstance()->fixedP()) {
    fprintf (stderr, "ERROR: method \"%s\" can only be applied to facility location instances.\n", method);
    exit(1);
  }
}

