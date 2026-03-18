
// Author: Haixia Jia
//
// Created on June 1 2008.
// A derivative class from solution class.


#ifndef VARSOLUTION_H
#define VARSOLUTION_H

#include <fstream>
#include <vector>

namespace NewSensorPlacement {
class VarSolution : public ProblemSolution{
  private:

  public:	

    VarSolution(ProblemInstance *inst);
    ~VarSolution(void);
    
    double moveEvaluator(int facToOpen, int& numWorstCases);
    double moveEvaluator(int facToOpen, double gamma);
    double cost(double gamma=0.0)const; 
    

};
};

#endif
