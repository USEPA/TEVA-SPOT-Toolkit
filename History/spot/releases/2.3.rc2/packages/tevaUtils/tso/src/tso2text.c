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

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tevautil.h"

int needToWriteID(char *id, char **ids, int len);

static char *usageString="usage: tso2text directoryOrFile [patternIfDirectory] -e [-sStorageMethod -fFileVersion] [-ident STRING] [-n numToWrite | -ids [id1 id2 ... idn]\n  -e prints the concentrations in scientific notation\n";

int main(int argc, char **argv)
{
	char *directoryOrFile,
		*pattern=NULL,
		*outputFileBase=NULL;
	char storageMethod=-1,
		fileVersion=-1,
		outputStorageMethod=-1,
		outputFileVersion=-1;
	char **ids=NULL;
	char *ident=NULL;
	int numToWrite=-1;
	int wroteDemands=0;
	int useExponentialNotation=0;

	PNodeInfo nodes;
	PLinkInfo links;
	PNetInfo net;
	PTSO tso;
	PTSO tsoOut=NULL;
	PSource sources;
	clock_t start,stop;
	int n;

	sources=(PSource)calloc(MAXSOURCES,sizeof(Source));

	if(argc <2) {
		printf(usageString);
		exit(9);
	}

	outputFileBase = "conc";

	directoryOrFile = argv[1];
	if(argc > 2) {
		int ai = 2;
		if(argv[ai][0] != '-') {
			pattern = argv[ai++];
		}
		while(ai<argc) {
			if(strncmp(argv[ai],"-s",2)==0) {
				storageMethod = atoi(&argv[ai][2]);
			} else if(strncmp(argv[ai],"-v",2)==0) {
				fileVersion = atoi(&argv[ai][2]);
			} else if(strncmp(argv[ai],"-e",2)==0) {
				useExponentialNotation = 1;
			} else if(strncmp(argv[ai],"-n",2)==0) {
				numToWrite = atoi(argv[++ai]);
			} else if(strncmp(argv[ai],"-ident",6)==0) {
				ident=argv[++ai];
			} else if(strncmp(argv[ai],"-ids",4)==0) {
				numToWrite=0;
				while(++ai < argc && argv[ai][0] != '-') {
					ids = (char **)realloc(ids,(numToWrite+1)*sizeof(char *));
					ids[numToWrite] = argv[ai];
					numToWrite++;
				}
			} else {
				printf(usageString);
				exit(9);
			}
			ai++;
		}
	}
	if(numToWrite == -1) {
		printf("either the -n or -ids must be specified\n");
		printf(usageString);
		exit(9);
	}

	start = clock();

	if(fileVersion == -1 && storageMethod == -1)
		tso = TSO_Open(directoryOrFile,pattern);
	else if (fileVersion != -1 && storageMethod != -1)
		tso = TSO_OpenNoHeader(directoryOrFile,pattern,fileVersion,storageMethod);
	else {
		printf("Both storage method and file version must be set or not set\n");
		exit(9);
	}
	TSO_ReadPrologue(tso,&net,&nodes,&links);
	n=1;
	while(n <= numToWrite && TSO_ReadSimulationResults(tso,net,nodes,sources)>0) {
		if(!wroteDemands) {
			char fn[256];
			int nn,t;
			FILE *fp;
			sprintf(fn,"demands%s%s.txt",ident==NULL?"":"_",ident==NULL?"":ident);
			fp = fopen(fn,"w");
			fprintf(fp,"NumReceptorNodes\t%d\n",net->nnodes);
			fprintf(fp,"NumSteps\t%d\n",net->nsteps);
			for(nn=0;nn<net->nnodes;nn++) {
				float *q = nodes[nn].q;  // assume only one species for now.
				fprintf(fp,"%s",nodes[nn].id);
				for(t=0;t<net->nsteps;t++) {
					fprintf(fp,"\t%f",q[t]);
				}
				fprintf(fp,"\n");
			}
			fclose(fp);

			sprintf(fn,"flow%s%s.txt",ident==NULL?"":"_",ident==NULL?"":ident);
			fp = fopen(fn,"w");
			fprintf(fp,"NumLinks\t%d\n",net->nlinks);
			fprintf(fp,"NumSteps\t%d\n",net->nsteps);
			for(nn=0;nn<net->nlinks;nn++) {
				float *q = links[nn].flow;
				fprintf(fp,"%s\t%s",nodes[links[nn].from-1].id,nodes[links[nn].to-1].id);
				for(t=0;t<net->nsteps;t++) {
					fprintf(fp,"\t%f",q[t]);
				}
				fprintf(fp,"\n");
			}
			fclose(fp);

			sprintf(fn,"velocity%s%s.txt",ident==NULL?"":"_",ident==NULL?"":ident);
			fp = fopen(fn,"w");
			fprintf(fp,"NumLinks\t%d\n",net->nlinks);
			fprintf(fp,"NumSteps\t%d\n",net->nsteps);
			for(nn=0;nn<net->nlinks;nn++) {
				float *q = links[nn].velocity;
				fprintf(fp,"%s\t%s",nodes[links[nn].from-1].id,nodes[links[nn].to-1].id);
				for(t=0;t<net->nsteps;t++) {
					fprintf(fp,"\t%f",q[t]);
				}
				fprintf(fp,"\n");
			}
			fclose(fp);
			wroteDemands=1;
		}
		if(ids == NULL || needToWriteID(sources[0].SourceNodeID,ids,numToWrite)) {
			char fn[256];
			int nn,t;
			FILE *fp;
			char *srcID=sources[0].SourceNodeID;
			float strength = sources[0].SourceStrength;
			long start = sources[0].SourceStart;
			long stop = sources[0].SourceStop;
			sprintf(fn,"%s%s%s_%s_%.0f_%ld_%ld.txt",outputFileBase,ident==NULL?"":"_",ident==NULL?"":ident,srcID,strength,start,stop);
			fp = fopen(fn,"w");
			fprintf(fp,"SourceNodeID\t%s\n",srcID);
			fprintf(fp,"NumReceptorNodes\t%d\n",net->nnodes);
			fprintf(fp,"NumSteps\t%d\n",net->nsteps);
			fprintf(fp,"Source strength\t%e\n",sources[0].SourceStrength);
			fprintf(fp,"Source start\t%d\n",sources[0].SourceStart);
			fprintf(fp,"Source stop\t%d\n",sources[0].SourceStop);
			for(nn=0;nn<net->nnodes;nn++) {
				float *c = nodes[nn].c[0];  // assume only one species for now.
				fprintf(fp,"%s",nodes[nn].id);
				for(t=0;t<net->nsteps;t++) {
					if(useExponentialNotation)
						fprintf(fp,"\t%e",c[t]);
					else
						fprintf(fp,"\t%f",c[t]);
				}
				fprintf(fp,"\n");
			}
			fclose(fp);
			n++;
		}
	}
	TSO_Close(&tso);
	TSO_ReleaseNetworkData(&nodes,&links,net);
	TSO_ReleaseNetworkInfo(&net);
	free(sources);
	stop = clock();
	printf("processed input in %7.3f seconds\n", (float)(stop-start)/(float)CLOCKS_PER_SEC);

}
int needToWriteID(char *id, char **ids, int len) {
	int i;
	for(i=0;i<len;i++) {
		if(strcmp(id,ids[i])==0) return 1;
	}
	return 0;
}
