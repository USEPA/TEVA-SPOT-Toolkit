/*
 * Copyright © 2008 UChicago Argonne, LLC
 * NOTICE: This computer software, TEVA-SPOT, was prepared for UChicago Argonne, LLC
 * as the operator of Argonne National Laboratory under Contract No. DE-AC02-06CH11357
 * with the Department of Energy (DOE). All rights in the computer software are reserved
 * by DOE on behalf of the United States Government and the Contractor as provided in
 * the Contract.
 * NEITHER THE GOVERNMENT NOR THE CONTRACTOR MAKES ANY WARRANTY, EXPRESS OR IMPLIED, OR
 * ASSUMES ANY LIABILITY FOR THE USE OF THIS SOFTWARE.
 *
 * This software is distributed under the BSD License.
 */
#ifndef _TEVA_ASSESS_H_
#define _TEVA_ASSESS_H_

#include "tevautil.h"
#include "ta_input.h"
#include "ta_utils.h"
#include "spline_if.h"

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
	potential,      /* Potential new sensor location */
	selected        /* newly selected sensor here */
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
/*
**-------------------------------------------------------------------
** Dose-response model model
** (Old will be removed in the future - included only for now for
**  testing)
**-------------------------------------------------------------------
*/ 
enum ResponseType 
{
	Probit,
	Old
};
/*
**-------------------------------------------------------------------
** Dose-response model model
** (Old will be removed in the future - included only for now for
**  testing)
**-------------------------------------------------------------------
*/ 
enum Population 
{
	DemandBased,
	FileBased
};

enum IngestionTiming
{
	IMATUS, IMRandom, IMFixed5, IMFixed2, IMDemand
};
enum IngestionVolume {
	VMMean, VMRandom, VMDemand
};
typedef struct
{
	char *nodeid;
	float population;
	int   used;
} NodePopulation, *PNodePopulation;

typedef struct
{
	enum  DoseType dtype;
	enum  ResponseType responseType;
// for sigmoid DoesResponse
	float a;
	float m;
	float n;
	float tau;
// for Probit
    float ld50;
	float beta;

	float bodymass;
	int   normalize;
	int   nL;
	int   nF;
	int   maxsteps;
	float frate;
	float protectlimit;
} DR;
typedef DR *PDR;

typedef struct {
	enum  Population population;
	PNodePopulation pop;
	int   popLength;
	float pcu;
}PopulationData, *PopulationDataPtr;

typedef struct {
	union VolumeData {
		double volume;  // will not have data for VM_DEMAND/IM_DEMAND case
		float *volumes; // Only used for VM_DEMAND/IM_DEMAND
	} v;
	double times[5];
	float response;  // only set for
} IndividualIngestionData, *IndividualIngestionDataPtr;

typedef struct {
	enum IngestionVolume volumeMode;
	enum IngestionTiming timingMode;
	SplineDataPtr fVol;
	SplineDataPtr *fTiming;

	float meanVolume;  // only used for volumeMode=VMMean
	float phi;         // only used for volumeMode=VMDemand
	int numNodes;
	int maxsteps;
	int nsteps;
	float **nodeDoses;  // temporary 2-d work array (saves lots of time due to alloc/free - esp. free)
	IndividualIngestionDataPtr **ingestionData;
} IngestionData, *IngestionDataPtr;

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
	int pop;
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
	char *injDef;
	float testh;
	float fh;
	float dosh;
	float resh;
	int *dot;
	int *doseBins;
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

typedef struct {
  int numBins;
  float *responseBins;
  float *doseBins; // bins will have been converted to doses before being assigned - length: numBins
  int *data; // length: numBins;
} DoseBinData, *PDoseBinData;

typedef struct {
  int numThresh; // number of thresholds specified
  float *thresholds;  // individual thresholds;
  char **threshIDs;  // threshold IDs - used in creating output filenames...
  int **numOver;  // first dimension: numThresh  second dimension: nnodes
  int *totOver; // dimension: numThresh - total over the threshold for the entire scenario
  int **numOverByTime; // first dimension: numThres second dimension: 
} DoseOverThresh, *PDoseOverThresh;

typedef struct
{
	PTSO         tso;
	PIO          io;
	PDR          dr;
	PNode        node;
	PLink        link;
	PopulationDataPtr popData;
	IngestionDataPtr  ingestionData;
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
	float        concentrationThreshold;
	PDoseOverThresh dot;
	PDoseBinData  doseBins;
} Mem;
typedef Mem *PMem;

extern char *VolumeModeDesc[];
extern char *VolumeModeAbbr[];
extern char *IngestionModeDesc[];
extern char *IngestionModeAbbr[];

#ifndef DLLEXPORT
  #ifdef DLL
    #ifdef __cplusplus
    #define DLLEXPORT extern "C" __declspec(dllexport) __stdcall
    #else
    #define DLLEXPORT __declspec(dllexport) __stdcall
    #endif
  #elif defined(CYGWIN)
    #define DLLEXPORT __stdcall
  #else
    #ifdef __cplusplus
    #define DLLEXPORT
  //  #define DLLEXPORT extern "C"
    #else
    #define DLLEXPORT
    #endif
  #endif
#endif
/* Function Prototypes */
int  DLLEXPORT Detect(PNet,PNode);
float cavg(int, int, int, float*);			 
void DLLEXPORT Population(PNet net,PopulationDataPtr pdp,PNode node);
void DLLEXPORT initializeIngestionData(IngestionDataPtr p, PNet net, PNode nodes, PDR dr, PopulationDataPtr pd);
void  IngestionRate(PNet,PNode);
void  ResponseOld(PNet,PDR,PU);
void DLLEXPORT Response(IndividualIngestionDataPtr *idp, IngestionDataPtr ingData, PDR dr,PNode node, PU u);

void  DoseOld(int,PNet,PDR,PU,PNode,float);
void DLLEXPORT Dose(IndividualIngestionDataPtr *idp, IngestionDataPtr ingData, PNode node, PNet net, int detectTime, PU u, PDoseOverThresh dot, int nodeidx, PDoseBinData doseBins, float *avgDose);

float DLLEXPORT DoseResponse(float,PDR);
float DLLEXPORT ResponseDose(float response, PDR dr);
void  Disease(PNet,PDR,PU);
/** Regan's modifications to the Disease progression code **/
void  DLLEXPORT DiseaseRM(PNet,PDR,PU);
void  DLLEXPORT initialize(PNet,PDR,PXA,PTA);
int   DLLEXPORT getnodeindex(PNet,PNode,char*);
//void  Accumulate(int,int,PNet,PDR,float,PU,PXA,PTA);
/** UPDATE **/
void DLLEXPORT Accumulate(int inod,int nz,PNet net,PDR dr,int pop,PU u,PXA xa,PTA ta,PHIST hist);
/** End UPDATED **/
/** NEW **/
void DLLEXPORT AccumulateFinal(PNet net,PDR dr,PXA totalXA,PXA interimXA);
/** End NEW **/
float findmax(int,float*);

Mem InitializeAssessment(char *taiFile, char *sssFile);
void DLLEXPORT processTAIData(PInputFileData taiIFD, PMem mem);
void DLLEXPORT InitializeAssessMemory(PMem mem,PNetInfo netinfo);
void DLLEXPORT dumpHIAInputParameters(PMem mem);

/** NEW **/
void DLLEXPORT resetDoseOverThreshold(PMem mem);
void DLLEXPORT resetDoseBinData(PMem mem);
void DLLEXPORT initXA(PXA xa, int len);
void DLLEXPORT resetXA(PXA xa, int len);
void DLLEXPORT freeXA(PXA xa, int len);
PHIST DLLEXPORT allocateHistData(char *id);
void DLLEXPORT freeHistogramData(PHIST hist);
void DLLEXPORT initTA(PTA ta, int len);
void DLLEXPORT resetTA(PTA ta, int len);
void DLLEXPORT freeTA(PTA ta, int len);
void initU(PU u, int len);
void resetU(PU u, int len);
void freeU(PU u, int len);
void DLLEXPORT FreeAssessMemory(PMem mem);
void DLLEXPORT freeIngestionData(IngestionDataPtr idp, PNode nodes);
void DLLEXPORT set_seed(unsigned long seed);
/** End NEW **/

#endif
