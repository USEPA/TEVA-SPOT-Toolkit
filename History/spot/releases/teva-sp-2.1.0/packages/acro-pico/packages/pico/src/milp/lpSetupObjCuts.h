/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file lpSetupObjCuts.h
 *
 * Defines the pico::lpSetupObjCuts class.
 */

#ifndef pico_lpSetupObjCuts_h
#define pico_lpSetupObjCuts_h

#include <acro_config.h>
#include <pico/lpSetupObj.h>



namespace pico {

  using utilib::BasicArray;
  
  class picoRowCut;
  class BCMipNode;

  class lpSetupObjCuts : public lpSetupObj
    {
    public:

      // Store the extra information needed by setBasisWithCuts

      BasicArray<picoRowCut*> persistentCuts;
      BasicArray<int>         persistentBasicSlack;
      BasicArray<picoRowCut*> addedBindingCuts;
      BasicArray<int>         addedBindingSlack;

      // Constructors and destructor

      lpSetupObjCuts() { };

      lpSetupObjCuts(BCMipNode* sp);

      virtual ~lpSetupObjCuts();

      // Configure an LP

      void setupLP(__LPSolver_Interface* lp);

      // Grab all binding and persistent cuts out of a subproblem
      // Static because we might want to invoke it without making
      // an object.

      static void findCuts(BCMipNode* sp,
			   BasicArray<picoRowCut*>& persistCuts,
			   BasicArray<int>&         persistSlack,
			   BasicArray<picoRowCut*>& addedCuts,
			   BasicArray<int>&         addedSlack);

      // Static method to load in both cuts and basis

      static void setBasisAndCuts(__LPSolver_Interface*    lp,
				  BasicArray<picoRowCut*>& persistCuts,
				  BasicArray<int>&         persistSlack,
				  BasicArray<picoRowCut*>& addedCuts,
				  BasicArray<int>&         addedSlack,
				  BasisArray               basis);

    protected:

      // Once we've made an object, we'll need to increment the
      // reference counters of the cuts it uses.

      void increaseRef(BasicArray<picoRowCut*>& cutList);

      // The destructor will need to decrement these reference counters
      // This method will automotically delete cuts with a count of zero

      void decreaseRef(BasicArray<picoRowCut*>& cutList);
  
    }; 
    
} // namespace pico

#endif
