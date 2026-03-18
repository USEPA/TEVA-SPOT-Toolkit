#ifndef impact_instance_h
#define impact_instance_h

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "basics.h"
#include "distance.h"
#include "instance.h"

#include <map>
#include <vector>
#include <iostream>
#include <iomanip>

class PMImpactInstance : public PMInstance 
{
 private:

  double myOracleTime;
  PMDistanceOracle *myOracle;

  void reset(void);
  void fatal(const char *func, const char *msg);

 protected:

  // this is as-read information, built up incrementally
  // from the .impact file. you have to do this first, 
  // specifically with respect to the total number of 
  // scenarios, because you don't know this until they've
  // been read.
  std::map<int,std::map<int,double> > myDetectedImpacts; 
  std::map<int,double> myUndetectedImpacts;     

  #ifdef MAP_BASED_GETDIST

  // impacts, compiled for faster access. the 0th entries 
  // are un-used, as the scenario indicies are 1-based
  std::vector<std::map<int,double> > myDetectedImpactsFastLookup;
  std::vector<double> myUndetectedImpactsFastLookup;

  #else

  // impacts, compiled for constant-time access. the 0th
  // entries are un-used, as the scenario indicies are 1-based.
  // first index is customer/scenario, second index is
  // facility/detection location.
  std::vector<std::vector<double> > myImpacts;

  #endif

  int myN; //number of customers
  int myM; //number of potential facilities
  int myP; //number of facilities we are aiming for

  void resetDistances(void);

  void initOracle(void) 
    {
      myOracle=new PMDistanceOracle(this);
    }

  inline void checkFacility(int f) 
    {
      if((f<1) || (f>myM)) 
	{
	  fprintf (stderr, "ERROR: facility %d is out of range.\n", f);
	  exit(1);
	}
    }

  inline void checkUser(int u) 
    {
      if (u<1 || u>myM) 
	{
	  fprintf (stderr, "ERROR: user %d is out of range.\n", u);
	  exit(-1);
	}
    }

 public:

  PMImpactInstance(void);		
  PMImpactInstance (PMInstance *original, int *of, int *oc);
  virtual ~PMImpactInstance(void);		

  virtual double getOracleTime(void) 
    {
      return myOracle->getInitTime();
    }

  virtual int getMetric(void) 
    {
      return IMPACT;
    }

  virtual IntDouble *getCloser(int i, double v) 
    {
      return myOracle->getCloser(i,v);
    }

  void readImpact(FILE *file,int _p=0);

  void printMatrix (FILE *file);

  // TBD - this doesn't make any sense in the context of PMM-like applications
  virtual double getFacDist(int f, int g) 
    {
      std::cout << "***** WARNING - SHOULDN'T BE CALLING THIS!!" << std::endl;
      //    if(f>m || g>m) fatal ("getFacDist", "facility number out of range");
      //    return d[f][g]; //assumes facilities and users are the same thing
      return 0.0;
    }
		
  virtual double getDist(int c, int f) 
    {
      //    std::cout << "PMImpactInstance::getDist(" << c << "," << f << ")" << std::endl;

      //    if((c<0)||(c>myN)) 
      //      {
      //	fatal("getDist","customer number out of range");
      //      }
      //    if((f<0)||(f>myM))
      //      { 
      //	fatal("getDist","facility number out of range");
      //      }
      //

      if((c==0)||(f==0))
      	{
      	  return POPSTAR_INFINITY;
      	}

      #ifdef MAP_BASED_GETDIST

      const std::map<int,double> &thisImpactMap(myDetectedImpactsFastLookup[c]);
      std::map<int,double>::const_iterator impactIter(thisImpactMap.find(f));
      if(impactIter==thisImpactMap.end())
	{
	  // the scenario is undetectable - assign to worse-case impact
	  return myUndetectedImpactsFastLookup[c];
	}
      else
	{
	  return (*impactIter).second;
	}

      // UNREACHABLE

      #else

      return myImpacts[c][f];

      #endif
    }

  virtual double getFacCost(int f)
    {
      return 0.0;
    }

  virtual int getM(void)const
    {
      return myM;
    } 
  
  virtual int getN(void)const
    {
      return myN;
    }

  virtual int getP(void)const
    {
      return myP;
    }

  virtual void setP(int _p) 
    {
      myP=_p;
    }
};

#endif
