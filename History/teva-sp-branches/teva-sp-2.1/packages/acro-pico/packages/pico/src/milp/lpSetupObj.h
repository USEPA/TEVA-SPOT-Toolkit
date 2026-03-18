/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file lpSetupObj.h
 * 
 * Defines the pico::lpSetupObj class.
 */

#ifndef pico_lpSetupObj_h
#define pico_lpSetupObj_h

#include <acro_config.h>
#include <OsiSolverInterface.hpp>
#include <CoinWarmStartBasis.hpp>
#include <utilib/IntVector.h>
#include <utilib/DoubleVector.h>
#include <utilib/CommonIO.h>
#include <pico/BasisArray.h>


namespace pico {

  using utilib::IntVector;
  using utilib::DoubleVector;
  
  class MILPProblem;
  class MILP;
  class MILPNode;

  class lpSetupObj
    {
    public:

      MILP*        mGlobal;
      MILPProblem* MIP;

      double bound;
      double mfValue;

      DoubleVector savedSolution;
      DoubleVector boundList;
      BasisArray   savedBasis;

      lpSetupObj() { };

      virtual ~lpSetupObj() { };

      lpSetupObj(MILPNode* sp);

      virtual void setupLP(__LPSolver_Interface* lp);

      static void setBounds(MILPProblem*          MIPPtr,
			    __LPSolver_Interface* lp,
			    DoubleVector&         bList);
  
      static void setBasis(__LPSolver_Interface* lp,
			   BasisArray&           basis);

      static void makeBoundList(MILPProblem*  MIPPtr,
				MILPNode*     sp,
				DoubleVector& bList);
    }; 
    
} // namespace pico

#endif
