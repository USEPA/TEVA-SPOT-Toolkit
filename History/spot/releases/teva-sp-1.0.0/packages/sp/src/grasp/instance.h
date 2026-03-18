/*----------------------------------------------------------------
  |
  | class PMInstance: 
  |   represents the distance matrix for the problem and the list
  |   of potential facilities. Instances can be read from an input
  |   file.                   
  |
  | author: Renato Werneck (rwerneck@princeton.edu)
  | log: 
  |      May 29, 2002: file created
  | 
  *-----------------------------------------------------------------*/

#ifndef instance_h
#define instance_h

//#include <stdlib.h>
#include <stdio.h>
//#include <math.h>
#include "basics.h"

class PMInstance {
 protected:
  enum {NOMETRIC, MATRIX, EUCLIDEAN, GEO, HYBRID, COVER,IMPACT}; 
  virtual int getMetric() = 0;

 public:
  PMInstance() {};		
  //PMInstance(PMInstance *original, int *of, int *oc) = 0;
  virtual ~PMInstance() {};
  virtual IntDouble *getCloser (int i, double v) = 0;
  virtual void setP (int _p) = 0; //number of facilities to open
  virtual double getDist (int u, int f) = 0;
  virtual int getM() = 0; 
  virtual int getN() = 0;
  virtual int getP() = 0;
  virtual double getFloydTime() {return 0.0;}
  virtual double getOracleTime() =  0;
  virtual double getFacDist (int f, int g) = 0;
  virtual double getFacCost (int f) = 0; //{return 0;}
  virtual bool fixedP () {return true;} //pmedian-> true ; facloc->false
  virtual void outputParameters(FILE *file) {
    fprintf (file, "users %d\n", getN());
    fprintf (file, "facilities %d\n", getM());
    fprintf (file, "centers %d\n", getP());
    fprintf (file, "rcenters %0.4f\n", (double)getP()/(double)getM());
    fprintf (file, "pcenters %0.4f\n", 100.0 * (double)getP()/(double)getM());
  }

};

#endif
