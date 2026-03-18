#ifndef _ENUTIL_H_
#define _ENUTIL_H_

#include <stdio.h>
#include <float.h>
#include "tevatypes.h"

#ifdef WIN32

#ifdef MSEPANET  /* Multi-species or Normal Epanet header */
#include "epanet2ms.h"
#else
#include "epanet2.h"
#endif

#else

#if defined(__linux__) || defined(__CYGWIN__)
#ifdef MSEPANET  /* Multi-species or Normal Epanet header */
#include "toolkitms.h"
#else
#include "toolkit.h"
#endif
#endif
#endif

#define MAXLINE  10239       /* Max input line chars       */
#define MAXTOKS  500         /* Max input line tokens      */
#define SEPSTR    " \t\n\r"  /* Token separator characters */
#define MAXERRMSG   100      /* Max size of epanet err message    */

int ERRNUM;
#define ENCHECK(x) (((ERRNUM = x) > 0) ? (ENTU_EpanetError(ERRNUM)) : (0))
#define UCHAR(x) (((x) >= 'a' && (x) <= 'z') ? ((x)&~32) : (x))
#define MOD(x,y) ((x)%(y))
#define FILEWRITE(x,y) (((x) < (unsigned int)(y)) ? (enutilerror(1,"Too few bytes written")) : (0))

/* Epanet section type */
#ifdef MSEPANET
enum SectType    {_TITLE,    _JUNCTIONS,_RESERVOIRS,
                  _TANKS,    _PIPES,    _PUMPS,
                  _VALVES,   _CONTROLS, _RULES,
                  _DEMANDS,  _SOURCES,  _EMITTERS,
                  _PATTERNS, _CURVES,   _QUALITY,
                  _STATUS,   _ROUGHNESS,_ENERGY,
                  _REACTIONS,_MIXING,   _REPORT,
                  _TIMES,    _OPTIONS,  _COORDS,
                  _VERTICES, _LABELS,   _BACKDROP,
                  _TAGS,     _END,      _MSVAR,    
                  _MSPIPE,   _MSTANK,   _MSOPTIONS};
#else
enum SectType    {_TITLE,    _JUNCTIONS,_RESERVOIRS,
                  _TANKS,    _PIPES,    _PUMPS,
                  _VALVES,   _CONTROLS, _RULES,
                  _DEMANDS,  _SOURCES,  _EMITTERS,
                  _PATTERNS, _CURVES,   _QUALITY,
                  _STATUS,   _ROUGHNESS,_ENERGY,
                  _REACTIONS,_MIXING,   _REPORT,
                  _TIMES,    _OPTIONS,  _COORDS,
                  _VERTICES, _LABELS,   _BACKDROP,
                  _TAGS,     _END};
#endif
  
/*
**-------------------------------------------------------------------
** Exported methods
**-------------------------------------------------------------------
*/
/* ENUTIL.C */
void ENTU_Initialize(char *epanetfname, char *tsofname, int storageMethod, int tsofileversion, PTSO *tso, PNetInfo *net, PNodeInfo *nodes, PLinkInfo *links, PSource *sources);
void ENTU_CreateDemandPatterns(PNetInfo net, PNodeInfo nodes);
void ENTU_GetLinkQData(PLinkInfo, PNetInfo, int, long );
void ENTU_GetNodeQData(PNodeInfo, PNetInfo, int, long );
void ENTU_ClearNodeQData(PNodeInfo, PNetInfo );
void ENTU_GetLinkHData(PLinkInfo, PNetInfo, int, long );
void ENTU_GetNodeHData(PNodeInfo, PNetInfo, int, long );
void ENTU_GetNodeXY(PNodeInfo, PLinkInfo, PNetInfo, char*);
void ENTU_EpanetError(int);
void ENTU_WriteTSI(PNetInfo, PSource, FILE*, FILE*);
int  ENTU_SetSource(PSource, PNetInfo, FILE*);

#endif
