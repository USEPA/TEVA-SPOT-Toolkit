/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file followerTree.h
 * \author Jonathan Eckstein
 *
 * Organize the followers in a group o clusters into a tree.  
 * Used for pseudocost broadcast
 */

#ifndef pebbl_followerTree_h
#define pebbl_followerTree_h

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <pebbl/clustering.h>
#include <pebbl/treeTopology.h>
#include <pebbl/fundamentals.h>

namespace pebbl {


class nAryFollowerTree : public nAryTreeRememberParent
{
private:
  
  clusterObj* cluster;

 
public:

  int currentChild()
    {
      return cluster->globalFollower(nAryTree::currentChild());
    };

  int parent()
    {
      return cluster->globalFollower(nAryTreeRememberParent::parent());
    };

  int operator++(int i)
    {
      return cluster->globalFollower(nAryTree::operator++(i));
    };

  int validChild(int where)
    {
      return nAryTree::validChild(cluster->followerRank(where));
    };

  int whichChild(int where)
    {
      return nAryTree::whichChild(cluster->followerRank(where));
    };

  nAryFollowerTree(clusterObj& cluster_,
		   int root_ = 0,
		   int radix_ = 2) :
  nAryTreeRememberParent(radix_,
			 root_,
			 cluster_.myGlobalFollowerRank,
			 cluster_.totalNumFollowers
			 ),
  cluster(&cluster_)
    { };

};

} // namespace pebbl

#endif

#endif
