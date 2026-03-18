/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
//
// clustering.cpp
//
// Logic for organizing things (typically processors) into clusters.
//
// Jonathan Eckstein
//


#include <acro_config.h>
#include <pebbl/fundamentals.h>
#include <pebbl/clustering.h>

using namespace std;

namespace pebbl {


void clusterObj::reset(int rank_,
		       int size_,
		       int sizeWanted,
		       int clustersWanted,
		       int forceSeparateSize)
{
  rank = rank_;
  size = size_;

  typicalSize = (int) ceil(((double) size)/std::max(clustersWanted,1));
  if (typicalSize > sizeWanted)
    typicalSize = sizeWanted;
  if (typicalSize < 1)
    typicalSize = 1;

  numClusters = (int) ceil(((double) size)/typicalSize);

  clusterNumber   = rank/typicalSize;
  leader          = clusterNumber*typicalSize;
  clusterSize     = std::min(typicalSize,size - leader);
  lastClusterSize = size - (numClusters - 1)*typicalSize;

  separateFunctions  = (clusterSize     >= forceSeparateSize);
  typicallySeparated = (typicalSize     >= forceSeparateSize);
  lastSeparated      = (lastClusterSize >= forceSeparateSize);

  typicalFollowers = typicalSize - typicallySeparated;

  numFollowers         = clusterSize - separateFunctions;
  positionInCluster    = rank - leader;
  followerPosInCluster = positionInCluster - separateFunctions;
  iAmLeader            = (positionInCluster == 0);
  iAmFollower          = !iAmLeader || !separateFunctions;

  numPureLeaders       = (numClusters - 1)*typicallySeparated + lastSeparated;
  totalNumFollowers    = size - numPureLeaders;
  myGlobalFollowerRank = clusterNumber*typicalFollowers + followerPosInCluster;

  typicalFollowFrac = ((double) typicalFollowers)/totalNumFollowers;
  int lastFollowers = lastClusterSize - lastSeparated;
  lastFollowFrac    = ((double) lastFollowers)/totalNumFollowers;
  myFollowFrac      = followFrac(clusterNumber);
  
}

} // namespace pebbl

