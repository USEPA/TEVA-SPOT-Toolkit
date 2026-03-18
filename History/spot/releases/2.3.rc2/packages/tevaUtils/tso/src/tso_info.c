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
#include <stdarg.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tevautil.h"


int main(int argc, char **argv)
{
	char *directoryOrFile,
		*pattern=NULL;
	char storageMethod=-1,
		fileVersion=-1;
	int debug=0;
	PNodeInfo nodes;
	PLinkInfo links;
	PNetInfo net;
	PTSO tso;
	PSource sources;
	int printCount=0;
	int printInjections=0;

	sources=(PSource)calloc(MAXSOURCES,sizeof(Source));

	if(argc <2 || argc >6) {
		printf("usage: tsoinfo directoryOrFile [patternIfDirectory] [-printCount] [-printInjections] [-sStorageMethod -fFileVersion]\n");
		exit(9);
	}

	directoryOrFile = argv[1];
	if(argc > 2) {
		int ai = 2;
		if(argv[ai][0] != '-') {
			pattern = argv[ai++];
		}
		while(ai<argc) {
			if(strncmp(argv[ai],"-s",2)==0) {
				storageMethod = atoi(&argv[ai][2]);
			} else if(strncmp(argv[ai],"-f",2)==0) {
				fileVersion = atoi(&argv[ai][2]);
			} else if(strcmp(argv[ai],"-printCount")==0) {
				printCount=1;
			} else if(strcmp(argv[ai],"-printInjections")==0) {
				printInjections=1;
			} else if(strcmp(argv[ai],"-debug")==0) {
				debug=1;
			} else {
				printf("usage: tsotest directoryOrFile [patternIfDirectory] [-printCount] [-printInjections] [-sStorageMethod] [-fFileVersion]\n");
				exit(9);
			}
			ai++;
		}
	}

	if(fileVersion == -1 && storageMethod == -1)
		tso = TSO_Open(directoryOrFile,pattern);
	else if (fileVersion != -1 && storageMethod != -1)
		tso = TSO_OpenNoHeader(directoryOrFile,pattern,fileVersion,storageMethod);
	else {
		printf("Both storage method and file version must be set or not set\n");
		exit(9);
	}
	TSO_ReadPrologue(tso,&net,&nodes,&links);
	fprintf(stdout,"File Version    %d\n",tso->fileVersion);
	fprintf(stdout,"Storage Method  %d\n",tso->storageMethod);

	fprintf(stdout,"Is multi-part   %s\n",tso->numFiles>0?"yes":"no");

	fprintf(stdout,"\n",tso->storageMethod);
	fprintf(stdout,"nsteps          %d\n",net->nsteps);
	fprintf(stdout,"stepsize (hrs)  %f\n",net->stepsize);
	fprintf(stdout,"nnodes          %d\n",net->nnodes);
	fprintf(stdout,"ntanks          %d\n",net->ntanks);
	fprintf(stdout,"nlinks          %d\n",net->nlinks);
	fprintf(stdout,"njunctions      %d\n",net->njunctions);

	if(printCount || printInjections) {
		int idwidth=MAXCHARID;
		char *srcType[] = {"CONC","MASS"};
		int count=0;
		if(debug)
			printf("before TSO_ReadSimulationResults()\n");
		while(TSO_ReadSimulationResults(tso,net,nodes,sources)>0) {
			int i;
			count++;
			if(printInjections) {
				fprintf(stdout,"ensemble[%5d]:",count);
				for(i=0;i<net->nsources;i++) {
					int nhdr=i==0?1:17;
					PSource psrc = &sources[i];
					fprintf(stdout,"%*s%*s %5d %s %2d %6ld %6ld %9.4e\n",nhdr," ",16,
									psrc->SourceNodeID,psrc->SourceNodeIndex,
									srcType[psrc->SourceType],psrc->SpeciesIndex,
									psrc->SourceStart,psrc->SourceStop,psrc->SourceStrength);
				}
				if(net->nsources>1)
					fprintf(stdout,"\n");
			}
			if(debug)
				printf("   read simulation results: %d\n",count);
		}
		fprintf(stdout,"Num Simulations %d\n",count);
	}
	
	TSO_Close(&tso);
	TSO_ReleaseNetworkData(&nodes,&links,net);
	TSO_ReleaseNetworkInfo(&net);
	free(sources);

}
