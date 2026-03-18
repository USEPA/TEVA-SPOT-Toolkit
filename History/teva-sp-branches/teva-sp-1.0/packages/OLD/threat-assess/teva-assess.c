/* System Headers */
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <float.h>

/* TEVA headers */
#include "teva-assess.h"
#include "ta_input.h"
#include "tai.h"
#include "sss.h"
#include "ta_utils.h"
#include "nrutil.h"

/*
**--------------------------------------------------------------
** Module:  TEVA-ASSESS
** Usage:   teva-assess.exe <input.tai>
** Input:   
** Output:
** Purpose:
**---------------------------------------------------------------
*/
int main(int argc, char **argv)
{
    int
        i,
        dtime,
        dtmin,
        scenario;
    time_t
        cpustart,
        cpustop;
    double
        cpuelapsed;
	char *taiFile,
		*sssFile;

    /* Data model - see mem structure */
    Mem       mem;
    PIO       io;
    PTSO      tso;
    PDR       dr;
    PNode     node;
    PLink     link;
	PNetInfo  netinfo;
    PNodeInfo nodeinfo;
    PLinkInfo linkinfo;
    PNet      net;
    PXA       xa;
	/** NEW  **/
	XA	interimXA;
	PHIST
		thisHist,
		prevHist;
	/** End NEW **/
    PTA       ta;
    PU        u;
    PSource   sources;
//    float*    nodedata;
	PInputFileData taiIFD,sssIFD;

	memset(&mem,0,sizeof(Mem));
    time( &cpustart );

    if((argc!=2)&&
       (argc!=3))
      {
	printf("***Incorrect invocation\n");
	printf("***Correct usage is: teva-assess <tai-input-file> [sensor-file]\n");
	return 0;
      }

    printf("T H R E A T  E N S E M B L E  V U L N E R A B I L I T Y  A N A L Y S I S\n");
    printf("                               (T E V A)\n");
    printf("               H E A L T H  I M P A C T  A N A L Y Z E R\n");
    printf("                              Version 1.0\n\n");

	taiFile = argv[1];
	sssFile = argc==3?argv[2]:NULL;

	taiIFD = loadTAIFileData(taiFile);
	if(sssFile != NULL)
		sssIFD = loadSSSFileData(sssFile);
	mem.tso = OpenTSOFile(taiIFD,"TSONAME");
	TSO_ReadPrologue(mem.tso,&netinfo,&mem.nodeinfo,&mem.linkinfo);
	InitializeAssessMemory(&mem,netinfo);
	processTAIData(taiIFD,&mem);
	freeInputFileData(&taiIFD);
	if(sssFile != NULL) {
		processSensorSetData(sssIFD,&mem);
		freeInputFileData(&sssIFD);
	}
    /* Copy data model addresses from mem for convenience */
    io=mem.io;
    dr=mem.dr;
    node=mem.node;
    link=mem.link;
    net=mem.net;
    xa=mem.xa;
    ta=mem.ta;
    u=mem.u;
    sources=mem.sources;
//    nodedata=mem.nodedata;
	tso=mem.tso;
	nodeinfo=mem.nodeinfo;
	linkinfo=mem.linkinfo;
	/** NEW **/
	prevHist = NULL;
	/** End NEW **/

    /* Estimate Population */
    Population(net,dr,node);

    /* Node ingestion rate variations */
    IngestionRate(net,node);

    /*****************************************************/
    /*  Health Impact analysis for existing sensors      */
    /*****************************************************/
    printf("Health Impact Analysis for Scenario..");
    scenario=0;
    initialize(net,dr,xa,ta);
	/** NEW **/
	initXA(&interimXA,dr->maxsteps);
	/** End NEW **/

    while (TSO_ReadSimulationResults(tso,net->info,nodeinfo,sources)) {
//    while (scenario < 10 && TSO_ReadSimulationResults(tso,net->info,nodeinfo,sources)) {
		/** NEW **/
		char tstr[100];
		resetXA(&interimXA,dr->maxsteps);
		/** End NEW **/

        scenario++;
        if (MOD(scenario,100)==0) printf("%u\n",scenario);
        fflush(stdout);

        /* Sampling network detection time is minimum among all sensors */
        dtmin=INT_MAX;
        for(i=0;i<net->info->nnodes;i++) {
            if(node[i].sensor.type==existing && node[i].info->nz[0]) {
                if( (dtime=Detect(net,&node[i]))<dtmin ) dtmin=dtime;
            }
        }
		/** NEW **/
		/* for each scenario read, enlarge size of histogram data structures */
		/* if (analyzed-1 > net->nnodes-1){
		MEMCHECK(hist = (PHIST)realloc(hist, (size_t) (analyzed+1)*sizeof(HIST)));  } */
		thisHist = (PHIST)calloc(1,sizeof(HIST));
		tstr[0]=0;
		for(i=0;i<net->info->nsources;i++) {
			if(i>0) strcat(tstr,",");
			strcat(tstr,node[sources[i].SourceNodeIndex-1].info->id);
		}
		thisHist->simID = (char *)calloc(strlen(tstr)+1,sizeof(char));
		strcpy(thisHist->simID,tstr);
		
		if(prevHist==NULL) {
			mem.hist = thisHist;
		} else {
			prevHist->next=thisHist;
		}
		prevHist = thisHist;
		/** End NEW **/

        /* Time-dependent cumulative expected dose and disease response */
        for(i=0;i<net->info->nnodes;i++) {
            if(node[i].info->nz[0]) {
                Dose(dtmin,net,dr,u,&node[i]);
                Response(net,dr,u);
                DiseaseRM(net,dr,u);
            }
			/** UPDATED **/
//			Accumulate(i,node[i].info->nz[0],net,dr,node[i].pop,u,xa,ta,thisHist); /* RM 11/9/04 add scenario remove ta*/
			Accumulate(i,node[i].info->nz[0],net,dr,node[i].pop,u,&interimXA,ta,thisHist); /* RM 11/9/04 add scenario remove ta*/
			/** End UPDATED **/
        }
		/** NEW **/
		AccumulateFinal(net,dr,xa,&interimXA);
		/** End NEW **/
    }  /* End of while */
    net->nscenario=scenario;
    printf("\n");

    /* Simple ASCII Output          */
    /* Ensemble Averages */
    for(i=0;i<dr->maxsteps;i++) {
        xa->dos[i]/=(float)net->nscenario;
        xa->res[i]/=(float)net->nscenario;
        xa->s[i]/=(float)net->nscenario;
        xa->i[i]/=(float)net->nscenario;
        xa->d[i]/=(float)net->nscenario;
        xa->f[i]/=(float)net->nscenario;
    }
    for(i=0;i<net->info->nnodes;i++) {
        ta->dos[i]/=(float)net->nscenario;
        ta->res[i]/=(float)net->nscenario;
        ta->s[i]/=(float)net->nscenario;
        ta->i[i]/=(float)net->nscenario;
        ta->d[i]/=(float)net->nscenario;
        ta->f[i]/=(float)net->nscenario;
    }
	if(mem.sap->fn[0] != '\0') {
		PHIST p;
		int nscenarios=0;
		float meanInfections=0,maxInfections=0;

	    FILEOPEN(mem.sap->stream = fopen(mem.sap->fn, "wt"));
		for(p=mem.hist;p!=NULL;p=p->next) {
			float infections = p->fh/dr->frate;
			nscenarios++;
	        meanInfections += infections;
			if(infections > maxInfections)
				maxInfections = infections;
		}
		if(mem.sap->meanInfections)
			fprintf(mem.sap->stream,"MeanInfections   %f\n",meanInfections/nscenarios);
		if(mem.sap->maxInfections)
			fprintf(mem.sap->stream,"MaxInfections    %f\n",maxInfections);
		fclose(mem.sap->stream);
	}
	if(mem.io->taoname[0] != '\0') {
		PHIST p;
	    FILEOPEN(mem.io->taostream = fopen(mem.io->taoname, "wt"));
	    fprintf(io->taostream,"%d\n",net->info->nnodes);
		fprintf(io->taostream,"%d\n",dr->maxsteps);
		fprintf(io->taostream,"%d\n",net->nscenario);

		for(i=0;i<dr->maxsteps;i++) fprintf(io->taostream,"%e ",xa->s[i]);
		fprintf(io->taostream,"\n");
		for(i=0;i<dr->maxsteps;i++) fprintf(io->taostream,"%e ",xa->i[i]);
		fprintf(io->taostream,"\n");
		for(i=0;i<dr->maxsteps;i++) fprintf(io->taostream,"%e ",xa->d[i]);
		fprintf(io->taostream,"\n");
		for(i=0;i<dr->maxsteps;i++) fprintf(io->taostream,"%e ",xa->f[i]);
		fprintf(io->taostream,"\n");
		for(i=0;i<dr->maxsteps;i++) fprintf(io->taostream,"%e ",xa->dos[i]);
		fprintf(io->taostream,"\n");
		for(i=0;i<dr->maxsteps;i++) fprintf(io->taostream,"%e ",xa->res[i]);
		fprintf(io->taostream,"\n");
		/** MODIFIED **/
		for(i=0;i<net->info->nnodes;i++) {
			fprintf(io->taostream,"%d\t%s\t%e\t%e\t%e\t%e\t%e\t%e\n",
				i,mem.node[i].info->id,ta->s[i],ta->i[i],ta->d[i],ta->f[i],ta->dos[i],ta->res[i]);
		}
		/** End MODIFIED **/
		/** NEW **/
		fprintf(io->taostream,"Scenario Results\n");

		fprintf(io->taostream,"ID\tFatalities\tDosage\tResponse\n");
		for(p=mem.hist;p!=NULL;p=p->next)
			fprintf(io->taostream,"%s\t%f\t%f\t%f\n",p->simID,p->fh,p->dosh,p->resh);
		fclose(io->taostream);
		/** Wnd NEW **/
	}
	TSO_Close(&tso);
    time( &cpustop );
    cpuelapsed = difftime(cpustop,cpustart);
    printf("\nElapsed time for sensor location / health impact analysis (seconds): %e\n",cpuelapsed);
    printf("T E V A  Normal  Termination\n");

	freeXA(&interimXA,dr->maxsteps);
	FreeAssessMemory(&mem);

	freeHistogramData(mem.hist);
	TSO_ReleaseNetworkData(&nodeinfo,&linkinfo,netinfo);
	TSO_ReleaseNetworkInfo(&netinfo);
    return(0);
}

