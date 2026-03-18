/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// parLoadObject.cpp
//
// Load object tracker for parallel branching search.
//
// Jonathan Eckstein
//


#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <pebbl/parBranching.h>

using namespace std;

namespace pebbl {


void parLoadObject::reset()
{
  loadObject::reset();
  messages.reset();
};


void parLoadObject::init(parallelBranching* pGlobal_, 
			 bool useSPCounts_, 
			 bool useMessageCounts_)
{
  loadObject::init(pGlobal_,useSPCounts_);
  
  pGlobal = pGlobal_;
  useMessageCounts = useMessageCounts_;

  if (pGlobal_)
    aggregateBound = (pGlobal->sense)*MAXDOUBLE;
  else
    aggregateBound = MAXDOUBLE;
  
  setIncumbentUnknown();
  messages.reset();   
}


void parLoadObject::update()
{
  loadObject::update();
  incumbentSource = pGlobal->incumbentSource;
  if (incumbentSource == MPI_ANY_SOURCE)
    {
      if (pGlobal->parameter_initialized("startIncumbent"))
	incumbentValue = pGlobal->startIncumbent;
      else
	incumbentValue = pGlobal->sense*MAXDOUBLE;
    }
  if (useMessageCounts)
    messages = pGlobal->messages;
  setCPBusy(!(pGlobal->suspending()));
};


void parLoadObject::setIncumbentUnknown()
{
  incumbentSource = MPI_ANY_SOURCE;
  if (pGlobal)
    {
      if (pGlobal->parameter_initialized("startIncumbent"))
	incumbentValue = pGlobal->startIncumbent;
      else
	incumbentValue = pGlobal->sense*MAXDOUBLE;
    }
  else
    incumbentValue = MAXDOUBLE;
}


void parLoadObject::operator+=(const parLoadObject& other)
{
  loadObject::operator+=(other);
  if (incumbentSource != other.incumbentSource)
    {
      DEBUGPRX(200,bGlobal,"Incumbent source mismatch: [" << incumbentSource
	       << "] versus [" << other.incumbentSource << "]\n");
      setMismatch();
    }
  messages += other.messages;
};


bool parLoadObject::senseClusterBusy()
{
  loadObject::senseBusy();
  if (!(pGlobal->suspending()))
    setCPBusy();
  if (mismatch())
    setBothBusy();
  else if (!messages.localScatter.inBalance() ||
	   !messages.hubDispatch.inBalance())
    setBothBusy();
  DEBUGPRX(100,bGlobal,"senseClusterBusy: flags set to " 
	   << (int) flags << endl);
  return busy();
}


bool parLoadObject::senseBusy()
{
  senseClusterBusy();
  if (!messages.nonLocalScatter.inBalance() ||
      !messages.general.inBalance())
    setBothBusy();
  return busy();
}


bool parLoadObject::reportablyDifferent(const parLoadObject& other)
{
  if (count() != other.count())
    return true;
  DEBUGPRX(1000,pGlobal,"reportablyDifferent: Counts match.\n");
  if (flags != other.flags)
    return true;
  DEBUGPRX(1000,pGlobal,"reportablyDifferent: flags match.\n");
  if ((incumbentValue  != other.incumbentValue)   ||
      (incumbentSource != other.incumbentSource))
    return true;
  DEBUGPRX(1000,pGlobal,"reportablyDifferent: incumbents match.\n");
  return messages != other.messages;
}


void parLoadObject::packEmpty(PackBuffer& buf, parallelBranching* pGlobal_)
{
  parLoadObject junkLoad(pGlobal_);
#ifdef ACRO_VALIDATING
  //
  // WEH - initialize this data to avoid memory warnings in purify.
  //	Note:  this _could_ hide some bugs.  
  //
  junkLoad.aggregateBound = -999.0;
  junkLoad.incumbentValue = -999.0;
  junkLoad.pCount = -999;
  junkLoad.boundedSPs = -999;
  junkLoad.createdSPs = -999;
  junkLoad.incumbentSource = -999;
  junkLoad.flags = '\000';
#ifdef MEMUTIL_PRESENT
  junkLoad.memUsed = -999.0;
#endif
#endif
  buf << junkLoad;
}


int parLoadObject::packSize(parallelBranching* pGlobal_)
{
  PackBuffer    junkBuffer(128);
  parLoadObject::packEmpty(junkBuffer,pGlobal_);
  junkBuffer << (double) 0.0;
  return junkBuffer.size();
};


void parLoadObject::write(PackBuffer& buff) const
{
  for(int i=pGlobal->loadMeasureDegree - 1; i>=0; i--)
    buff << powerSum[i];
  buff << aggregateBound;
  buff << incumbentValue;
  buff << pCount;
  buff << boundedSPs;
  buff << createdSPs;
  buff << incumbentSource;
  buff << messages;
  buff << flags;
#ifdef MEMUTIL_PRESENT
  buff << memUsed;
#endif
}


void parLoadObject::read(UnPackBuffer& buff)
{
  for(int i=pGlobal->loadMeasureDegree - 1; i>=0; i--)
    buff >> powerSum[i];
  buff >> aggregateBound;
  buff >> incumbentValue;
  buff >> pCount;
  buff >> boundedSPs;
  buff >> createdSPs;
  buff >> incumbentSource;
  buff >> messages;
  buff >> flags;
#ifdef MEMUTIL_PRESENT
  buff >> memUsed;
#endif
}

void parLoadObject::operator+=(const spToken& sp)
{ 
  addLoad(sp.boundEstimate(),sp.loadXFactor()); 
}


void parLoadObject::operator-=(const spToken& sp)
{ 
  subtractLoad(sp.boundEstimate(),sp.loadXFactor()); 
}


void parLoadObject::operator+=(const parallelBranchSub &sp)
{
  addLoad(sp.boundEstimate(),sp.loadXFactor());
}

void parLoadObject::operator-=(const parallelBranchSub &sp)
{
  subtractLoad(sp.boundEstimate(),sp.loadXFactor());
}


void parLoadObject::dump(std::ostream& stream, const char* name)
{
  stream << "Par";
  loadObject::dump(stream,name);
  stream << "incumbentSource = " << incumbentSource << '\n';
  stream << "localScatter: " << messages.localScatter << '\n';
  stream << "nonLocalScatter: " << messages.nonLocalScatter << '\n';
  stream << "hubDispatch: " << messages.hubDispatch << '\n';
  stream << "general: " << messages.general << '\n';
}


} // namespace pebbl

#endif
