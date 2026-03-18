/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file PicoGlpkLP.h
 *
 * Defines GLPK interface stuff.
 */

#ifndef pico_PicoGlpkLP_h
#define pico_PicoGlpkLP_h

#include <acro_config.h>
#ifdef ACRO_USING_GLPK

#include <utilib/std_headers.h>
#include <OsiGlpkSolverInterface.hpp>
#include <pico/PicoLPCutMngmnt.h>
#include <pico/PicoLPSubclass.h>

namespace pico
  {
  template<>
  PicoLPSubclass<OsiGlpkSolverInterface,PicoLPCutMngmnt>::PicoLPSubclass()
  {
    solverMethod = primalSimplex;
  }

  template <>
  void PicoLPSubclass < OsiGlpkSolverInterface, PicoLPCutMngmnt> ::setRowBounds(int elementIndex, double lower, double upper )
  {
    OsiGlpkSolverInterface::setRowBounds(elementIndex,lower,upper);
  }

  // CAP: compilers seem to need this at least for now.
  template <>
  void PicoLPSubclass<OsiGlpkSolverInterface, PicoLPCutMngmnt>::initialize()
{}

  template <>
  bool PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt >::isTimeLimitReached () const
    {
      LPX *const model = getMutableModelPtr ();
      double tmlim = lpx_get_real_parm (model, LPX_K_TMLIM);

      if (fabs (tmlim) < 1e-8)
        return true;
      else
        return false;

    }

  template <>
  bool PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt >::isProvenPrimalUnbounded () const
    {
      LPX *model = getMutableModelPtr ();
      int prim = lpx_get_prim_stat (model);
      int dual = lpx_get_dual_stat (model);

      if (prim == LPX_P_FEAS && dual == LPX_D_NOFEAS)
        return true;
      else
        return false;
    }

  template <>
  bool PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt >::isProvenDualUnbounded () const
    {
      LPX *model = getMutableModelPtr ();
      int prim = lpx_get_prim_stat (model);
      int dual = lpx_get_dual_stat (model);

      if (prim == LPX_P_NOFEAS && dual == LPX_D_FEAS)
        return true;
      else
        return false;
    }

  template <>
  void PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt >::getBasis (BasisArray & basisBuffer) const
    {
      LPX *model = getMutableModelPtr ();

      /* Get the basis status for all the structural variables */
      for (int i = 0; i < getNumCols (); i++)
        switch (lpx_get_col_stat (model, i + 1))
          {
          case LPX_BS:
            basisBuffer.put (i, basic);
            break;
          case LPX_NL:
            basisBuffer.put (i, atLower);
            break;
          case LPX_NU:
            basisBuffer.put (i, atUpper);
            break;
          default:
            basisBuffer.put (i, nonbasic);
            break;
          }

      /* Get the basis status for all the auxillary variables */
      for (int j = 0; j < getNumRows (); j++)
        switch (lpx_get_row_stat (model, j + 1))
          {
          case LPX_BS:
            basisBuffer.put (getNumCols () + j, basic);
            break;
          case LPX_NL:
            basisBuffer.put (getNumCols () + j, atLower);
            break;
          case LPX_NU:
            basisBuffer.put (getNumCols () + j, atUpper);
            break;
          default:
            basisBuffer.put (getNumCols () + j, nonbasic);
            break;
          }
    }

  template <>
  basisState PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt >::getBasis (int whichVariable) const
    {
      LPX *model = getMutableModelPtr ();
      int var;

      // If we need an structural variable
      if (whichVariable < getNumCols ())
        var = lpx_get_col_stat (model, whichVariable + 1);

      // If we need an auxillary variable
      else
        var =
          lpx_get_row_stat (model, whichVariable - getNumCols () + 1);

      // Convert to the correct return code
      switch (var)
        {
        case LPX_BS:
          return basic;
        case LPX_NL:
          return atLower;
        case LPX_NU:
          return atUpper;
        default:
          return nonbasic;
        }
    }


  template <>
  void PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt >::setBasis (BasisArray & basisBuffer)
  {
    LPX *model = getMutableModelPtr ();

    /* Set the basis status of all the structural variables */
    for (int i = 0; i < getNumCols (); i++)
      switch (basisBuffer (i))
        {
        case basic:
          lpx_set_col_stat (model, i + 1, LPX_BS);
          break;
        case atLower:
          lpx_set_col_stat (model, i + 1, LPX_NL);
          break;
        case atUpper:
          lpx_set_col_stat (model, i + 1, LPX_NU);
          break;
        default:
          lpx_set_col_stat(model, i+1,LPX_NL);
          break;
        }

    /* Set the basis status for all the auxillary variables */
    for (int j = 0; j < getNumRows (); j++)
      switch (basisBuffer (getNumCols () + j))
        {
        case basic:
          lpx_set_row_stat (model, j + 1, LPX_BS);
          break;
        case atLower:
          lpx_set_row_stat (model, j + 1, LPX_NL);
          break;
        case atUpper:
          lpx_set_row_stat (model, j + 1, LPX_NU);
          break;
        default:
	  if(j < num_core_rows)
		  lpx_set_row_stat (model, j + 1, LPX_NL);
	  else
		  lpx_set_row_stat (model, j + 1, LPX_NU);
          break;
        }
  }


  template <>
  priceRule PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt >::getPricingRule ()const
    {
      LPX *model = getMutableModelPtr ();

      switch (lpx_get_int_parm (model, LPX_K_PRICE))
        {
          /* Textbook pricing */
        case 0:
          return defaultPricing;
          /* Steepest edge pricing */
        case 1:
          return steepestEdgePricing;
        default:
          EXCEPTION_MNGR (std::runtime_error, "Unknown pricing rule" << std::endl);
          break;
        }
      return defaultPricing;
    }

  template <>
  void PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt >::setPricingRule (priceRule rule)
  {
    LPX *model = getMutableModelPtr ();
    switch (rule)
      {
        /* Textbook pricing */
      case defaultPricing:
        lpx_set_int_parm (model, LPX_K_PRICE, 0);
        break;
        /* Steepest edge pricing */
      case steepestEdgePricing:
        lpx_set_int_parm (model, LPX_K_PRICE, 1);
      default:
        EXCEPTION_MNGR (std::runtime_error, "Unknown pricing rule" << std::endl);
        break;
      }
  }

  template <>
  double PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt >::getTimeLimit () const
    {
      LPX *model = getMutableModelPtr ();
      return lpx_get_real_parm (model, LPX_K_TMLIM);
    }

  template <>
  void PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt >::setTimeLimit (double time)
  {
    LPX *model = getMutableModelPtr ();
    lpx_set_real_parm (model, LPX_K_TMLIM, time);
  }

  template <>
  clockType PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt >::getClockType ()const
    {
      return wallTime;
    }

  template <>
  void PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt >::setClockType (clockType cType)
  {
    /* GLPK only allows a time limit to be set using the wall clock */
    switch (cType)
      {
      case cpuTime:
        EXCEPTION_MNGR (std::runtime_error,
                        "GLPK does not support a time limit based on CPU time"
                        << std::endl);
        break;
      case wallTime:
        break;

      }
  }

  template <>
  void PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt >::write (char *filename, ilp_format format)
  {
    LPX *model = getMutableModelPtr ();

    switch (format)
      {
      case mps_format:
      case glpk_mps_format:
        lpx_write_mps (model, filename);
        break;
      case cplex_mps_format:
        lpx_write_cpxlp (model, filename);
        break;
      default:
        EXCEPTION_MNGR (std::runtime_error,
                        "File format not support by GLPK" << std::endl);
        break;
      }

  }

  template <>
  void PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt >::printBrokenInfo ()
  {
    LPX *model=getMutableModelPtr();
    lpx_print_prob(model,"broke.txt");

    std::cout << lpx_get_status(model) << std::endl;
    std::cout << lpx_get_prim_stat(model) << std::endl;
    std::cout << lpx_get_dual_stat(model) << std::endl;
  }

  /* There's no good way to change a single element in the constraint matrix.
  Therefore, we are forced to either change an entire row or an entire col
  in order to modify a single element. */
  template <>
  void PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt >::setMatrixCoeff (int row, int col, double newValue)
  {
    LPX *model = getMutableModelPtr();

    // Get the number of elements in our row
    int nnz_row=lpx_get_mat_row(model,row+1,NULL,NULL);

    // Get the number of elements in our col
    int nnz_col=lpx_get_mat_col(model,col+1,NULL,NULL);

    // Allocate memory for the indices and values
    int min=nnz_row<nnz_col?nnz_row:nnz_col;
    int *inds=(int*)malloc((min+2)*sizeof(int));
    double *vals=(double*)malloc((min+2)*sizeof(double));


    // Work with the row if we have less elements by row than by col
    if(nnz_row<=nnz_col)
      {
        // Grab the current row
        lpx_get_mat_row(model,row+1,inds,vals);

        // See if we have already set that particular col element
        int j;
        for(j=1;j<=nnz_row && inds[j]!=col+1;j++)
          ;

        /* If we've already set the element, change it.  Otherwise
        add a new element. */
        nnz_row=j>nnz_row?nnz_row+1:nnz_row;
        inds[j]=col+1;
        vals[j]=newValue;

        // Set the row
        lpx_set_mat_row(model,row+1,nnz_row,inds,vals);

      }
    // Work with the col if we have less elements by col than by row
    else
      {
        // Grab the current col
        lpx_get_mat_col(model,col+1,inds,vals);

        // See if we have already set that particular row element
        int i;
        for(i=1;i<=nnz_col && inds[i]!=row+1;i++)
          ;

        /* If we've already set the element, change it.  Otherwise
        add a new element. */
        nnz_col=i>nnz_col?nnz_col+1:nnz_col;
        inds[i]=row+1;
        vals[i]=newValue;

        // Set the col
        lpx_set_mat_col(model,col+1,nnz_col,inds,vals);
      }

    free(inds);
    free(vals);
  }

  typedef PicoLPSubclass < OsiGlpkSolverInterface,
  PicoLPCutMngmnt > PicoGlpkLP;
}				// namespace pico

#endif

#endif
