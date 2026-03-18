/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file clustering.h
 * \author Jonathan Eckstein
 *
 * Logic for organizing things (typically processors) into clusters.
 */

#ifndef pebbl_clustering_h
#define pebbl_clustering_h

#include <acro_config.h>

namespace pebbl {


class clusterObj
{
public:
  
  // Initialize things to avoid valgrind complaints when registering
  // parameters for parMIP.
  // Meaningful initialization is by reset(...) with real arguments.

  clusterObj() { reset(0,1,1,1,0); };

  void reset(int rank_,
	     int size_,
	     int sizeWanted,
	     int clustersWanted,
	     int forceSeparateSize);

  int rank;
  int size;
  int separateFunctions;
  int iAmLeader;
  int iAmFollower;
  int clusterNumber;
  int typicalSize;
  int clusterSize;
  int lastClusterSize;
  int leader;
  int numPureLeaders;
  int numFollowers;
  int positionInCluster;
  int followerPosInCluster;
  int numClusters;
  int totalNumFollowers;
  int typicallySeparated;
  int lastSeparated;
  int typicalFollowers;
  int myGlobalFollowerRank;

  double typicalFollowFrac;
  double lastFollowFrac;
  double myFollowFrac;
  
  int follower(int whichFollower)
    {
      return leader + whichFollower + separateFunctions;
    };

  int whichFollower(int n)
    {
      return n - leader - separateFunctions;
    };

  int leaderOfCluster(int whichCluster)
    {
      return whichCluster*typicalSize;
    };

  int whichCluster(int n)
    {
      return n/typicalSize;
    };

  int hasPureLeader(int cNum)
    {
      if (cNum + 1 == numClusters)
	return lastSeparated;
      else
	return typicallySeparated;
    };

  // Translates an index into the total collection of followers
  // into an index into the total collection of things.

  int globalFollower(int n)
    {
      int targetCluster = n/typicalFollowers;
      int targetPos     = n - targetCluster*typicalFollowers;
      return (targetCluster*typicalSize + 
	      hasPureLeader(targetCluster) + targetPos);
    };

  // Translates an index into the total collection of things into
  // an index into the total collection of followers.

  int followerRank(int globalRank)
    {
      int targetCluster = whichCluster(globalRank);
      int relPos        = globalRank - targetCluster*typicalSize;
      return (targetCluster*typicalFollowers + 
	      relPos - hasPureLeader(targetCluster));
    };

  // Translates an index into the set of clusters into the fraction of
  // the total followers owned by that cluster.

  double followFrac(int cNum)
    {
      if (cNum + 1 == numClusters)
	return lastFollowFrac;
      return typicalFollowFrac;
    };

};
  
}

#endif
