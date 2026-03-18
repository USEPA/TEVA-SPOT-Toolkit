/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file SNLKnapsack.h
 *
 * Defines the pico::SNLKnapsack class.
 */

#ifndef pico_SNLKnapsack_h
#define pico_SNLKnapsack_h

#include <acro_config.h>
#include "CoinPackedMatrix.hpp"
#include "cutFinder.h"

namespace pico {

//BFLAG allows all eight bits of the unsigned character to be used as 
// Boolean flags.
typedef unsigned char BFLAG;
#define BF_1(q) ((unsigned char) (q)&0x1)
#define BF_2(q) ((unsigned char)((q)&0x02) ? 1 : 0)
#define BF_3(q) ((unsigned char)((q)&0x04) ? 1 : 0)
#define BF_4(q) ((unsigned char)((q)&0x08) ? 1 : 0)
#define BF_5(q) ((unsigned char)((q)&0x10) ? 1 : 0)
#define BF_6(q) ((unsigned char)((q)&0x20) ? 1 : 0)
#define BF_7(q) ((unsigned char)((q)&0x40) ? 1 : 0)
#define BF_8(q) ((unsigned char)((q)&0x80) ? 1 : 0)
#define BF_1ON(q) ((BFLAG) ((q)&0xfe) ^ 0x01)
#define BF_2ON(q) ((BFLAG) ((q)&0xfd) ^ 0x02)
#define BF_3ON(q) ((BFLAG) ((q)&0xfb) ^ 0x04)
#define BF_4ON(q) ((BFLAG) ((q)&0xf7) ^ 0x08)
#define BF_5ON(q) ((BFLAG) ((q)&0xef) ^ 0x10)
#define BF_6ON(q) ((BFLAG) ((q)&0xdf) ^ 0x20)
#define BF_7ON(q) ((BFLAG) ((q)&0xbf) ^ 0x40)
#define BF_8ON(q) ((BFLAG) ((q)&0x7f) ^ 0x80)
#define BF_1OFF(q) ((BFLAG) ((q)&0xfe))
#define BF_2OFF(q) ((BFLAG) ((q)&0xfd))
#define BF_3OFF(q) ((BFLAG) ((q)&0xfb))
#define BF_4OFF(q) ((BFLAG) ((q)&0xf7))
#define BF_5OFF(q) ((BFLAG) ((q)&0xef))
#define BF_6OFF(q) ((BFLAG) ((q)&0xdf))
#define BF_7OFF(q) ((BFLAG) ((q)&0xbf))
#define BF_8OFF(q) ((BFLAG) ((q)&0x7f))
#define BF_1FLIP(q) ((BFLAG) ((q)^0x01))
#define BF_2FLIP(q) ((BFLAG) ((q)^0x02))
#define BF_3FLIP(q) ((BFLAG) ((q)^0x04))
#define BF_4FLIP(q) ((BFLAG) ((q)^0x08))
#define BF_5FLIP(q) ((BFLAG) ((q)^0x10))
#define BF_6FLIP(q) ((BFLAG) ((q)^0x20))
#define BF_7FLIP(q) ((BFLAG) ((q)^0x40))
#define BF_8FLIP(q) ((BFLAG) ((q)^0x80))
#define BF_ON(q,i) (((i<0) || (i>7)) ? q : ((BFLAG) ((q)&(0xff ^ (1<<i))) ^ (1<<i)))
#define BF_OFF(q,i) (((i<0) || (i>7)) ? q : ((BFLAG) ((q)&(0xff ^ (1<<i)))))
#define BF_FLIP(q,i) (((i<0) || (i>7)) ? q : ((BFLAG) ((q)^(1<<i))))
#define BF_IS_ON(q,i) (((i<0) || (i>7)) ? 0 : ((q&(1<<i)) ? 1 : 0))
#define GREY_OFF(q,on_off) (((q)&0xfc) ^ ((on_off) ? 1 :0))
#define GREY_ON(q) (((q)&0xfc) ^ 0x02)
#define GREY(q) (((q)&0x02) ? 1 : 0) // returns 1 if grey flag is on, zero 
/* CAP.  This seems incomplete.  Comment out for now
BFLAG BF_ON(BFLAG q,int i)
  {
  switch (i)
    {
    case 0:
    }
  }
*/

class SNLKnapsack : public cutFinder
  {
  public:

    SNLKnapsack(BCMip *searcher): cutFinder(searcher)
      {};
 
//current assumption is that ALL variables are binary (x\in {0,1}); 

    bool instanceApplies();

    virtual void generateCuts(const PicoLPInterface &lp, BasicArray<picoRowCut *> &new_cuts, int &num_cuts, DoubleVector &lp_solution, bool &problemInfeasible);

    virtual const char *typeString() {return "SNLKnapsack";}

    // The type of cut returned from this finder
    virtual int cutType() {return (int)SNL_KC;}

    // Documentation string for type of cut produced.  Don't make this const
    // because cuts' documentation string is set after construction in parallel
    // versions (and cannot be set from a const char *)
    virtual char* cutName() {return "SNLKnapsack";}
    bool globallyValid()
      {
	// Don't think it uses variable upper bounds, but could make cuts on cuts
	// and seems to use the solution value. Let's be conservative for now.
	return(false);
      }

    bool LPNeedsResolve()
      {
	return(false);
      }
  };


} // namespace pico

#endif
