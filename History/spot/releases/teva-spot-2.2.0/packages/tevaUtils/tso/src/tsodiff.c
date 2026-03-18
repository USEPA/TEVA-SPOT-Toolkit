#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tevautil.h"

#define RMSENORM 1.0  /* If RMS concentration time series > this value, RMSE will be normalized by it */

static char *usageString="usage: tsodiff directoryOrFile1 [-pPatternIfDirectory1] directoryOrFile2 [-pPatternIfDirectory2]\n";
int compareLoaded(int *scens1,int *scens2,PTSO tso1,PTSO tso2);
int findIndex(PDataIndex index, PTSO tso1, PTSO tso2, PSource sources1, PSource sources2, int *scensUsed);
int compareSources(int nsources1, int nsources2,PSource sources1,PSource sources2, PTSO tso1, PTSO tso2,float strengthTol);

#define SOURCE_SAME          0x00
#define SOURCE_DIFF_ID       0x01
#define SOURCE_DIFF_START    0x02
#define SOURCE_DIFF_STOP     0x04
#define SOURCE_DIFF_STRENGTH 0x08
#define SOURCE_DIFF_TYPE     0x10
#define SOURCE_DIFF_SPECIES  0x20
#define SOURCE_DIFF_NUM      0x40


/*
**--------------------------------------------------------------
** Module:  TSODIFF
** Usage:   tsodiff <file1.tso> <file2.tso>
** Input:   Two different TSO files to compare
** Output:  None
** Purpose: Compare two TSO files to characterize differences.
** JGU 7/22/05
**---------------------------------------------------------------
*/
int main(int argc, char **argv)
{
	char *directoryOrFile1,
        *directoryOrFile2,
		*pattern1=NULL,
        *pattern2=NULL;
	char storageMethod=-1,
		fileVersion=-1;
	int loaded,loaded1,loaded2;
    int id1,id2;
    int *scens1,*scens2;
    int ok,sameprologue;
    int ai,i,j,k,maxnodeidx,maxlinkidx,maxspecie,samesource;
    float a,e,maxrmse,maxerr,rmse,rms1,rms2;
    int scenario,maxscenario,maxstep;
    float maxscenariormse;

	PNodeInfo nodes1,nodes2;
	PLinkInfo links1,links2;
	PNetInfo net1,net2;
	PTSO tso1,tso2;
	PSource sources1,sources2;

	sources1=(PSource)calloc(MAXSOURCES,sizeof(Source));
	sources2=(PSource)calloc(MAXSOURCES,sizeof(Source));

	if(argc < 3 || argc > 5) {
		printf(usageString);
		exit(9);
	}

    ai=1;
    directoryOrFile1 = argv[ai++];
    if(argv[ai][0] == '-') {
        pattern1 = &argv[ai++][2];
    }
    directoryOrFile2 = argv[ai++];
    if(argc > 3) {
        if(argv[ai][0] == '-') {
            pattern2 = &argv[ai++][2];
        }
        else {
            printf(usageString);
            exit(9);
        }
    }


    if(fileVersion == -1 && storageMethod == -1) {
        tso1 = TSO_Open(directoryOrFile1,pattern1);
        tso2 = TSO_Open(directoryOrFile2,pattern2);
    }
    else if (fileVersion != -1 && storageMethod != -1) {
        tso1 = TSO_OpenNoHeader(directoryOrFile1,pattern1,fileVersion,storageMethod);
        tso2 = TSO_OpenNoHeader(directoryOrFile2,pattern2,fileVersion,storageMethod);
    }
    else {
        printf("Both storage method and file version must be set or not set\n");
        exit(9);
    }

    TSO_ReadPrologue(tso1,&net1,&nodes1,&links1);
    TSO_ReadPrologue(tso2,&net2,&nodes2,&links2);

    /* Check prologue size info */
    printf("Analyzing Prologue\n");
    sameprologue=1;
    ok = (tso1->count == tso2->count);
    if(ok)
        printf("Num Injections:   %8d %8d [OK]\n",tso1->count,tso2->count);
    else {
        printf("Num Injections:   %8d %8d [***]\n",tso1->count,tso2->count);
        sameprologue=0;
    }
    ok = (net1->njunctions == net2->njunctions);
    if(ok)
        printf("NJunctions:   %8d %8d [OK]\n",net1->njunctions,net2->njunctions);
    else {
        printf("NJunctions:   %8d %8d [***]\n",net1->njunctions,net2->njunctions);
        sameprologue=0;
    }
    ok = (net1->nlinks == net2->nlinks);
    if(ok)
        printf("NLinks:       %8d %8d [OK]\n",net1->nlinks,net2->nlinks);
    else {
        printf("NLinks:       %8d %8d [***]\n",net1->nlinks,net2->nlinks);
        sameprologue=0;
    }
    ok = (net1->nnodes == net2->nnodes);
    if(ok)
        printf("NNodes:       %8d %8d [OK]\n",net1->nnodes,net2->nnodes);
    else {
        printf("NNodes:       %8d %8d [***]\n",net1->nnodes,net2->nnodes);
        sameprologue=0;
    }
    ok = (net1->nsteps == net2->nsteps);
    if(ok)
        printf("NSteps:       %8d %8d [OK]\n",net1->nsteps,net2->nsteps);
    else {
        printf("NSteps:       %8d %8d [***]\n",net1->nsteps,net2->nsteps);
        sameprologue=0;
    }
    ok = (net1->ntanks == net2->ntanks);
    if(ok)
        printf("NTanks:       %8d %8d [OK]\n",net1->ntanks,net2->ntanks);
    else {
        printf("NTanks:       %8d %8d [***]\n",net1->ntanks,net2->ntanks);
        sameprologue=0;
    }
    if(tso1->fileVersion >= 4 && tso2->fileVersion >= 4) {
        ok = (net1->nspecies == net2->nspecies);
        if(ok)
            printf("NSpecies:     %8d %8d [OK]\n",net1->nspecies,net2->nspecies);
        else {
            printf("NSpecies:     %8d %8d [***]\n",net1->nspecies,net2->nspecies);
            sameprologue=0;
        }
    }
    ok = (net1->stepsize == net2->stepsize);
    if(ok)
        printf("StepSize:     %12e %12e [OK]\n",net1->stepsize,net2->stepsize);
    else {
        printf("StepSize:     %12e %12e [***]\n",net1->stepsize,net2->stepsize);
        sameprologue=0;
    }
    if(tso1->fileVersion >= 6 && tso2->fileVersion >= 6) {
        ok = (net1->qualcode == net2->qualcode);
        if(ok)
            printf("QualityCode: %8d %8d [OK]\n",net1->qualcode,net2->qualcode);
        else {
            printf("QualityCode: %8d %8d [***]\n",net1->qualcode,net2->qualcode);
            sameprologue=0;
        }
        ok = (net1->nbulkspecies == net2->nbulkspecies);
        if(ok)
            printf("NBulkSpecies: %8d %8d [OK]\n",net1->nbulkspecies,net2->nbulkspecies);
        else {
            printf("NBulkSpecies: %8d %8d [***]\n",net1->nbulkspecies,net2->nbulkspecies);
            sameprologue=0;
        }
        ok = (net1->reportstart == net2->reportstart);
        if(ok)
            printf("ReportStart:  %8d %8d [OK]\n",net1->reportstart,net2->reportstart);
        else {
            printf("ReportStart:  %8d %8d [***]\n",net1->reportstart,net2->reportstart);
            sameprologue=0;
        }
        ok = (net1->reportstep == net2->reportstep);
        if(ok)
            printf("ReportStep:   %8d %8d [OK]\n",net1->reportstep,net2->reportstep);
        else {
            printf("ReportStep:   %8d %8d [***]\n",net1->reportstep,net2->reportstep);
            sameprologue=0;
        }
        ok = (net1->simduration == net2->simduration);
        if(ok)
            printf("SimDuration:  %8d %8d [OK]\n",net1->simduration,net2->simduration);
        else {
            printf("SimDuration:  %8d %8d [***]\n",net1->simduration,net2->simduration);
            sameprologue=0;
        }
    }
    if (!sameprologue) {
        printf("Files %s and %s have different prologues\n",directoryOrFile1,directoryOrFile2);
        exit(9);
    }

    /* Specie Prologue info */
    if(tso1->fileVersion >= 4 && tso2->fileVersion >= 4) {
        for(i=0;i<net1->nspecies;i++) {
            if(strcmp(net1->species[i],net2->species[i])) 
                printf("Warning: Specie %d ID mismatch: %s != %s\n",i+1,net1->species[i],net2->species[i]);
        }
    }

    /* Node Prologue info */
    maxerr=0.;
    maxnodeidx=0;
    maxstep=0;
    for(i=0;i<net1->nnodes;i++) {
        if(strcmp(nodes1[i].id,nodes2[i].id)) 
            printf("Warning: Node %d ID mismatch: %s != %s\n",i+1,nodes1[i].id,nodes2[i].id);
        for(j=0;j<net1->nsteps;j++) {
            if((e=(float)fabs(nodes1[i].q[j]-nodes2[i].q[j])) > maxerr) {
                maxerr = e;
                maxnodeidx = i+1;
                maxstep = j+1;
            }
        }
    }
    if(maxerr > 0.)
        printf("Max. abs. demand discrepancy = %e at node idx %d and time step %d\n",maxerr,maxnodeidx,maxstep);
    else
        printf("Identical node demands [OK]\n");

    /* Link Prologue info */
    if(tso1->fileVersion >= 4 && tso2->fileVersion >= 4) {
        maxerr=0.;
        maxlinkidx=0;
        maxstep=0;
        for(i=0;i<net1->nlinks;i++) {
            for(j=0;j<net1->nsteps;j++) {
                if((e=(float)fabs(links1[i].velocity[j]-links2[i].velocity[j])) > maxerr) {
                    maxerr = e;
                    maxlinkidx = i+1;
                    maxstep = j+1;
                }
            }
        }
        if(maxerr > 0.)
            printf("Max. abs. velocity discrepancy = %e at link idx %d and time step %d\n",maxerr,maxlinkidx,maxstep);
        else
            printf("Identical link velocities [OK]\n");

        maxerr=0.;
        maxlinkidx=0;
        maxstep=0;
        for(i=0;i<net1->nlinks;i++) {
            for(j=0;j<net1->nsteps;j++) {
                if((e=(float)fabs(links1[i].flow[j]-links2[i].flow[j])) > maxerr) {
                    maxerr = e;
                    maxlinkidx = i+1;
                    maxstep = j+1;
                }
            }
        }
        if(maxerr > 0.)
            printf("Max. abs. flow rate discrepancy = %e at link idx %d and time step %d\n",maxerr,maxlinkidx,maxstep);
        else
            printf("Identical link flow rates [OK]\n");
    }
    if(tso1->fileVersion >= 6 && tso2->fileVersion >= 6) {
        for(i=0;i<net1->nlinks;i++) {
            if(links1[i].length != links2[i].length) 
                printf("Warning: Link %d length mismatch: %e != %e\n",i+1,links1[i].length,links2[i].length);
        }
    }
    
    /* Scenario concentrations */
    printf("\nAnalyzing Scenario Concentrations: [Max RMSE/RMSC] [Max Node] [Max Specie]\n");
    scenario=1;
    maxscenario=0;
    maxscenariormse=0.;
    id1=0;
    scens1=(int*)calloc(net1->nnodes,sizeof(int));
    scens2=(int*)calloc(net2->nnodes,sizeof(int));
    loaded1=TSO_GetSimulationResults(id1,tso1,net1,nodes1,sources1);
    id2=findIndex(tso1->index[id1],tso1,tso2,sources1,sources2,scens2);
    printf("id1=%d  id2=%d\n",id1,id2);
    if(id2 != -1) {
      loaded2=TSO_GetSimulationResults(id2,tso2,net2,nodes2,sources2);
      if(loaded2) scens2[id2]=1;
      if(loaded1) scens1[id1]=1;
    } else {
      loaded2=0;
    }
    loaded = (loaded1 && loaded2);
    while ( id1 < tso1->count) {
        /* Sources */
      if(loaded1 && loaded2) {
	samesource = compareSources(net1->nsources, net2->nsources, sources1, sources2, tso1, tso2,(float)0);
	
        if(samesource!=SOURCE_SAME) {
	  printf("Warning: Inconsistent source description for scenario %6d\n",scenario);
	  if(samesource & SOURCE_DIFF_NUM)       printf("   Number of sources\n");
	  if(samesource & SOURCE_DIFF_ID)        printf("   Source Node ID\n");
	  if(samesource & SOURCE_DIFF_START)     printf("   Start Time\n");
	  if(samesource & SOURCE_DIFF_STOP)      printf("   Stop Time\n");
	  if(samesource & SOURCE_DIFF_STRENGTH)  printf("   Source Strength\n");
	  if(samesource & SOURCE_DIFF_TYPE)      printf("   Source Type\n");
	  if(samesource & SOURCE_DIFF_SPECIES)   printf("   Source Species\n");
	}
        else {
	  int printed=0;
	  maxnodeidx=0;
	  maxspecie=0;
	  maxrmse=0.;
	  
	  for(j=0;j<net1->nbulkspecies;j++) {
	    for(i=0;i<net1->nnodes;i++) {
	      rmse=0.;
	      rms1=0.;
	      rms2=0.;
	      for(k=0;k<net1->nsteps;k++) {
		e=(float)(nodes1[i].c[j][k]-nodes2[i].c[j][k]);
		if(abs(e)> 1e-10 && (nodes2[i].c[j][k] == 0.0 || nodes1[i].c[j][k] == 0)) {
		  if(!printed)
		    printf("non-zero vs. zero concentration values: scen,node,t: %d,%s,%d tso1: %e tso2: %e\n",
			   scenario,nodes1[i].id,k,nodes1[i].c[j][k],nodes2[i].c[j][k]);
		  printed++;
		} else {
		  if(printed>1)
		    printf("there were %d additional messages regarding the non-zero vs. zero concentration values\n",
			   printed);
		  printed=0;
		}
		rmse+=e*e;
		rms1+=nodes1[i].c[j][k]*nodes1[i].c[j][k];
		rms2+=nodes2[i].c[j][k]*nodes2[i].c[j][k];
	      }
	      rmse=(float)sqrt(rmse/(float)net1->nsteps);
	      rms1=(float)sqrt(rms1/(float)net1->nsteps);
	      rms2=(float)sqrt(rms2/(float)net1->nsteps);
	      if((a=(float)((rms1+rms2)/2.0)) > RMSENORM) rmse/=a;
	      if(rmse>maxrmse) {
		maxrmse=rmse;
		maxnodeidx=i+1;
		maxspecie=j+1;
	      }
	    }
	  }
	  printf("Scenario %6d: %12e %6d %4d\n",scenario,maxrmse,maxnodeidx,maxspecie);
	  if(maxrmse>maxscenariormse) {
	    maxscenariormse=maxrmse;
	    maxscenario=scenario;
	  }
        }
      } /** end of if loaded1 && loaded2 */
      id1++;
      if(id1 < tso1->count) {
	loaded1=TSO_GetSimulationResults(id1,tso1,net1,nodes1,sources1);
	id2=findIndex(tso1->index[id1],tso1,tso2,sources1,sources2,scens2);
	if(id2 != -1) {
	  loaded2=TSO_GetSimulationResults(id2,tso2,net2,nodes2,sources2);
	  if(loaded2) scens2[id2]=1;
	  if(loaded1) scens1[id1]=1;
	} else {
	  loaded2=0;
	}
	loaded = (loaded1 && loaded2);
      }
      scenario++;
    } /* end of while */

    compareLoaded(scens1,scens2,tso1,tso2);
    TSO_Close(&tso1);
    TSO_Close(&tso2);

    printf("\nMaximum (RMS error)/(RMS conc) = %12e at scenario %6d\n",maxscenariormse,maxscenario);
}

int compareLoaded(int *scens1,int *scens2,PTSO tso1,PTSO tso2) {
  int i,s,errors;
  errors=0;
  for(i=0;i<tso1->count;i++) {
    if(!scens1[i]) {
      if(!errors) {
	errors=1;
	printf("unused scenarios from TSO #1:\n");
      }
      for(s=0;s<tso1->index[i]->nsources;s++) {
	printf("  %s%s",(s>0?",":""),tso1->index[i]->source[s].SourceNodeID);
      }
    }
  }
  errors=0;
  for(i=0;i<tso2->count;i++) {
    if(!scens2[i]) {
      if(!errors) {
	errors=1;
	printf("unused scenarios from TSO #1:\n");
      }
      for(s=0;s<tso2->index[i]->nsources;s++) {
	printf("  %s%s",(s>0?",":""),tso2->index[i]->source[s].SourceNodeID);
      }
    }
  }
}
int findIndex(PDataIndex index, PTSO tso1, PTSO tso2, PSource sources1, PSource sources2, int *scensUsed)
{
  int i;
  for(i=0;i<tso1->count;i++) {
    if(!scensUsed[i]) {
      PDataIndex idx=tso2->index[i];
      // now find out if this one is the same as index...
      int samesource= compareSources(index->nsources, idx->nsources, index->source, idx->source, tso1, tso2,0.001f);
      if(samesource==SOURCE_SAME) return i;
    }
  }
  return -1;
}

int compareSources(int nsources1, int nsources2,PSource sources1,PSource sources2, PTSO tso1, PTSO tso2,float strengthTol)
{
  int i,samesource=SOURCE_DIFF_NUM;
  if(nsources1 == nsources2) {
    samesource=SOURCE_SAME;
    for(i=0;i<nsources1;i++) {
      if(strcmp(sources1[i].SourceNodeID,sources2[i].SourceNodeID)) samesource|=SOURCE_DIFF_ID;
      if(tso1->fileVersion >= 2 && tso2->fileVersion >= 2) {
	if(sources1[i].SourceStart!=sources2[i].SourceStart) samesource|=SOURCE_DIFF_START;
	if(sources1[i].SourceStop!=sources2[i].SourceStop) samesource|=SOURCE_DIFF_STOP;
	if(strengthTol==0) {
	  if(sources1[i].SourceStrength!=sources2[i].SourceStrength)
	    samesource|=SOURCE_DIFF_STRENGTH;
	  else if(abs(sources1[i].SourceStrength-sources2[i].SourceStrength)>strengthTol)
	    samesource|=SOURCE_DIFF_STRENGTH;
	}
      }
      if(tso1->fileVersion >= 4 && tso2->fileVersion >= 4) {
	if(sources1[i].SourceType!=sources2[i].SourceType) samesource|=SOURCE_DIFF_TYPE;
	if(sources1[i].SpeciesIndex!=sources2[i].SpeciesIndex) samesource|=SOURCE_DIFF_SPECIES;
      }
    }
  }
  return samesource;
}
