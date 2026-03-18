#include "solution.h"

/*---------------
  | error message
  *--------------*/

void PMSolution::fatal (const char *func, const char *msg) {
  fprintf (stderr, "PMSolution::%s: %s\n", func, msg);
  exit (-1);
}


/*--------------------------------------
  | swap facilities in positions i and j
  *-------------------------------------*/
		
void PMSolution::swappos (int i, int j) {
  int prev_i = faclist[i];
  int prev_j = faclist[j];

  faclist[i] = prev_j;
  faclist[j] = prev_i;
  facpos[faclist[i]] = i;
  facpos[faclist[j]] = j;
}



/*----------------------------------------
  | constructor: creates an empty solution
  | (a solution with no facilities)
  *---------------------------------------*/
		
PMSolution::PMSolution (PMInstance *inst) {
  instance = inst;
  n = instance->getN();
  m = instance->getM();
			
  faclist    = new int [m+1];
  facpos     = new int [m+1];
  closest    = new int [n+1];
  closest2   = new int [n+1];
  closestd   = new double [n+1];
  closest2d  = new double [n+1];
  consistent = new bool [n+1];

  for (int i=m; i>0; i--) faclist[i] = facpos[i] = i;

  reset(); //empty solution, everybody consistent	
}


/*-----------------------------------------
  | remove all facilities from the solution
  *----------------------------------------*/
 
void PMSolution::reset() {
  inconsistencies = 0;
  for (int i=n; i>0; i--) {
    setClosest(i,0,POPSTAR_INFINITY);
    setClosest2(i,0,POPSTAR_INFINITY);
    consistent[i] = true;
  }
  p = 0;
}


/*---------------------------------------------
  | copies the contents of an existing solution
  *--------------------------------------------*/

void PMSolution::copyFrom (PMSolution *s) {
  this->reset();        
  s->ensureConsistency(); //make sure closest and second closest are correctly defined
  if (s->getInstance() != getInstance()) fatal ("copyFrom", "can't solution for different instance");
  int i;
  int k = s->getOpenCount();
  for (i=1; i<=k; i++) add (s->getFacility(i), false);
  for (i=1; i<=n; i++) {
    setClosest  (i, s->getClosest(i));
    setClosest2 (i, s->getClosest2(i));
    consistent[i] = true;
  }
  inconsistencies = 0;
}


/*---------------------------------------------------------------------------
  | Compute the symmetric difference between this solution and s. Defined as
  | the total number of facilities that belong to one solution, but not both.
  *--------------------------------------------------------------------------*/

int PMSolution::computeDifference (PMSolution *s) {
  int i;
  int diff = 0;

  //there but not here
  for (i=s->getOpenCount(); i>0; i--) {if (!contains(s->getFacility(i))) diff++;}
	
  //here but not there
  for (i=getOpenCount(); i>0; i--) {if (!s->contains(getFacility(i))) diff++;}

  return diff;
}


/*--------------------------------------------
  | equals: checks if this solution is equal 
  | to s (i.e., has the exact same facilities)
  *-------------------------------------------*/

bool PMSolution::equals (PMSolution *s) {
  int k=s->getOpenCount();             //number of facilities in s
  if (k!=getOpenCount()) return false; //different number of facilities
	
  for (int i=1; i<=k; i++)              //every facility in s
    if (!contains(s->getFacility(i))) //must also be in this solution
      return false;                 //or else the solutions are different

  return true; //couldn't find anything wrong, the solutions are the same
}


/*------------
  | destructor
  *-----------*/
			
PMSolution::~PMSolution () {
  delete [] closest;
  delete [] closest2;
  delete [] closestd;
  delete [] closest2d;
  delete [] faclist;
  delete [] facpos;
  delete [] consistent;
}


/*-----------------------------------------------------
  | compute the cost of the entire solution, which must 
  | be consistent. Runs in O(n) time.
  *----------------------------------------------------*/
		
double PMSolution::calcCost() {
  if (p==0) return POPSTAR_INFINITY; //nobody served
  if (!isConsistent()) fatal ("calcCost", "function cannot be called for inconsistent solution");
  double cost = 0.0;
  int i;
  for (i=n; i>0; i--) {cost += getDistClosest(i);} //cost of serving all users
  for (i=p; i>0; i--) {cost += instance->getFacCost(faclist[i]);} //cost of opening all facilities
  return cost;
}	
		

/*-----------------------------------------------------------
  | ensures that a solution is consistent; runs in O(n + i*m) 
  | time, where i is the number of inconsistent elements
  *----------------------------------------------------------*/

void PMSolution::ensureConsistency () {
  if (inconsistencies == 0) return;
  for (int i=n; i>0; i--) updateClosest(i); //only the inconsistent ones will be udpated
  if (inconsistencies!=0) fatal ("ensureConsistency", "all nodes should be consistent");
}


/***********************************************
 *
 * adds facility f to the solution
 * - if 'update' is true, closest and closest2
 *   will be updated; this takes O(n) time
 * - if 'update' is false, the procedure takes
 *   O(1) time
 *
 ***********************************************/

void PMSolution::add (int f, bool update) {
  if (contains(f)) return; //nothing to do

  swappos (facpos[f], ++p); //increase solution size, and put f in the last (p-th) position
			
  if (update) {
    for (int i=n; i>0; i--) {
      if (consistent[i]) {
	double d = instance->getDist(i,f);
	upgrade(i,f,d); //inconsistent remain inconsistent for now
      }
    }
  } else {
    if (inconsistencies!=n) {
      for (int i=n; i>0; i--) makeInconsistent(i);
      if (inconsistencies!=n) fatal ("add", "all users should have become inconsistent");
    }
  }
}


/*-----------------------------
  | update the closest/closest2
  *----------------------------*/

void PMSolution::updateClosest (int i) {
  if (consistent[i]) return; //nothing to do
  setClosest (i, 0, POPSTAR_INFINITY);
  setClosest2 (i, 0, POPSTAR_INFINITY);
  for (int j=p; j>0; j--) {
    int f = this->getFacility(j);
    double dif = instance->getDist(i,f);   //distance from i to f
    upgrade(i,f,dif);
  }
  consistent[i] = true;
  inconsistencies --;
}


/*---------------------------------------------------
  | swap facilities: rem must belong to the solution,
  | ins must not. Either can be zero (meaning that
  | just the other action will be performed).
  *--------------------------------------------------*/
		
void PMSolution::swap (int rem, int ins, bool update) {
  if (rem == ins) return;
  if (rem == 0) {add (ins, update); return;}
  if (ins == 0) {remove (rem, update); return;}

  if (!contains(rem) || contains(ins)) fatal ("swap", "invalid move");

  swappos (facpos[rem], facpos[ins]);

  if ((!update) && (inconsistencies==n)) return; //WARNING: IS THIS REALLY THE TEST I WANT TO MAKE?

  for	(int i=n; i>0; i--) {
    if (update) {
      double d = instance->getDist(i,ins);
      upgrade (i, ins, d); //make ins the closest or second closest, if necessary
      if (closest[i]==rem || closest2[i]==rem) {
	makeInconsistent(i);
	updateClosest(i);
      }
    } else {makeInconsistent(i);}
  }
}

		
/*-------------------------------------
  | remove facility f from the solution
  *------------------------------------*/
		
void PMSolution::remove (int f, bool update) {
  if (!contains(f)) return; //nothing to do

  swappos (facpos[f], p); //place the p-th element where i was
  p--; //decrease solution size
  if (!update && inconsistencies==n) return; //if everybody inconsistent and there's no need to update, go away

  for (int i=1; i<=n; i++) {
    if (update) {
      if (closest[i]==f || closest2[i]==f) {
	makeInconsistent(i);
	updateClosest(i);
      }
    } else makeInconsistent(i);
  }		
}			




/*----------------------------------------------
  | print solution to file in DIMACS-like format
  *---------------------------------------------*/
		
void PMSolution::printSolution (FILE *file, const char *name) {
  int i;
  if (name != NULL) fprintf (file, "i %s\n", name);
  fprintf (file, "n %d\n", getN());
  fprintf (file, "p %d\n", getOpenCount());
  fprintf (file, "v %f\n", calcCost());
  for (i=1; i<=getM(); i++) {
    if (contains(i)) fprintf (file, "f %d\n", i);
  }
  for (i=1; i<=getN(); i++) {
    fprintf (file, "a %d %d %f\n", i, getClosest(i), getDistClosest(i));
  }
}


/*--------------------------------------------------
  | print solution to filename in DIMACS-like format
  *-------------------------------------------------*/

void PMSolution::printSolution (const char *filename, const char *name) {
  FILE *file = fopen (filename, "w");
  if (file==NULL) {
    fprintf (stderr, "WARNING: Could not open file %s for writing.\n", filename);
  } else {
    printSolution (file, name);
    fclose (file);
  }
}


void PMSolution::readSolution (const char *filename) {
  fprintf (stderr, "Reading solution... ");

  //auxiliary variables to help in reading
  const int LINESIZE = 256;
  int line = 0;
  char buffer [LINESIZE + 1];

  FILE *file = fopen (filename, "r");
  if (file == NULL) {
    fprintf (stderr, "Error reading file %s: could not open file.\n", filename);
    exit (-1);
  }
	
  reset();

  while (fgets (buffer, LINESIZE, file)) {
    line ++;
    int f;
		
    switch (buffer[0]) {
    case 'f': //line describing problem size
      if (sscanf (buffer, "f %d", &f)==1) {
	if (f<1 || f>m) {
	  fprintf (stderr, "Error reading file %s: facility label in line %d out of range (%d).\n", filename, line, f);
	} else {
	  add (f, false);
	}
      }
    default:
      continue;
    }
  }
  fclose (file);
	
  fprintf (stderr, "%d facilities inserted (inconsistencies:%d).\n", getOpenCount(), inconsistencies);
  fprintf (stderr, "Assigning users to facilities... ");
  ensureConsistency();
  fprintf (stderr, "done.\n");
  double sc = calcCost();
  fprintf (stderr, "Solution cost is %f.\n", sc);
}
