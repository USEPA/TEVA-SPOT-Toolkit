/*  _________________________________________________________________________
 *
 *  TEVA-SPOT Toolkit: Tools for Designing Contaminant Warning Systems
 *  Copyright (c) 2008 Sandia Corporation.
 *  This software is distributed under the LGPL License.
 *  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 *  the U.S. Government retains certain rights in this software.
 *  For more information, see the README file in the top software directory.
 *  _________________________________________________________________________
 */

#include <Detection.h>
#include <list>

int DetectionMinQuality::Calculate(){

    // Check that Set* have been called
    this->detectionNodeList.clear();
    this->detectionNodeList.resize(this->nSteps, std::vector<int>());

    std::vector<bool> nodeHits(this->net->nnodes, false);

    // each source node detects 
    for (int i=0; i<net->nsources; i++){
      int nodeIndex = this->source[i].SourceNodeIndex - 1;
      long detStep = long((this->source[i].SourceStart / 60.0) / this->reportStep);
      if (detStep >= this->nSteps){
        detStep = this->nSteps - 1;
      }
      this->detectionNodeList[detStep].push_back(nodeIndex);
      nodeHits[nodeIndex] = true;
    }

    for (int nodeIndex = 0; nodeIndex < this->net->nnodes; nodeIndex++){
      if (nodeHits[nodeIndex] == false){
        for (int ts=0; ts < this->nSteps; ts++){
          double q = this->node[nodeIndex].c[0][ts];
          if ( (q > 0.0) && (q > this->minQuality)){
            this->detectionNodeList[ts].push_back(nodeIndex);
            break;
          }
        }
      }
    }
    return 0;  // success
} 

int DetectionNWay::Calculate(){
  
    this->detectionNodeList.clear();
    this->detectionNodeList.resize(this->nSteps, std::vector<int>());

    std::vector<bool> nodeHits(this->net->nnodes, false);
    std::list<int> nodeIDs;
    int numHits = 1;  // the source nodes

    // Find the source nodes
    for (int i=0; i<net->nsources; i++){
      int nodeIndex = this->source[i].SourceNodeIndex - 1;
      nodeHits[nodeIndex] = true;
      nodeIDs.push_back(nodeIndex);
    }
    //
    // Don't begin recording detection until at least N nodes have detected contaminant
    //
    for (int ts=0; ts < this->nSteps; ts++){
      for (int nodeIndex = 0; nodeIndex < this->net->nnodes; nodeIndex++){
        double q = this->node[nodeIndex].c[0][ts];
        if ( (q > 0.0) && (q > this->minQuality)){
          if (nodeHits[nodeIndex] == false){   // first time this node has contaminant
            if (++numHits > this->N){
              this->detectionNodeList[ts].push_back(nodeIndex);
              if (nodeIDs.size() > 0){
                // Allow the previous nodes to detect now as well
                std::list<int>::iterator iter;
                for (iter=nodeIDs.begin(); iter != nodeIDs.end(); iter++){
                  this->detectionNodeList[ts].push_back(*iter);
                }
                nodeIDs.clear();
              }
            }
            else{
              nodeIDs.push_back(nodeIndex);
            }
            nodeHits[nodeIndex] = true;
          }
        }
      }
    }

    return 0;  // success
}
