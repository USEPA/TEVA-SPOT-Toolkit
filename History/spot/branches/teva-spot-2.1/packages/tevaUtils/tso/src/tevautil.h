#ifndef _TEVAUTIL_H_
#define _TEVAUTIL_H_

#include <stdio.h>
#include <float.h>
#include "tevatypes.h"

#define MAXFNAME    512      /* Maximum chars in file name */
#define MAXSPECIEID 16       /* Maximum chars in Specie ID */

/*
**-------------------------------------------------------------------
** Exported methods
**-------------------------------------------------------------------
*/
/* TSO.C */
PTSO TSO_Open(char *filenameOrDirectory, char *pattern);
PTSO TSO_OpenNoHeader(char *filenameOrDirectory, char *pattern, char fileVersion, char storageMethod);
void TSO_PositionAfterPrologue(PTSO tso);
void TSO_ReadPrologue(PTSO tso, PNetInfo*, PNodeInfo *nodes,PLinkInfo *links);
int TSO_ReadSimulationResults(PTSO tso, PNetInfo net,  PNodeInfo nodes, PSource sources);
int TSO_GetSimulationResults(int id,PTSO tso, PNetInfo net,  PNodeInfo nodes, PSource sources);
int TSO_GetRawSimulationResults(int id, PTSO tso, PNetInfo net, PNodeInfo nodes,PSource sources, char *buf, int len);
PTSO TSO_Create(char *filename, int compressionLevel,PNetInfo net);
PTSO TSO_CreateSpecificVersion(char *filename, int compressionLevel, int fileVersion, PNetInfo ne);
int TSO_WritePrologue(PTSO tso, PNetInfo net, PNodeInfo nodes, PLinkInfo links);
int TSO_WriteSimResults(PTSO tso, PNetInfo net,PSource sources, PNodeInfo nodes);
void TSO_Close(PTSO *tso);
PNetInfo TSO_AllocateNetworkInfo(int nspecies);
void TSO_ReleaseNetworkInfo(PNetInfo *net);
void TSO_AllocateNetworkData(PNodeInfo *nodes, PLinkInfo *links, PNetInfo net);
void TSO_ReleaseNetworkData(PNodeInfo *nodes, PLinkInfo *links, PNetInfo net);

#endif
