#include <stdlib.h>
#include <string.h>
#include "NamedData.h"
#include "HealthImpacts.h"

enum SensorType getSensorType(char *typeStr);
enum SampleType getSampleType(char *sampleTypeStr);

JNIEXPORT PMem loadHIAOptions(void *analysisOptionsRef, void *simResultsRef, ModuleType componentType)
{
	int 
		i,
		nnodes,
		nlinks,
		ntanks,
		njunctions,
		nsteps;
	int *tankIndices; /* used when building nodes */
	float
		stepsize;

	PMem mem;
	PDR dr;
//	PSensor   sensor;
	PNode     node;
	PLink     link;
//	PEnsemble ensemble;
	PNet      net;
	PXA       xa;
	PTA       ta;
	PU        u;
	PDoseOverThresh pdot;
	PopulationDataPtr pd;
	IngestionDataPtr ing;
	PSource   sources;
	char **nodeIDs;
	char **linkIDs;
	int *linkStarts;
	int *linkEnds;
	float *linkLengths;
/* 	float**   nodedata;  */
	float tf;
	char *tstr;
	JNIEnv *env = ((NamedDataRef*)simResultsRef)->env;
	char msg[256];

	if(getInt(simResultsRef, "numTimeSteps", &nsteps)==ND_FAILURE) return NULL;
	if(getInt(simResultsRef, "numNodes", &nnodes)==ND_FAILURE) return NULL;
	if(getInt(simResultsRef, "numLinks", &nlinks)==ND_FAILURE) return NULL;
	if(getInt(simResultsRef, "numJunctions", &njunctions)==ND_FAILURE) return NULL;
	if(getFloat(simResultsRef, "stepSize", &stepsize)==ND_FAILURE) return NULL;

	mem = (PMem)calloc(1,sizeof(Mem));

	/** network parameters */
	net = (PNet)calloc(1,sizeof(Net));                     /* Network */
	net->info = (PNetInfo)calloc(1,sizeof(NetInfo));
	mem->net = net;
/*	net->fltmax=fltmax; */
	net->info->njunctions=njunctions;
	net->info->nlinks=nlinks;
	net->info->nnodes=nnodes;
	net->info->nsteps=nsteps;
	net->info->stepsize=stepsize;

	mem->nodeinfo=(PNodeInfo)calloc(nnodes,sizeof(NodeInfo));
	node = (PNode)calloc(nnodes,sizeof(Node));
	mem->node=node;
	if(getStringArray(simResultsRef, "nodeIDs", &nodeIDs)==ND_FAILURE) return NULL;
	for(i=0;i<nnodes;i++) {
		node[i].rho= (float *)calloc(nsteps,sizeof(float));
		node[i].info = &mem->nodeinfo[i];  //share the one from mem
		node[i].info->c = (float **)calloc(1,sizeof(float*));
		node[i].info->c[0] = (float *)calloc(nsteps,sizeof(float));
		node[i].info->q = (float *)calloc(nsteps,sizeof(float));
		node[i].info->id = (char *)calloc(strlen(nodeIDs[i])+1,sizeof(char));
		strcpy(node[i].info->id,nodeIDs[i]);
		node[i].info->nz = (int *)calloc(1,sizeof(int)); // for now only one species...
	}
	nd_freeStringArray(nodeIDs,nnodes);

	mem->linkinfo=(PLinkInfo)calloc(nlinks,sizeof(LinkInfo));
	link = (PLink)calloc(nlinks,sizeof(Link));
	mem->link=link;
	if(getStringArray(simResultsRef, "linkIDs", &linkIDs)==ND_FAILURE) return NULL;
	if(getIntArray(simResultsRef, "linkStartNodeIdx", &linkStarts)==ND_FAILURE) return NULL;
	if(getIntArray(simResultsRef, "linkEndNodeIdx",   &linkEnds)==ND_FAILURE) return NULL;
	if(getFloatArray(simResultsRef, "linkLengths", &linkLengths)==ND_FAILURE) return NULL;
	for(i=0;i<nlinks;i++) {
		link[i].info = &mem->linkinfo[i];  //share the one from mem
		link[i].info->from=linkStarts[i];
		link[i].info->to = linkEnds[i];
		link[i].info->length = linkLengths[i];
		link[i].info->velocity = (float *)calloc(nsteps,sizeof(float));
		link[i].info->flow = (float *)calloc(nsteps,sizeof(float));
		link[i].info->id = (char *)calloc(strlen(linkIDs[i])+1,sizeof(char));
		strcpy(link[i].info->id,linkIDs[i]);
		// ignore vertices - not supplied in named data struct anyway
	}
	nd_freeStringArray(linkIDs,nlinks);
	nd_free(linkStarts);
	nd_free(linkEnds);
	nd_free(linkLengths);
	/** Dose-response info */
	dr = (PDR)calloc(1,sizeof(DR));
	mem->dr=dr;

	if(getString(analysisOptionsRef, "DoseResponseType", &tstr)==ND_FAILURE) return NULL;
	dr->responseType=Probit;
	if(strcmp(tstr,"probit")==0) dr->responseType=Probit;
	else if(strcmp(tstr,"old")==0) dr->responseType=Old;
	nd_free(tstr);

	// sigmoid
	if(getFloat(analysisOptionsRef, "DoseResponseA", &dr->a)==ND_FAILURE) return NULL;
	if(getFloat(analysisOptionsRef, "DoseResponseM", &dr->m)==ND_FAILURE) return NULL;
	if(getFloat(analysisOptionsRef, "DoseResponseN", &dr->n)==ND_FAILURE) return NULL;
	if(getFloat(analysisOptionsRef, "DoseResponseTau", &dr->tau)==ND_FAILURE) return NULL;
	//probit
	if(getFloat(analysisOptionsRef, "LD50", &dr->ld50)==ND_FAILURE) return NULL;
	if(getFloat(analysisOptionsRef, "Beta", &dr->beta)==ND_FAILURE) return NULL;

	if(getFloat(analysisOptionsRef, "BodyMass", &dr->bodymass)==ND_FAILURE) return NULL;
	if(getInt(analysisOptionsRef, "Normalize", &dr->normalize)==ND_FAILURE) return NULL;
	if(getInt(analysisOptionsRef, "LatencyTime", &dr->nL)==ND_FAILURE) return NULL;
	if(getInt(analysisOptionsRef, "FatalityTime", &dr->nF)==ND_FAILURE) return NULL;
	if(getFloat(analysisOptionsRef, "FatalityRate", &dr->frate)==ND_FAILURE) return NULL;
	if(getString(analysisOptionsRef, "DoseType", &tstr)==ND_FAILURE) return NULL;
	if(strcmp(tstr,"total")==0) dr->dtype=totalmass;
	nd_free(tstr);
    dr->maxsteps = net->info->nsteps + dr->nL + dr->nF;       /* Max number   of time steps for disease response */

	ing=(IngestionDataPtr)calloc(1,sizeof(IngestionData));
	mem->ingestionData=ing;
	if(getFloat(analysisOptionsRef, "IngestionRate", &tf)==ND_FAILURE) return NULL;
	ing->phi=(float)(tf/24.);
	
	if(getString(analysisOptionsRef, "IngestionTimingType", &tstr)==ND_FAILURE) return NULL;
	if(strcmp(tstr,"demand")==0) {
		ing->timingMode=IMDemand;
		ing->volumeMode=VMDemand;
	} else if(strcmp(tstr,"atus")==0) {
		ing->timingMode=IMATUS;
	} else if(strcmp(tstr,"fixed5")==0) {
		ing->timingMode=IMFixed5;
	}
	nd_free(tstr);
	if(getString(analysisOptionsRef, "IngestionVolumeType", &tstr)==ND_FAILURE) return NULL;
	if(strcmp(tstr,"demand")==0) {
		ing->volumeMode=VMDemand;
	} else if(strcmp(tstr,"random")==0) {
		ing->volumeMode=VMRandom;
	} else if(strcmp(tstr,"mean")==0) {
		ing->volumeMode=VMMean;
		if(getFloat(analysisOptionsRef, "IngestionRate", &ing->meanVolume)==ND_FAILURE) return NULL;
	}
	nd_free(tstr);


	pd=(PopulationDataPtr)calloc(1,sizeof(PopulationData));
	mem->popData=pd;
	if(getString(analysisOptionsRef, "Population", &tstr)==ND_FAILURE) return NULL;
	if(strcmp(tstr,"demand")==0) pd->population=DemandBased;
	else if(strcmp(tstr,"scenario")==0) pd->population=FileBased;
	nd_free(tstr);

	if(pd->population==FileBased) {
		float *population;
		char **popNodes;
		int popLength;
		int np;

		if(getFloatArray(simResultsRef, "population", &population)==ND_FAILURE) return NULL; 
		if(getStringArray(simResultsRef, "popNodes", &popNodes)==ND_FAILURE) return NULL; 
		if(getInt(simResultsRef, "popLength", &popLength)==ND_FAILURE) return NULL; 

		pd->pop = (PNodePopulation)calloc(popLength,sizeof(NodePopulation));
		pd->popLength=popLength;
		for(np=0;np<popLength;np++) {
			PNodePopulation p = &pd->pop[np];
			p->nodeid = (char *)calloc(strlen(popNodes[np])+1,sizeof(char));
			strcpy(p->nodeid,popNodes[np]);
			p->population=population[np];
			p->used=0;
		}
		nd_free(population);
		nd_freeStringArray(popNodes,popLength);
	} else {
		if(getFloat(analysisOptionsRef, "UsageRate", &pd->pcu)==ND_FAILURE) return NULL;
	}

	xa = (PXA)calloc(1,sizeof(XA));                        /* spatial averages */
	initXA(xa,dr->maxsteps);
	mem->xa=xa;
	
	ta = (PTA)calloc(1,sizeof(TA));                        /* spatial averages */
	initTA(ta,net->info->nnodes);
	mem->ta=ta;

	/** Really only need to allocate the next structure in the intermediate analysis server */
	if(componentType == IntermediateAnalysis) {
		u = (PU)calloc(1,sizeof(U));                           /* state variables */
		u->dos = (float *)calloc(dr->maxsteps,sizeof(float));    /* Basic dose-respone state variables */
		u->res = (float *)calloc(dr->maxsteps,sizeof(float));
		u->s = (float *)calloc(dr->maxsteps,sizeof(float));
		u->i = (float *)calloc(dr->maxsteps,sizeof(float));
		u->d = (float *)calloc(dr->maxsteps,sizeof(float));
		u->f = (float *)calloc(dr->maxsteps,sizeof(float));
		mem->u=u;
		for(i=0;i<(int)(dr->maxsteps);i++) {
			u->dos[i] = 0;				/* Spatial averages */
			u->res[i] = 0;				
			u->s[i] = 0;
			u->i[i] = 0;
			u->d[i] = 0;
			u->f[i] = 0;
		}
	}

	sources = (PSource)calloc(MAXSOURCES,sizeof(Source));/* Sources */
	mem->sources=sources;
	if(componentType == IntermediateAnalysis || componentType == Aggregation) {
		int numSensors;
		if(getInt(simResultsRef, "numSensors", &numSensors)==ND_FAILURE) return NULL;
		if(numSensors > 0) {
			char **sensorData=NULL;
			if(getStringArray(simResultsRef, "sensors", &sensorData)==ND_FAILURE) return NULL;
			for(i=0;i<numSensors;i++) {
				char sd[1024],*token;
				int idx;
				strcpy(sd,sensorData[i]);
				token = strtok(sd,","); // SAMPLELOC keyword
				token = strtok(NULL,",");  // nodeid
				idx = getnodeindex(net,node,token);
				if(idx != -1) {
					PNodeInfo nodeInfo = node[idx].info;
					PSensor sensor = &node[idx].sensor;
					token = strtok(NULL,",");  // sensor type (EXISTING | IGNORED | POTENTIAL | SELCTED)
					sensor->type=getSensorType(token);
					token = strtok(NULL,",");  // sensor sample type (REALTIME | COMPOSITE | FILTERED)
					sensor->sample=getSampleType(token);
					sensor->limit=(float)atof(strtok(NULL,","));
					sensor->vol=(float)atof(strtok(NULL,","));
					sensor->freq=atoi(strtok(NULL,","));
					sensor->start=atoi(strtok(NULL,","));
					sensor->delay=atoi(strtok(NULL,","));
				}
			}

		}
	}

	for(i=0;i<nsteps;i++) {
		float *tres;
		int j;
		/** IMPORTANT!!! Need to free these later via call to nd_free */
		if(getFloatArrayAtIndex(simResultsRef,"demands",i,&tres)==ND_FAILURE) {
			return NULL;
		}
		for(j=0;j<nnodes;j++) {
			node[j].info->q[i] = tres[j];
		}
		nd_free(tres);

		if(getFloatArrayAtIndex(simResultsRef,"linkFlow",i,&tres)==ND_FAILURE) { return NULL; }
		for(j=0;j<nlinks;j++) { link[j].info->flow[i] = tres[j]; }
		nd_free(tres);

		if(getFloatArrayAtIndex(simResultsRef,"linkVelocity",i,&tres)==ND_FAILURE) { return NULL; }
		for(j=0;j<nlinks;j++) { link[j].info->velocity[i] = tres[j]; }
		nd_free(tres);
	}
	{
		int *lens;
		if(getIntArray(simResultsRef, "DemandProfileLengths", &lens)==ND_FAILURE) return NULL;
		for(i=0;i<nnodes;i++) {
			float *tres;
			node[i].info->demandProfileLength=lens[i];
			if(lens[i]>0) {
				if(getFloatArrayAtIndex(simResultsRef,"DemandProfiles",i,&tres)==ND_FAILURE) { return NULL; }
				node[i].info->demandProfile=(float*)calloc(lens[i],sizeof(float));
				memcpy(node[i].info->demandProfile,tres,lens[i]*sizeof(float));
				nd_free(tres);
			}
		}		
	}
	if(getInt(simResultsRef, "numTanks", &ntanks)==ND_FAILURE) return NULL;
	net->info->ntanks=ntanks;
	if(getIntArray(simResultsRef, "tankIndices", &tankIndices)==ND_FAILURE) return NULL;
	for(i=0;i<ntanks;i++)
		node[tankIndices[i]].info->ntype = tank;
	nd_free(tankIndices);

	if(getStringArray(simResultsRef, "nodeIDs", &nodeIDs)==ND_FAILURE) return NULL;
	for(i=0;i<nnodes;i++) {
		if (node[i].info->ntype != tank) node[i].info->ntype=junction;
		node[i].pop=0;
		strcpy(node[i].info->id,nodeIDs[i]);
	}
	nd_freeStringArray(nodeIDs,nnodes);

	{
	  float *thresholds;
	  int numThresh,numDThresh,numRThresh;
	  if(getInt(analysisOptionsRef, "NumDoseThresholds", &numDThresh)==ND_FAILURE) return NULL;
	  if(getInt(analysisOptionsRef, "NumResponseThresholds", &numRThresh)==ND_FAILURE) return NULL;
	  numThresh=numDThresh+numRThresh;
	  pdot=NULL;
	  if(numThresh>0) {
		  int n=0;
		  pdot=(PDoseOverThresh)calloc(1,sizeof(DoseOverThresh));
		  pdot->numThresh=numThresh;
		  pdot->thresholds=(float*)calloc(numThresh,sizeof(float));
		  pdot->threshIDs=(char**)calloc(numThresh,sizeof(char *));
		  if(numDThresh > 0) {
			  if(getFloatArray(analysisOptionsRef, "DoseThreshold", &thresholds)==ND_FAILURE) return NULL;
			  for(i=0;i<numDThresh;i++,n++) {
				  char tmp[512];
				  pdot->thresholds[n]=thresholds[i];
				  sprintf(tmp,"%f",pdot->thresholds[n]);
				  pdot->threshIDs[n]=(char *)calloc(strlen(tmp)+1,sizeof(char));
				  strcpy(pdot->threshIDs[n],tmp);
			  }
			  nd_free(thresholds);
		  }
		  if(numRThresh > 0) {
			  if(getFloatArray(analysisOptionsRef, "ResponseThresholds", &thresholds)==ND_FAILURE) return NULL;
			  for(i=0;i<numRThresh;i++,n++) {
				  char tmp[512];
				  pdot->thresholds[n]=ResponseDose(thresholds[i],mem->dr);
				  sprintf(tmp,"%f",pdot->thresholds[n]);
				  pdot->threshIDs[n]=(char *)calloc(strlen(tmp)+1,sizeof(char));
				  strcpy(pdot->threshIDs[n],tmp);
			  }
			  nd_free(thresholds);
		  }
		  pdot->numOver = (int **)calloc(numThresh,sizeof(int*));
		  pdot->numOverByTime = (int **)calloc(numThresh,sizeof(int*));
		  for(i=0;i<numThresh;i++) {
			  pdot->numOver[i] = (int *)calloc(nnodes,sizeof(int));
			  pdot->numOverByTime[i] = (int *)calloc(mem->net->info->nsteps,sizeof(int));
		  }
		  pdot->totOver = (int *)calloc(numThresh,sizeof(int));
	  }
	  mem->dot=pdot;
	}
	{
	  float *bins;
	  int numBins;
	  char msg[256];
	  if(getInt(analysisOptionsRef, "NumDoseBins", &numBins)==ND_FAILURE) return NULL;
	  if(numBins>0) {
	    char *dbType; // Dose or Response;
	    int isResponse=0;
	    if(getFloatArray(analysisOptionsRef, "DoseBins", &bins)==ND_FAILURE) return NULL;
	    if(getString(analysisOptionsRef, "DoseBinType", &dbType)==ND_FAILURE) return NULL;

        PDoseBinData pdb = (PDoseBinData)calloc(1,sizeof(DoseBinData));
        pdb->numBins=numBins;
        pdb->responseBins=NULL;
		isResponse=strcasecmp(dbType,"Response")==0;
		pdb->doseBins=(float*)calloc(numBins,sizeof(float));
		pdb->responseBins=(float*)calloc(numBins,sizeof(float));
        for(i=0;i<numBins;i++) {
          if(isResponse) {
              pdb->doseBins[i]=ResponseDose(bins[i],mem->dr);
              pdb->responseBins[i]=bins[i];
          } else {
            pdb->doseBins[i]=bins[i];
            pdb->responseBins[i]=DoseResponse(bins[i],mem->dr);
          }
        }
        pdb->data=(int*)calloc(numBins,sizeof(int));
        mem->doseBins=pdb;
	  }
	}


	return mem;
}

JNIEXPORT void freeHIAMemory(PMem mem, ModuleType componentType) {
	int i;
	int nnodes=mem->net->info->nnodes;
	char msg[256];

	// do this first (or at least before mem->node is freed!)
//ANL_UTIL_LogInfo(env,"teva.analysis.server","freeHIAMemory: 01");
	if(componentType == IntermediateAnalysis) {
		freeIngestionData(mem->ingestionData,mem->node);
		free(mem->ingestionData);
	}

	for(i=0;i<nnodes;i++) {
		free(mem->node[i].rho);
		free(mem->node[i].info->c[0]);
		free(mem->node[i].info->c);
		free(mem->node[i].info->q);
		free(mem->node[i].info->nz);
		free(mem->node[i].info->id);
	}
	free(mem->node);
	free(mem->nodeinfo);

	for(i=0;i<mem->net->info->nlinks;i++) {
		free(mem->link[i].info->flow);
		free(mem->link[i].info->velocity);
		free(mem->link[i].info->id);
	}
	free(mem->link);
	free(mem->linkinfo);

	freeXA(mem->xa,mem->dr->maxsteps);
	free(mem->xa);
	freeTA(mem->ta,nnodes);
	free(mem->ta);

	/** Really only need to allocate the next structure in the intermediate analysis server */
	if(componentType == IntermediateAnalysis) {
		int len = mem->dr->maxsteps-1;
		free(mem->u->s);
		free(mem->u->i);
		free(mem->u->d);
		free(mem->u->f);
		free(mem->u->dos);
		free(mem->u->res);
		free(mem->u);
	}

	free(mem->sources);
	if(mem->popData->popLength > 0) {
		for(i=0;i<mem->popData->popLength;i++) {
			free(mem->popData->pop[i].nodeid);
		}
		free(mem->popData->pop);
	}
	free(mem->popData);
	free(mem->dr);

	free(mem->net->info);
	free(mem->net);

	if(mem->dot != NULL) {
	  for(i=0;i<mem->dot->numThresh;i++) {
	    free(mem->dot->numOver[i]);
	    free(mem->dot->threshIDs[i]);
	  }
	  free(mem->dot->numOver);
	  free(mem->dot->totOver);
	  free(mem->dot->thresholds);
	  free(mem->dot->threshIDs);
	  free(mem->dot);
	}
	if(mem->doseBins) {
		if(mem->doseBins->responseBins) free(mem->doseBins->responseBins);
		if(mem->doseBins->doseBins) free(mem->doseBins->doseBins);
		if(mem->doseBins->data) free(mem->doseBins->data);
		free(mem->doseBins); 
	}
	free(mem);
}

enum SensorType getSensorType(char *typeStr)
{
	if(stricmp(typeStr,"existing")==0) return existing;
	if(stricmp(typeStr,"ignored")==0) return ignore;
	if(stricmp(typeStr,"potential")==0) return potential;
	if(stricmp(typeStr,"selected")==0) return selected;
	return ignore;
}
enum SampleType getSampleType(char *sampleTypeStr)
{
	if(stricmp(sampleTypeStr,"filtered")==0) return filtered;
	if(stricmp(sampleTypeStr,"composite")==0) return composite;
	if(stricmp(sampleTypeStr,"realtime")==0) return realtime;
	return realtime;
}
