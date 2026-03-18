/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file QSSProblem.h
 * \author William Hart
 *
 * Defines the pebbl::QSSProblem class.
 */

#ifndef QSSProblem_h
#define QSSProblem_h

//
// Defines data structures needed for a quadratic semi-assignment problem
//

#include <iostream>
#include <utilib/BasicArray.h>
#include <utilib/exception_mngr.h>
#include <utilib/CharString.h>
#include <utilib/sort.h>
#include <utilib/BitArray.h>
#include <utilib/_math.h>


//#define DEBUG_BOUND

using namespace utilib;


class IntraInfo
{
public:

  IntraInfo() : n(0) {}

  BasicArray<unsigned int> r;

  BasicArray<unsigned int> s;

  BasicArray<double> e;

  int n;

  BasicArray<int> order;

  BasicArray<BasicArray<int> > index;
};


class QSSProblem
{
public:

  QSSProblem() : n(0), Edefault(0.0), Etolerance(1e-7) {}

  void read(const char* filename, bool rotamer_weighting_flag);
  void read_old(std::istream& is, bool rotamer_weighting_flag);

  double find_greedy(BasicArray<int>& x, int maxiters=1000);

  /*
  double inter(int i, int r)
	{return E_inter[i][r];}

  double intra(int i, int r, int j, int s)
	{return E_inter[i][r];}
	*/

  double energy(BasicArray<int>& x);

  double eval_move(BasicArray<int>& x, unsigned int i, int r, double ans);

  double compute_simple_bound(BasicArray<BitArray>& free_rotamers);

  double compute_bound(BasicArray<BitArray>& free_rotamers, int type)
	  	{ if (type == 0) return compute_simple_bound(free_rotamers);
		if (type == 2) return compute_bound2(free_rotamers);
		return compute_bound(free_rotamers);
		}
  double compute_bound(BasicArray<BitArray>& free_rotamers);
  double compute_bound2(BasicArray<BitArray>& free_rotamers);

  Ereal<double> compute_min_rotamer(unsigned int i, unsigned int k,
					BasicArray<BitArray>& free_rotamers);
  Ereal<double> compute_min_rotamer2(unsigned int i, unsigned int k,
					BasicArray<BitArray>& free_rotamers);

  unsigned int n;

  BasicArray<int> rCount;

  BasicArray<int> rorder;

  BasicArray<BasicArray<double> > E_inter;

  BasicArray<BasicArray<int> > inter_order;

  BasicArray<BasicArray<int> > weighted_inter_order;

  BasicArray<BasicArray<IntraInfo> > E_intra;

  double Edefault;

  double Etolerance;

  BasicArray<BasicArray<BasicArray<double> > > bound2_info;
  BasicArray<BasicArray<BasicArray<unsigned int> > > bound2_ctr;
};


double QSSProblem::energy(BasicArray<int>& x)
{
//	std::cerr << "POINT: " << x << std::endl;
double ans=0.0;

for (unsigned int i=0; i<n; i++)
  if (rCount[i] > 0)	 {
     ans += E_inter[i][ x[i] ];
   //  std::cerr << "HERE " << (i+1) << " " << (x[i]+1) << " " << E_inter[i][x[i]] << std::endl;
     }
  //else
     //std::cerr << "HERE " << (i+1) << " " << (x[i]+1) << " " << 0.0 << std::endl;

for (unsigned int i=0; i<n; i++) {
  if (rCount[i] == 0) continue;
  for (unsigned int j=i+1; j<n; j++) {
    if (rCount[j] == 0) continue;
    IntraInfo& info = E_intra[i][j];
    int k = info.index[ x[i] ][ x[j] ];
    if (k != -1) {
       ans += info.e[ k ];
//       std::cerr << "HERE " << (i+1) << " " << (x[i]+1) << " " << (j+1) << " " << (x[j]+1) << " " << k << " " << info.e[k] << std::endl;
       }
 //   else
  //     std::cerr << "HERE " << (i+1) << " " << (x[i]+1) << " " << (j+1) << " " << (x[j]+1) << " " << k << " " << std::endl;
    }
  }

//std::cerr << "VALUE " << ans << std::endl;
return ans;
}



double QSSProblem::compute_simple_bound(BasicArray<BitArray>& free_rotamers)
{
double ans=0.0;

if (free_rotamers.size() != n)
   EXCEPTION_MNGR(runtime_error,"Bad list length for free_rotamers")

for (unsigned int i=0; i<n; i++)
  if ((rCount[i] > 0) && (free_rotamers[i].nbits() == 0))
     EXCEPTION_MNGR(runtime_error, "No free bits for amino acid " << i)

for (unsigned int i=0; i<n; i++) {
  if (rCount[i] == 0) continue;
  //std::cerr << i << " " << E_inter[i][ inter_order[i][x[i]] ] << std::endl;
  unsigned int k=0;
  //bool first=false;
  //double minval=0.0;
  while (k<inter_order[i].size()) {
#ifdef DEBUG_BOUND
    std::cerr << i << " " << E_inter[i][ inter_order[i][k] ] << " " << inter_order[i][k] << std::endl;
#endif
    if (free_rotamers[i](inter_order[i][k]))
       break;
    k++;
    }

#ifdef DEBUG_BOUND
  std::cerr << i << " " << E_inter[i][ inter_order[i][k] ] << std::endl;
#endif
  ans += E_inter[i][ inter_order[i][k] ];
  }

for (unsigned int i=0; i<n; i++) {
  if (rCount[i] == 0) continue;
  for (unsigned int j=i+1; j<n; j++) {
    if (rCount[j] == 0) continue;
    IntraInfo& info = E_intra[i][j];
    int k=-1;
    unsigned int num_feasible=0;
    for (unsigned int kk=0; kk<info.order.size(); kk++) {
      if (free_rotamers[i](info.r[ info.order[kk]] )  && 
	  free_rotamers[j](info.s[ info.order[kk]] ) ) {
         num_feasible++;
         if (k == -1) {
	    k = kk;
	    if (info.e[info.order[k]] < 0.0) break;
	    }
         }
      }
#ifdef DEBUG_BOUND
      //std::cerr << i << " " << j << " " << info.e[info.order[k]] << " " << info.r[ info.order[k]] << " " << info.s[info.order[k]] << " " << k << std::endl;
#endif

    if ((num_feasible > 0) && 
	((info.e[info.order[k]] < 0.0) ||
	 (num_feasible == free_rotamers[i].nbits()*free_rotamers[j].nbits()))) {
       ans += info.e[ info.order[k] ];
#ifdef DEBUG_BOUND
       std::cerr << ans << std::endl;
       std::cerr << i << " " << j << " " << info.e[info.order[k]] << " " << k << std::endl;
#endif
       }
    }
  }

return ans;
}


// Bound the self energies as before.  Generate an energy value for 
// each rotamer
double QSSProblem::compute_bound2(BasicArray<BitArray>& free_rotamers)
{
double ans=0.0;

//
// Check for errors
//
if (free_rotamers.size() != n)
   EXCEPTION_MNGR(runtime_error,"Bad list length for free_rotamers")
for (unsigned int i=0; i<n; i++)
  if ((rCount[i] > 0) && (free_rotamers[i].nbits() == 0))
     EXCEPTION_MNGR(runtime_error, "No free bits for amino acid " << i)

//
// Bound the self-energies
//
for (unsigned int i=0; i<n; i++) {
  if (rCount[i] == 0) continue;
  //std::cerr << i << " " << E_inter[i][ inter_order[i][x[i]] ] << std::endl;
  unsigned int k=0;
  while (k<inter_order[i].size()) {
#ifdef DEBUG_BOUND
    std::cerr << i << " " << E_inter[i][ inter_order[i][k] ] << " " << inter_order[i][k] << std::endl;
#endif
    if (free_rotamers[i](inter_order[i][k]))
       break;
    k++;
    }
#ifdef DEBUG_BOUND
  std::cerr << i << " " << E_inter[i][ inter_order[i][k] ] << std::endl;
#endif
  ans += E_inter[i][ inter_order[i][k] ];
  }

//
// Bound the internal energies
//
for (unsigned int i=0; i<n; i++) {
  for (int j=0; j<rCount[i]; j++) {
    bound2_info[i][j] << 0.0;
    bound2_ctr[i][j] << static_cast<unsigned int>(0);
  }
}

for (unsigned int i=0; i<n; i++) {
  if (rCount[i] == 0) continue;
  BasicArray<BasicArray<double> >&       info_i = bound2_info[i];
  BasicArray<BasicArray<unsigned int> >& ctr_i = bound2_ctr[i];
  for (unsigned int j=i+1; j<n; j++) {
    if (rCount[j] == 0) continue;
    BasicArray<BasicArray<double> >&       info_j = bound2_info[j];
    BasicArray<BasicArray<unsigned int> >& ctr_j = bound2_ctr[j];
    IntraInfo& info = E_intra[i][j];
    unsigned int termination_ctr=0;
    unsigned int termination_val=
	    		free_rotamers[i].nbits()+free_rotamers[j].nbits();
    bool no_zeros = (info.order.size() == free_rotamers[i].nbits()*free_rotamers[j].nbits());
    bool positive_val=false;
    for (unsigned int k=0; 
	    (k<info.order.size()) && (termination_ctr < termination_val); k++) {
      if (free_rotamers[i](info.r[ info.order[k]] )  && 
	  free_rotamers[j](info.s[ info.order[k]] ) ) {

         double val = info.e[info.order[k]];
         if ((ctr_i[ info.r[ info.order[k] ] ][j] == 0) ||
             (val < info_i[ info.r[ info.order[k] ] ][j])) {
            info_i[ info.r[ info.order[k] ] ][j] = val;
	    positive_val = positive_val | (val > 0.0);
	    }
         ctr_i[ info.r[ info.order[k] ] ][j]++;
	 if ((ctr_i[ info.r[ info.order[k] ] ][j] == 1) &&
             (no_zeros || (info_i[ info.r[ info.order[k] ] ][j] < 0.0)))
	    termination_ctr++;

         if ((ctr_j[ info.s[ info.order[k] ] ][i] == 0) ||
             (val < info_j[ info.s[ info.order[k] ] ][i])) {
            info_j[ info.s[ info.order[k] ] ][i] = val;
	    positive_val = positive_val | (val > 0.0);
            }
         ctr_j[ info.s[ info.order[k] ] ][i]++;
	 if ((ctr_j[ info.s[ info.order[k] ] ][i] == 1) &&
             (no_zeros || (info_j[ info.s[ info.order[k] ] ][i])))
	     termination_ctr++;
         }
    }
    if (positive_val && (termination_ctr < termination_val)) {
       for (unsigned int k=0; k<ctr_i.size(); k++)
         if ((ctr_i[ k ][j] == 0) ||
             ((ctr_i[ k ][j] < free_rotamers[j].nbits()) &&
              (info_i[ k ][j] > 0.0)))
            info_i[ k ][j] = 0.0;
       for (unsigned int k=0; k<ctr_j.size(); k++)
         if ((ctr_j[ k ][i] == 0) ||
             ((ctr_j[ k ][i] < free_rotamers[i].nbits()) &&
              (info_j[ k ][i] > 0.0)))
            info_j[ k ][i] = 0.0;
       }
  }
}
for (unsigned int i=0; i<n; i++) {
  if (rCount[i] == 0) continue;
  BasicArray<BasicArray<double> >&       info_i = bound2_info[i];
  Ereal<double> minval = Ereal<double>::positive_infinity;
  for (int kk=0; kk<rCount[i]; kk++) {
    double tmp = utilib::sum(info_i[kk])/2.0;
    if (tmp < minval)
       minval = tmp;
    //std::cerr << tmp << " " << minval << std::endl;
    }
  ans += minval;
  //std::cerr << "ANS " << ans << " " << minval << std::endl;
}

return ans;
}


double QSSProblem::compute_bound(BasicArray<BitArray>& free_rotamers)
{
if (free_rotamers.size() != n)
   EXCEPTION_MNGR(runtime_error,"Bad list length for free_rotamers")

for (unsigned int i=0; i<n; i++)
  if ((rCount[i] > 0) && (free_rotamers[i].nbits() == 0))
     EXCEPTION_MNGR(runtime_error, "No free bits for amino acid " << i)

Ereal<double> bound_val=0.0;
double tmpn = (double)n;

for (unsigned int i=0; i<n; i++) {
  if (rCount[i] == 0) continue;
#ifdef DEBUG_BOUND
  ucout << "SideChain " << i << std::endl;
#endif
  unsigned int k=0;
  for (; k<inter_order[i].size(); k++) {
    if (free_rotamers[i](k))
       break;
    }
  Ereal<double> minval = E_inter[i][k]/tmpn + compute_min_rotamer(i,k,free_rotamers);
#ifdef DEBUG_BOUND
  ucout << "  Rotamer " << k << "  ";
  ucout << "Best: " << minval << "\tPrev: Infty" << std::endl;
#endif
  //int    minndx = k;
  k++;
  while (k < inter_order[i].size()) {
    if (!free_rotamers[i](k)) { k++; continue; }
    Ereal<double> tmp = E_inter[i][k]/tmpn + compute_min_rotamer(i,k,free_rotamers);
#ifdef DEBUG_BOUND
    ucout << "  Rotamer " << k << "  ";
#endif
    if (tmp < minval) {
#ifdef DEBUG_BOUND
       ucout << "Best: " << tmp << "\tPrev: " << minval << std::endl;
#endif
       minval = tmp;
       }
#ifdef DEBUG_BOUND
    else
       ucout << "Best: " << minval << "\tPrev: " << minval << std::endl;
#endif
    k++;
    }
  bound_val += minval;
#ifdef DEBUG_BOUND
  ucout << "BOUND: " << bound_val << std::endl;
#endif
  }

#if 0
double test = compute_bound2(free_rotamers);
if (test > bound_val + 1e-7) {
   ucout << "Bound val: " << bound_val << " " << test << std::endl;
   test = compute_simple_bound(free_rotamers);
   ucout << "Bound val: " << bound_val << " " << test << std::endl;
   if (test > bound_val + 1e-7)
      EXCEPTION_MNGR(runtime_error, "Something weird is going on in the bounder! " << (test-bound_val))
   }
#endif
return bound_val;
}


Ereal<double> QSSProblem::compute_min_rotamer(unsigned int i, unsigned int k,
					BasicArray<BitArray>& free_rotamers)
{
double bound_val=0.0;
double tmpn = (double)n;

for (unsigned int j=0; j<n; j++) {
  if (rCount[j] == 0) continue;
  if (j == i) continue;

  IntraInfo* info;
  if (i<j) info = &E_intra[i][j];
  else     info = &E_intra[j][i];

  bool first=true;
  Ereal<double> minval=Ereal<double>::positive_infinity;
  for (unsigned int kk=0; kk<info->order.size(); kk++) {
    if ((i < j) && free_rotamers[j](info->s[kk]) && (info->r[kk]==k))
       if (first || (info->e[kk]/2.0 + E_inter[j][info->s[kk]]/tmpn < minval)) {
	  minval = info->e[kk]/2.0 + E_inter[j][info->s[kk]]/tmpn;
#ifdef DEBUG_BOUND
	  ucout << i << " " << j << " " <<minval << std::endl;
#endif
	  first=false;
          }
    if ((i > j) && free_rotamers[j](info->r[kk]) && (info->s[kk]==k))
       if (first || (info->e[kk]/2.0 + E_inter[j][info->r[kk]]/tmpn < minval)) {
	  minval = info->e[kk]/2.0 + E_inter[j][info->r[kk]]/tmpn;
#ifdef DEBUG_BOUND
	  ucout << i << " " << j << " " <<minval << std::endl;
#endif
	  first=false;
          }
    }
  if (first) return Ereal<double>::positive_infinity;
  bound_val += minval;
  }

return bound_val;
}


Ereal<double> QSSProblem::compute_min_rotamer2(unsigned int i, unsigned int k,
					BasicArray<BitArray>& free_rotamers)
{
double bound_val=0.0;

for (unsigned int j=0; j<n; j++) {
  if (rCount[j] == 0) continue;
  if (j == i) continue;

  IntraInfo* info;
  if (i<j) info = &E_intra[i][j];
  else     info = &E_intra[j][i];

  bool first=true;
  Ereal<double> minval=Ereal<double>::positive_infinity;
  for (unsigned int kk=0; kk<info->order.size(); kk++) {
    if ((i < j) && free_rotamers[j](info->s[kk]) && (info->r[kk]==k))
       if (first || (info->e[kk]/2.0 < minval)) {
	  minval = info->e[kk]/2.0;
#ifdef DEBUG_BOUND
	  ucout << i << " " << j << " " <<minval << std::endl;
#endif
	  first=false;
          }
    if ((i > j) && free_rotamers[j](info->r[kk]) && (info->s[kk]==k))
       if (first || (info->e[kk]/2.0 < minval)) {
	  minval = info->e[kk]/2.0;
#ifdef DEBUG_BOUND
	  ucout << i << " " << j << " " <<minval << std::endl;
#endif
	  first=false;
          }
    }
  if (first) return Ereal<double>::positive_infinity;
  bound_val += minval;
  }

return bound_val;
}


double QSSProblem::find_greedy(BasicArray<int>& x, int maxiters)
{
//
// We use 'internal coordinates' until the end, when we translate
// using the inter_order arrays
//
for (unsigned int i=0; i<n; i++) {
  if (rCount[i] == 0) x[i] = -1;
  else x[i] = 0;
  }
Ereal<double> ans=0;

//
// Compute initial energy
//
for (unsigned int i=0; i<n; i++) {
  if (rCount[i] == 0) continue;
  std::cerr << i << " " << E_inter[i][ weighted_inter_order[i][x[i]] ] << std::endl;
  ans += E_inter[i][ weighted_inter_order[i][x[i]] ];
  }

for (unsigned int i=0; i<n; i++) {
  if (rCount[i] == 0) continue;
  for (unsigned int j=i+1; j<n; j++) {
    if (rCount[j] == 0) continue;
    IntraInfo& info = E_intra[i][j];
    int k = info.index[ weighted_inter_order[i][x[i]] ][ weighted_inter_order[j][x[j]] ];

    if (k == -1) {
       ans += Edefault;
       //std::cerr << i << " " << j << " " << Edefault << " " << k << std::endl;
       }
    else {
       ans += info.e[ k ];
       //std::cerr << i << " " << j << " " << info.e[k] << " " << k << std::endl;
       }
    }
}

std::cerr << "Initial energy: " << ans << std::endl;

for (int iter=0; iter<maxiters; iter++) {

  double tans = ans;

  for (unsigned int i_=0; i_<n; i_++) {
    if (rCount[i_] == 0) continue;
    int i = rorder[i_];

    int    curr_r   = x[i];
    double curr_ans = ans;
    for (int r_=0; r_<rCount[i]; r_++) {
      if (r_ == x[i]) continue;
      int r = weighted_inter_order[i][r_];
      double tval = eval_move(x, i, r, ans);
      if (tval < curr_ans) {
         //std::cerr << "Improving: " << iter << " " << i << " " << tval << " " << r << " " << x[i] << std::endl;
         curr_r   = r_;
         curr_ans = tval;
         }
      }

    if (curr_ans < ans) {
       ans = curr_ans;
       x[i] = curr_r;
       }
    std::cerr << "Iteration: " << iter << " " << i << " " << ans << " " << x << std::endl;
    }

  if (tans == ans)
     break;
  }

for (unsigned int i=0; i<n; i++)
  if (rCount[i] > 0) x[i] = weighted_inter_order[i][x[i]];
return ans;
}



double QSSProblem::eval_move(BasicArray<int>& x, unsigned int i, int r, double ans)
{
double val = ans;
int ndx = weighted_inter_order[i][x[i]];

val -= E_inter[i][ ndx ];
for (unsigned int j=0; j<n; j++) {
  if (rCount[j] == 0) continue;
  if (j == i) continue;
  if (i < j) {
     IntraInfo& info = E_intra[i][j];
     int k = info.index[ ndx ][ weighted_inter_order[j][x[j]] ];
     if (k == -1)
	val -= Edefault;
     else
        val -= info.e[ k ];
     }
  if (i > j) {
     IntraInfo& info = E_intra[j][i];
     int k = info.index[ weighted_inter_order[j][x[j]] ][ ndx ];
     if (k == -1)
	val -= Edefault;
     else
        val -= info.e[ k ];
     }
  }

val += E_inter[i][ r ];
for (unsigned int j=0; j<n; j++) {
  if (rCount[j] == 0) continue;
  if (j == i) continue;
  if (i < j) {
     IntraInfo& info = E_intra[i][j];
     int k = info.index[ r ][ weighted_inter_order[j][x[j]] ];
     if (k == -1)
	val += Edefault;
     else
        val += info.e[ k ];
     }
  if (i > j) {
     IntraInfo& info = E_intra[j][i];
     int k = info.index[ weighted_inter_order[j][x[j]] ][ r ];
     if (k == -1)
	val += Edefault;
     else
        val += info.e[ k ];
     }
  }

return val;
}


void QSSProblem::read_old(std::istream& is, bool advanced_rotamer_weighting)
{
int state=0;

CharString tmp;
int ctr1=0,ctr2=0;

while (1) {
  is >> tmp;
  if (!is) break;

  switch (state) {
	case 0:
		{
		if (tmp != "param") break;
		is >> tmp;
		if (tmp != "n") EXCEPTION_MNGR(runtime_error,"Badly formated input - 0")
		is >> tmp;
		is >> tmp;
		if (tmp[tmp.size()-1] == ';')
		   tmp.resize(tmp.size()-1);
		n = atoi(tmp.data());
		state = 1;
		}
		break;

	case 1:
		{
		if (tmp != "param") break;
		is >> tmp;
		if (tmp != "rCount") EXCEPTION_MNGR(runtime_error,"Badly formated input - 1")
		is >> tmp;
		rCount.resize(n);
		rCount << 0;
		ctr1=0;
		state = 2;
		}
		break;

	case 2:
		{	
		if (tmp == ";") {
		   state = 3;
		   break;
		   }
		is >> rCount[ctr1++];
		}
		break;

	case 3:
		{
		if (tmp != "param") break;
		is >> tmp;
		if (tmp != "E_inter") EXCEPTION_MNGR(runtime_error,"Badly formated input - 3")
		is >> tmp;
		is >> tmp;
		E_inter.resize(n);
		for (unsigned int i=0; i<n; i++)
		  E_inter[i].resize(rCount[i]);
		state = 4;
		}
		break;

	case 4:
		{	
		if (tmp == ";") {
		   state = 5;
		   break;
		   }
		ctr1 = atoi(tmp.data());
		is >> ctr2;
		is >> E_inter[ctr1-1][ctr2-1];
		}
		break;

	case 5:
		{
		if (tmp != "param") break;
		is >> tmp;
		if (tmp != "E_intra") EXCEPTION_MNGR(runtime_error,"Badly formated input - 5")
		is >> tmp;
		is >> tmp;
		E_intra.resize(n);
		for (unsigned int i=0; i<n; i++) {
		  E_intra[i].resize(n);
		  for (unsigned int j=0; j<n; j++) {
		    if (i < j) {
		       E_intra[i][j].r.resize(rCount[i]*rCount[j]);
		       E_intra[i][j].s.resize(rCount[i]*rCount[j]);
		       E_intra[i][j].e.resize(rCount[i]*rCount[j]);
		       }
   		    }
		  }
		state = 6;
		}
		break;

	case 6:
		{	
		if (tmp == ";") {
		   state = 7;
		   break;
		   }
		ctr1 = atoi(tmp.data());
		is >> tmp;
		is >> ctr2;
		int i = E_intra[ctr1-1][ctr2-1].n;
		E_intra[ctr1-1][ctr2-1].r[i] = atoi(tmp.data())-1;
		is >> tmp;
		E_intra[ctr1-1][ctr2-1].s[i] = atoi(tmp.data())-1;
		is >> E_intra[ctr1-1][ctr2-1].e[i];
		E_intra[ctr1-1][ctr2-1].n++;
		/*std::cerr << ctr1 << " " << ctr2 << " "
			<< E_intra[ctr1-1][ctr2-1].r[i] << " "
			<< E_intra[ctr1-1][ctr2-1].s[i] << std::endl;*/
		}
		break;

	case 7:
		break;
    };
  }

BasicArray<BitArray> free_rotamers(n);
for (unsigned int i=0; i<n; i++) {
  if (rCount[i] == 0) continue;
  free_rotamers[i].resize(rCount[i]);
  free_rotamers[i].set();
  }

//std::cerr << "rCount " << rCount << std::endl;
inter_order.resize(n);
weighted_inter_order.resize(n);
bound2_info.resize(n);
bound2_ctr.resize(n);
for (unsigned int i=0; i<n; i++) {
  if (rCount[i] == 0) continue;
  for (unsigned int j=i+1; j<n; j++) {
    if (rCount[j] == 0) continue;
    if (i < j) {
       IntraInfo& info = E_intra[i][j];
       info.index.resize(rCount[i]);
       for (int k=0; k<rCount[i]; k++) {
         info.index[k].resize(rCount[j]);
         info.index[k] << -1;
         }
       for (int k=0; k<E_intra[i][j].n; k++) {
	 //std::cerr << i << " " << j << " " << info.index.size() << " " << info.r[k] << " " << info.s[k] << std::endl;
         info.index[ info.r[k] ][ info.s[k] ] = k;
         }
       info.r.resize( info.n );
       info.s.resize( info.n );
       info.e.resize( info.n );
       info.order.resize( info.n );
       order(info.order, info.e);
       //std::cerr << std::endl;
       //std::cerr << i << " " << j << std::endl;
       //std::cerr << info.e << std::endl;
       //std::cerr << info.order << std::endl;
       }
    }
  inter_order[i].resize(rCount[i]);
  weighted_inter_order[i].resize(rCount[i]);
  BasicArray<double> E_inter_tmp(rCount[i]);
  order(inter_order[i], E_inter[i]);
  if (advanced_rotamer_weighting) {
     double tmpn = static_cast<double>(rCount[i]);
     bound2_info[i].resize(rCount[i]);
     bound2_ctr[i].resize(rCount[i]);
     for (int k=0; k<rCount[i]; k++) {
       bound2_info[i][k].resize(n);
       bound2_ctr[i][k].resize(n);
       E_inter_tmp[k] = E_inter[i][k]/tmpn + compute_min_rotamer(i,k,free_rotamers);
       }
     order(weighted_inter_order[i], E_inter_tmp);
     }
  else
     weighted_inter_order[i] << inter_order[i];
  }

rorder.resize(n);
order(rorder,rCount);

ucout << "rCount: " << rCount << std::endl;
}


void QSSProblem::read(const char* filename, bool advanced_rotamer_weighting)
{
std::ifstream is(filename);
if (!is) EXCEPTION_MNGR(runtime_error, "Bad filename \"" <<
			                                filename << "\"")

int state=0;

CharString tmp;
int ctr1=0,ctr2=0;
int pctr1=0,pctr2=0;
int maxrotamers=-1;

while (1) {
  is >> tmp;
  if (!is) break;

  switch (state) {
	case 0:
		{
		if (tmp != "param:") break;
		is >> tmp;
		if (tmp != "ValidInterIndices:") EXCEPTION_MNGR(runtime_error,"Badly formated input - 0")
		is >> tmp;
		is >> tmp;
		is >> tmp;
		int nsites=-1;
		while (tmp != ";") {
		  if (atoi(tmp.data()) > nsites)
			  nsites = atoi(tmp.data());
		  is >> tmp;
		  if (atoi(tmp.data()) > maxrotamers)
			  maxrotamers = atoi(tmp.data());
		  is >> tmp;
		  is >> tmp;
		}
		n = nsites;
		is.close();
		is.open(filename);
		state = 1;
		}
		break;

	case 1:
		{
		if (tmp != "param:") break;
		is >> tmp;
		if (tmp != "ValidInterIndices:") EXCEPTION_MNGR(runtime_error,"Badly formated input - 1")
		is >> tmp;
		//std::cerr << tmp << std::endl;
		is >> tmp;
		//std::cerr << tmp << std::endl;
		rCount.resize(n);
		rCount << 0;
		E_inter.resize(n);
		for (unsigned int i=0; i<n; i++) {
		  E_inter[i].resize(maxrotamers);
		  E_inter[i] << 0.0;
		}
		ctr1=0;
		state = 2;
		}
		break;

	case 2:
		{	
		if (tmp == ";") {
		   rCount[pctr1-1] = pctr2;
		   E_inter[pctr1-1].resize(pctr2);
		   state = 3;
		   break;
		   }
		//std::cerr << tmp << std::endl;
		ctr1 = atoi(tmp.data());
		is >> ctr2;
		is >> E_inter[ctr1-1][ctr2-1];
#if 0
		E_inter[ctr1-1][ctr2-1] = floor(E_inter[ctr1-1][ctr2-1]);
#endif
		if ((pctr1 != 0) && (ctr1 != pctr1)) {
		   rCount[pctr1-1] = pctr2;
		   E_inter[pctr1-1].resize(pctr2);
		}
		pctr1=ctr1;
		pctr2=ctr2;
		}
		break;

	case 3:
		{
		if (tmp != "param:") break;
		is >> tmp;
		if (tmp != "ValidIntraIndices:") EXCEPTION_MNGR(runtime_error,"Badly formated input - 3")
		is >> tmp;
		is >> tmp;
		E_intra.resize(n);
		for (unsigned int i=0; i<n; i++) {
		  if (rCount[i] == 0) continue;
		  E_intra[i].resize(n);
		  for (unsigned int j=0; j<n; j++) {
		    if (rCount[j] == 0) continue;
		    if (i < j) {
		       E_intra[i][j].r.resize(rCount[i]*rCount[j]);
		       E_intra[i][j].s.resize(rCount[i]*rCount[j]);
		       E_intra[i][j].e.resize(rCount[i]*rCount[j]);
		       }
   		    }
		  }
		state = 4;
		}
		break;

	case 4:
		{	
		if (tmp == ";") {
		   state = 5;
		   break;
		   }
		ctr1 = atoi(tmp.data());
		is >> tmp;
		is >> ctr2;
		int i = E_intra[ctr1-1][ctr2-1].n;
		E_intra[ctr1-1][ctr2-1].r[i] = atoi(tmp.data())-1;
		is >> tmp;
		E_intra[ctr1-1][ctr2-1].s[i] = atoi(tmp.data())-1;
		is >> E_intra[ctr1-1][ctr2-1].e[i];
		if ((E_intra[ctr1-1][ctr2-1].e[i] < -Etolerance) ||
		    (E_intra[ctr1-1][ctr2-1].e[i] >  Etolerance))
		   E_intra[ctr1-1][ctr2-1].n++;
		/*std::cerr << ctr1 << " " << ctr2 << " "
			<< E_intra[ctr1-1][ctr2-1].r[i] << " "
			<< E_intra[ctr1-1][ctr2-1].s[i] << std::endl;*/
		}
		break;

	case 5:
		break;
    };
  }

BasicArray<BitArray> free_rotamers(n);
for (unsigned int i=0; i<n; i++) {
  free_rotamers[i].resize(rCount[i]);
  free_rotamers[i].set();
  }

//std::cerr << "rCount " << rCount << std::endl;
inter_order.resize(n);
weighted_inter_order.resize(n);
bound2_info.resize(n);
bound2_ctr.resize(n);
for (unsigned int i=0; i<n; i++) {
  if (rCount[i] == 0) continue;
  for (unsigned int j=i+1; j<n; j++) {
    if (rCount[j] == 0) continue;
       IntraInfo& info = E_intra[i][j];
       info.index.resize(rCount[i]);
       for (int k=0; k<rCount[i]; k++) {
         info.index[k].resize(rCount[j]);
         info.index[k] << -1;
         }
       for (int k=0; k<E_intra[i][j].n; k++) {
	 //std::cerr << i << " " << j << " " << info.index.size() << " " << info.r[k] << " " << info.s[k] << std::endl;
         info.index[ info.r[k] ][ info.s[k] ] = k;
         }
       info.r.resize( info.n );
       info.s.resize( info.n );
       info.e.resize( info.n );
       info.order.resize( info.n );
       order(info.order, info.e);
       //std::cerr << std::endl;
       //std::cerr << i << " " << j << std::endl;
       //std::cerr << info.e << std::endl;
       //std::cerr << info.order << std::endl;
    }
  inter_order[i].resize(rCount[i]);
  weighted_inter_order[i].resize(rCount[i]);
  BasicArray<double> E_inter_tmp(rCount[i]);
  order(inter_order[i], E_inter[i]);
  bound2_info[i].resize(rCount[i]);
  bound2_ctr[i].resize(rCount[i]);
  for (int k=0; k<rCount[i]; k++) { 
    bound2_info[i][k].resize(n);
    bound2_ctr[i][k].resize(n);
    }
  if (advanced_rotamer_weighting) {
     double tmpn = static_cast<double>(rCount[i]);
     for (int k=0; k<rCount[i]; k++) {
       E_inter_tmp[k] = E_inter[i][k]/tmpn + compute_min_rotamer(i,k,free_rotamers);
       }
     order(weighted_inter_order[i], E_inter_tmp);
     }
  else
     weighted_inter_order[i] << inter_order[i];
  }

rorder.resize(n);
order(rorder,rCount);

ucout << "rCount: " << rCount << std::endl;
}

#endif


#if 0

void exit_fn()
{
}


int main()
{
utilib::exception_mngr::set_exit_function(exit_fn);
utilib::exception_mngr::set_mode(utilib::exception_mngr::Abort);

try {
  //ifstream ifstr("../../data/1abo.dat");
  //ifstream ifstr("../../data/1bbz.dat");
  //ifstream ifstr("../../data/1ddv.dat");
  std::ifstream ifstr("../../data/1n5z.dat");
  QSSProblem info;
  info.read(ifstr);

  BasicArray<int> x(info.n);
  std::cerr << "Greedy solution value: " << info.find_greedy(x) << std::endl;
  std::cerr << x << std::endl;

  BasicArray<BitArray> free_rotamers(info.n);
  for (int i=0; i<info.n; i++) {
    free_rotamers[i].resize(info.rCount[i]);
    free_rotamers[i].set();
    }
  std::cerr << "Root bound " << info.compute_bound(free_rotamers) << std::endl;

  for (int k=0; k<5; k++) {
    for (int i=0; i<info.n; i++) {
      if ((k+1) < info.rCount[i]) {
         free_rotamers[i].reset( info.inter_order[i][k] );
         std::cerr << "Bound " << i << " " << k << ": " << info.compute_bound(free_rotamers) << std::endl;
         }
      }
    }

  }
catch (std::runtime_error& err) {
  std::cerr << err.what() << std::endl;
  }
}

#endif
