/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file branching.h
 * \author Jonathan Eckstein
 *
 * Defines pebbl::branching and related classes.
 */

#ifndef pebbl_branching_h
#define pebbl_branching_h

#include <acro_config.h>
#include <utilib/seconds.h>
#include <utilib/LinkedList.h> 
#include <utilib/GenericHeap.h>
#include <utilib/exception_mngr.h>
#include <utilib/memdebug.h>
#include <utilib/ParameterList.h>
#include <pebbl/memUtil.h>
#include <pebbl/fundamentals.h>
#include <pebbl/pebblBase.h>
#include <pebbl/pebblParams.h>
#include <pebbl/loadObject.h>

extern "C" void pebbl_abort_handler(int code);

namespace pebbl {

using utilib::Comparison;
using utilib::ComparisonBase;
using utilib::LinkedList;
using utilib::ListItem;
using utilib::GenericHeapCompare;
using utilib::GenericHeapItem;
using utilib::GenericHeap;
using utilib::uMPI;

class branchSub;
class branching;
class spHandler;


///
template <class Type>
struct DynamicSPCompare : public ComparisonBase<Type>, public Comparison
{
public:

  ///
  compare_type operator()(const Type& _arg1, const Type& _arg2) const
  {
    int ans = _arg1.dynamicSPCompare(_arg2);
    if (ans < 0) return better;
    if (ans > 0) return worse;
    return equal;
  }
};


///
///  We define a general pool class that can hold either subproblems
///  or tokens (for the parallel code).
///
enum poolType {heapPoolType, stackPoolType, queuePoolType};


///
template <class SUB, class LOAD> class branchPool : public pebblBase
{
public:
 
  virtual ~branchPool() {}

  virtual void reset() = 0;

  branching* global() { return globalPtr; };

  void setGlobal(branching* master) { globalPtr = master; };

  virtual int size() = 0;

  virtual int insert(SUB* p) = 0;

  virtual SUB* select() = 0;

  virtual SUB* remove(SUB* p) = 0;
  virtual SUB* remove() { return remove(select()); };
  virtual int kill(SUB* p);

  virtual void clear();

  virtual int  prune() = 0;
  virtual void pretendPrunedOnce() { };

  virtual void resetScan() = 0;
  virtual SUB* scan() = 0;

  virtual SUB* firstToUnload() = 0;
  virtual SUB* nextToUnload()  = 0;

  LOAD& load() { return myLoad; };
  LOAD& updatedLoad()
    {
      myLoad.update();
      if (knowsGlobalBound())
	{
	  myLoad.setBound(globalBound());
	  myLoad.setBoundUnknown(false);
	}
      else
	myLoad.setBoundUnknown();
      return myLoad;
    }

  virtual double loadMeasure() 
    {
      return myLoad.loadMeasure();
    };

  virtual bool    knowsGlobalBound() { return false; };
  virtual double globalBound()
    {
      EXCEPTION_MNGR(std::runtime_error, "globalBound() called, but none available.");
      return 0;
    }

  virtual void myPrint()=0;

 protected:

  LOAD myLoad;

 private:

  branching*  globalPtr;

};

//-----------------------------------------------------------------------
//
//  Standard pool implementations for depth and
//  breadth-first. 
//

template <class SUB, class LOAD> 
class doublyLinkedPool : public branchPool <SUB,LOAD> 
{
 public:

  int size() { return list.size(); }; 

  SUB* select() { return list.top(); };

  int insert(SUB* p)  
    {
      ListItem<SUB*> *item = list.add(p); 
      p->poolPtr = item; 
      this->myLoad += *p;
      return size();
    };

  SUB* remove(SUB* p) 
    {    
      return removeListItem( (ListItem<SUB*> *)p->poolPtr );
    };

  SUB* remove() { return remove(select()); };
  
  int prune()
    {
      ListItem<SUB*> *l1,*l2;  
        
      l1 = list.head();
      while (l1)
	{  
	  l2 = list.next(l1);
	  if (l1->data()->canFathom())
	    {
	      removeListItem(l1)->recycle();
	    }
	  l1=l2;
	}
      return size();
    };

  void resetScan() { scanCursor = list.head(); };

  SUB* scan()
    {
      SUB* toReturn = scanCursor->data();
      scanCursor = list.next(scanCursor);
      return toReturn;
    }

  SUB* firstToUnload() 
    {
      SUB* toReturn = 0;   
      
      unloadCursor = list.head();
      if (list.next(unloadCursor))
	unloadCursor = list.next(unloadCursor);
      if (unloadCursor)
	toReturn = unloadCursor->data();
      
      advanceUnloadCursor();
      advanceUnloadCursor();
      return toReturn;
    };
  
  SUB* nextToUnload() 
    {
      if (unloadCursor == 0)
	return firstToUnload();
      SUB *toReturn = unloadCursor->data();

      advanceUnloadCursor();
      advanceUnloadCursor();
      return toReturn;
    };
  
  doublyLinkedPool(bool stack_or_queue) 
    { 
      if (stack_or_queue) 
	list.stack_mode(); 
      else 
	list.queue_mode();
    };

  // Nothing special to be done here.  Mode should be set from 
  // first construction.
  void reset() { this->clear(); };

  ~doublyLinkedPool() { this->clear(); };  

  virtual void myPrint()
    {
      std::cout<<"\n======doublyLinkedPool=========\n";
      ListItem<SUB*>* l = list.head();
      while(l) 
	{
	  std::cout<<l->data()<<" | ";
	  l = list.next(l);
	}
      std::cout<<std::endl<<std::endl;
    };

 protected:

  LinkedList< SUB* > list; 

  ListItem< SUB* > *scanCursor;

  ListItem< SUB* > *unloadCursor; 

  SUB* removeListItem(ListItem<SUB*> *item)
    {
      SUB *p;

      list.remove(item,p);
      this->myLoad -= *p;
      return p;
    };

  void advanceUnloadCursor() 
    {
      if (unloadCursor)       
	{
	  unloadCursor = list.next(unloadCursor);
	}
      else
	unloadCursor = 0;
    };

};

//--------------------------------------------------------------------------
//
//  A best-first pool implemented via a heap.
//

template <class SUB, class LOAD, class COMPARE=GenericHeapCompare<SUB> > 
class heapPool : public branchPool <SUB,LOAD> 
{
 public:

 int size() { return heap.size(); };
    
 virtual int insert(SUB* p) 
 {
   GenericHeapItem<SUB> *item = heap.add(*p);
   p->poolPtr = item;      
   this->myLoad += *p;
   return 1;// it can return anything
 };
  
 virtual SUB* select () { return  &(heap.top()->key()); };
   
 SUB* remove(SUB* p) 
 { 
   return removeHeapItem( (GenericHeapItem<SUB> *)p->poolPtr );
 };

 SUB* remove() { return remove(select()); };
  
 virtual void clear() 
 {
   for(int e = heap.size(); e > 0; e--)
     removeHeapItem(heap.member(e))->recycle();
 };
    
 virtual int prune() 
 {
   GenericHeapItem<SUB> *item;
   int n = heap.size();

   if (this->global()->initialDive && firstPrune) 
     { 
       heap.reheapify();
       firstPrune = false;
     }

  
   while( n > (heap.size()>>1) )
     {
       item = heap.member(n);
       if ( item->key().canFathom() )
	 {
	   removeHeapItem(item)->recycle(); 
	   if ( n > heap.size() ) n = heap.size();
	 }
       else
	 n--;
     }

   return heap.size();
 };

 virtual void pretendPrunedOnce()
 {
   firstPrune = false;
 };
  
 heapPool() :
 unloadCursor(0), 
 firstPrune(true) 
 { };

 void reset()
 {
   clear();
   unloadCursor = 0;
   firstPrune = true;
   scanCursor = 1;
 }
  
 virtual ~heapPool() { clear(); };
  
  bool knowsGlobalBound() 
     { 
       // Though the globalBound() method tries to estimate the bound when the pool
       // is empty, it really doesn't know it.  Other methods should then know to use
       // the bound in the current subproblem (not in the pool).
       return (!(this->global()->initialDive && firstPrune) && size() > 0);
     };

 double globalBound()    
 { 
   if (size() > 0)
     return select()->bound;
   else
     return this->global()->sense*MAXDOUBLE;
 };

 void resetScan() { scanCursor = 1; };
  
 SUB* scan() { return &(heap.member(scanCursor++)->key()); };

 SUB* firstToUnload()
    {
      unloadCursor = std::min(heap.size(),2); 
      return nextToUnload();
    };
  
  SUB* nextToUnload()
    {
      if (size() == 0) return 0;
      if (unloadCursor <= heap.size()) 
	return &(heap.member(unloadCursor++)->key());
      return firstToUnload();
    };

  virtual void myPrint()
    {
      std::cout<<"\n=======heapPool=====================\n";
      SUB* he;
      for(int i=1;i<=heap.size();i++) 
	{
	  he = (& (heap.member(i)->key() ) );
	  std::cout<<he<<" || ";
	}
      std::cout<<std::endl<<std::endl;

    };
  
 protected:

  GenericHeap<SUB,COMPARE> heap;
  int unloadCursor; 
  int scanCursor;
  bool firstPrune;

  SUB* removeHeapItem(GenericHeapItem<SUB> *item)
    {
      bool status;
      SUB *p;
      
      p = &( item->key() );
      heap.remove(item,status);
      if (!status) 
	EXCEPTION_MNGR(std::runtime_error,"The item was not found in the heap");
      this->myLoad -= *p;
      return p;
    };
 
};


//
//  Class to identify subproblems.
//  Includes a creating processor field for parallel appliations
//  (Eventually could make that conditiionally compiled.)
//

// CAP: This was derived from pebblBase, but it doesn't seem to need
// anything in pebblBase and it's cluttering output in gdb.
// I'm removing for now.
// class branchSubId : public pebblBase

class branchSubId
{

public:

  int serial;
  int creatingProcessor;

  // Check if two id's are the same

  inline int operator ==(branchSubId& other)
  {
    return 
      (serial == other.serial) && 
      (creatingProcessor == other.creatingProcessor);
  };

  // Copy operator

  inline branchSubId& operator= (const branchSubId& other)
  {
    serial = other.serial;
    creatingProcessor = other.creatingProcessor;
    return(*this);
  }

  // Null constructor

  branchSubId() :
    serial(-1),
#ifdef ACRO_HAVE_MPI
    creatingProcessor(uMPI::running() ? uMPI::rank : 0)
#else
    creatingProcessor(0)
#endif
  { };

  // Copy constructor was replaced by a copy operator

  // Initializer for a new subproblem

  void branchSubIdFromBranching(branching* global);

  // This can be used to make an ID "empty", and check if its empty.

  enum { emptyCode = -10 };

  inline void setEmpty() 
  {
    serial = emptyCode;
  };

  inline bool isEmpty()
  {
    return serial == emptyCode;
  }

};


//
//  Base class for each branching structure. 
//

class branching : public pebblBase, public pebblParams
{
friend class branchSub;
friend class branchSubId;

public:

  double      initialGuessValue;
  optimType   sense;
  double      incumbentValue;

  ///
  /// This generalizes the notion of the value needed to fathom
  /// subproblems, which is needed for enumeration
  ///
  virtual double      fathomValue() {return incumbentValue;}

  branchPool<branchSub,loadObject>* pool;

  branchSub*  currentSP;
  branchSubId currentSPId;
  branchSubId previousSPId;

  bool needPruning;

  int probCounter;
#ifndef COUGAR
  int subCount[numStates];
#else
  // Get weird subscript range errors on Janus unless we add lots of
  // padding to this array.  (JE)
  int subCount[numStates + 14];
#endif

  bool incHeuristicExists;

  double searchTime;

  double nextOutputTime;
  double lastSolValOutput;

  std::string problemName;

  branching() : 
    sense(minimization),    // Default is now minimization unless specified...
    pool(0), 
    performing_enumeration(false),
    parameters_registered(false),
    min_num_required_args(0)
    {
      version_info = "PEBBL 1.0";
    }

  double absGap(double boundValue) 
    { return (incumbentValue - boundValue)*sense; };
  double relGap(double boundValue);
  virtual bool canFathom(double boundValue);

  // These are no longer used -- setupProblem does the work now
  //   virtual void readIn(int argc,char** argv) {}
  //   virtual void inputFile(int argc,char** argv, char*& filename);
  //   virtual void inputFile(int argc,char** argv, char*& filename1,
  // 				char*& filename2);
  //   virtual std::istream* inputStream(int argc,char** argv);

  virtual void setName(const char* cname);

  virtual void preprocess() { };

  virtual double aPrioriBound() { return -sense*MAXDOUBLE; };

  virtual double initialGuess() { return initialGuessValue; };

  virtual branchSub* blankSub() = 0;
  virtual branchSub* makeRoot();

  virtual void signalIncumbent() { needPruning = true; };

  // Do any operations that all go along with the acquisition 
  // of a new incumbent value, whether generated locally or 
  // (in the parallel case) received from another processor

  virtual void newIncumbentEffect(double new_value) {};

  virtual bool haveIncumbentHeuristic() { return false; };

  bool haveIncumbent() {return (incumbentValue != sense * MAXDOUBLE);};

  double searchFramework(spHandler *handler = NULL);
  virtual double search() { return searchFramework(NULL); };

  bool shouldAbort(int boundCount);
  const char* abortReason;

  inline int haveCurrentSP()
  {
    return currentSP != NULL;
  };
  
  virtual void loadCurrentSP(branchSub* p);
  virtual void unloadCurrentSP();
  virtual void eraseCurrentSP();

  virtual void unloadCurrentSPtoPool()
    {
      pool->insert(currentSP);
      unloadCurrentSP();
    };

  virtual std::ostream* openSolutionFile();
  virtual void closeSolutionFile(std::ostream* fileStream);

  void solutionToFile();

  virtual void serialPrintSolution(const char* header = "",
				   const char* footer = "",
				   std::ostream& outStream = std::cout);

  virtual void printSolution(const char* header = "",
			     const char* footer = "",
			     std::ostream& outStream = std::cout)  
    {
      serialPrintSolution(header,footer,outStream);
    };

  virtual void serialPrintSolValue(std::ostream& stream = std::cout);
  virtual void printSolValue(std::ostream& stream = std::cout)
    {
      serialPrintSolValue(stream);
    }

  virtual void printSPStatistics(std::ostream& stream = std::cout);
  virtual void printTimings(std::ostream& stream = std::cout);
  virtual void printAllStatistics(std::ostream& stream = std::cout);

  void printAbortStatistics(loadObject& load);

  virtual ~branching();  // Note: default action is to delete the pool.

  /// Initialize base classes and reset the state of the solver
  virtual void reset(bool resetVB = true);

  /// Setup the solver parameters using command-line information.
  /// This returns false if there is a problem in the setup, and true
  /// if the setup appeared to work normally.
  virtual bool setup(int& argc, char**& argv);

  ///
  virtual void solve();

  ///
  static std::string version_info;

 protected:

  double gapDenom(double boundValue)
    { return std::max(std::fabs(boundValue),std::fabs(incumbentValue)); };

  virtual void statusPrint(int&        lastPrint, 
			   double&     lastPrintTime,
			   const char* tag="");
  virtual void statusPrint(int&        lastPrint,
			   double&     lastPrintTime,
			   loadObject& l, 
			   const char* tag="");
  virtual void statusLine(loadObject& l, 
			  const char* tag="");

  int statusLinePrecision;

  virtual loadObject load();
  virtual loadObject updatedLoad();

  void branchingInit(optimType   direction = minimization,
		    double      relTolSet = -1.0,
		    double      absTolSet = -1.0,
		    double      bluff = -1,
		    bool bluffReal = false); 

  void branchingInitGuts();

  void directSolutionToFile();
  int  serialNeedEarlyOutput();
  void recordEarlyOutput(double outputVal);

  void printSPTable(std::ostream& stream,int* table,int nameWidth);
  void printSPLine(std::ostream&    stream,
		   int         n,
		   const char* name,
		   int         denominator,
		   int         nameWidth,
		   int         numWidth);

  virtual bool     valLogOutput() { return validateLog;  };
  virtual std::ostream* valLogFile();
  void             valLogInit();
  void             valLogFathomPrint();

  int lastHeurPrint;

  double startTime;
  double startWall;

  bool performing_enumeration;
  virtual unsigned int enumeration_count() { return 0; }
   
  ///
  virtual void write_usage_info(char* progName,std::ostream& os) const;

  ///
  virtual void writeCommandUsage(char* progName,std::ostream& os) const;

  ///
  utilib::ParameterList plist;

  ///
  bool parameters_registered;

  ///
  std::string solver_name;

  //  Looks like Bill or Cindy added this -- redundant?
  //  double start_time;

  unsigned int min_num_required_args;

  ///
  virtual bool processParameters(int& argc, 
				 char**& argv, 
				 unsigned int min_num_required_args__=0);

  /// Register the parameters into a ParameterList object
  virtual void register_parameters()
        {
        plist.register_parameters(*this);
        }

  /// Check parameters for setup problems and perform debugging I/O
  virtual bool checkParameters(char* progName = "");

  /// Sets up the problem from command line arguments.  Should return
  /// true if the setup worked, and false if there was a problem.  
  /// The default implementation is just to return true -- in that case, 
  /// the instance must get set up in some internal way.

  virtual bool setupProblem(int& argc, char**& argv) { return true; }

};



  

//
//  The following class is used as a common base class by subproblems
//  and tokens (in the parallel code).
//

class coreSPInfo : public pebblBase
{
 public:

  int dynamicSPCompare(const coreSPInfo& sp2) const;
  int compare(const coreSPInfo& sp2) const;

  void write(std::ostream& s) const {}
  void read(std::istream& s){}

  branchSubId id;
  
  double   bound;
  double   integralityMeasure;
  subState state;
  int      depth;

  void*    poolPtr;

  virtual branching* bGlobal() const = 0;

  virtual bool canFathom();

  coreSPInfo() : 
    integralityMeasure(MAXDOUBLE)
  { }

  void CSPInitfromBranching(branching *global);
  void CPSInitAsCopy(coreSPInfo& toCopy);

  virtual double boundEstimate() const { return bound; }

  virtual int loadXFactor() const { return 1; }

  virtual void recycle() { delete this; }

  virtual ~coreSPInfo() { }

  // To assist in debug printouts

  virtual void debugPrint(std::ostream& s) const;
  
};



//
//  Root class for all subproblems.         
//


class branchSub : public coreSPInfo
{
friend class branching;

public:
  
  branching* bGlobal() const = 0;
  
  REFER_DEBUG(bGlobal())

  int childrenLeft;
  int totalChildren;
  
  void* poolPtr;             // For use by pool.
    
  double relGap()         { return bGlobal()->relGap(bound);  }
  double absGap()         { return bGlobal()->absGap(bound);  }
  double incumbentValue() { return bGlobal()->incumbentValue; }
  
  virtual void setRoot();
  virtual void setRootComputation() = 0;

  virtual bool  forceStayCurrent()      { return false; };
  virtual void makeCurrentEffect()     { };
  virtual void noLongerCurrentEffect() { };
  
  virtual void boundComputation(double* controlParam) = 0;
  virtual void computeBound(double* controlParam);
  virtual void computeBound();
  
  virtual int splitProblem();
  virtual int splitComputation() = 0;
  
  virtual branchSub* child(int whichChild = anyChild);
  virtual branchSub* makeChild(int whichChild = anyChild) = 0;
  virtual int chooseChild(int desired)
    {
      if ((desired >= 0) && (desired < totalChildren))
	return desired;
      if (desired != anyChild)
	EXCEPTION_MNGR(std::runtime_error, "Request for child " << desired << " out of range");
      return totalChildren - childrenLeft;
    };
  
  virtual bool candidateSolution() = 0;
  
  virtual void incumbentHeuristic() { };

#if 0
  /**
   * Update the incumbent value with an explicitly provided value
   */
  virtual void updateIncumbent(double value)
    {
      // Derived classes that support enumeration will have to
      // override this and check that the new value is in fact
      // better before updating the incumbent value.
      bGlobal()->incumbentValue = value;
      bGlobal()->signalIncumbent();
      bGlobal()->newIncumbentEffect(value);
    }
#endif

  /**
   * Update the incumbent value
   * Note that this method tacitly assumes that the value of the incumbent
   * is the bound of the subproblem.
   */
  virtual void updateIncumbent()
    {
      bGlobal()->incumbentValue = bound;
      bGlobal()->signalIncumbent();
      bGlobal()->newIncumbentEffect(bound);
    }

  int loadXFactor() const
    {
      if (state == separated)
	return childrenLeft;
      else
	return state != dead;
    };


  // Constructors:
  // Moving these to regular functions to help (I hope) with debugging.
  // We can move back later for inlining
  branchSub();
  
  void branchSubAsChildOf(branchSub* parent);
  
  void setState(subState newState,branching* globalP)
    {
      if (state != newState) 
	{
	  state = newState;
	  (globalP->subCount[state])++;
	}
    }

  void setState(subState newState)
    {
      setState(newState,bGlobal());
    };

  void recycle();

  // Memory tracking

#ifdef MEMORY_TRACKING
  virtual void printMemDetails() { std::cout << this << ' '; };
#endif

  // Validation log stuff...

  int valLogOutput() { return bGlobal()->valLogOutput(); };

  virtual int valLogProc() { return 0; };

  virtual void valLogWriteID(char separator = 0);
  virtual void valLogWriteBound(char separator = 0);

  virtual void valLogCreatePrint(branchSub* parent);
  virtual void valLogBoundPrint();
  virtual void valLogSplitPrint();
  virtual void valLogDestroyPrint();

  // Application-specific validation log stuff -- null by default

  virtual void valLogCreateExtra(branchSub* parent) { };
  virtual void valLogBoundExtra()                   { };
  virtual void valLogSplitExtra()                   { };
  virtual void valLogDestroyExtra()                 { };

  // This has to be moved back to private after we fix the constructor-
  // calling problem

  // If this changes, it will affect packGenericChild, which constructs a
  // packed child.

  void branchSubInit(branching* master,
		    subState initialState = boundable) 
    {
      totalChildren = childrenLeft = 0;
      state = initialState;
      (master->subCount[state])++;
      poolPtr = 0;
    };


private:

};


//  Some stuff related to memory tracking

#ifdef MEMORY_TRACKING
#define MEMORY_TRACK_SP(sp,s) \
    MEMORY_IF(sp->id.serial) {sp->printMemDetails(); memUtil::trackPrint(s);}
#else
#define MEMORY_TRACK_SP(sp,s)  // Nothing
#endif


// A base class for all subproblem handlers.

class spHandler : public pebblBase
{
 public:

  REFER_DEBUG(global)

  virtual ~spHandler() { };

  virtual void setGlobal(branching* global_) { global = global_; };

  virtual void execute() = 0;

 protected:

  branching* global;   // Pointer back to branching class
  branchSub* p;        // Subproblem currently being operated on
  branchSub* c;        // A child of that problem

  virtual void setProblem()    { p = global->currentSP;    };
  subState state()             { return p->state;          };
  virtual void erase()         { global->eraseCurrentSP(); };
  virtual void computeBound()  { p->computeBound();        };
  virtual bool canFathom()     { return p->canFathom();    };   
  virtual void heuristic();    // Defined in cpp file
  virtual void split()         { p->splitProblem();        };
  virtual bool shouldRelease() { return false;             };
  virtual void releaseChild(); // Returns error, defined in cpp file
  virtual void getChild()      { c = p->child();           };
  virtual void insertChild()   { global->pool->insert(c); };
  virtual void eraseChild()    { c->recycle();             };

  void spinOffChild();         // Defined in cpp file

};


// The lazy version of the handler.

class lazyHandler : virtual public spHandler
{
 public:

  virtual ~lazyHandler() { };

  void execute();

};


// The hybrid version

class hybridHandler : virtual public spHandler
{
 public:

  virtual ~hybridHandler() { };

  void execute();

 protected:

  virtual bool stillNeedSP() { return (p->childrenLeft > 0) && !canFathom(); };

};


// The eager version needs a few special functions of its own

class eagerHandler : virtual public spHandler
{
 public:

  virtual ~eagerHandler() { };

  void execute();

 protected:

  branchSub* bp;     // The subproblem to be bounded.
  
  virtual void setToBoundCurrent()   { bp = p; };
  virtual void setToBoundChild()     { bp = c; };

  virtual bool boundSubHandler()      // Try to bound bp and return true
    {                                // if it can be thrown away now.
      if (bp->canFathom())
	return true;
      bp->computeBound();
      return bp->canFathom();
    };

};


template <class SUB, class LOAD>
int branchPool<SUB,LOAD>::kill(SUB* p)  
{
remove(p);
p->recycle();
return size();
}

template <class SUB, class LOAD>
void branchPool<SUB,LOAD>::clear() 
{
while(size() > 0)
  remove()->recycle();
}

} // namespace pebbl

std::ostream& operator<<(std::ostream& s, const pebbl::branchSubId& spid);

inline std::ostream& operator<<(std::ostream& s, pebbl::coreSPInfo* p)
{
  p->debugPrint(s);
  return s;
};


#endif
