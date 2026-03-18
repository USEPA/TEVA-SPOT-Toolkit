#ifndef SOLUTION_H
#define SOLUTION_H

#include "instance.h"

class PMSolution {
 private:
  PMInstance *instance;
  int p; //number of facilities in the solution (may differ from instance)
  int n; //number of users
  int m; //number of potential facilities
  int *faclist;  //list of all facilities, first p are the solution
  int *facpos;   //facpos[i] = j  -> facility i is in faclist[j]		
  int *closest;  //closest[i] = j -> facility j (which must be in the solution) is the closest to i
  int *closest2; //closest[i] = j -> facility j is the second closest to i
  double *closestd; //distance to the closest
  double *closest2d; //distance to the second closest
  bool *consistent; 
  int inconsistencies;

  inline bool isConsistent() {return (inconsistencies==0);}

  void fatal (const char *func, const char *msg); //standard error message
  void swappos (int i, int j); //swap facilities in positions i and j

 public:	
  void printSolution (FILE *file, const char *name); //print solution in DIMACS-like format
  void printSolution (const char *filename, const char *name); //same thing

  //------------
  // basic data
  //------------
  inline PMInstance *getInstance() const {return instance;} //instance
  inline int getFacility (int i) const {return faclist[i];} //label of the i-th facility
  inline int getOpenCount() const {return p;} //number of facilities in the solution
  inline int getN() const {return n;} //number of cities
  inline int getM() const {return m;} //number of potential facilities
  inline int getPosition (int f) const {return facpos[f];} //position of the i'th facility
  inline bool contains(int i) const {return (facpos[i]<=p);}

  //-----------------------
  // closeness information
  //-----------------------
  inline int    getClosest      (int i) const {return closest[i];}
  inline int    getClosest2     (int i) const {return closest2[i];}
  inline double getDistClosest  (int i) const {return closestd[i];}
  inline double getDistClosest2 (int i) const {return closest2d[i];}
  inline void   setClosest      (int i, int f) {setClosest (i,f,instance->getDist(i,f));}		
  inline void   setClosest2     (int i, int f) {setClosest2(i,f,instance->getDist(i,f));}
  inline void   setClosest      (int i, int f, double v) {closest[i] = f; closestd[i] = v;}
  inline void   setClosest2     (int i, int f, double v) {closest2[i] = f; closest2d[i] = v;}
  void ensureConsistency (); //ensure solution is consistent
  void updateClosest (int i);

  //------------------------------
  // dealing with other solutions
  //------------------------------
  void copyFrom (PMSolution *s);          //copy the contents of an existing solution
  int computeDifference (PMSolution *s);  //symmetric diff. between this and another solution
  bool equals (PMSolution *s);            //check if this is identical to s

  //---------------------
  // solution management
  //---------------------
  void readSolution (const char *filename);  //read a solution from the input file
  PMSolution (PMInstance *inst);             //constructor: empty solution (no facilities)		
  double calcCost();	                       //compute solution cost
  void reset();                              //remove all facilities
  void add (int f, bool update);             //add f to the solution
  void swap (int rem, int ins, bool update); //swap facilities rem and int
  void remove (int f, bool update);          //remove f from solution
  ~PMSolution ();                            //destructor

  //------------------------------------------------
  // check if f is close enough so as to become the
  // closest or second closest --- if it is, update
  //------------------------------------------------
  inline void upgrade (int u, int f, double duf) {
    if (duf <= getDistClosest2(u)) {       //only interested if closer than 2nd closest
      double d1 = getDistClosest(u);
      if (duf <= d1) {                   //if also closer than first closest...
	setClosest2 (u,closest[u],d1); //old closest becomes 2nd closetst
	setClosest  (u, f, duf);       //f is the new closest
      } else {
	setClosest2 (u, f, duf);       //f becomes new second closest
      }
    }
  }

  //--------------------------------
  // Marks vertex i as inconsistent
  //--------------------------------
  inline void makeInconsistent(int i) {
    if (consistent[i]) {
      consistent[i]=false;
      inconsistencies++;
    }
  }

  //--------------------------------------------
  // marks vertex i as consistent --- call this 
  // only if you know what you're doing
  //--------------------------------------------
  void forceConsistent (int i) {
    if (!consistent[i]) {
      consistent[i] = true;
      inconsistencies--;
    }
  }
};

#endif
