/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file serialQSScache.h
 * \author William Hart
 *
 * Defines the pebbl::serialQSSCache class.
 */

#ifndef __serialQSSCache_h
#define __serialQSSCache_h

#include <pebbl/BestIncumbentRepository.h>


template <class PointT>
class serialQSSCache : public pebbl::BestIncumbentRepository<PointT>
{
public:

  serialQSSCache() {}

  virtual ~serialQSSCache() {}

  void write(std::ostream& os) const;

};


template <class PointT>
void serialQSSCache<PointT>::write(std::ostream& os) const
{
os << "Size " << this->tree.size() << std::endl;
const utilib::GenericSplayTreeItem<pebbl::PointList<PointT> >* node = this->tree.find_rank(0);

int plength = node->key().points.begin()->size();
BasicArray<BasicArray<double> > mat(20);
for (int i=0; i<20; i++) {
  mat[i].resize(plength);
  mat[i] << 0.0;
  }

int count=0;
int i=1;

bool flag=true;
while (flag && node) {
  LinkedList<BasicArray<int> >::const_iterator curr = node->key().points.begin();
  LinkedList<BasicArray<int> >::const_iterator end = node->key().points.end();
  while (flag && (curr != end)) {
    count++;
    os << "Value: " << node->key().val << "   Solution: ";
    const BasicArray<int>& vec = *curr;
    for (size_type i=0; i<vec.size(); i++) {
      if (vec[i] >= 20) {flag = false; break;}
      if (vec[i] < 0)
         os << " .";
      else {
         os << " " << (vec[i]+1);
         mat[vec[i]][i]++;
         }
      }
    os << std::endl;
    curr++;
    }
  node = this->tree.find_rank(i++);
  }
if (!flag) {
   os << std::endl << "WARNING: more then 20 amino acid types!" << std::endl;
   return;
   }
//
// Normalize matrix
//
for (int i=0; i<20; i++)
  for (unsigned int j=0; j<mat[i].size(); j++)
    mat[i][j] /= count;
//
// Print matrix
//
os << std::endl << "Amino acid likelihoods:" << std::endl;
int oldw = os.width(4);
int oldp = os.precision(2);
os.setf(std::ios::fixed,std::ios::floatfield);
os.fill('0');
for (int i=0; i<20; i++) {
  for (unsigned int j=0; j<mat[i].size(); j++) {
    if (mat[i][j] == 0.0)
       os << "0.00 ";
    else
       os << mat[i][j] << " ";
    }
  os << std::endl;
  }
os.width(oldw);
os.precision(oldp);
}

#endif
