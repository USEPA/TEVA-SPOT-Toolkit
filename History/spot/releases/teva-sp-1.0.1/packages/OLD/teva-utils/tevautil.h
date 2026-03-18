#ifndef _TEVAUTIL_H_
#define _TEVAUTIL_H_

#include <stdio.h>
#define   MAXSOURCES  100      /* Maximum number of sources  */
#define	  MAXCHARID   64       /* Maximum chars in Node ID   */
#define   MAXFNAME    512      /* Maximum chars in file name */

#define TSOFILEVERSION   5
#define TSOSTORAGEMETHOD 3    // default if not specified
/*
**-------------------------------------------------------------------
** Node Type
**-------------------------------------------------------------------
*/
enum NodeType
{
	junction,
	tank,
};
typedef struct
{
	char  SourceNodeID[MAXCHARID+1];
	int   SourceNodeIndex;
	int   SourceType;
	int   SpeciesIndex;
	long  SourceStart;
	long  SourceStop;
	float SourceStrength;
} Source, *PSource;

typedef struct {
    char groupType;
    short count;
} GroupHeader, *PGroupHeader;

typedef struct {
	char header[32];
	FILE *stream;
	int numFiles;
	char **names;
	int curFile;
	// 0 = all data - time x nodes
	// 1 = nodes x time - only nodes w/non-zero
	// 2 = nodes x time - only nodes w/non-zero, grouped
	int storageMethod;
	// 1 = orignal version
	// 2 = added source start/stop/strength to each simulation output
	// 3 = added file header
	int fileVersion;
	float *linkdata;
	int nBytesForNodeID;
	fpos_t pos_cdata;
	float *nodedata;
} TSO, *PTSO;

typedef struct
{
	int nnodes;
	int nlinks;
	int ntanks;
	int njunctions;
	int nsteps;
	int nsources;
	int nspecies;
	char  **species;
	float stepsize;
	float fltmax;
} NetInfo, *PNetInfo;

typedef struct
{
	enum  NodeType ntype;
	float x;
	float y;
	float *q;
	float **c;
	int *nz; // one for each species
	char  *id;
} NodeInfo, *PNodeInfo;

typedef struct
{
	int  from;
	int  to;
	float *velocity;
	float *flow;
} LinkInfo, *PLinkInfo;


PTSO TSO_Open(char *filenameOrDirectory, char *pattern);
PTSO TSO_OpenNoHeader(char *filenameOrDirectory, char *pattern, char fileVersion, char storageMethod);
void TSO_PositionAfterPrologue(PTSO tso);
void TSO_ReadSizes(PTSO tso, PNetInfo);
void TSO_ReadPrologue(PTSO tso, PNetInfo*, PNodeInfo *nodes,PLinkInfo *links);
int TSO_ReadSimulationResults(PTSO tso, PNetInfo net,  PNodeInfo nodes, PSource sources);
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
