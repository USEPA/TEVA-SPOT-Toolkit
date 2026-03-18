/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

//
// lpSetupObjCuts.cpp
//
// 
//
// 
//

#include <acro_config.h>
#include <utilib/CommonIO.h>
#include <pico/BCMip.h>


using namespace std;

namespace pico {


  // Virtual method to set up an LP from a stored solution.  
  // Sets bounds the way lpSetupObj does, but then sets a basis and some 
  // cuts.

  void lpSetupObjCuts::setupLP(__LPSolver_Interface* lp)
  {
    setBounds(MIP,lp,boundList);
    setBasisAndCuts(lp,
		    persistentCuts,persistentBasicSlack,
		    addedBindingCuts,addedBindingSlack,
		    savedBasis);
  }


  // Set basis and introduce cuts

  void lpSetupObjCuts::setBasisAndCuts(__LPSolver_Interface*    lp,
				       BasicArray<picoRowCut*>& persistCuts,
				       BasicArray<int>&         persistSlack,
				       BasicArray<picoRowCut*>& addedCuts,
				       BasicArray<int>&         addedSlack,
				       BasisArray               basis)
  {
    lp->addRows(persistCuts,persistCuts.size());
    lp->addRows(addedCuts,addedCuts.size());
    lp->setBasisWithCuts(persistCuts,persistSlack,
			 addedCuts,addedSlack,
			 basis);
  }


  // Constructor that takes an branch-and-cut search node and saves
  // the necessary cut, basis, bound, and solution info for a later
  // LP setup.

  lpSetupObjCuts::lpSetupObjCuts(BCMipNode* sp)
  {
    // Set up basic pointers and scalars

    mGlobal = sp->mGlobal();
    MIP     = mGlobal->MIP;
    bound   = sp->lpBound;
    mfValue = sp->mfValue;

    // Get bound information. This is the same as for the plain version.

    boundList.resize(2*(MIP->numIntegerVars()));
    makeBoundList(MIP,sp,boundList);

    // Get the solution vector

    savedSolution << sp->solution;

    // Get basis for core rows

    savedBasis.resize(sp->basis.size());
    savedBasis << sp->basis;

    // Find the binding and persistent cuts, along with their basis information

    findCuts(sp,
	     persistentCuts,persistentBasicSlack,
	     addedBindingCuts,addedBindingSlack);

    // Increment reference counts for all the cuts we found

    increaseRef(persistentCuts);
    increaseRef(addedBindingCuts);

  }


  // Static methods that extracts cut information into our four arrays

  void lpSetupObjCuts::findCuts(BCMipNode* sp,
				BasicArray<picoRowCut*>& persistCuts,
				BasicArray<int>&         persistSlack,
				BasicArray<picoRowCut*>& addedCuts,
				BasicArray<int>&         addedSlack)
  {
    // Just use list of persistent cuts as is

    persistCuts << sp->persistent_cuts;

    // If the problem is active, compute binding cut and basis info;
    // otherwise, grab it from stored values.

    if (sp->active)
      sp->bcGlobal()->getBindingRows(sp->persistent_cuts, 
				     persistSlack,
				     addedCuts,
				     addedSlack);
    else
      {
	persistSlack << sp->persistent_basic_slacks;
	addedCuts    << sp->binding;
	addedSlack   << sp->basic_slack_cuts;
      }

  }


  // Destructor -- just managed cut memory -- everything else should be
  // automatic.

  lpSetupObjCuts::~lpSetupObjCuts()
  {
    decreaseRef(persistentCuts);
    decreaseRef(addedBindingCuts);
  }
  

  // Increase reference count of all cuts in an array

  void lpSetupObjCuts::increaseRef(BasicArray<picoRowCut*>& cutList)
  {
    for(size_type i=0; i < cutList.size(); i++)
      (cutList[i])->incrementRefCount();
  }


  // Decrease the reference cout of all cuts in an array,
  // and delete them if they become unused

  void lpSetupObjCuts::decreaseRef(BasicArray<picoRowCut*>& cutList)
  {
    for(size_type i=0; i < cutList.size(); i++)
      {
	picoRowCut* cut = cutList[i];
	cut->decrementRefCount();
	if (cut->deletable())
	  delete cut;
      }
  }

} // namespace pico
