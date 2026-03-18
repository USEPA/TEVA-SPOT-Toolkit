/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file parLoadObject.h
 * \author Jonathan Eckstein
 *
 * Load object tracker for parallel branching search.
 */

#ifndef pebbl_parLoadObject_h
#define pebbl_parLoadObject_h

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <utilib/PackBuf.h>
#include <pebbl/loadObject.h>

namespace pebbl {


class parallelBranching;
class spToken;
class parallelBranchSub;

class messageCounter
{
public:

  int sent;
  int received;

  int inBalance() { return sent == received; };

  int operator==(const messageCounter& other)
    {
      return (sent == other.sent) && (received == other.received);
    };

  int operator!=(const messageCounter& other) { return !operator==(other); };

  void reset()
    {
      sent     = 0;
      received = 0;
    };

  messageCounter() { reset(); };

  void operator+=(const messageCounter& other)
    {
      sent     += other.sent;
      received += other.received;
    };

};




class pbMessageBlock
{
public:

  messageCounter localScatter;
  messageCounter nonLocalScatter;
  messageCounter hubDispatch;
  messageCounter general;

  int operator==(const pbMessageBlock& other)
    {
      return ((localScatter    == other.localScatter)    &&
	      (nonLocalScatter == other.nonLocalScatter) &&
	      (hubDispatch     == other.hubDispatch)     &&
	      (general         == other.general));
    };

  int operator!=(const pbMessageBlock& other) { return !operator==(other); };

  void reset()
    {
      localScatter.reset();
      nonLocalScatter.reset();
      hubDispatch.reset();
      general.reset();
    };

  void operator+=(const pbMessageBlock& other)
    {
      localScatter    += other.localScatter;
      nonLocalScatter += other.nonLocalScatter;
      hubDispatch     += other.hubDispatch;
      general         += other.general;
    };

};



class parLoadObject : public loadObject
{
public:

  void reset();

  void init(parallelBranching* pGlobal_, 
	    bool useSPCounts_=false, 
	    bool useMessageCounts_=false);
  
  parLoadObject() { init(NULL,false,false); };

  parLoadObject(parallelBranching* pGlobal_,
		bool useSPCounts_ = false,
		bool useMessageCounts_ = false) 
    { init(pGlobal_,useSPCounts_,useMessageCounts_); }; 

  void write(PackBuffer& pack) const;
  void read(UnPackBuffer& unpack);

  virtual ~parLoadObject() { };

  void setIncumbentUnknown();

  void update();

  void operator+=(const parLoadObject& other);

  bool countIncomplete()
    {
      return (!messages.localScatter.inBalance()    ||
	      !messages.nonLocalScatter.inBalance());
    }

  bool senseClusterBusy();
  bool senseBusy();

  bool reportablyDifferent(const parLoadObject& other);

  static int  packSize(parallelBranching* pGlobal_);
  static void packEmpty(PackBuffer& buff, parallelBranching* pGlobal_);

  void dump(std::ostream& stream, const char* name);

  // Public data members

  int incumbentSource;
  
  pbMessageBlock messages;

  // Methods for adding and subtracting subproblem loads

  void operator+=(const spToken& sp);
  void operator-=(const spToken& sp);

  void operator+=(const parallelBranchSub &sp);
  void operator-=(const parallelBranchSub &sp);

private:

  parallelBranching* pGlobal;

  int useMessageCounts;

};

} // namespace pebbl

inline utilib::PackBuffer& operator<<(utilib::PackBuffer& buff,
			      const pebbl::parLoadObject& counter)
{ counter.write(buff); return buff; }


inline utilib::UnPackBuffer& operator>>(utilib::UnPackBuffer& buff,
				pebbl::parLoadObject& counter)
{ counter.read(buff); return buff; }


inline utilib::PackBuffer& operator<<(utilib::PackBuffer& buff,
			      const pebbl::messageCounter& counter)
{ return buff << counter.sent << counter.received; }


inline utilib::UnPackBuffer& operator>>(utilib::UnPackBuffer& buff,
				pebbl::messageCounter& counter)
{ return buff >> counter.sent >> counter.received; }


inline std::ostream& operator<<(std::ostream& s, const pebbl::messageCounter& counter)
{ return s << "sent " << counter.sent << ", received " << counter.received; }

inline utilib::PackBuffer& operator<<(utilib::PackBuffer& buff,
			      const pebbl::pbMessageBlock& block)
{
  buff << block.localScatter;
  buff << block.nonLocalScatter;
  buff << block.hubDispatch;
  buff << block.general;
  return buff;
}


inline utilib::UnPackBuffer& operator>>(utilib::UnPackBuffer& buff,
				pebbl::pbMessageBlock& block)
{
  buff >> block.localScatter;
  buff >> block.nonLocalScatter;
  buff >> block.hubDispatch;
  buff >> block.general;
  return buff;
}


#endif

#endif
