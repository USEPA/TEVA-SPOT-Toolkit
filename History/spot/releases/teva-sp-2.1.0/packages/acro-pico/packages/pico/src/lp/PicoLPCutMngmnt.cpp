/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// PicoLPCutMngmnt.cpp
//

#include <acro_config.h>
#include <pico/PicoLPCutMngmnt.h>
#include <pico/lpEnum.h>

namespace pico {


void PicoLPCutMngmnt::execAddRows(BasicArray<picoRowCut*>& cutlist, int num_rows)
{
for (size_type i=0; i< (size_type)num_rows; i++) {
  cutlist[i]->lp_cut_index = rowNameCtr;
  PicoLPInterface::addRow(cutlist[i]->lpform(), cutlist[i]->lb(),
			  cutlist[i]->ub());
  }
}



void PicoLPCutMngmnt::replaceRows(BasicArray<picoRowCut*>& cutlist)
{
//
// Delete all of the rows
//
deleteRows(num_core_rows,getNumRows()-1);

//
// Add the current cutlist
//
addRows(cutlist, cutlist.size());
}


// This sets the core variables from the basis.  All the slack
// variables for the additional cuts are set to nonbasic except for
// the basic ones (from degenerate binding or nonbinding cuts).
// basic_list (1 or 2) is a list of the indices (in cutlist 1 or 2) of
// the cuts with basic slack variables.  The first pair is for
// persistent cuts, the second for binding (normal) cuts.  The
// "nonbasic" setting for row slacks/auxilliary variables is
// interpreted by each solver individually.  For CLP, cplex, etc, that
// use slacks (Ax + Iy = b representation), then this is usually
// atLower.  For solvers like glpk that use auxilliary variables set
// to Ax and set bounds on these variables, the value may be
// atUpper. Warning: rows must be loaded into the LP before calling
// this.

void PicoLPCutMngmnt::setBasisWithCuts(BasicArray<picoRowCut*>& cutlist1,
				       BasicArray<int>& basic_list1,
				       BasicArray<picoRowCut*>& cutlist2,
				       BasicArray<int>& basic_list2,
				       BasisArray& basis)
{
//
// Setup the basis array.
//

size_type num_rows = getNumRows();
size_type num_cols = getNumCols();
size_type basis_size = num_rows + num_cols;

// Make sure this leaves the current data intact
if (basis.size() < basis_size)
   basis.resize(basis_size);
size_type i;
// Most will be at lower
for (i= num_cols + num_core_rows; i< basis_size; i++)
  basis.put(i,nonbasic);
size_type stop_point = basic_list1.size();
for (i=0; i<stop_point; i++)
  basis.put(num_cols + getCurrRowNum(*cutlist1[basic_list1[i]]),basic);
stop_point = basic_list2.size();
for (i=0; i<stop_point; i++)
  basis.put(num_cols + getCurrRowNum(*cutlist2[basic_list2[i]]),basic);
setBasis(basis);
}

  /// Put the (integer) ranks for the cuts in cutlist into the rankList.

void PicoLPCutMngmnt::getRanks(BasicArray<picoRowCut *>& cutlist, IntVector &rankList)
{
size_type num_to_delete = cutlist.size();
if (rankList.size() != num_to_delete)
  rankList.resize(num_to_delete);
for (size_type i = 0; i < num_to_delete; i++)
  {
    rankList[i] = rowInfo.rank(LPInfoKey(cutlist[i]->lp_cut_index,0));
  }
}

} // namespace pico
