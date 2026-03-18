                                                                                
#include <iostream>
#include <stdlib.h>
 
#include <vector>
 
using namespace std;
 
int main(int argc, char *argv[])
{
  if (argc < 2) {
	cerr << "usage: genattack <numattacks>" << endl;
	exit(1);
  }
  int num_probs = atoi(argv[1]);
  vector<double> probs(num_probs);
  double sumProbs=0.0;
  double each_prob = 1.0/num_probs;
  for(size_t j=0;j<size_t(num_probs);j++)
    {
      probs[j]= each_prob;  // drand48();
      sumProbs+=probs[j];
    }
  double newSumProbs = 0.0;
  for(size_t j=0;j<int(num_probs);j++)
    {
      probs[j]=probs[j]/sumProbs;
      newSumProbs += probs[j];
    }
  for(size_t j=0;j<size_t(num_probs);j++)
    {
      cout << probs[j] << endl;
    }
  return 1;
}
