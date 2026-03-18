#include "tevautil.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#ifdef WIN32
#include <io.h>
#else
#ifdef __linux__
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fnmatch.h>
#endif
#endif

/* methods local to this file*/
static int x_TSO_WriteDemands(PTSO tso, PNetInfo net,PNodeInfo nodes);
static PTSO x_TSO_Open(char *filenameOrDirectory, char *pattern, char fileVersion, char storageMethod) ;
static void x_TSO_ReadSizes(PTSO tso, PNetInfo *net);
static int x_TSO_WriteDemands(PTSO tso, PNetInfo net,PNodeInfo nodes);
static int x_compressAndStore2(float *dtc,int nsteps,FILE *stream);
static int x_compressAndStore3(float *dtc,int nsteps,FILE *stream);
static void MEMCHECK(void *x,char *msg);
static void tsoerror(int exitCode,const char *errmsg, ...);

static void MEMCHECK(void *x,char *msg) {
	if(x == NULL)
		tsoerror(1,"Allocating memory: %s",msg);
}

/*
**--------------------------------------------------------------
**  tsoerror
**  Input:   exit code
**           errmsg: printf-like varargs for the error message
**           to print before exiting
**  Output:  none
**  Purpose: Print an error message and exit the program
**--------------------------------------------------------------
*/
static void tsoerror(int exitCode,const char *errmsg, ...)
{
	va_list ap;

	fprintf(stderr,"\n\n********************************************************************************\n\n");
	va_start(ap,errmsg);
	vfprintf(stderr,errmsg,ap);
	va_end(ap);
	fprintf(stderr,"\n\n********************************************************************************\n\n");

	exit(exitCode);
}

/*
**--------------------------------------------------------------
**  x_TSO_Open
**  Input:   filenameOrDirectory: the filename of the TSO file,
**           or the directory containing the set of TSO file
**           pieces to read.  The pattern parameter MUST be NULL
**           if it is a single file.
**           fileVersion and storageMethod are only needed if
**           the input file does not have the header
**  Output:  A pointer to the structure used by the TSO_* routines
**  Purpose: Opens the TSO database file or files
**--------------------------------------------------------------
*/
static PTSO x_TSO_Open(char *filenameOrDirectory, char *pattern, char fileVersion, char storageMethod) {
	PTSO tso;
	char fn[MAXFNAME+1];
	char header[32];
	MEMCHECK(tso = (PTSO)calloc(1,sizeof(TSO)),"tso in x_TSO_Open");
	if(pattern == NULL) { // a single tso file
		printf("  TSO File Name: %s \n\n", filenameOrDirectory); /* Oct 6 */
		strcpy(fn,filenameOrDirectory);
	} else {
#ifdef WIN32
		char tsodir[MAXFNAME+1];
		char patt[MAXFNAME+1];
		struct _finddata_t files; 
		long hFile;
		char **filenames=NULL;
		int n;

		strcpy(tsodir,filenameOrDirectory);
 		sprintf(patt,"%s\\%s",tsodir,pattern);
		
		if((hFile=_findfirst(patt,&files)) == -1L) {
			tsoerror(1,"\n   No files found matching: '%s' in directory: '%s'\n",pattern,tsodir);
		}
		n=0;
		do {
			if(filenames==NULL)
				MEMCHECK(filenames = (char **)calloc(1,sizeof(char *)),"filenames in x_TSO_Open");
			else
				MEMCHECK(filenames = (char **) realloc(filenames,(n+1)*sizeof(char *)),"filenames in x_TSO_Open");
			MEMCHECK(filenames[n] = (char *)calloc(MAXFNAME+1,sizeof(char)),"filenames[n] in x_TSO_Open");
			sprintf(filenames[n],"%s\\%s",tsodir,files.name);
			n++;
		} while( _findnext(hFile,&files) == 0);
//		for(i=0;i<n;i++) {
//			printf("%s\n",filenames[i]);
//		}
		sprintf(fn,"%s\\output.hdr.tso",tsodir);
		tso->names=filenames;
		tso->numFiles=n;
		tso->curFile=-1;
		_findclose(hFile);
#else
#ifdef __linux__
		char tsodir[MAXFNAME+1];
		//		char patt[MAXFNAME+1];
		struct stat info;
		DIR *dir;
		struct dirent *entry;
		char **filenames=NULL;
		int i,n;

		strcpy(tsodir,filenameOrDirectory);
		// 		sprintf(patt,"%s/%s",tsodir,pattern);
		if(!(stat(tsodir,&info) == 0 && info.st_mode & S_IFDIR))
			tsoerror(2,"TSO directory is NOT a directory: %s\n",tsodir);

		if((dir=opendir(tsodir))==NULL) {
			tsoerror(1,"\n   No files found matching: '%s' in directory: '%s'\n",pattern,tsodir);
		}
		n=0;
		while(entry = readdir(dir)) {
			if(fnmatch(pattern,entry->d_name,0) ==0) {
				if(filenames==NULL)
					MEMCHECK(filenames = (char **)calloc(1,sizeof(char *)),"filenames in x_TSO_Open");
				else
					MEMCHECK(filenames = (char **) realloc(filenames,(n+1)*sizeof(char *)),"filenames in x_TSO_Open");
				MEMCHECK(filenames[n] = (char *)calloc(MAXFNAME+1,sizeof(char)),"filenames[n] in x_TSO_Open");
				sprintf(filenames[n],"%s/%s",tsodir,entry->d_name);
				n++;
			}
		}
//		for(i=0;i<n;i++) {
//			printf("%s\n",filenames[i]);
//		}
		sprintf(fn,"%s/output.hdr.tso",tsodir);
		tso->names=filenames;
		tso->numFiles=n;
		tso->curFile=-1;
#endif
#endif

	}
	if((tso->stream=fopen(fn,"rb"))==NULL) {
		tsoerror(0,"\nUnable to open file %s\nError: %d %s\n",filenameOrDirectory,errno,strerror(errno));
	}
	fread(&header,32,sizeof(char),tso->stream);
	if((header[0]&0xFF) != 0xFF) {
		if((fileVersion&0xFF) == 0xFF || (storageMethod&0xFF)==0xFF) {
			tsoerror(1,"TSO file does not have the header, use TSO_OpenNoHeader instead\n");
		} else {
			tso->fileVersion = fileVersion;
			tso->storageMethod = storageMethod;
			tso->nBytesForNodeID = 16;
			fseek(tso->stream,0L,SEEK_SET);
		}
	} else {
		tso->fileVersion = header[1];
		tso->storageMethod = header[2];
		if(tso->fileVersion >= 5) {
			tso->nBytesForNodeID = header[3];
		} else {
			tso->nBytesForNodeID = 16;
		}
	}
	if(tso->fileVersion > TSOFILEVERSION) {
		tsoerror(1,"TSO File version %d not supported by this version of the API (max file version supported: %d)\n",tso->fileVersion,TSOFILEVERSION);
	}
	if(tso->storageMethod > TSOSTORAGEMETHOD) {
		tsoerror(1,"TSO storage method %d not supported by this version of the API (max storage method supported: %d)\n",tso->storageMethod,TSOSTORAGEMETHOD);
	}
	return tso;
}

/*
**--------------------------------------------------------------
**  TSO_Open
**  Input:   filenameOrDirectory: the filename of the TSO file,
**           or the directory containing the set of TSO file
**           pieces to read.  The pattern parameter MUST be NULL
**           if it is a single file.
**  Output:  A pointer to the structure used by the TSO_* routines
**  Purpose: Opens the TSO database file or files
**--------------------------------------------------------------
*/
PTSO TSO_Open(char *filenameOrDirectory, char *pattern) {
	return x_TSO_Open(filenameOrDirectory,pattern,(char)0xFF,(char)0xFF);
}
/*
**--------------------------------------------------------------
**  TSO_OpenNoHeader
**  Input:   filenameOrDirectory: the filename of the TSO file,
**           or the directory containing the set of TSO file
**           pieces to read.  The pattern parameter MUST be NULL
**           if it is a single file.
**           fileVersion and storageMethod are only needed if
**           the input file does not have the header
**  Output:  A pointer to the structure used by the TSO_* routines
**  Purpose: Opens the TSO database file or files.  This method
**           MUST be used when the TSO database has no header
**           (i.e. it is version 1 or 2)
**--------------------------------------------------------------
*/
PTSO TSO_OpenNoHeader(char *filenameOrDirectory, char *pattern, char fileVersion, char storageMethod) {
	return x_TSO_Open(filenameOrDirectory,pattern,fileVersion, storageMethod);
	
}
/*
**--------------------------------------------------------------
**  x_TSO_ReadSizes
**  Input:   tso: the pointer returned by one of the TSO_Open methods
**           net: A pointer to the PNetInfo structure to poulate
**                from the TSO file
**  Output:  
**  Purpose: 
**--------------------------------------------------------------
*/
static void x_TSO_ReadSizes(PTSO tso, PNetInfo *net)
{
	int status=0;
	int nnodes,nlinks,ntanks,njunctions,nspecies,nsteps;
	float stepsize,fltmax;

	if ( fread(&nnodes,sizeof(int),1,tso->stream) 		< 1 )  status=1; /* nnodes */
	if ( fread(&nlinks,sizeof(int),1,tso->stream) 		< 1 )  status=1; /* nlinks */
	if ( fread(&ntanks,sizeof(int),1,tso->stream) 		< 1 )  status=1; /* ntanks */
	if ( fread(&njunctions,sizeof(int),1,tso->stream) < 1 ) status=1; /* njunctions */
	if(tso->fileVersion >= 4) {
		if ( fread(&nspecies,sizeof(int),1,tso->stream) < 1 ) status=1; /* nspecies */
	} else {
		nspecies = 1;
	}

	if ( fread(&nsteps,sizeof(int),1,tso->stream) < 1 )     status=1; /* nsteps */
	if ( fread(&stepsize,sizeof(float),1,tso->stream) < 1 ) status=1; /* stepsize */
	if ( fread(&fltmax,sizeof(float),1,tso->stream) < 1 )   status=1; /* max float */

	*net = TSO_AllocateNetworkInfo(nspecies);
	(*net)->nnodes = nnodes;
	(*net)->nlinks = nlinks;
	(*net)->ntanks = ntanks;
	(*net)->njunctions = njunctions;
	(*net)->nsteps = nsteps;
	(*net)->stepsize = stepsize;
	(*net)->fltmax = fltmax;

	if(status) tsoerror(1,"Reading size header in ReadSize");
}

/*
**--------------------------------------------------------------
**  TSO_ReadPrologue
**  Input:   Pointer to TSO data structure (returned from call
**           to one of the TSO_Open routines)
**  Output:  node: populated set of NodeInfo structures (including
**           demands)
**           link: populated set of LinkInfo structures
**
**           Note: the node and link arrays are allocated by 
**           this routine
**  Purpose: Reads the file prologue and populates the node and
**           link data
**--------------------------------------------------------------
*/
void TSO_ReadPrologue(PTSO tso, PNetInfo *netInfo, PNodeInfo *nodes,PLinkInfo *links)
{
    int i,j,status=0;
    FILE *TSO=tso->stream;
	int ntanks,
		nnodes,
		nlinks,
		nsteps;
	PNodeInfo node;
	PLinkInfo link;
	PNetInfo net;

	x_TSO_ReadSizes(tso,netInfo);
	net=*netInfo;
	MEMCHECK(tso->nodedata = (float*)calloc(net->nnodes,sizeof(float)),"tso->nodedata in TSO_ReadPrologue");
	MEMCHECK(tso->linkdata = (float*)calloc(net->nlinks,sizeof(float)),"tso->linkdata in TSO_ReadPrologue");
	TSO_AllocateNetworkData(nodes,links,net);
	node=*nodes;
	link=*links;
	ntanks = net->ntanks,
	nnodes = net->nnodes,
	nlinks = net->nlinks,
	nsteps = net->nsteps;

    for (i=0;i<ntanks;i++) {
        if(fread(&j,sizeof(int),1,TSO) < 1) status=1;
        else node[j-1].ntype=tank;
    }
    for (i=0;i<nnodes;i++) {
        if(fread(node[i].id,sizeof(char),tso->nBytesForNodeID,TSO) < (unsigned int)tso->nBytesForNodeID) status=1;
    }
	for (i=0;i<net->nspecies;i++) {
		if(tso->fileVersion >= 4) {
		    if(fread(net->species[i],sizeof(char),16,TSO) < 16) status=1;
		} else {
			strcpy(net->species[i],"Unknown");
		}
	}
    for (i=0;i<nnodes;i++) {
        if(fread(&node[i].x,sizeof(float),1,TSO) < 1) status=1;
    }
    for (i=0;i<nnodes;i++) {
        if(fread(&node[i].y,sizeof(float),1,TSO) < 1) status=1;
    }
    for (i=0;i<nlinks;i++) {
        if(fread(&link[i].from,sizeof(int),1,TSO) < 1) status=1;
    }
    for (i=0;i<nlinks;i++) {
        if(fread(&link[i].to,sizeof(int),1,TSO) < 1) status=1;
    }
    for(i=0;i<nsteps;i++) {
        if(fread(tso->nodedata,sizeof(float),nnodes,TSO) < (unsigned int)nnodes) status=1;
        for(j=0;j<nnodes;j++) {
            node[j].q[i] = tso->nodedata[j];
        }
	}
	if(tso->fileVersion >=4) {
		for(i=0;i<nsteps;i++) {
			if(fread(tso->linkdata,sizeof(float),nlinks,TSO) < (unsigned int)nlinks) status=1;
			for(j=0;j<nlinks;j++) {
				link[j].flow[i] = tso->linkdata[j];
			}
		}
		for(i=0;i<nsteps;i++) {
			if(fread(tso->linkdata,sizeof(float),nlinks,TSO) < (unsigned int)nlinks) status=1;
			for(j=0;j<nlinks;j++) {
				link[j].velocity[i] = tso->linkdata[j];
			}
		}
	}
    if(status) tsoerror(1,"Reading TSO prologue in ReadPrologue");
    if(fgetpos(TSO,&tso->pos_cdata)) tsoerror(1,"Reading TSO prologue in ReadPrologue");
//	printf("tso->pos_cdata: %d\n",tso->pos_cdata);
	*nodes=node;
	*links=link;
}
/*
**--------------------------------------------------------------
**  TSO_PositionAfterPrologue
**  Input:   Pointer to TSO data structure (returned from call
**           to one of the TSO_Open routines)
**  Output:  
**  Purpose: Resets the TSO file pointer to the beginning of the
**           simulation data so the file can be re-processed
**--------------------------------------------------------------
*/
void TSO_PositionAfterPrologue(PTSO tso)
{
	if(tso->numFiles==0) {
		fsetpos(tso->stream,&tso->pos_cdata);
	} else {
		if(tso->stream != NULL)
			fclose(tso->stream);
		tso->curFile=0;
		tso->stream = fopen(tso->names[tso->curFile],"rb");
	}

}
/*
**--------------------------------------------------------------
**  TSO_eof
**  Input:   Pointer to TSO data structure (returned from call
**           to one of the TSO_Open routines)
**  Output:  
**  Purpose: Checks to see if the end of the TSO file or files
**           has been reached
**--------------------------------------------------------------
*/
int TSO_eof(PTSO tso)
{
	fgetc(tso->stream);
	if(feof(tso->stream)){
		if(tso->numFiles>0) {
			fclose(tso->stream);
			tso->stream=NULL;
			tso->curFile++;
			if(tso->curFile>=tso->numFiles) return 1;
			else {
//				printf("Opening next TSO File: %s\n", tso->names[tso->curFile]);
				tso->stream=fopen(tso->names[tso->curFile],"rb");
			}
		} else {
			return 1;
		}
	} else {
		fseek(tso->stream,-1,SEEK_CUR);
	}
	return 0;
}
/*
**--------------------------------------------------------------
**  TSO_ReadQuality
**  Input:   Pointer to TSO data structure (returned from call
**           to one of the TSO_Open routines)
**           net: pointer to network information
**  Output:  nodes: NodeInfo structures with the conecntration
**                  values poulated
**           sources: populated set of Source structures
**           net->nsources: Set to the number of sources for
**                          the current simulation results
**  Purpose: Reads output from one simulation (source information
**           and concentration data)
**--------------------------------------------------------------
*/
int TSO_ReadSimulationResults(PTSO tso, PNetInfo net, PNodeInfo nodes,PSource sources)
{

    int idx,
        nnz,
        i,
		s,
        status=0,
		nnodes=net->nnodes,
		nsteps=net->nsteps;
	int nspecies=net->nspecies;
	FILE *TSO;
	if(TSO_eof(tso)) return(0);
	TSO=tso->stream;
    if(fread(&net->nsources,sizeof(int),1,TSO) < 1);
    if(net->nsources>MAXSOURCES) tsoerror(1,"Too many sources: %d.",net->nsources);

    for(i=0;i<net->nsources;i++) {
        if(fread(&sources[i].SourceNodeIndex,sizeof(int),1,TSO) < 1) status=1;
		strcpy(sources[i].SourceNodeID,nodes[sources[i].SourceNodeIndex-1].id);
		if(tso->fileVersion>=4) {
			if(fread(&sources[i].SpeciesIndex,sizeof(int),1,TSO) < 1) status=1;
			if(fread(&sources[i].SourceType,sizeof(int),1,TSO) < 1) status=1;
		} else {
			sources[i].SpeciesIndex=0;
			sources[i].SourceType=1;
		}
		if(tso->fileVersion>=2) {
			if(fread(&sources[i].SourceStrength,sizeof(float),1,TSO) < 1) status=1;
			if(fread(&sources[i].SourceStart,sizeof(long),1,TSO) < 1) status=1;
			if(fread(&sources[i].SourceStop,sizeof(long),1,TSO) < 1) status=1;
		}
    }

	for(s=0;s<nspecies;s++) {
		if(tso->storageMethod==0) {
			/* Read concentrations */
			for(i=0;i<nsteps;i++) {
				int j;
				if(fread(tso->nodedata,sizeof(float),nnodes,TSO) < (unsigned int)nnodes) status=1;
				for(j=0;j<nnodes;j++) {
					nodes[j].c[s][i] = tso->nodedata[j];
				}
			}
		} else {
			unsigned short groupTypeMask;
			unsigned short countMask;
			unsigned short countBits;
			if(tso->storageMethod==2) {
				groupTypeMask = 0x8000;
				countMask =     0x7FFF;
				countBits = 15;
			} else {
				groupTypeMask = 0xC000;
				countMask =     0x3FFF;
				countBits = 14;
			}

			for(i=0;i<nnodes;i++) {
				nodes[i].nz[s]=0;
				memset(nodes[i].c[s],0,sizeof(float)*nsteps);
			}  /* default is all zeros */
			if(fread(&nnz,sizeof(int),1,TSO) < 1) status=1;
			for(i=0;i<nnz;i++) {
				if(fread(&idx,sizeof(int),1,TSO) < 1) status=1;
				idx--;
				nodes[idx].nz[s]=1;
				if(tso->storageMethod==1) {
					// if compressed using type 1, simply read in the entire data array
					if(fread(nodes[idx].c[s],sizeof(float),nsteps,TSO) < (unsigned int)nsteps) status=1;

				} else {
					float *c = nodes[idx].c[s];
					// must be type 2 or 3 - read in the compressed data
					unsigned short numGroups;
					int p=1,
						n=0,
						i,
						max=0;
					if(tso->storageMethod==2) {
						unsigned char ng;
						if(fread(&ng,sizeof(char),1,TSO) < 1) status=1;
						numGroups=ng;
					} else {
						if(fread(&numGroups,sizeof(short),1,TSO) < 1) status=1;
					}
					for(i=0;i<numGroups;i++) {
						GroupHeader gh;
						short sval;
						if(fread(&sval,1,sizeof(short),TSO) < 1) status=1;
						gh.groupType = (sval & groupTypeMask)>>countBits;
						gh.count = sval & countMask;
						max += gh.count;
						if (gh.groupType==1) {
							if(fread(&c[n],sizeof(float),gh.count,TSO) < (unsigned int)gh.count) status=1;
							n = max;
						} else { 
							float v;
							if(gh.groupType==0) v = 0;
							else if (gh.groupType==2) {
								if(fread(&v,sizeof(float),1,TSO) < 1) status=1;
							} else {
								tsoerror(1,"Invalid groupType in ReadNextC");
							}
							while(n < max) { c[n++] = v; }
						}
					}
				}
			}
		}
	}

    if(status) tsoerror(1,"Reading TSO concentrations in ReadNextC");
    return(1);
}
/*
**--------------------------------------------------------------
**  TSO_Create
**  Input:   Filename of the TSO file to create
**           The storage method to use when writing data to the
**           file
**  Output:  Pointer to TSO data structure representing
**           a TSO file to write to.
**  Purpose: Creates a new TSO file utilizing the specified
**           storage method
**--------------------------------------------------------------
*/
PTSO TSO_Create(char *filename, int storageMethod, PNetInfo net)
{
	PTSO tso;
	MEMCHECK(tso = (PTSO)calloc(1,sizeof(TSO)),"tso in TSO_Create");

	tso->fileVersion=TSOFILEVERSION;
	tso->storageMethod=storageMethod==-1?TSOSTORAGEMETHOD:storageMethod;
	tso->nBytesForNodeID = 64;
	tso->header[0] = (char)0xff;
	tso->header[1] = tso->fileVersion;
	tso->header[2] = tso->storageMethod;
	tso->header[3] = tso->nBytesForNodeID;
	MEMCHECK(tso->nodedata = (float *)calloc(net->nnodes,sizeof(float)),"tso->nodedata in TSO_Create");
	MEMCHECK(tso->linkdata = (float *)calloc(net->nlinks,sizeof(float)),"tso->linkdata in TSO_Create");

	if((tso->stream = fopen(filename,"wb"))==NULL) {
		tsoerror(1,"Unable to open file %s\nError: %d %s\n",filename,errno,strerror(errno));
	}
	if(tso->fileVersion >= 3)
		if( fwrite(tso->header,sizeof(tso->header),1,tso->stream) < 1) return NULL;
	
	return tso;
}

/*
**--------------------------------------------------------------
**  TSO_CreateSpecofocVersion
**  Input:   Filename of the TSO file to create
**           The storage method to use when writing data to the
**           file
**  Output:  Pointer to TSO data structure representing
**           a TSO file to write to.
**  Purpose: Creates a new TSO file utilizing the specified
**           storage method
**--------------------------------------------------------------
*/
PTSO TSO_CreateSpecificVersion(char *filename, int storageMethod, int fileVersion, PNetInfo net)
{
	PTSO tso;
	
	MEMCHECK(tso = (PTSO)calloc(1,sizeof(TSO)),"tso in TSO_CreateSpecificVersion");

	tso->fileVersion=fileVersion;
	tso->storageMethod=storageMethod;
	tso->nBytesForNodeID = tso->fileVersion >= 5 ? 64 : 16;
	tso->header[0] = (char)0xff;
	tso->header[1] = tso->fileVersion;
	tso->header[2] = tso->storageMethod;
	tso->header[3] = tso->nBytesForNodeID;
	MEMCHECK(tso->nodedata = (float *)calloc(net->nnodes,sizeof(float)),"tso->nodedata in TSO_CreateSpecificVersion");
	MEMCHECK(tso->linkdata = (float *)calloc(net->nlinks,sizeof(float)),"tso->linkdata in TSO_CreateSpecificVersion");

	if((tso->stream = fopen(filename,"wb"))==NULL) {
		tsoerror(1,"Unable to open file %s\nError: %d %s\n",filename,errno,strerror(errno));
	}
	if(tso->fileVersion >= 3)
		if( fwrite(tso->header,sizeof(tso->header),1,tso->stream) < 1) return NULL;
	
	return tso;
}

/*
**--------------------------------------------------------------
**  TSO_WritePrologue
**  Input:   Pointer to TSO data structure (returned from call
**           to TSO_Create)
**           network size information
**           node collection
**           link collection
**  Output:  
**  Purpose: Write the file prologue from the node and link data
**--------------------------------------------------------------
*/
int TSO_WritePrologue(PTSO tso, PNetInfo net, PNodeInfo nodes, PLinkInfo links)
{
	int i,
		status;
	fwrite(&net->nnodes,sizeof(int),1,tso->stream);
	fwrite(&net->nlinks,sizeof(int),1,tso->stream);
	fwrite(&net->ntanks,sizeof(int),1,tso->stream);
	fwrite(&net->njunctions,sizeof(int),1,tso->stream);
	if(tso->fileVersion >= 4)
		fwrite(&net->nspecies,sizeof(int),1,tso->stream);

	fwrite(&net->nsteps,sizeof(int),1,tso->stream);
	fwrite(&net->stepsize,sizeof(int),1,tso->stream);
	fwrite(&net->fltmax,sizeof(int),1,tso->stream);

	/* Tank indices */
	for (i=1; i<=net->nnodes; i++) {
		if(nodes[i-1].ntype==tank)
			if ( fwrite(&i,sizeof(int),1,tso->stream) < 1 ) {status = 400; return(status);}
	}
	/* Node IDs */
	for (i=0; i<net->nnodes; i++) {
		if ( fwrite(nodes[i].id,sizeof(char),tso->nBytesForNodeID,tso->stream) < (unsigned int)tso->nBytesForNodeID ) {status = 400; return(status);}
	}
	if(tso->fileVersion >=4) {
		for(i=0;i<net->nspecies;i++) {
			if ( fwrite(net->species[i],sizeof(char),16,tso->stream) < 16 ) {status = 400; return(status);}
		}
	}
	/* Node X locations */
	for (i=0; i<net->nnodes; i++) {
		if ( fwrite(&nodes[i].x,sizeof(float),1,tso->stream) < 1 ) {status = 400; return(status);}
	}
	/* Node Y locations */
	for (i=0; i<net->nnodes; i++) {
		if ( fwrite(&nodes[i].y,sizeof(float),1,tso->stream) < 1 ) {status = 400; return(status);}
	}
	for (i=0; i<net->nlinks; i++) {                                                         /* From link nodes */
		if ( fwrite(&links[i].from,sizeof(int),1,tso->stream) < 1 ) {status = 400; return(status);}
	}
	for (i=0; i<net->nlinks; i++) {                                                         /* To link nodes */
		if ( fwrite(&links[i].to,sizeof(int),1,tso->stream) < 1 ) {status = 400; return(status);}
	}
	x_TSO_WriteDemands(tso,net,nodes);
	if(tso->fileVersion >= 4) {
		int t;
		int nsteps = net->nsteps;
		for(t=0;t<nsteps;t++) {
			for (i=0; i<net->nlinks; i++) {                                                         /* link flow */
				if ( fwrite(&links[i].flow[t],sizeof(float),1,tso->stream) < 1 ) {status = 400; return(status);}
			}
		}
		for(t=0;t<nsteps;t++) {
			for (i=0; i<net->nlinks; i++) {                                                         /* link velocity*/
				if ( fwrite(&links[i].velocity[t],sizeof(float),1,tso->stream) < 1 ) {status = 400; return(status);}
			}
		}
	}
	return 0;
}
/*
**--------------------------------------------------------------
**  x_TSO_WriteDemands
**  Input:   Pointer to TSO data structure (returned from call
**           to TSO_Create)
**           network size information
**           node collection with the demand data populated
**  Output:  
**  Purpose: Write the file prologue from the node and link data
**--------------------------------------------------------------
*/
static int x_TSO_WriteDemands(PTSO tso, PNetInfo net,PNodeInfo nodes)
{
	int t,
		n,
		nsteps=net->nsteps,
		nnodes=net->nnodes;
	for(t=0;t<nsteps;t++) {
		for(n=0;n<nnodes;n++) {
			if( fwrite(&nodes[n].q[t],sizeof(float),1,tso->stream) < 1) return 0;
		}
	}
	return 1;
}

/*
**--------------------------------------------------------------
**  x_compressAndStore3
**  Input:   Pointer to file stream to write the compressed data
**           the array of floats to "compress" using storage method 3
**           the number of elements in the data
**  Output:  
**  Purpose: "compress" the conecntration data and write the data
**           to the TSO file.
**--------------------------------------------------------------
*/
static int x_compressAndStore3(float *dtc,int nsteps,FILE *stream)
{
	char *cd;
	float *values;
	short *counts;
	// numGroups is a short (rather than a char) to allow for the rare (at least currently)
	// condition of having more than 255 groups in one record (I have recently seen one
	// with 177 groups.  I chose to make this a short rather than doing anything more complicated
	// algorithmically because adding one byte to every record is negligable, and any algorithm
	// implemented to deal with more than 255 groups would be time intensive and would use extra space,
	short numGroups=0;
	int numValues;
	int p=2;
	int i,j,rv;
	
	MEMCHECK(cd = (char *)calloc((nsteps+10)*sizeof(float),sizeof(char)),"cd in x_compressAndStore3");

	MEMCHECK(values = (float *)calloc(nsteps,sizeof(float)),"values in x_compressAndStore3");
	MEMCHECK(counts = (short *)calloc(nsteps,sizeof(short)),"counts in x_compressAndStore3");

	values[0]=dtc[0];
	counts[0]=1;
	i=1;
	j=0;
	while(i<nsteps) {
		if(dtc[i] != values[j]) {
			j++;
			values[j] = dtc[i];
		}
		counts[j]++;
		i++;
	}
	numValues=j+1;
	i=0;
	while(i<numValues) {
		if (counts[i]>1 || values[i]==0) {
			float v = values[i];
			short mask = v==0 ? (short)0x0000 : (short)0x8000;
			short sv = counts[i] | mask;
			memcpy(&cd[p],&sv,sizeof(short));
			p+=2;
			if(v!=0) {
				memcpy(&cd[p],&v,sizeof(float));
				p+=4;
			}
			i++;
		} else {
			// save the start pos (to write out the total later)
			int cp = p;
			short count=0;
			p += 2; // start after the size word
			while(counts[i]==1 && i<numValues) {
				memcpy(&cd[p],&values[i],sizeof(float));
				p+=4;
				count++;
				i++;
			}
			count |= 0x4000;
			memcpy(&cd[cp],&count,sizeof(short));
		}
		numGroups++;
	}
	memcpy(&cd[0],&numGroups,sizeof(short));
	rv = fwrite(cd,sizeof(char),p,stream) == (unsigned int)p;
	free(cd);
	free(values);
	free(counts);
	return rv;
}
/*
**--------------------------------------------------------------
**  x_compressAndStore2
**  Input:   Pointer to file stream to write the compressed data
**           the array of floats to "compress" using storage method 2
**           the number of elements in the data
**  Output:  
**  Purpose: "compress" the conecntration data and write the data
**           to the TSO file.
**--------------------------------------------------------------
*/
static int x_compressAndStore2(float *dtc,int nsteps,FILE *stream)
{
	char *cd;
	unsigned char numGroups=0;
	int p=1;
	int i=0,
		rv;
	MEMCHECK(cd = (char *)calloc((nsteps+10)*sizeof(float),sizeof(char)),"cd in x_compressAndStore2");
	while(i<nsteps) {
		// if we have more than 254 groups, then output all the rest as one group with group type = 1.
		// this may not be the most efficient, but it is the quickest to implement.  An example of a bad case:
		// group 255 is type 1 with one value and group #256 is a type 0 (all zeros) with 100 values
		// this logic will cause 101 floats to be written out with most f them being 0.
		// this is probably not too likely to happen currently.  The following solution is likely
		// the best solution that will be implemented for type2 storage because storage method 3
		// should be used because it is more efficient
		if(numGroups==254) {
			int cp = p;
			short count=0;
			p += 2; // start after the size word
			while(i<nsteps) {
				memcpy(&cd[p],&dtc[i],sizeof(float));
				p+=4;
				count++;
				i++;
			}
			count |= 0x8000;
			memcpy(&cd[cp],&count,sizeof(short));
		} else if(dtc[i]==0) { // start of a series of zeros
			short numZeros=0;
			while(i<nsteps && dtc[i] == 0) {numZeros++; i++;}
			// write the short to the buffer at p
			memcpy(&cd[p],&numZeros,sizeof(short));
			p+=2;
		} else {  // start of a series of non-zero data
			// save the start pos (to write out the total later)
			int cp = p;
			short count=0;
			p += 2; // start after the size word
			while(i<nsteps && dtc[i] != 0) {
				memcpy(&cd[p],&dtc[i],sizeof(float));
				p+=4;
				count++;
				i++;
			}
			count |= 0x8000;
			memcpy(&cd[cp],&count,sizeof(short));
		}
		numGroups++;
	}
	cd[0] = numGroups;
	rv = fwrite(cd,sizeof(char),p,stream) == (unsigned int)p;
	free(cd);
	return rv;
}
/*
**--------------------------------------------------------------
**  TSO_WriteSimResults
**  Input:   Pointer to TSO data structure (returned from call
**           to TSO_Create)
**           network size information
**           source information
**           node collection
**  Output:  
**  Purpose: Write the file prologue from the node and link data
**--------------------------------------------------------------
*/
int TSO_WriteSimResults(PTSO tso, PNetInfo net,PSource sources,PNodeInfo nodes)
{
	int i,
		t,
		n,
		s,
		nsteps=net->nsteps,
		nnodes=net->nnodes;
	int nsources=net->nsources;
	if( fwrite(&nsources,sizeof(float),1,tso->stream) < 1) return 0;
	for(i=0;i<nsources;i++) {
		if( fwrite(&sources[i].SourceNodeIndex,sizeof(int),1,tso->stream) < 1) return 0;
		if(tso->fileVersion >= 4) {
			if( fwrite(&sources[i].SpeciesIndex,sizeof(int),1,tso->stream) < 1) return 0;
			if( fwrite(&sources[i].SourceType,sizeof(int),1,tso->stream) < 1) return 0;
		}
		if(tso->fileVersion >= 2) {
			if( fwrite(&sources[i].SourceStrength,sizeof(float),1,tso->stream) < 1) return 0;
			if( fwrite(&sources[i].SourceStart,sizeof(long),1,tso->stream) < 1) return 0;
			if( fwrite(&sources[i].SourceStop,sizeof(long),1,tso->stream) < 1) return 0;
		}
	}
	for(s=0;s<net->nspecies;s++) {
		for(n=0;n<nnodes;n++) {
			nodes[n].nz[s]=0;
		}
		if(tso->storageMethod == 0) {
			for(t=0;t<nsteps;t++) {
				for(n=0;n<nnodes;n++) {
					if( fwrite(&nodes[n].c[s][t],sizeof(float),1,tso->stream) < 1) return 0;
				}
			}
		} else {
			int n,t,nnz;

			for(n=0;n<nnodes;n++) {
				for(t=0;t<nsteps;t++) {
					float v = nodes[n].c[s][t];
					if(v != 0) nodes[n].nz[s]=1;
				}
			}
			nnz=0;
			for(n=0;n<nnodes;n++) nnz += nodes[n].nz[s];
			if(fwrite(&nnz,sizeof(int),1,tso->stream) < 1) return 0;
			for(n=1;n<=nnodes;n++) {
				PNodeInfo node = &nodes[n-1];
				if(node->nz[s]) {
					if(fwrite(&n,sizeof(int),1,tso->stream) < 1) return 0;
					if(tso->storageMethod==1) {
						if(fwrite(node->c[s],sizeof(float),nsteps,tso->stream) < (unsigned int)nsteps) return 0;
					} else if(tso->storageMethod==2) {
						if(!x_compressAndStore2(node->c[s],nsteps,tso->stream)) return 0;
					} else if(tso->storageMethod==3) {
						if(!x_compressAndStore3(node->c[s],nsteps,tso->stream)) return 0;
					}
				}
			}
		}
	}
	return 1;
}
/*
**--------------------------------------------------------------
**  TSO_Close
**  Input:   Pointer to TSO data structure (returned from call
**           to TSO_Create or TSO_Open)
**  Output:  
**  Purpose: Close the file and free up any memory used
**--------------------------------------------------------------
*/
void TSO_Close(PTSO *tso)
{
	int i;
	if(tso == NULL) return;
	if(*tso == NULL) return;
	if((*tso)->nodedata != NULL)
		free((*tso)->nodedata);
	if((*tso)->linkdata != NULL)
		free((*tso)->linkdata);
	for(i=0; i < (*tso)->numFiles; i++) {
		free((*tso)->names[i]);
	}
	free((*tso)->names);
	if((*tso)->stream != NULL)
	  fclose((*tso)->stream);
	free(*tso);
	*tso=NULL;
}

/*
**--------------------------------------------------------------
**  TSO_AllocateNetworkInfo
**  Input:   number of species that will be used
**  Output:  Pointer to newly allocated NetInfo data structure
**  Purpose: Allocate memory needed by the NetInfo data structure
**--------------------------------------------------------------
*/
PNetInfo TSO_AllocateNetworkInfo(int nspecies) {
	PNetInfo net;
	int i;
	MEMCHECK(net = (PNetInfo)calloc(1,sizeof(NetInfo)),"net in TSO_AllocateNetworkInfo");

	net->nspecies=nspecies;
	MEMCHECK( net->species = (char **)calloc(net->nspecies,sizeof(char*)),"net->species in TSO_AllocateNetworkInfo" );
	for(i=0;i<net->nspecies;i++) {
		MEMCHECK( net->species[i] = (char *)calloc(16+1,sizeof(char)),"net->species[i] in TSO_AllocateNetworkInfo" );
	}
	return net;
}

/*
**--------------------------------------------------------------
**  TSO_ReleaseNetworkInfo
**  Input:   Pointer to NetInfo data structure
**  Output:  None
**  Purpose: Release memory allocated to the NetInfo data structure
**--------------------------------------------------------------
*/
void TSO_ReleaseNetworkInfo(PNetInfo *net) {
	int i;
	for(i=0;i<(*net)->nspecies;i++) {
		free((*net)->species[i]);
	}
	free((*net)->species);
	free(*net);
	*net=NULL;
}

/*
**--------------------------------------------------------------
**  TSO_AllocateNetworkData
**  Input:   NetInfo data
**  Output:  References to newly allocated memory for the
**           NodeInfo and LinkInfo data structures based on the
**           parameters in the input data
**  Purpose: Allocate memory needed by the NodeInfo and LinkInfo
**           data structures
**--------------------------------------------------------------
*/
void TSO_AllocateNetworkData(PNodeInfo *nodes, PLinkInfo *links, PNetInfo net) {
    int i,s;
	PNodeInfo tnodes;
	PLinkInfo tlinks;
    MEMCHECK(tnodes=*nodes = (PNodeInfo)calloc(net->nnodes,sizeof(NodeInfo)),"*nodes in TSO_AllocateNetworkData");
    for (i=0; i<net->nnodes; i++) {                                        /* Tank indices */
        tnodes[i].ntype = junction;
		tnodes[i].x = net->fltmax;
		tnodes[i].y = net->fltmax;
		MEMCHECK(tnodes[i].id = (char*)calloc(64+1,sizeof(char)),"nodes[i].id in TSO_AllocateNetworkData");
        MEMCHECK(tnodes[i].nz = (int*)calloc(net->nspecies,sizeof(int)),"tnodes[i].nz in TSO_AllocateNetworkData");
        MEMCHECK(tnodes[i].c = (float**)calloc(net->nspecies,sizeof(float*)),"tnodes[i].c in TSO_AllocateNetworkData");
		for(s=0;s<net->nspecies;s++) {
			MEMCHECK(tnodes[i].c[s] = (float*)calloc(net->nsteps,sizeof(float)),"tnodes[i].c[s] in TSO_AllocateNetworkData");
		}
        MEMCHECK(tnodes[i].q = (float*)calloc(net->nsteps,sizeof(float)),"tnodes[i].q in TSO_AllocateNetworkData");
    }

    MEMCHECK(tlinks = *links = (PLinkInfo)calloc(net->nlinks,sizeof(LinkInfo)),"tlinks[i] in TSO_AllocateNetworkData");
    for (i=0; i<net->nlinks; i++) {                                        /* From link nodes */
		MEMCHECK(tlinks[i].flow=(float *)calloc(net->nsteps,sizeof(float)),"tlinks[i].flow in TSO_AllocateNetworkData");;
		MEMCHECK(tlinks[i].velocity=(float *)calloc(net->nsteps,sizeof(float)),"tlinks[i].velocity in TSO_AllocateNetworkData");;
    }
}

/*
**--------------------------------------------------------------
**  TSO_ReleaseNetworkData
**  Input:   references to NodeInfo, LinkInfo and NetInfo data
**  Output:  None
**  Purpose: Rerlease memory previously allocated to the NodeInfo
**           and LinkInfo data structures
**--------------------------------------------------------------
*/
void TSO_ReleaseNetworkData(PNodeInfo *nodes, PLinkInfo *links, PNetInfo net) {
    int i,s;
	PNodeInfo tnodes=*nodes;
	PLinkInfo tlinks=*links;
    for (i=0; i<net->nnodes; i++) {                                        /* Tank indices */
        free(tnodes[i].nz);
		free(tnodes[i].id);
		for(s=0;s<net->nspecies;s++) {
			free(tnodes[i].c[s]);
		}
        free(tnodes[i].c);
        free(tnodes[i].q);
    }
	free(*nodes);
	*nodes=NULL;

    for (i=0; i<net->nlinks; i++) {                                        /* From link nodes */
		free(tlinks[i].flow);
		free(tlinks[i].velocity);
    }
	free(*links);
	*links=NULL;
}
