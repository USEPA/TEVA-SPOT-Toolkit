#ifndef _TEVAUTIL_H_
#define _TEVAUTIL_H_

#include <stdio.h>
#include <float.h>
#include "tevatypes.h"

#define MAXFNAME    512      /* Maximum chars in file name */
#define MAXSPECIEID 16       /* Maximum chars in Specie ID */

// Error codes returned
#define TSO_UNABLE_TO_POSITION    -1
#define TSO_DATA_BUFFER_TOO_SMALL -2
#define TSO_INVALID_SOURCE_ID     -3
/*
**-------------------------------------------------------------------
** Exported methods
**-------------------------------------------------------------------
*/
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
/* TSO.C */
PTSO DLLEXPORT TSO_Open(char *filenameOrDirectory, char *pattern);
PTSO DLLEXPORT TSO_OpenNoHeader(char *filenameOrDirectory, char *pattern, char fileVersion, char storageMethod);
void DLLEXPORT TSO_PositionAfterPrologue(PTSO tso);
void DLLEXPORT TSO_ReadPrologue(PTSO tso, PNetInfo*, PNodeInfo *nodes,PLinkInfo *links);
int DLLEXPORT TSO_ReadSimulationResults(PTSO tso, PNetInfo net,  PNodeInfo nodes, PSource sources);
int DLLEXPORT TSO_GetSimulationResults(int id,PTSO tso, PNetInfo net,  PNodeInfo nodes, PSource sources);
int DLLEXPORT TSO_GetRawSimulationResults(int id, PTSO tso, PNetInfo net, PNodeInfo nodes,PSource sources, char *buf, int len);
PTSO DLLEXPORT TSO_Create(char *filename, int compressionLevel,PNetInfo net);
PTSO DLLEXPORT TSO_CreateSpecificVersion(char *filename, int compressionLevel, int fileVersion, PNetInfo ne);
int DLLEXPORT TSO_WritePrologue(PTSO tso, PNetInfo net, PNodeInfo nodes, PLinkInfo links);
int DLLEXPORT TSO_WriteSimResults(PTSO tso, PNetInfo net,PSource sources, PNodeInfo nodes);
void DLLEXPORT TSO_Close(PTSO *tso);
PNetInfo DLLEXPORT TSO_AllocateNetworkInfo(int nspecies);
void DLLEXPORT TSO_ReleaseNetworkInfo(PNetInfo *net);
void DLLEXPORT TSO_AllocateNetworkData(PNodeInfo *nodes, PLinkInfo *links, PNetInfo net);
void DLLEXPORT TSO_ReleaseNetworkData(PNodeInfo *nodes, PLinkInfo *links, PNetInfo net);

#endif
