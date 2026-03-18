/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file loadObject.h
 * \author Jonathan Eckstein
 *
 * Defines the pebbl::loadObject class.
 */

#ifndef pebbl_loadObject_h
#define pebbl_loadObject_h

#include <acro_config.h>
#include <utilib/CommonIO.h>
#include <utilib/DoubleVector.h>
#include <pebbl/memUtil.h>
#include <pebbl/fundamentals.h>
#include <pebbl/pebblBase.h>

namespace pebbl {

using utilib::CommonIO;
using utilib::DoubleVector;

class branching;
class coreSPInfo;
class parLoadObject;


///
class loadObject : public pebblBase
{
public:

  /// Constructor.  If bGlobal_ is non-null use it as a pointer
  /// to the branching class.  useSPCounts_ sets a flag that 
  /// says whether this object should count the total number
  /// of subproblems bounded or not.

  loadObject(branching* bGlobal_, bool useSPCounts_ = true) :
    powerSum(maxLoadDegree)
    {
      init(bGlobal_,useSPCounts);
    };

  loadObject() :
    powerSum(maxLoadDegree)
    {
      init(NULL,false);
    };

  /// Destructor
  virtual ~loadObject() { }

  /// Return number of subproblems measured
  long count() const
	{ return pCount; }

  /// Return total weight of subproblems
  double loadMeasure() const;

  /// Initialize (see constructor for args)
  void init(branching* bGlobal_ = 0, bool useSPCounts_ = true);
  
  /// Set load to zero
  virtual void reset();

  /// Update from the information in bGlobal
  virtual void update();

  /// Clear and update
  void resetAndUpdate()
    {
      reset();
      update();
    }

  /// Set the bound
  void setBound(double b);

  /// Change the bound, but only if it is known and the passed argument
  /// is better (with regard to the objective function)
  void adjustBound(double b);

  /// Add the load of a subproblem witha give bound multiplicity times
  /// (attempts to update aggregateBound).
  void addLoad(double bound,int multiplicity);

  /// Add the weight of a subproblem 
  /// (attempts to update aggregateBound too)
  void operator+=(const coreSPInfo& sp);

  /// Combine in the load measured in another load object
  void operator+=(const loadObject& other);

  /// Take away the weight of a given bound multiplicity times
  /// Does not attempt to update aggregateBound!
  void subtractLoad(double bound,int multiplicity);

  /// Subtract the weight of a given subproblem
  /// Does not attempt to update aggregateBound!
  void operator-=(const coreSPInfo& sp);

  /// Return true if it looks like some information is missing
  virtual bool countIncomplete() { return false; }

  /// Set busy bit
  void setBusy()         { flags |= busyBit;         }

  /// Set mismatch bit (the infamous '(x)')
  void setMismatch()     { flags |= mismatchBit;     }

  /// Set bit that says we do not know the global bound for this
  /// collection of subproblems
  void setBoundUnknown() { flags |= boundUnknownBit; }

  /// Set or clear bound unknown flag
  void setBoundUnknown(bool flag)
    {
      if (flag)
	setBoundUnknown();
      else
	flags &= ~boundUnknownBit;
    }

  /// Set the bit that says it is NOT possible to write a checkpoint
  /// or to abort (if we're trying to abort)
  void setCPBusy() { flags |= cpBusyBit; }

  /// Set or clear bit that says it is NOT possible to write a checkpoint
  /// or to abort (if we're trying to abort)
  void setCPBusy(bool flag) 
    { 
      if (flag)
	setCPBusy();
      else
	flags &= ~cpBusyBit; 
    };

  /// Set both the regular busy bit and the checkpoint/abort busy bit
  void setBothBusy() { flags |= (busyBit | cpBusyBit); }

  /// Is any processor contributing to this load busy?
  bool busy()         { return (flags & busyBit) != 0;         }

  /// Are there any incumbent mismatches in this load?
  bool mismatch()     { return (flags & mismatchBit) != 0;     }

  /// Is the global bound for this load unknown?
  bool boundUnknown() { return (flags & boundUnknownBit) != 0; }

  /// Update the aggregate bound to "bound" if it is better.
  void updateAggBound(double bound);

  /// Is any there any reason we can't write a checkpoint or abort now?
  bool cpBusy()       { return (flags & cpBusyBit) != 0;       }

  /// Set the busy bit according to whether there are any subproblems 
  /// counted by this load object
  virtual bool senseBusy() 
    { 
      if (count() > 0)
	setBusy();
      return busy();
    }

#ifdef MEMUTIL_PRESENT
  /// Set memory usage to the amount of memory on this processor
  void setMemory()
    {
      memUsed = memUtil::inUse();
    }
#endif  

  ///
  virtual void dump(std::ostream& stream, const char* name);


  ///
  double aggregateBound;

  ///
  double incumbentValue;

  ///
  int boundedSPs;

  ///
  int createdSPs;

#ifdef MEMUTIL_PRESENT
  ///
  double memUsed;
#endif

protected:

  ///
  enum 
    {                       // Set if...
      busyBit = 1,          //  anybody is working
      mismatchBit = 2,      //  there is any disagreement on the inc val
      boundUnknownBit = 4,  //  anybody does not know the agg. bound
      cpBusyBit = 8         //  there is anybody not ready to checkpoint/abort
    };
  
  ///
  branching* bGlobal;

  ///
  int pCount;

  ///
  DoubleVector powerSum;

  ///
  char flags;

  ///
  int useSPCounts;

  ///
  void resetSums();

};

} // namespace pebbl


inline std::ostream& operator<<(std::ostream& s,const pebbl::loadObject& load)
{
  return s << load.count() << '/' << load.loadMeasure();
}

#endif
