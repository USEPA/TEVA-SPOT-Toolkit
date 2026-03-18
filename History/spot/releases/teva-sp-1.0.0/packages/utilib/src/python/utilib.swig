%module utilib

%rename NumArray_double RealVector;
%rename NumArray_int    IntVector;

%{
#include <iostream.h>
#include <fstream.h>

#include "BitArray.h"
#include "CharString.h"
#include "BasicArray_int.h"
#include "BasicArray_double.h"
#include "SimpleArray_int.h"
#include "SimpleArray_double.h"
#include "NumArray_int.h"
#include "NumArray_double.h"
#include "IntVector.h"
#include "DoubleVector.h"
#include "RealVector.h"

#include "seconds.h"
#include "_math.h"
#include "nr.double.h"
#include "nr.int.h"
#include "nr.bit.h"

#include "RNG.h"
#include "CRandVar.h"
#include "PM_LCG.h"
#include "Uniform.h"
#include "Normal.h"
#include "Triangular.h"

#include "CommonIO.h"

static char* read_common_cin(int n)
{
char* str = new char [n+1];
int i=0;
while (cin && (i<n))
  cin.get(str[i++]);
str[i] = '\000';
return str;
}

RealVector foo()
{
RealVector* tmp = new RealVector;
return *tmp;
}

%}

enum EnumDataOwned { DataNotOwned=0, AcquireOwnership=1, AssumeOwnership=2 };

/** adt **/

%include BitArray.h
%include CharString.h
%include BasicArray_int.h
%include BasicArray_double.h
%include SimpleArray_int.h
%include SimpleArray_double.h
%include NumArray_int.h
%include NumArray_double.h

%addmethods BitArray {
  int __call__(int i) {
	return self->operator()(i);
	};
}

%addmethods BasicArray_int {
  int __getitem__(int i) {
	return self->operator[](i);
        };
  void __setitem__(int i, int val) {
	self->operator[](i) = val;
        };
}

%addmethods BasicArray_double {
  double __getitem__(int i) {
	return self->operator[](i);
        };
  void __setitem__(int i, double val) {
	self->operator[](i) = val;
        };
}

%addmethods CharString {
  char __getitem__(int i) {
	return self->operator[](i);
        };
  void __setitem__(int i, char val) {
	self->operator[](i) = val;
        };
}

typedef double REAL;
typedef RealVector DoubleVector;

/** math **/

/* Functions defined in _math.h */

int calc_filesize(char*);

double CPUSeconds();
double WallClockSeconds();
double ElapsedCPUSeconds();
double ElapsedWallClockSeconds();
int    InitializeTiming();

int round (double x);

/* Functions defined in nr.double.h */


void shuffle(BasicArray_double& vec, RNG* rng=&default_rng);
void subshuffle(BasicArray_double& vec, RNG* rng, int start, int len);

double sum(const BasicArray_double& vec);
double sum(const Basic2DArray_double& a);
double length(const BasicArray_double& a);
double dist(const BasicArray_double& a, const BasicArray_double& b);

extern "C" {
  int covariance(double** X, int m, int n, double** COV);
  int calc_filesize(char*);
}

int argmedian(double* x, int n, int* ws=0, RNG* rng=0);
double median(double* x, int n, int* ws=0, RNG* rng=0);

BasicArray_double concat(const BasicArray_double& a, const BasicArray_double& b);



double trimmed_mean(BasicArray_double& vec, const double percentage=0.1);
double mean(const BasicArray_double& vec);
double var(const BasicArray_double& vec, const int sampleflag = FALSE);
double mad(BasicArray_double& vec, BasicArray_double& work);
double mad(BasicArray_double& vec);

DoubleVector mean(const DoubleMatrix& vec, const int stats_flag=FALSE);
DoubleVector var(const DoubleMatrix& mat, const int sampleflag=FALSE);
DoubleVector var(const DoubleMatrix& mat, DoubleVector& mean, 
						const int sampleflag=FALSE);
int  covariance(Basic2DArray_double& X, Basic2DArray_double& COV);

void rowscale(Basic2DArray_double& a);

int argmedian(const BasicArray_double& vec, RNG* rng=0);
int argmedian(const BasicArray_double& vec, const BasicArray_int& ws, RNG* rng=0);
inline double  median(const BasicArray_double& vec, RNG* rng=0) 
	{return median(vec.data(),vec.len(),0,rng);}

int argmin(const BasicArray_double& vec);
inline double min(const BasicArray_double& vec)	{ return vec[argmin(vec)]; }
double min(const Basic2DArray_double& mat);

int argmax(const BasicArray_double& vec);
inline double max(const BasicArray_double& vec)	{ return vec[argmax(vec)]; }
double max(const Basic2DArray_double& mat);

inline double min(double x, double y) { return ((x)<(y)? (x) : (y));}
inline double max(double x, double y) { return ((x)>(y)? (x) : (y));}


/** io **/

%include CommonIO.h

/** ranlib **/

class RNG {

public:

  virtual unsigned long asLong();
  virtual void reset();
  virtual float asFloat();
  virtual double asDouble();

};

class CRandVar {

public:

  RNG* generator();
  void generator(RNG* p);

};

%addmethods CRandVar {
  double __call__() {
		return self->operator()();
		}
  void set_generator(RNG* p) {
		self->generator(p);
		}
  }

%include PM_LCG.h
%include Uniform.h


%include Normal.h
%include Triangular.h

RealVector foo();


##
## Dummy definitions for ostream, istream, ifstream and ofstream.  These
## definitions allow the construction of ifstream and ofstream objects
## from within Python that are subsequently passed into I/O methods.
##
class ostream
{
public:
  void flush();
private:
  ostream();
};

%addmethods ostream {
  void write(char* str) { (*self) << str; }
  }

class ofstream: public ostream
{
public:
  ofstream(char* fname);
  void open(char* fname);
  void close();
};

%addmethods ofstream {
  ~ofstream() {
  self->flush();
  self->close();
  }
}

class istream
{
private:
  istream();
};

class ifstream: public istream
{
public:
  ifstream(char* fname);
  void open(char* fname);
  void close();
};
