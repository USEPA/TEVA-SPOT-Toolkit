/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file knapsack.h
 * \author Jonathan Eckstein
 *
 *  Example class to use object-oriented branching framework.
 *  Solves binary knapsack problems.
 */

#ifndef pebbl_knapsack_h
#define pebbl_knapsack_h

#include <acro_config.h>
#include <utilib/CharString.h>
#include <utilib/BasicArray.h>
#include <utilib/BitArray.h>
#include <utilib/IntVector.h>
#include <utilib/DoubleVector.h>
#include <utilib/_math.h>
#include <utilib/ParameterSet.h>
#ifdef ACRO_HAVE_MPI
#include <utilib/PackBuf.h>
#endif
#include <pebbl/branching.h>
#include <pebbl/chunkAlloc.h>


namespace pebbl {

using utilib::IntVector;
using utilib::BasicArray;


// Static class for enums and parameters

class binKnapBase : virtual public utilib::ParameterSet
{
public:

  enum { noItem = -1 };

  binKnapBase()
	{
	randomCompletionGreediness=0.9;
	create_parameter("randomCompletionGreediness",randomCompletionGreediness,
		"<double>","0.9",
		"TODO",
		utilib::ParameterBounds<double>(0.0,1.0));

	randomBackTrackPower=2;
	create_parameter("randomBackTrackPower",randomBackTrackPower,
		"<int>","2",
		"TODO",
		utilib::ParameterLowerBound<int>(1));

	randomSearchPersistence=0.5;
	create_parameter("randomSearchPersistence",randomSearchPersistence,
		"<double>","0.5",
		"TODO",
		utilib::ParameterBounds<double>(0.0,1.0));
	}

protected:

  double randomCompletionGreediness;
  int    randomBackTrackPower;
  double randomSearchPersistence;

};


// Helper classes for storing information about the objects to be packed.

class knapsackItem 
{
public:

  // Basic properties of the item

  double     weight;
  double     value;
  utilib::CharString name;

  // Constructors

  knapsackItem(double weight_, 
	       double value_, 
	       utilib::CharString& name_) :
    weight(weight_),
    value(value_),
    name(name_)
    { };

  knapsackItem() :
    weight(0),
    value(0),
    name()
    { };

  // Assignment operator

  knapsackItem& operator=(knapsackItem& other)
    {
      weight = other.weight;
      value  = other.value;
      name  &= other.name;   // Memory for names is shared.
      return *this;
    };
  
    
  // These are needed for heap operations.  Actual comparison is
  // between the values (value/weight), but things are multiplied
  // through for efficiency.

  int compare(const knapsackItem& other) const
    {
      return utilib::sgn((other.value)*weight - value*(other.weight));
    };
  
  int write(std::ostream& os) const

    {
      os << name << ' ' << weight << ' ' << value;
      return 0;
    };

  int read(std::istream& is)
    {
      is >> name >> weight >> value;
      return 0;
    };

  // This causes us to use efficient block memory allocation for
  // knapsackItems.

  INSERT_CHUNKALLOC

};


// Shortcut operators for reading writing knapsack items to/from streams


// Forward declarations...

class binaryKnapsack;
class binKnapSub;       



//  Class that describes solutions to binary knapsack problems.
//  This version uses a compacted representation: if the solution starts with 
//  k most desirable items in sequence with no gaps, we just store k rather 
//  than explicitly storing the sequence 0,1,...,k-1.

class binKnapSolution :
public binKnapBase
{
 public:

  // Publically accessible data members.

  double obj;
  double left;

  // These let you access the actual explicit set of items.

  int items() { return initialSequence + genItems; };

  int item(int i)
    {
      if (i < initialSequence)
	return i;
      else
	return genItem[i - initialSequence];
    };

  void addItem(int i);      // Insert an item
  void reset(int k);        // Roll back to just the k best items

  // These relate to the incumbent heuristic

  void heuristic();            // Full incumbent heuristic
  void completeGreedy();       // Fill out knapsack from end
  void squeezeInGreedy();      // Fill out, possibly using items we've skipped
  void completeRandom();       // Fill out from end, but randomly skip items
  int  chooseBackTrackItem();  // Randomly choose an amout to roll back
  void backTrack(int toWhere); // Roll back to "toWhere" first items here

  void copy(binKnapSolution& toCopy);

#ifdef ACRO_HAVE_MPI
  void pack(utilib::PackBuffer& outBuf);
  void unpack(utilib::UnPackBuffer& inBuf);
  size_type maxPackSize();
#endif

  void updateIncumbent();

  binKnapSolution(binaryKnapsack* global_) : 
    global(global_),
    genItem(maxGenSize)
    { };

  binKnapSolution(binKnapSolution& toCopy) { copy(toCopy); };

  ~binKnapSolution();

private:

  binaryKnapsack* global;

  int       initialSequence;
  int       genItems;
  IntVector genItem;

  int lastItem;

  static int maxGenSize;

  // How many slots at a time to add to genItem array
  enum { sizeQuantum = 8 };

  inline int         numItems();
  inline double      itemWeight(int i);
  inline double      itemValue(int i);
  inline utilib::CharString& itemName(int i);

};


//  The branching class...

class binaryKnapsack : virtual public branching 
{
public:

  double capacity;
  int    numItems;

  BasicArray<knapsackItem> item;  // Items sorted by "bang for buck"

  DoubleVector partialSumW;       // Element k is sum of all items with
  DoubleVector partialSumV;       // index < k.  W is weight, V is value.

  int fitSize;    // Largest sequence of initial elemets that fits the knapsack

  double minWeight;  // Size of smallest element

  double sumOfAllValues;

  binKnapSolution incumbent;
  binKnapSolution workingSol;

  binaryKnapsack(); 
                
  ~binaryKnapsack() { };

  bool setupProblem(int& argc,char**& argv);

  void preprocess();
  double aPrioriBound() { return sumOfAllValues; };
  double initialGuess();
  branchSub* blankSub();
  bool haveIncumbentHeuristic() { return true; };

  void updateIncumbent(binKnapSolution& newSolution);

  virtual void serialPrintSolution(const char* header = "",
				   const char* footer = "",
				   std::ostream& outStream = ucout);

  int fitSequence(double weightLimit,int maxLength);

#ifdef MEMORY_TRACKING
  // Temporary(?) addition to test memory use
  void memoryUsageTest();
#endif

};


//  For use in the constructor compiled code, and also in derived
//  parallel classes
#ifdef  BRANCHING_CONSTRUCTOR
#undef  BRANCHING_CONSTRUCTOR
#endif
#define BRANCHING_CONSTRUCTOR \
branching(maximization,relTolerance(),absTolerance())


// These definitions for the binKnapSolution class had to be deferred
// until binaryKnapsack was defined.

inline int binKnapSolution::numItems() 
{
  return global->numItems; 
};
  
inline double binKnapSolution::itemWeight(int i) 
{ 
  return global->item[i].weight; 
};
  
inline double binKnapSolution::itemValue(int i)
{ 
  return global->item[i].value; 
};
  
inline utilib::CharString& binKnapSolution::itemName(int i)
{ 
  return global->item[i].name; 
};

inline void binKnapSolution::updateIncumbent() 
{ 
  global->updateIncumbent(*this); 
};


//  The branchSub class...

class binKnapSub : 
  virtual public branchSub,
  public binKnapBase
{
public:

  inline binaryKnapsack* global() const { return globalPtr; };

  branching* bGlobal() const { return global(); };

  REFER_DEBUG(global())

  double capBase;

  binKnapSolution& solution() { return globalPtr->workingSol; };

  IntVector inList;
  IntVector outList;
  int numIn;
  int numOut;

  int splitItem;
  enum { terminal = -1, notSplit = -2 };

  binKnapSub() {};

  void binKnapSubFromKnapsack(binaryKnapsack* master);
  void binKnapSubAsChildOf(binKnapSub* parent,int whichChild);
  virtual ~binKnapSub();

  virtual void setRootComputation();

  void boundComputation(double* controlParam);

  void incumbentHeuristic();

  virtual int splitComputation() 
    { 
      setState(separated);
      return 2; 
    };

  virtual branchSub* makeChild(int whichChild)
    {
    binKnapSub *temp = new binKnapSub;
    temp->binKnapSubAsChildOf(this, whichChild);
    return temp;
    };
  
  bool candidateSolution() { return (splitItem == terminal); };

  void updateIncumbent() { solution().updateIncumbent(); };

  void makeCurrentEffect();

#ifdef MEMORY_TRACKING
  void printMemDetails();
#endif

protected:

  binaryKnapsack* globalPtr;

  static void growList(IntVector& newList, 
		       IntVector& oldList, 
		       int newElement);

  inline int         numItems()        { return global()->numItems;       };
  inline double      itemWeight(int i) { return global()->item[i].weight; };
  inline double      itemValue(int i)  { return global()->item[i].value;  };
  inline utilib::CharString& itemName(int i)   { return global()->item[i].name;   };

  void dumpLists(const char* extraString = "");
};


// Now we have enough information to define...

inline branchSub* binaryKnapsack::blankSub()
{
  binKnapSub *temp = new binKnapSub();
  temp->binKnapSubFromKnapsack(this);
  return temp;
};


// This little utility class is used by binaryKnapsack::boundComputation...

class itemListCursor :
public binKnapBase
{
public:

  IntVector* array;
  int size,cursor,next;

  inline void advance() 
    { 
      if (cursor < size) 
	next = (*array)[cursor++]; 
      else
	next = noItem;
    };     

  itemListCursor(int size_,IntVector* array_) :
    array(array_),
    size(size_),
    cursor(0)
    {
      advance();
    };
};

} // namespace pebbl

std::ostream& operator<<(std::ostream& os, pebbl::knapsackItem& item);
std::istream& operator>>(std::istream& is, pebbl::knapsackItem& item);

#endif
