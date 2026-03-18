/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

//
// lpSetupObj.cpp
//
// 
//
// 
//

#include <acro_config.h>
#include <utilib/CommonIO.h>
#include <pico/milpNode.h>

using namespace std;

namespace pico {

  // This method configures a passed-in LP to conform to stored bounds,
  // basis, etc.  It can be overloaded to throw in cuts and other stuff too.

  void lpSetupObj::setupLP(__LPSolver_Interface* lp)
  {
    setBounds(MIP,lp,boundList);
    setBasis(lp,savedBasis);
  }
  

  // Static subroutines used in the above, which can also be used by
  // MILPNode-derived classes.

  void lpSetupObj::setBounds(MILPProblem*          MIPPtr,
			     __LPSolver_Interface* lp,
			     DoubleVector&         bList)
  {
    int* intListPtr = MIPPtr->integerVarNdx.data();
    int  numInts    = MIPPtr->numIntegerVars();
    lp->setColSetBounds(intListPtr,intListPtr+numInts,bList.data());
  }


  void lpSetupObj::setBasis(__LPSolver_Interface* lp,
			    BasisArray&           basis)
  {
    lp->setBasis(basis);
  }


  // Creates a new lpSetupObj using information in a MILP subproblem

  lpSetupObj::lpSetupObj(MILPNode* sp) :
    mGlobal(sp->mGlobal()),
    MIP(sp->mGlobal()->MIP),
    bound(sp->lpBound),
    mfValue(sp->mfValue),
    savedSolution(sp->solution),
    boundList(2*(MIP->numIntegerVars()))
  {
    makeBoundList(MIP,sp,boundList);
    savedBasis.resize(sp->basis.size());
    savedBasis << sp->basis;
  }

      
  // Set up bound list.  Note that this does *not* narrow bounds
  // on general integer variables -- bound modifications are
  // the responsibility of the caller.

  void lpSetupObj::makeBoundList(MILPProblem*  MIPPtr,
				 MILPNode*     sp,
				 DoubleVector& bList)
  {
#if defined(ACRO_VALIDATING) || defined(WARNINGS)
    CommonIO* debugPtr = sp->mGlobal()->mipHeuristicPtr;

    DEBUGPRX(10,debugPtr,"Setting up bound list for " << sp << endl);
#endif

    int numInts = MIPPtr->numIntegerVars();

    for (int i=0; i<numInts; i++) 
      {
	int thisVarIndex = MIPPtr->integerVarNdx[i]; //index into LP
	variableType thisVarType = MIPPtr->vtype[thisVarIndex];
	switch (thisVarType) 
	  {
	  case binary:
	    if (sp->fixed(i)) 
	      {
		double tmp = sp->binary_vals(i);
		bList[2*i] = tmp;
		bList[2*i + 1] = tmp;
	      } 
	    else 
	      {
		bList[2*i] = 0.0;
		bList[2*i + 1] = 1.0;
	      }
	    break;
	  case generalInteger:
	    bList[2*i]     = sp->intLowerBounds[MIPPtr->reverseBoundsNdx[i]];
	    bList[2*i + 1] = sp->intUpperBounds[MIPPtr->reverseBoundsNdx[i]];
	    break;
	  default:
	    EXCEPTION_MNGR(runtime_error,
			   "lpSetupObj(MILPNode*): "
			   "mismatch in integrality info, "
			   << "integerVarNdx[" << i << "]=" << thisVarIndex 
			   << " but vtype[" << thisVarIndex << "]=" 
			   << thisVarType);
	    break;
	  }
#if defined(ACRO_VALIDATING) || defined(WARNINGS)
	DEBUGPRX(100,debugPtr,'x' << thisVarIndex << ": ["
		 << bList[2*i] << ',' << bList[2*i + 1] 
		 << "], sp->solution[" << i << "]="
		 << sp->solution[i] << endl);
#endif
      }
  }


} // namespace pico
