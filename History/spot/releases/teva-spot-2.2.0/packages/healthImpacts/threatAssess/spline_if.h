
#ifndef _SPLINE_IF_H_
#define _SPLINE_IF_H_

typedef struct SplineDataRec {
	int n;
	int method;
	double *x;
	double *y;
	double *b;
	double *c;
	double *d;
	double *e;
	double dv[1];
} SplineData, *SplineDataPtr;

SplineDataPtr init_spline(int method, int n, double *x, double *y);
double eval_spline(SplineDataPtr sdp,double x);
void free_spline(SplineDataPtr sdp);

#endif
