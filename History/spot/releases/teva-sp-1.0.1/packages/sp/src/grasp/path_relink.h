#ifndef PATH_RELINK_H
#define PATH_RELINK_H

#include "solution.h"
#include "elite.h"

class PMRelink {
 private:
  static void fatal (const char *func, const char *msg);

 public:
  static double combinePR (PMSolution *t, PMSolution *s1, PMSolution *s2);
  //static double combineSub (PMSolution *t, PMSolution *s1, PMSolution *s2);	
  static double combineSubPR (PMSolution *t, PMSolution *s1, PMSolution *s2);
  static double combinePartial (PMSolution *t, PMSolution *s1, PMSolution *s2);
  static double combine_r (PMSolution *t, PMSolution *s1, PMSolution *s2, const char* method);
  static double combine (PMSolution *t, PMSolution *s1, PMSolution *s2, const char* method);
  //static double combine2 (PMSolution *t, PMSolution *s1, PMSolution *s2);
  static void pathRelinking (PMSolution *best_s, PMElite *elite, const char *method, bool stats = false);
};

#endif
