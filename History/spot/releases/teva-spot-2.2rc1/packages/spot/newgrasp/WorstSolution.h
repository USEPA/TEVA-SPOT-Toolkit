

// Author: Haixia Jia
//
// Created on June 1 2008.
// A derivative class from solution class.


#ifndef WORSTSOLUTION_H
#define WORSTSOLUTION_H

#include <fstream>
#include <vector>

namespace NewSensorPlacement {
class WorstSolution : public ProblemSolution{
  private:
  
  public:	

    WorstSolution(ProblemInstance *inst);
    ~WorstSolution(void);

    double moveEvaluator(int facToOpen, double gamma);
    double moveEvaluator(int facToOpen, int& numWorstCases);
    double cost(double gamma=0.0)const; 
    int numWorstCaseScenarios(double gamma=0.0)const;
};
};

#endif
