#include "solution.h"

#include <stdlib.h>

#include <iostream>
#include <fstream>
#include <iomanip>

void PMSolution::fatal(const char *func, const char *msg)const
{
  std::cerr << "PMSolution::" << func << ": " << msg << std::endl;
  exit (-1);
}

void PMSolution::swappos(int i, int j) 
{
  int prev_i(myFacilityList[i]);
  int prev_j(myFacilityList[j]);

  myFacilityList[i]=prev_j;
  myFacilityList[j]=prev_i;
  myFacilityPosition[myFacilityList[i]]=i;
  myFacilityPosition[myFacilityList[j]]=j;
}

PMSolution::PMSolution(PMInstance *inst):
  myInstance(inst),
  myN(myInstance->getN()),
  myM(myInstance->getM())
{
  myFacilityList     =new int[myM+1];
  myFacilityPosition =new int[myM+1];

  for(int i=1;i<=myM;i++) 
    {
      myFacilityList[i]=i;
      myFacilityPosition[i]=i;
    }

  myClosestFacility                  =new int[myN+1];
  mySecondClosestFacility            =new int[myN+1];
  myDistanceToClosestFacility        =new double[myN+1];
  myDistanceToSecondClosestFacility  =new double[myN+1];

  myConsistent=new bool[myN+1];

  reset(); //empty solution, everybody consistent	
}

void PMSolution::copyFrom(PMSolution &s)
{
  this->reset();        

  s.ensureConsistency(); // make sure closest and second closest are correctly defined

  if(s.getInstance()!=myInstance)
    { 
      fatal("copyFrom", "can't solution for different instance");
    }

  int k(s.getP());
  for(int i=1;i<=k;i++)
    {
      add(s.getFacility(i),false);
    }

  for(int i=1;i<=myN;i++) 
    {
      setClosest(i,s.getClosest(i));
      setClosest2(i,s.getClosest2(i));
      myConsistent[i] = true;
    }
  myInconsistencies = 0;
}

PMSolution::~PMSolution(void) 
{
  delete [] myClosestFacility;
  delete [] mySecondClosestFacility;
  delete [] myDistanceToClosestFacility;
  delete [] myDistanceToSecondClosestFacility;
  delete [] myFacilityList;
  delete [] myFacilityPosition;
  delete [] myConsistent;
}


void PMSolution::readSolution (const char *fileName) 
{
  std::cout << "Reading solution..." << std::endl;

  //auxiliary variables to help in reading
  const int LINESIZE = 256;
  int line = 0;
  char buffer [LINESIZE + 1];

  FILE *file = fopen (fileName, "r");
  if (file == 0) 
    {
      std::cerr << "Error opening input file=" << fileName << std::endl;
      exit (-1);
    }
	
  reset();

  while(fgets (buffer, LINESIZE, file)) 
    {
    line ++;
    int f;
		
    switch (buffer[0]) {
    case 'f': //line describing problem size
      if (sscanf (buffer, "f %d", &f)==1) 
	{
	if (f<1 || f>myM) 
	  {
	    std::cerr << "Error reading file " << fileName << ": facility label in line " << line << " is out of range (" << f << ")." << std::endl;
	  } 
	else 
	  {
	    add(f, false);
	  }
      }
    default:
      continue;
    }
  }
  fclose (file);
	
  std::cout << getP() << " facilities inserted (inconsistencies:" << myInconsistencies << std::endl;
  std::cout << "Assigning customers to facilities..." << std::endl;
  ensureConsistency();
  std::cout << "Done." << std::endl;
  double sc=cost();
  std::cout << "Solution cost is " << sc << std::endl;
}
		
void PMSolution::printSolution(std::ostream &ostr, const char *name)const
{
  if(name!=0)
    {
      ostr << "i " << name << std::endl;
    }

  ostr << "n " << getN() << std::endl;
  ostr << "p " << getP() << std::endl;
  ostr << "v " << cost() << std::endl;

  for(int i=1;i<=getM();i++) 
    {
      if(contains(i)==true)
	{ 
	  ostr << "f " << i << std::endl;
	}
    }
  for(int i=1;i<=getN();i++) 
    {
      ostr << "a " << i << " " << getClosest(i) << " " << getDistClosest(i) << std::endl;
    }
}

void PMSolution::printSolution(const char *fileName, const char *name)const
{
  std::ofstream outputFile(fileName);
  if(!outputFile)
    {
      std::cerr << "WARNING: Failed to open output file=" << fileName << std::endl;
      exit(-11);
    } 

  printSolution(outputFile,name);

  outputFile.close();
}

/*---------------------------------------------------------------------------
  | Compute the symmetric difference between this solution and s. Defined as
  | the total number of facilities that belong to one solution, but not both.
  *--------------------------------------------------------------------------*/

int PMSolution::computeDifference (PMSolution *s) {
  int i;
  int diff = 0;

  //there but not here
  for (i=s->getP(); i>0; i--) {if (!contains(s->getFacility(i))) diff++;}
	
  //here but not there
  for (i=getP(); i>0; i--) {if (!s->contains(getFacility(i))) diff++;}

  return diff;
}


/*--------------------------------------------
  | equals: checks if this solution is equal 
  | to s (i.e., has the exact same facilities)
  *-------------------------------------------*/

bool PMSolution::equals (PMSolution *s) {
  int k=s->getP();             //number of facilities in s
  if (k!=getP()) return false; //different number of facilities
	
  for (int i=1; i<=k; i++)              //every facility in s
    if (!contains(s->getFacility(i))) //must also be in this solution
      return false;                 //or else the solutions are different

  return true; //couldn't find anything wrong, the solutions are the same
}
		
double PMSolution::cost(void) const
{
  // TBD - why not a cost of 0?
  if(myP==0) //nobody served
    {
      return POPSTAR_INFINITY; 
    }

  if(isConsistent()==false)
    {
      fatal ("cost", "function cannot be called for inconsistent solution");
    }

  double result(0.0);

  for(int i=myN; i>0; i--) // cost of serving all users
    {
      result+=getDistClosest(i);
    } 
  for(int i=myP; i>0; i--) // cost of opening all facilities
    {
      result+=myInstance->getFacCost(myFacilityList[i]);
    } 

  return result;
}	

void PMSolution::reset(void) 
{
  myInconsistencies=0;
  for(int i=1;i<=myN;i++)
    {
      setClosest(i,0,POPSTAR_INFINITY);
      setClosest2(i,0,POPSTAR_INFINITY);
      myConsistent[i]=true;
    }
  myP=0;
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

void PMSolution::add(int f, bool update) 
{
  if(contains(f)==true) 
    {
      // nothing to do
      return; 
    }

  swappos(myFacilityPosition[f],++myP); // increase solution size, and put f in the last (p-th) position
			
  if(update==true) 
    {
      for(int i=1;i<=myN;i++)
	{
	  if(myConsistent[i]==true) 
	    {
	      double d(myInstance->getDist(i,f));
	      upgrade(i,f,d); // inconsistent remain inconsistent for now
	    }
	}
    } 
  else 
    {
      if(myInconsistencies!=myN) 
	{
	  for(int i=myN; i>0; i--)
	    { 
	      makeInconsistent(i);
	    }

	  if(myInconsistencies!=myN) 
	    {
	      fatal("add","all users should have become inconsistent");
	    }
	}
    }
}

/*---------------------------------------------------
  | swap facilities: rem must belong to the solution,
  | ins must not. Either can be zero (meaning that
  | just the other action will be performed).
  *--------------------------------------------------*/
		
void PMSolution::swap(int rem, int ins, bool update) 
{
  if (rem == ins) return;
  if (rem == 0) {add (ins, update); return;}
  if (ins == 0) {remove(rem, update); return;}

  if (!contains(rem) || contains(ins)) fatal ("swap", "invalid move");

  swappos (myFacilityPosition[rem], myFacilityPosition[ins]);

  if ((!update) && (myInconsistencies==myN)) return; //WARNING: IS THIS REALLY THE TEST I WANT TO MAKE?

  for(int i=myN; i>0; i--) {
    if (update) {
      double d = myInstance->getDist(i,ins);
      upgrade (i, ins, d); //make ins the closest or second closest, if necessary
      if (myClosestFacility[i]==rem || mySecondClosestFacility[i]==rem) {
	makeInconsistent(i);
	updateClosest(i);
      }
    } else {makeInconsistent(i);}
  }
}

		
/*-------------------------------------
  | remove facility f from the solution
  *------------------------------------*/
		
void PMSolution::remove(int f, bool update) 
{
  if(contains(f)==false) 
    {
      // nothing to do
      return; 
    }

  swappos(myFacilityPosition[f], myP); // place the p-th element where i was
  myP--; // decrease solution size
  if((update==false)&&(myInconsistencies==myN)) 
    {
      // if everybody inconsistent and there's no need to update, go away
      return; 
    }

  for(int i=1;i<=myN;i++) 
    {
      if(update==true) 
	{
	  if((myClosestFacility[i]==f)||(mySecondClosestFacility[i]==f)) 
	    {
	      makeInconsistent(i);
	      updateClosest(i);
	    }
	} 
      else
	{ 
	  makeInconsistent(i);
	}
    }		
}			

/*-----------------------------------------------------------
  | ensures that a solution is consistent; runs in O(n + i*m) 
  | time, where i is the number of inconsistent elements
  *----------------------------------------------------------*/

void PMSolution::ensureConsistency(void)
{
  if (myInconsistencies == 0) return;
  for (int i=myN; i>0; i--) updateClosest(i); //only the inconsistent ones will be udpated
  if (myInconsistencies!=0) fatal ("ensureConsistency", "all nodes should be consistent");
}

/*-----------------------------
  | update the closest/closest2
  *----------------------------*/

void PMSolution::updateClosest(int i) 
{
  if(myConsistent[i])
    {
      // nothing to do
      return; 
    }

  setClosest(i,0,POPSTAR_INFINITY);
  setClosest2(i,0,POPSTAR_INFINITY);

  for(int j=1;j<=myP;j++)
    {
      int f(this->getFacility(j));
      double diff(myInstance->getDist(i,f)); // distance from i to f
      upgrade(i,f,diff);
    }

  myConsistent[i]=true;
  myInconsistencies --;
}

