/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file PicoCglGomory.hpp
 * \author Cindy Phillips
 *
 * Defines the picoCglGomory class, which is a 
 * frozen version of the COIN-OR CGL Gomory cut code, modified to return
 * globally valid cuts.
 */

// This is documentation from the original COIN-OR version
// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef picoCglGomory_H
#define picoCglGomory_H

#include <string>
#include <utilib/exception_mngr.h>

#include "CglCutGenerator.hpp"

class CoinWarmStartBasis;
/** Gomory Cut Generator Class */
class picoCglGomory : public CglCutGenerator {
   friend void picoCglGomoryUnitTest(const OsiSolverInterface * siP,
				  const std::string mpdDir );
 
public:
    
  
  /**@name Generate Cuts */
  //@{
  /** Generate Mixed Integer Gomory cuts for the model of the 
      solver interface, si.

      Insert the generated cuts into OsiCut, cs.

      There is a limit option, which will only generate cuts with
      less than this number of entries.

      We can also only look at 0-1 variables a certain distance
      from integer.
  */
  virtual void generateCuts( const OsiSolverInterface & si, OsiCuts & cs,
			     const CglTreeInfo info = CglTreeInfo()) const;
  /** Generates cuts given matrix and solution etc,
      returns number of cuts generated */
  int generateCuts( const OsiRowCutDebugger * debugger, 
		    OsiCuts & cs,
		    const CoinPackedMatrix & columnCopy,
		    const double * objective, const double * colsol,
		    const double * colLower, const double * colUpper,
		    const double * rowLower, const double * rowUpper,
		    const char * intVar ,
		    const CoinWarmStartBasis* warm,
		    double solverInfinity,
                    const CglTreeInfo info = CglTreeInfo()) const;

  //@}

  /// Return true if needs optimal basis to do cuts (will return true)
  virtual bool needsOptimalBasis() const;
  //@}

  /**@name Change limit on how many variables in cut (default 50) */
  //@{
  /// Set
  void setLimit(int limit);
  /// Get
  int getLimit() const;
  /// Set at root (if <normal then use normal)
  void setLimitAtRoot(int limit);
  /// Get at root
  int getLimitAtRoot() const;
  //@}

  /**@name Change criterion on which variables to look at.  All ones
   more than "away" away from integrality will be investigated 
  (default 0.05) */
  //@{
  /// Set
  void setAway(double value);
  /// Get
  double getAway() const;
  //@}


  /**@name Constructors and destructors */
  //@{
  /// Default constructor 
  picoCglGomory ();
 
  /// Copy constructor 
  picoCglGomory (
    const picoCglGomory &);

  /// Clone
  virtual CglCutGenerator * clone() const;

  /// Assignment operator 
  picoCglGomory &
    operator=(
    const picoCglGomory& rhs);
  
  /// Destructor 
  virtual
    ~picoCglGomory ();
  //@}
      
private:
  
 // Private member methods

  // Private member data

  /**@name Private member data */
  //@{
  /// Only investigate if more than this away from integrality
  double away_;
  /// Limit - only generate if fewer than this in cut
  int limit_;
  /// Limit - only generate if fewer than this in cut (at root)
  int limitAtRoot_;
  //@}
};

//#############################################################################
/** A function that tests the methods in the picoCglGomory class. The
    only reason for it not to be a member method is that this way it doesn't
    have to be compiled into the library. And that's a gain, because the
    library should be compiled with optimization on, but this method should be
    compiled with debugging. */
void picoCglGomoryUnitTest(const OsiSolverInterface * siP,
			const std::string mpdDir );
  
#endif
