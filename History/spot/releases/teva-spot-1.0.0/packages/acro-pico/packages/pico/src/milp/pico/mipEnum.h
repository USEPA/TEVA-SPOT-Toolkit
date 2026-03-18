/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file mipEnum.h
 *
 * Defines enumeration types for MIPs.
 */

#ifndef pico_mipEnum_h
#define pico_mipEnum_h

#include <acro_config.h>
#include <utilib/std_headers.h>
#include <utilib/enum_def.h>

namespace pico {


// Types of variables (Column)
enum variableType 
{
	continuous=0,
	binary=1,
	generalInteger=2,
	initiallyFixed=3
};

// Types of constraints (Rows)
enum constraintGroup
{
		///
	ROW_TYPE_INVALID=0,

		/// mixed vars, rhs!=0
	ROW_TYPE_MIXUB=1,
		/// mixed vars, rhs!=0
	ROW_TYPE_MIXEQ=2,

		/// no binary vars
	ROW_TYPE_NOBINUB=3,
		/// no binary vars
	ROW_TYPE_NOBINEQ=4,

		/// binary vars
	ROW_TYPE_ALLBINUB=5,
		/// binary vars
	ROW_TYPE_ALLBINEQ=6,

		/// mixed vars, conditional upper bounds on sum of variables
	ROW_TYPE_SUMVARUB=7,
		/// mixed vars, conditional equality on sum of variables
	ROW_TYPE_SUMVAREQ=8,

		/// variable upper bound
	ROW_TYPE_VARUB=9,
		/// variable equality
	ROW_TYPE_VAREQ=10,
		/// variable lower bound
	ROW_TYPE_VARLB=11,

		/// binary vars, rhs=\sum {rval[j] | rval[j]<0} - rval[k]
	ROW_TYPE_BINSUMVARUB=12,
		/// binary vars, rhs=\sum {rval[j] | rval[j]<0} - rval[k]
	ROW_TYPE_BINSUMVAREQ=13,

		/// binary vars, all but one coeffs -1/1 rhs=-bncnt
	ROW_TYPE_BINSUM1VARUB=14,
		/// binary vars, all but one coeffs -1/1 rhs=-bncnt
	ROW_TYPE_BINSUM1VAREQ=15,

		// binary vars, -1/1 coeffs, rhs=-bncnt+1
		// Note: feasibility maintained if at most one of the bv's 
		// w/negative coeffs is false.
	ROW_TYPE_BINSUM1UB=16,
		// binary vars, -1/1, rhs=-bncnt+1 or rhs=bpcnt-1
		// Note: feasibility maintained if at least one of the bv's 
		// w/positive coeffs is false.
	ROW_TYPE_BINSUM1EQ=17,
		// binary vars, -1/1 coeffs, rhs=bpcnt-1
	ROW_TYPE_BINSUM1LB=18,

		// binary vars, -1/1 coeffs
	ROW_TYPE_BINSUM1UBK=19,
		// binary vars, -1/1 coeffs
	ROW_TYPE_BINSUM1EQK=20,

		// nzcnt=0
	ROW_TYPE_UNCLASSIFIED=21
};

} // namespace pico

ENUM_STREAMS( pico::constraintGroup )

ENUM_STREAMS( pico::variableType )

#endif
