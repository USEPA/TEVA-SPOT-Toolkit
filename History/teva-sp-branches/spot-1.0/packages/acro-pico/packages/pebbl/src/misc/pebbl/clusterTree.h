/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file clusterTree.h
 * \author Jonathan Eckstein
 *
 * Organize some clusters into a tree.  Used for PEBBL load balancing.
 */

#ifndef pebbl_clusterTree_h
#define pebbl_clusterTree_h

#include <acro_config.h>
#ifdef ACRO_HAVE_MPI

#include <pebbl/clustering.h>
#include <pebbl/treeTopology.h>
#include <pebbl/fundamentals.h>

namespace pebbl {


class nAryClusterTree : public nAryTreeRememberParent
{
private:
  
  clusterObj* cluster;

  int clusterNumberOf(int where)
    {
      int whereCluster = cluster->whichCluster(where);
#ifdef ACRO_VALIDATING
      if (where != cluster->leaderOfCluster(whereCluster))
	EXCEPTION_MNGR(std::runtime_error,"Cluster tree finding address of non-leader");
#endif
      return whereCluster;
    };
 
public:

  int currentChild()
    {
      return cluster->leaderOfCluster(nAryTree::currentChild());
    };

  int parent()
    {
      return cluster->leaderOfCluster(nAryTreeRememberParent::parent());
    };

  int operator++(int i)
    {
      return cluster->leaderOfCluster(nAryTree::operator++(i));
    };

  int validChild(int where)
    {
      return nAryTree::validChild(clusterNumberOf(where));
    };

  int whichChild(int where)
    {
      return nAryTree::whichChild(clusterNumberOf(where));
    };

  nAryClusterTree(clusterObj& cluster_,
		  int radix_ = 2) :
  nAryTreeRememberParent(radix_,
			 0,
			 cluster_.clusterNumber,
			 cluster_.numClusters
			 ),
  cluster(&cluster_)
    { };

};

} // namespace pebbl

#endif

#endif
