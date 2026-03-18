/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

#include <acro_config.h>
#include <pico/cutGen.h>

using namespace std;

namespace pico {


// TODO: use the static array of cut strings to return cutName() (delete
// these and use the type as an index in the cutFinder class.

int dummy_CglKnapsackCover = PicoCGL<CglKnapsackCover>::advertise();
int dummy_CglOddHole = PicoCGL<CglOddHole>::advertise();
int dummy_CglSimpleRounding = PicoCGL<CglSimpleRounding>::advertise();
int dummy_picoCglGomory = PicoCGL<picoCglGomory>::advertise();
int dummy_CglLiftAndProject = PicoCGL<CglLiftAndProject>::advertise();
int dummy_CglProbing = PicoCGL<CglProbing>::advertise();
int dummy_CglMixedIntergerRounding = PicoCGL<CglMixedIntegerRounding>::advertise();
int dummy_CglMixedIntergerRounding2 = PicoCGL<CglMixedIntegerRounding2>::advertise();
int dummy_CglTwomir = PicoCGL<CglTwomir>::advertise();
int dummy_CglFlowCover = PicoCGL<CglFlowCover>::advertise();
int dummy_CglClique = PicoCGL<CglClique>::advertise();
int dummy_DuplicateRow = PicoCGL<CglDuplicateRow>::advertise();

template<>
const char *PicoCGL<CglKnapsackCover>::typeString()
{
return("CglKnapsackCover");
}

template<>
char* PicoCGL<CglKnapsackCover>::cutName()
{
return("CglKnapsackCover");
}

template<>
int PicoCGL<CglKnapsackCover>::cutType()
{
return((int) CGL_kc);
}

template<>
bool PicoCGL<CglKnapsackCover>::globallyValid()
{
return(false);
}

template<>
bool PicoCGL<CglKnapsackCover>::LPNeedsResolve()
{
return(false);
}

template<>
const char *PicoCGL<CglOddHole>::typeString()
{
return("CglOddHole");
}

template<>
char* PicoCGL<CglOddHole>::cutName()
{
return("CglOddHole");
}

template<>
int PicoCGL<CglOddHole>::cutType()
{
return((int) CGL_oh);
}

template<>
bool PicoCGL<CglOddHole>::globallyValid()
{
return(false);
}

template<>
bool PicoCGL<CglOddHole>::LPNeedsResolve()
{
return(false);
}

template<>
const char *PicoCGL<CglSimpleRounding>::typeString()
{
return("CglSimpleRound");
}


template<>
char* PicoCGL<CglSimpleRounding>::cutName()
{
return("CglSimpleRounding");
}

template<>
int PicoCGL<CglSimpleRounding>::cutType()
{
return((int) CGL_sr);
}

template<>
bool PicoCGL<CglSimpleRounding>::globallyValid()
{
return(false);
}

template<>
bool PicoCGL<CglSimpleRounding>::LPNeedsResolve()
{
return(false);
}

template<>
const char *PicoCGL<picoCglGomory>::typeString()
{
return("picoCglGomory");
}

template<>
bool PicoCGL<picoCglGomory>::globallyValid()
{
  // Turn off global validity until we've fixed the core-row problem
  return(false);
  //return(globallyValidAnswer);
}

template<>
char* PicoCGL<picoCglGomory>::cutName()
{
return("picoCglGomory");
}

template<>
int PicoCGL<picoCglGomory>::cutType()
{
return((int) CGL_gom);
}

template<>
bool PicoCGL<picoCglGomory>::LPNeedsResolve()
{
return(false);
}

template<>
const char *PicoCGL<CglLiftAndProject>::typeString()
{
return("CglLiftAndProject");
}


template<>
char* PicoCGL<CglLiftAndProject>::cutName()
{
return("CglLiftAndProject");
}

template<>
int PicoCGL<CglLiftAndProject>::cutType()
{
return((int) CGL_LAP);
}

template<>
bool PicoCGL<CglLiftAndProject>::globallyValid()
{
return(true);
}


// Clones the LP solver and claims it cleans up, but I think it shares
// memory and doesn't clean up enough.

template<>
bool PicoCGL<CglLiftAndProject>::LPNeedsResolve()
{
return(true);
}

template<>
const char *PicoCGL<CglProbing>::typeString()
{
return("CglProbing");
}


template<>
char* PicoCGL<CglProbing>::cutName()
{
return("CglProbing");
}

template<>
int PicoCGL<CglProbing>::cutType()
{
return((int) CGL_P);
}

template<>
bool PicoCGL<CglProbing>::globallyValid()
{
return(false);
}

template<>
bool PicoCGL<CglProbing>::LPNeedsResolve()
{
return(false);
}

template<>
const char *PicoCGL<CglMixedIntegerRounding>::typeString()
{
return("CglMixedIntegerRounding");
}

template<>
char* PicoCGL<CglMixedIntegerRounding>::cutName()
{
return("CglMixedIntegerRounding");
}

template<>
int PicoCGL<CglMixedIntegerRounding>::cutType()
{
return((int) CGL_MIR);
}

// TODO
template<>
bool PicoCGL<CglMixedIntegerRounding>::globallyValid()
{
return(false);
}

template<>
bool PicoCGL<CglMixedIntegerRounding>::LPNeedsResolve()
{
return(false);
}

template<>
const char *PicoCGL<CglMixedIntegerRounding2>::typeString()
{
return("CglMixedIntegerRounding2");
}

template<>
char* PicoCGL<CglMixedIntegerRounding2>::cutName()
{
return("CglMixedIntegerRounding2");
}

template<>
int PicoCGL<CglMixedIntegerRounding2>::cutType()
{
return((int) CGL_MIR2);
}

// TODO
template<>
bool PicoCGL<CglMixedIntegerRounding2>::globallyValid()
{
return(false);
}

template<>
bool PicoCGL<CglMixedIntegerRounding2>::LPNeedsResolve()
{
return(false);
}

template<>
const char *PicoCGL<CglTwomir>::typeString()
{
return("CglTwomir");
}

template<>
char* PicoCGL<CglTwomir>::cutName()
{
return("CglTwomir");
}

template<>
int PicoCGL<CglTwomir>::cutType()
{
return((int) CGL_2MIR);
}

// TODO
template<>
bool PicoCGL<CglTwomir>::globallyValid()
{
return(false);
}

// Might want to take a closer look.  Seems to copy things it changes
template<>
bool PicoCGL<CglTwomir>::LPNeedsResolve()
{
return(false);
}

template<>
const char *PicoCGL<CglFlowCover>::typeString()
{
return("CglFlowCover");
}

template<>
char* PicoCGL<CglFlowCover>::cutName()
{
return("CglFlowCover");
}

template<>
int PicoCGL<CglFlowCover>::cutType()
{
return((int) CGL_FC);
}

// TODO
template<>
bool PicoCGL<CglFlowCover>::globallyValid()
{
return(false);
}

// Might want to take a closer look.  Seems to copy things it changes
template<>
bool PicoCGL<CglFlowCover>::LPNeedsResolve()
{
return(false);
}

template<>
const char *PicoCGL<CglClique>::typeString()
{
return("CglClique");
}

template<>
char* PicoCGL<CglClique>::cutName()
{
return("CglClique");
}

template<>
int PicoCGL<CglClique>::cutType()
{
return((int) CGL_C);
}

// TODO
template<>
bool PicoCGL<CglClique>::globallyValid()
{
return(false);
}

// Might want to take a closer look.  Seems to copy things it changes
template<>
bool PicoCGL<CglClique>::LPNeedsResolve()
{
return(false);
}

template<>
void PicoCGL<CglClique>::FinderSpecificInit()
{
setStarCliqueReport(false);
setRowCliqueReport(false);
}

template<>
const char *PicoCGL<CglDuplicateRow>::typeString()
{
return("CglDuplicateRow");
}

template<>
char* PicoCGL<CglDuplicateRow>::cutName()
{
return("CglDuplicateRow");
}

template<>
int PicoCGL<CglDuplicateRow>::cutType()
{
return((int) CGL_DR);
}

// TODO
template<>
bool PicoCGL<CglDuplicateRow>::globallyValid()
{
return(false);
}

// Might want to take a closer look.  Seems to copy things it changes
template<>
bool PicoCGL<CglDuplicateRow>::LPNeedsResolve()
{
return(false);
}

// Pulling this out to a .cpp file to try to deal with linking errors
// for gcc 2.x

poolScanner:: poolScanner(BCMip *searcher):cutFinder(searcher)
  // TODO Check this
{
strcpy(nameBuffer, "cutPool");
}

poolScanner:: ~poolScanner()
{}

} // namespace pico
