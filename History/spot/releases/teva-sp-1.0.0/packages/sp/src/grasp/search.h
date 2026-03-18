#ifndef SEARCH_H
#define SEARCH_H

#include "basics.h"
#include "solution.h"
#include "instance.h"
#include "bossa_random.h"
#include "elite.h"
#include "search_tables.h"

/*-----------------------
  |
  | Generic Search Class
  |
  *----------------------*/

class PMSearch {
 private: 
  static void fatal (const char *func, const char *msg);

  virtual double bestNeighbor (PMSolution *s, SearchTables *tab, int *rcand, int *icand, int &bestr, int &besti, bool first, bool *temp_array);
  virtual void   updateArrays (PMSolution *s, bool *affected, SearchTables *tab, bool add, double *stats_total_prefix_size = NULL);

 public:
  virtual double search (PMSolution *s, bool best_improvement, PMElite *elite, bool use_list, double *nz = NULL, double *profile=NULL);
  virtual void   getBounds (int &insert, int &remove) = 0; //number of candidates for insertion and deletion
  virtual void   getCandidates (int *rcand, int *icand) = 0;
  virtual bool   reportMove (int r, int i, double profit) = 0;
  virtual bool   firstImprovement () = 0;

  static void move (PMSolution *s, int in, int *out, double *w);
  static void fastInterchange (PMSolution *s, int *itcount = NULL);
  static void localSearch (PMSolution *s, bool best_improvement, bool uselist, int *itcount=NULL, bool stats=false, bool output_density=false, double *profile=NULL);
  static void update (PMSolution *s, int in, int out);
  static void runLocalSearch (PMSolution *s, char *lsmethod, bool output_density = false, double *profile = NULL);

};


/*----------------
  | Path-relinking
  *---------------*/

class PMSearchPathRelinking:public PMSearch {
 private:
  //int p;       //number of facilities
  int *insert; //list of candidates for insertion (insert[0] is the size)
  int *remove; //list of candidates for removal   (remove[0] is the size)
  PMInstance *inst;

  void fatal (const char *func, const char *msg);	
 public:
  virtual void getBounds (int &ins, int &rem) {ins = rem = inst->getM();} //number of candidates for insertion and deletion
  virtual bool firstImprovement () {return false;}
  void setSolutions (PMSolution *source, PMSolution *target);
  PMSearchPathRelinking (PMInstance *_inst);
  virtual void getCandidates (int *rcand, int *icand);
  virtual bool reportMove (int r, int i, double profit);		
  virtual ~PMSearchPathRelinking ();
};


/*--------------
  | local search
  *-------------*/

class PMSearchLocal:public PMSearch {
 private:

	
  PMInstance *inst;
  bool *solution;
		
  int p;
  int m;
  int it; //number of local search iterations performed
  void fatal (const char *func, const char *msg); 

 public:
  virtual bool firstImprovement () {return false;}
  virtual bool reportMove (int r, int i, double profit);

  virtual void getBounds (int &insert, int &remove) {
    if (inst->fixedP()) {insert = m - inst->getP(); remove = inst->getP();}
    else {insert = remove = m;}
  }

  virtual void getCandidates (int *rcand, int *icand); //get sorted lists of candidates
  inline int getIterations () {return it;}		
  PMSearchLocal (PMSolution *s);
  virtual ~PMSearchLocal ();
};


#endif

