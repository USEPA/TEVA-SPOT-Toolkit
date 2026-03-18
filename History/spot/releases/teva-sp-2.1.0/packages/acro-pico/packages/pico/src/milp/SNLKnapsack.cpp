/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// SNLKnapsack.cpp
//
// Generator for SNL-style knapsack cuts
//

#include <acro_config.h>
#include <pico/SNLKnapsack.h>
#include <pico/BCMip.h>

using namespace std;

namespace pico {

bool SNLKnapsack::instanceApplies()
      {
	if (MIPptr->BCMIP->numGeneralIntVars() ==0)
	  return true;
	else return false;
      }

//current assumption is that ALL variables are binary (x\in {0,1}); 
void SNLKnapsack::generateCuts(const PicoLPInterface &lp, BasicArray<picoRowCut *> &new_cuts,
			       int & num_cuts, DoubleVector &lp_solution, bool &problemInfeasible)
  {
    // Assume for now that this cut finder will not determine infeasibility
  problemInfeasible = false;
  const CoinPackedMatrix *A = lp.getMatrixByRow();
  BFLAG *bar_flags,cut_found;
  OsiRowCut *Ocut;
  picoRowCut* cut;
  double ks_bnd = MIPptr->initial_snlks_bound;
  double reduction_rate = MIPptr->snlks_reduction_rate;
  double low_b, up_b, const_value;
  if (A->isColOrdered())
    EXCEPTION_MNGR(runtime_error,"SNL Knapsack cut finder needs row-ordered matrix.  Time to implement a copy\n");
  //    A->reverseOrdering(); //command for transposing the matrix
  
  const int *st = A->getVectorStarts();
  const int *len = A->getVectorLengths();
  const int *ind = A->getIndices();
  const int *const_ind;
  const double* elm = A->getElements();
  int num_r = A->getNumRows();
  int num_c = A->getNumCols();
  double *const_elems;
  register int i,j,k,tmpc,bar_mem,nc_len = (int)new_cuts.size();
  num_cuts = 0;

  bar_flags = new BFLAG[bar_mem = 1+((num_c-1)>>3)]; //allocate memory for x bar flags
  const_elems = new double[num_c];
  for (i=0;i<num_r;++i)
    {
    for (i=0;i<bar_mem;++i) bar_flags[i] = 0;
    //For each constraint,test if simple Knapsack cutting plane applies; if it does add it to the row cut list

    //step 1: make sure constraint is in correct form: all coefficients non-negative and a greater upper bound only
    const_ind = ind+st[i];
    low_b = lp.getRowLower(i);
    up_b = lp.getRowUpper(i);
    tmpc = len[i];
    
    //put constraint into proper form
    if ((low_b!=0.0) && (up_b == 0.0))
      {
      up_b = -1.0*low_b;
      low_b = 0.0;
      //only using/storing non-zero coefficients
      for (j=0;j<tmpc;++j)
        {
        if (elm[st[i]+j]>0.0) 
          {
          up_b += elm[st[i]+j];
          bar_flags[(j>>3)] = BF_FLIP(bar_flags[j>>3],j&0x7);
          const_elems[j] = elm[st[i]+j];
          }
        else const_elems[j] = (-1.0)*elm[st[i]+j];
        }
      }
    else if ((low_b==0.0) && (up_b!=0.0))
      {
      for (j=0;j<tmpc;++j)
        {
        if (elm[st[i]+j]<0.0)
          {
          const_elems[j] = (-1.0)*elm[st[i]+j];
          up_b += const_elems[j];
          bar_flags[(j>>3)] = BF_FLIP(bar_flags[j>>3],j&0x7);
          }
        else const_elems[j] = elm[st[i]+j];
        }
      }  //end upper bound -- need not change

    //constraint is now in it's proper form -- all coefficients >=0 and bounded by an upper bound
    if (up_b>0.0) 
      {
      cut_found = BF_1OFF(cut_found);
      while (!BF_1(cut_found) && !BF_2(cut_found) && ks_bnd>0.001) //first bit of cut_found indicates if a cut was found; second bit if one was found but was a duplicate
        {
        for (j=0;j<tmpc;++j) if ((lp_solution[const_ind[j]]>=ks_bnd) && (up_b>=const_elems[j])) // move it!
          {
          up_b -= const_elems[j];
          const_elems[j] = 0.0;
          }
        for (j=0;j<tmpc;++j)
          {
          if (const_elems[j]>=up_b) const_elems[j] = 1.0; 
          else if (const_elems[j]>0.0) const_elems[j] /= up_b;
          }
        up_b = 1.0;
        
        //check for violation
        const_value = 0.0;
        for (j=0;j<tmpc;++j) if ((lp_solution[const_ind[j]]<ks_bnd) && (const_elems[j]>0.0)) 
          const_value += (lp_solution[const_ind[j]]*const_elems[j]);
        if (const_value<1.0) //violation!!
          {
          //convert any x-bar (x-bar= 1-x) elements back to x
          for (j=0;j<tmpc;++j) if (BF_IS_ON(bar_flags[j>>3],j&0x7)) 
            {
            const_elems[j]*=-1.0;
            up_b += const_elems[j];
            bar_flags[j>>3] = BF_OFF(bar_flags[j>>3],j&0x7);
            }
          //normalize so that the bound is 1
          if ((up_b!=0.0) && (up_b!=1.0) || (up_b!=-1.0))
            {
            low_b = ((up_b<0.0) ? -1.0 : 1.0)/up_b;
            for (j=0;j<tmpc;++j) const_elems[j] /= low_b;
            up_b /=low_b;
            low_b = 0.0;
            } 
          //make sure the upper bound is positive
          if (up_b<0.0)
            {
            low_b = up_b*-1.0;
            up_b = 0.0;
            for (j=0;j<tmpc;++j) const_elems[j] *= -1.0;
            }
          //check that the row does not already exist
          cut_found = BF_2OFF(cut_found);
          for (k=0;!BF_2(cut_found) && k<num_r;++k)
            {
            if ((up_b == lp.getRowUpper(k)) && (low_b==lp.getRowLower(k)))
              {
              cut_found = BF_2ON(cut_found);
              for (j=0;BF_2(cut_found) && j<len[k];++j)
                if (const_elems[j] != elm[st[k]+j]) cut_found = BF_2OFF(cut_found);
              }
            }
          if (!BF_2(cut_found))
            {
            //copy constraint to return array
            Ocut = new OsiRowCut();
            Ocut->setLb(low_b);
            Ocut->setUb(up_b);
            Ocut->setRow(tmpc,const_ind,const_elems);

            if (nc_len<=num_cuts)
              {
              new_cuts.resize(num_cuts + 5);
              nc_len = (int)new_cuts.size();
              }
            cut = new picoRowCut(MIPptr, Ocut, cutType(), cutName());
            new_cuts[num_cuts++] = cut;
            cut_found = BF_1ON(0);
            }
          } // end adding cut
        }       
      //update -- reduce -- the knapsack cut bound
      ks_bnd = ks_bnd - (ks_bnd*reduction_rate);
      } // end while the ks bound is still large enough
    } // end for each constraint
  }

} // namespace pico
