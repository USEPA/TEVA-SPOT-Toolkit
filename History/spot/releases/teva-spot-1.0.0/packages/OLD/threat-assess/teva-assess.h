#ifndef _TEVA_ASSESS_H_
#define _TEVA_ASSESS_H_

#include "tevautil.h"
#include "ta_input.h"
#include "ta_utils.h"

/* Global Conversion macros */
#define FILEOPEN(x) (((x) == NULL) ? (ta_error(1,"Opening files")) : (0))
#define MEMCHECK(x) (((x) == NULL) ? (ta_error(1,"Allocating memory")) : (0))
#define RES(x)  (((x) < 0) ? (0) : (res[x]))
#define INT(x)   ((int)(x))                   /* integer portion of x  */
#define FRAC(x)  ((x)-(int)(x))               /* fractional part of x  */
#define ABS(x)   (((x)<0) ? -(x) : (x))       /* absolute value of x   */
#define MIN(x,y) (((x)<=(y)) ? (x) : (y))     /* minimum of x and y    */
#define MAX(x,y) (((x)>=(y)) ? (x) : (y))     /* maximum of x and y    */
#define ROUND(x) (((x)>=0) ? (int)((x)+.5) : (int)((x)-.5)) /* round-off of x        */
#define MOD(x,y) ((x)%(y))                    /* x modulus y           */
#define SGN(x)   (((x)<0) ? (-1) : (1))       /* sign of x             */

/* Global Data Types */

typedef unsigned int UINT;


/*
**-------------------------------------------------------------------
** Sample or sensor technology
**-------------------------------------------------------------------
*/
enum SampleType
{
	realtime,       /* Realtime continuous measurement */
	composite,      /* Composite grab sampling */
	filtered        /* Continuous filtered sampling */
};

/*
**-------------------------------------------------------------------
** Sensor locations
**-------------------------------------------------------------------
*/
enum SensorType
{
	ignore,         /* no sensor here */
	existing,       /* Current sensor location */
	potential       /* Potential new sensor location */
};

/*
**-------------------------------------------------------------------
** Ingestion quantity model
** (Placeholder - only DEMAND available)
**-------------------------------------------------------------------
*/ 
enum IngType
{
	demand
};

/*
**-------------------------------------------------------------------
** Characteristic dose model
** (Placeholder - only TOTAL available)
**-------------------------------------------------------------------
*/ 
enum DoseType
{
	totalmass
};

typedef struct
{
	enum  DoseType dtype;
	enum  IngType itype;
	float a;
	float m;
	float n;
	float tau; 
	float bodymass;
	int   normalize;
	int   nL;
	int   nF;
	int   maxsteps;
	float phi;
	float frate;
	float pcu;
	float protectlimit;
} DR;
typedef DR *PDR;

typedef struct {
	enum  SensorType type;
	enum  SampleType sample;
    int  rank;
	float limit;
	float vol;
	int  freq;
	int  start;
	int  delay;
} Sensor, *PSensor;

typedef struct
{
	PNodeInfo info;
	float pop;
	float *rho;
	Sensor sensor;
} Node;
typedef Node *PNode;

typedef struct
{
	PLinkInfo info;
} Link;
typedef Link *PLink;

typedef struct {
	char fn[MAXFNAME];
	FILE *stream;
	unsigned char meanInfections:1;
	unsigned char maxInfections:1;
} SensorParms, *PSensorParms;
typedef struct
{ 
//	FILE *taistream;
	FILE *taostream;
//	char tainame[MAXFNAME];
	char taoname[MAXFNAME];
	PTSO tso;
} IO;
typedef IO *PIO;

typedef struct
{
	PNetInfo info;
	int nscenario;
} Net;
typedef Net *PNet;

typedef struct
{
	float *dos;
	float *res;
	float *s;
	float *i;
	float *d;
	float *f;
} XA;
typedef XA *PXA;

typedef struct
{
	float *dos;
	float *res;
	float *s;
	float *i;
	float *d;
	float *f;
} TA;
typedef TA *PTA;

/** NEW **/
typedef struct HIST
{
	char *simID;
	float testh;
	float fh;
	float dosh;
	float resh;
	struct HIST *next;
} HIST;
typedef HIST *PHIST;
/** End NEW **/

typedef struct
{
	float *dos;
	float *res;
	float *s;
	float *i;
	float *d;
	float *f;
} U;
typedef U *PU;

typedef struct
{
	PTSO         tso;
	PIO          io;
	PDR          dr;
	PNode        node;
	PLink        link;
	PNodeInfo    nodeinfo;
	PLinkInfo    linkinfo;
	PNet         net;
	PXA          xa;
	PTA          ta;
	/** NEW **/
	PHIST	     hist; 
	/** End NEW **/
	PU           u;
	PSource      sources;
	PSensorParms sap;
} Mem;
typedef Mem *PMem;

/* Function Prototypes */
int   Detect(PNet,PNode);
float cavg(int, int, int, float*);			 
void  Population(PNet,PDR,PNode);
void  IngestionRate(PNet,PNode);
void  Response(PNet,PDR,PU);
void  Dose(int,PNet,PDR,PU,PNode);
float DoseResponse(float,PDR);
void  Disease(PNet,PDR,PU);
/** Regan's modifications to the Disease progression code **/
void  DiseaseRM(PNet,PDR,PU);
void  initialize(PNet,PDR,PXA,PTA);
int   getnodeindex(PNet,PNode,char*);
//void  Accumulate(int,int,PNet,PDR,float,PU,PXA,PTA);
/** UPDATE **/
void Accumulate(int inod,int nz,PNet net,PDR dr,float pop,PU u,PXA xa,PTA ta,PHIST hist);
/** End UPDATED **/
/** NEW **/
void AccumulateFinal(PNet net,PDR dr,PXA totalXA,PXA interimXA);
/** End NEW **/
float findmax(int,float*);

Mem InitializeAssessment(char *taiFile, char *sssFile);
void processTAIData(PInputFileData taiIFD, PMem mem);
void InitializeAssessMemory(PMem mem,PNetInfo netinfo);

/** NEW **/
void initXA(PXA xa, int len);
void resetXA(PXA xa, int len);
void freeXA(PXA xa, int len);
void freeHistogramData(PHIST hist);
void freeTA(PTA ta, int len);
void freeU(PU u, int len);
void FreeAssessMemory(PMem mem);
/** End NEW **/

#endif
