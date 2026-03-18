/*------------------------
  |
  | class PMGraphInstance
  | 
  *-----------------------*/

#ifndef graph_instance_h
#define graph_instance_h

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "matrix_instance.h"
#include "graph.h"

class PMGraphInstance:public PMMatrixInstance {
 private:
  double floyd_time;  //time spent running floyd's algorithm
  virtual void fatal (const char *func, const char *msg);
  void floyd();
  PMGraph *graph;

 public:
  PMGraphInstance(void);		
  PMGraphInstance(PMInstance *original, int *of, int *oc);
  ~PMGraphInstance();		
  void readDimacs (FILE *file, int _p=0);
  virtual double getFloydTime() {return floyd_time;}
};

#endif

