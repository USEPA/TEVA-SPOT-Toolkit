/*
*******************************************************************

MSTOOLKIT.H - Prototypes for EPANET Functions Exported to DLL Toolkit

VERSION:    2.00
DATE:       5/8/00
            10/25/00
            3/1/01
AUTHOR:     L. Rossman
            US EPA - NRMRL

*******************************************************************
*/

#ifdef DLL
  #ifdef __cplusplus
  #define DLLEXPORT extern "C" __declspec(dllexport) __stdcall
  #else
  #define DLLEXPORT __declspec(dllexport) __stdcall
  #endif
#else
#define DLLEXPORT
#endif

/* These are additional codes used by the MS DLL functions */

#define EN_SPECIESCOUNT 6   /* Component counts */
#define EN_BULKSPECIESCOUNT 7

#define EN_BULK			0	/* Species types */
#define EN_PIPESURFACE  1
#define EN_TANKSURFACE  2

#define EN_MULTIS       4   /* Multi-species quality type */


/* These are the external functions that comprise the DLL */

int  DLLEXPORT ENMSgetspeciesindex(char *, int *);
int  DLLEXPORT ENMSgetspeciesid(int, char *);
int  DLLEXPORT ENMSgetspeciestype(int, int *);
int  DLLEXPORT ENMSgetparameterindex(char *, int *);
int  DLLEXPORT ENMSgetparameterid(int, char *);
int  DLLEXPORT ENMSgetnodequality(int, int, int, float *);
int  DLLEXPORT ENMSsetnodequality(int, int, int, float);
int  DLLEXPORT ENMSgetlinkquality(int, int, float *);
int  DLLEXPORT ENMSgetparametervalue(int, int, int, float *);
int  DLLEXPORT ENMSsetparametervalue(int, int, int, float);
int  DLLEXPORT ENMSgetmassreact(int, float *);
