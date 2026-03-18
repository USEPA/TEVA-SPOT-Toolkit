
// Author: Haixia Jia
//
// Created on June 1 2008.
// A derivative class from solution class.


#ifndef TCESOLUTION_H
#define TCESOLUTION_H

#include <fstream>
#include <vector>

namespace NewSensorPlacement {
class TceSolution : public ProblemSolution{
  private:
  public:	

    TceSolution(ProblemInstance *inst);
    ~TceSolution(void);

    double moveEvaluator(int facToOpen, int& numWorstCases);
    double moveEvaluator(int facToOpen, double gamma);
    double cost(double gamma=0.0)const; 

};
};

#endif
