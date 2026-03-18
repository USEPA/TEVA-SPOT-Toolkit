/*
 * Copyright � 2008 UChicago Argonne, LLC
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
/*
 * Modified 2010 Sandia National Laboratories
 * Modified to run flushing commands if decfname is provided and present
 */
/* TEVA header */

#include "teva.h"
#ifdef WIN32
#include "mem_mon.h"
#endif

#ifdef WIN32
#include <fcntl.h>
int mkstemp(char *tmpl)
{
	int ret=-1;
	mktemp(tmpl);
	ret=open(tmpl,O_RDWR|O_BINARY|O_CREAT|O_EXCL|_O_SHORT_LIVED,_S_IREAD|_S_IWRITE);
	return ret;
}
#endif
int run_flushing(FILE *decfile);
int run_hydraulics(PERD db,FILE *decfile);
int run_quality(PERD db, FILE *tsifile, PSourceData source);
int run_quality_msx(PERD db, FILE *tsifile, const char *msx_species, PSourceData source, double **initQual);
void printSpecies(PERD db);

void run_flushsim(const char* tsgfname, const char* tsifname, const char* erdName, const char* epanetinpfname, const char* epanetoutfname, const char* decfname, const char *msxfname, const char *msx_species)
{
	// original tevasim variables
    int version;
    int storageMethod=-1, runensemble=0;
    time_t cpustart, cpustop;
    double cpuelapsed;
//	PNetInfo net;
	PERD db;
//	PNodeInfo nodes;
//	PLinkInfo links;
	PSourceData source=(PSourceData)calloc(1,sizeof(SourceData));
	int isMSX;
	FILE *tsgfile=NULL, *tsifile=NULL;
    FILE *decfile=NULL;
	char tTSIFname[32]="";

	double **initQual;  // species,node: initial quality from msx input file

    source->source=(PSource)calloc(MAXSOURCES,sizeof(Source));
    source->nsources=0;
#ifdef WIN32
	startMemoryMonitorThread("memlog.txt",1000);
#endif
	isMSX=msxfname!=NULL;
    if (tsgfname) {
       TEVAFILEOPEN(tsgfile = fopen(tsgfname, "r"),tsgfname);
       runensemble=1;
       }
    if (tsifname) {
       tsifile = fopen(tsifname, "r+t");
       if (!tsifile)
          TEVAFILEOPEN(tsifile = fopen(tsifname, "w+t"),tsifname);
    }
	if (decfname) {
		decfile = fopen(decfname, "r");
		if (!decfile)
			TEVAFILEOPEN(decfile = fopen(decfname, "r"),decfname);
	}

    /* Open EPANET/MSEPANET and process input file */
    printf("T H R E A T  E N S E M B L E  V U L N E R A B I L I T Y  A N A L Y S I S\n");
    printf("                               (T E V A)\n");
    printf("                    T H R E A T  S I M U L A T O R\n");
    printf("                              Version %d.%d\n\n",TSMAJORVERSION,TSMINORVERSION);
    ENCHECK( (ENopen(epanetinpfname, epanetoutfname, "")) );
	if(isMSX) {
		MSXCHECK( (MSXopen(msxfname)) );
	}
    ENgetversion(&version);   /* MS/Epanet version */
	if(isMSX) {
        printf("T E V A is using MSEpanet release %d\n",version);
        printf("Initializing MSEpanet \n");
	} else {
        printf("T E V A is using Epanet release %d\n",version);
        printf("Initializing Epanet \n");
	}

	ERDCHECK(ERD_create(&db, erdName, teva, lzma));
	ERDCHECK(ENL_getNetworkData(db, epanetinpfname, msxfname, msx_species));
	// set hydraulic storage flags
	ERDCHECK(ERD_setHydStorage(db, TRUE, TRUE, TRUE, FALSE, TRUE));
	if(isMSX)
		ERDCHECK(ENL_saveInitQual(db->network,&initQual));
//    ENTU_Initialize(epanetinpfname, tsofname, storageMethod, tsofileversion, &tso, &net, &nodes, &links, &sources);

    /*****************************************************/
    /* Process demands and demand patterns to create     */
    /* proper demand patterns and agerage demands for    */
    /* use in demand-based population and ingestion.     */
    /*****************************************************/
	ENL_createDemandPatterns(db);
    /* (Optionally) Process threat simulation generator file and write simulation input file */
    if(tsgfile) {
        if(!tsifile) { /* temporary tsi file */
			int tsifd;
			strcpy(tTSIFname,"tsiXXXXXX");
			tsifd=mkstemp(tTSIFname);
            if( (tsifile=fdopen(tsifd,"w+b") ) == NULL ) {
                TEVASimError(1,"Can not create temporary TSI file\n");
			}
        }
		ENL_writeTSI(db->network, db->nodes, source->source, tsgfile, tsifile);
    }
    /*****************************************************/
    /*             Hydraulic Simulation                  */
    /*****************************************************/
	run_hydraulics(db,decfile);

    /* That's it if no -tsi or -tsg flag */
    time( &cpustart );
    if(tsifile == NULL) goto stop;

    /************************************************************/
    /*             Water Quality Simulations                    */
    /************************************************************/
	if(isMSX)
		run_quality_msx(db,tsifile,msx_species,source,initQual);
	else
		run_quality(db,tsifile,source);
	printSpecies(db);
stop:
    time( &cpustop );
    cpuelapsed = difftime(cpustop,cpustart);
    printf("Elapsed time for quality simulations (seconds): %e\n", cpuelapsed);
    printf("T E V A  Normal  Termination\n");

	/* release all memory allocated */
	if(isMSX)
		ERDCHECK(ENL_releaseInitQual(db->network,&initQual));
    free(source->source); /* I'd rather this be part of ReleaseNetworkData... */
    free(source); /* I'd rather this be part of ReleaseNetworkData... */

	/* close any files that have been opened */
	if(tsgfile) fclose(tsgfile);
	if(tsifile) fclose(tsifile);
	if(strlen(tTSIFname)!=0) {
		unlink(tTSIFname);
	}
	ERD_close(&db);

	if(isMSX)
		MSXclose();
	/* release EPANET memory */
	ENclose();
}
void printSpecies(PERD db)
{
	size_t maxLen=7; // length of header
	int i;
	for(i=0;i<db->network->numSpecies;i++) {
		size_t idLen=strlen(db->network->species[i]->id);
		maxLen=idLen>maxLen?idLen:maxLen;
	}
	printf("\nSpecies generated/saved:\n");
	printf("  %-*s idx stored?\n",maxLen,"Species");
	for(i=0;i<db->network->numSpecies;i++) {
		PSpeciesData sp=db->network->species[i];
		printf("  %-*s %3d %sstored\n",maxLen,sp->id,sp->index,sp->index==-1?"not ":"");
	}
}
char *TEVAclocktime(char *atime, long time)
{
    int h, m;
    h = time/3600;
    m = (time%3600)/60;
    sprintf(atime, "%01d:%02d", h, m);
    return(atime);
}

/*
**--------------------------------------------------------------
**  TEVASimError
**  Input:   exit code
**           errmsg: printf-like varargs for the error message
**           to print before exiting
**  Output:  none
**  Purpose: Print an error message and exit the program
**--------------------------------------------------------------
*/
void TEVASimError(int exitCode,const char *errmsg, ...)
{
        va_list ap;
 
        va_start(ap,errmsg);
        vfprintf(stderr,errmsg,ap);
        va_end(ap);
 
        exit(exitCode);
}

int run_flushing(FILE *decfile) {
	// flushing variables
	int cidx, nidx, pidx, patLen, ndPatIdx, nPatInSim, period, ival, nodeID, pipeID;
	int status,wrncnt=0;
	int runflushing=0, prnDebugFile=0;
    long pstep, simtime;
	int nFlushNodes, nClosePipes, flushLen, responseDelay, pipeDelay, detectTime, newPeriod;
	float cvtime, ovtime, val, baseDem, flushrate;
	float* patvals;
	char flushPatName[] = "FLUSH-PAT-0000";
	int *flushNodes, *closePipes;
	// read in decision variables
	fscanf(decfile,"%d",&prnDebugFile);
	fscanf(decfile,"%d",&detectTime);
	fscanf(decfile,"%d",&responseDelay);
	fscanf(decfile,"%d",&pipeDelay);
	fscanf(decfile,"%f",&flushrate);
	fscanf(decfile,"%d",&flushLen);
	fscanf(decfile,"%d",&ival);
	// Read decision variables for flushing
	fscanf(decfile,"%d",&nFlushNodes);
	if (ival > 0) {
		simtime = (long)ival * 60 * 60;
		ENsettimeparam(EN_DURATION,simtime);
	}
	flushNodes = calloc(1+nFlushNodes,sizeof(int));
	if(!flushNodes){
		TEVASimError(1,"Insufficient memory to load decision variables for\nanalysis. Check your system memory.\n");
	}
	for (pidx = 0; pidx < nFlushNodes; pidx++)
	{
		fscanf(decfile,"%d",&ival);
		flushNodes[pidx] = ival;
	}
	// Read decision variables for closing pipes
	fscanf(decfile,"%d",&nClosePipes);
	closePipes = calloc(1+nClosePipes,sizeof(int));
	if(!closePipes){
		TEVASimError(1,"Insufficient memory to load decision variables for\nanalysis. Check your system memory.\n");
	}
	for (pidx = 0; pidx < nClosePipes; pidx++)
	{
		fscanf(decfile,"%d",&ival);
		closePipes[pidx] = ival;
	}

	fclose(decfile);
	ENgettimeparam(EN_PATTERNSTEP,&pstep);
	ENgettimeparam(EN_DURATION,&simtime);
	nPatInSim = (int) simtime / pstep;
	flushLen = flushLen * 60 * 60;
	responseDelay = responseDelay * 60 * 60;
	pipeDelay = pipeDelay * 60  * 60;
	detectTime = detectTime * 60 * 60;
	cidx = 0;
	pidx = 1;
	cvtime = (float)((detectTime+responseDelay)/pstep); // TODO: Check this - is this for pipes or nodes?
	ovtime = (float)((detectTime+responseDelay+flushLen)/pstep);
	printf(" Flushing: Adjusting junction demands.\n");
	for ( nidx = 0; nidx < nFlushNodes; nidx++)
	{
		nodeID = flushNodes[nidx];
		cidx++;
		sprintf(flushPatName,"FLUSH-PAT-%.4d",cidx);
		if ( status = ENaddpattern(flushPatName) )
		{
			printf(" Flushing: Status %d Adding pattern %s.\n",status,flushPatName);
			if (status < 100) wrncnt++;
			else epanetError(status);
		}
		ENgetpatternindex(flushPatName, &pidx);
		ENgetnodevalue(nodeID,EN_PATTERN,&val);
		ndPatIdx = (int)val;
		ENgetpatternlen(ndPatIdx,&patLen);
		patvals = calloc(nPatInSim,sizeof(float));
		ENgetnodevalue(nodeID,EN_BASEDEMAND,&val);
		baseDem = val;
		newPeriod = 0;
		while ( newPeriod < nPatInSim ) {
			for ( period = 0; period < patLen && newPeriod < nPatInSim ; period++ )
			{
				ENgetpatternvalue(ndPatIdx,period+1,&val);
				val = val * baseDem;
				if ( newPeriod < cvtime || newPeriod >= ovtime )
				{
					patvals[newPeriod] = val;
				} else {
					patvals[newPeriod] = val + flushrate;
				}
				newPeriod++;
			}
		}
		ENsetpattern(pidx,patvals,nPatInSim);
		free(patvals);
		val = (float)pidx;
		ENsetnodevalue(nodeID,EN_PATTERN,val);
		ENsetnodevalue(nodeID,EN_BASEDEMAND,1.0);
	}
	// close pipes
	printf(" Flushing: Closing pipes.\n");
	cvtime = (float)(detectTime + responseDelay + pipeDelay);
	ovtime = (float)(detectTime + responseDelay + flushLen + pipeDelay);
	cidx = 0;
	for ( nidx = 0; nidx < nClosePipes; nidx++)
	{
		pipeID = closePipes[nidx];
		cidx++;
		ENsetcontrol(cidx, EN_TIMER, pipeID, 0, 0, cvtime);
		cidx++;
		ENsetcontrol(cidx, EN_TIMER, pipeID, 1, 0, ovtime);
	}
	free(flushNodes);
	free(closePipes);
	if (prnDebugFile) {
		ENsaveinpfile("ts_debug.inp");
	}
	return wrncnt;
}
int run_hydraulics(PERD db,FILE *decfile)
{
    long entime, tstep, rtime, rstep, astep;
	int status,wrncnt,hsteps;
    char atime[10];
	PNetInfo net=db->network;
	PNodeInfo nodes=db->nodes;
	PLinkInfo links=db->links;

	tstep = net->reportStep - net->reportStart; /* initial averaging step when reportstart < reportstep */
    hsteps = 0;
    wrncnt = 0;
    ENCHECK( ENopenH() );
    /*****************************************************/
    /*             Change Flushing Patterns              */
    /*             and Close Pipes                       */
    /* -dbhart, 2010-Jan                                 */
    /*****************************************************/
	if (decfile != NULL) {
		wrncnt+=run_flushing(decfile);
	}
	ENCHECK( ENinitH(1) );
    printf("Computing network hydraulics at time = ");
    do {
        if ( status = ENrunH(&entime) )
        {
            if (status < 100) wrncnt++;
            else epanetError(status);
        }
        /* Compute time of, and step to, next reporting interval boundary */
        if (entime <= net->reportStart) rtime = net->reportStart;
        else if (MOD(entime - net->reportStart,net->reportStep) != 0) {
            rtime = net->reportStart + net->reportStep*(1 + (entime - net->reportStart)/net->reportStep);
        } 
        else rtime = entime;
        rstep = rtime - entime;

        printf("%-7s", TEVAclocktime(atime, entime));
        if (rstep < net->reportStep && hsteps < net->numSteps ) {
            /* Accumulate time averaged node and link data in TSO data structures */
            astep = MIN(tstep,net->reportStep - rstep);
			ENL_getHydResults(hsteps,astep,db);
            if ( rstep == 0 ) hsteps++; /* End of current averaging report interval */
        }

        if ( status = ENnextH(&tstep) )
        {
            if (status < 100) wrncnt++;
            else epanetError(status);
        }
        printf("\b\b\b\b\b\b\b");
    } while (tstep > 0);
    ENcloseH();
    ENsavehydfile("hydraulics.hyd");
    MSXusehydfile("hydraulics.hyd");

    /* Check for hydraulic simulation errors and issue warnings */
    printf("\n");
    if (wrncnt) {
        printf("One or more warnings occurred during the hydraulic\n");
        printf("analysis.  Check the EPANET output file.\n");
    }
    if ( hsteps != net->numSteps ) TEVASimError(1,"Inconsistent calculated/actual hydraulic time steps");

	ERDCHECK(ERD_newHydFile(db));

	ERDCHECK(ERD_writeHydResults(db));

	return wrncnt;
}


int run_quality(PERD db, FILE *tsifile, PSourceData source)
{
    int  is, wqsteps, wrncnt, status, scenario;
    int  storageMethod=-1, runensemble=0;
    long entime, tstep, rtime, rstep, astep;
	char atime[10];
	PNetInfo net=db->network;
	PNodeInfo nodes=db->nodes;
	PLinkInfo links=db->links;

	ENCHECK( ENopenQ() );
    printf("Computing network water quality:\n");
    scenario=0;
	while ( ENL_setSource(source, net, tsifile,0) ) /* Load the next scenario define in TSI file */
    {
    	PSource sources=source->source;
    	int prevEntime;
		PTEVAIndexData indexData;
    	int stopTime=net->simDuration;
        tstep = net->reportStep - net->reportStart; /* initial averaging step when reportstart < reportstep */
        wrncnt = 0;
        wqsteps = 0;
        entime=0;
        scenario++;
        printf("Scenario %05d, injection nodes ",scenario);
        for (is=0; is < source->nsources; is++) printf("%+15s, ",sources[is].sourceNodeID);
        printf(" time");

		ERD_clearQualityData(db);
        ENCHECK( ENinitQ(0) );
        do {
        	prevEntime=entime;
            if ( status = ENrunQ(&entime) ) {
                if (status < 100)
                    wrncnt++;
                else epanetError(status);
            }
            printf("%-7s", TEVAclocktime(atime, entime));

            /* Compute time of, and step to, next reporting interval boundary */
            if (entime <= net->reportStart) rtime = net->reportStart;
            else if (MOD(entime - net->reportStart,net->reportStep) != 0) {
                rtime = net->reportStart + net->reportStep*(1 + (entime - net->reportStart)/net->reportStep);
            } 
            else rtime = entime;
            rstep = rtime - entime;

            /* Set source strengths and types - order is important if duplicate source nodes */
			for (is=0; is < source->nsources; is++) {
				if ( entime >= sources[is].sourceStart && entime < sources[is].sourceStop ) { /* Source is on */
					ENCHECK( ENsetnodevalue(sources[is].sourceNodeIndex,EN_SOURCEQUAL,sources[is].sourceStrength) );
					ENCHECK( ENsetnodevalue(sources[is].sourceNodeIndex,EN_SOURCETYPE,(float)sources[is].sourceType) );
				} else { /* Source is off */
					ENCHECK( ENsetnodevalue(sources[is].sourceNodeIndex,EN_SOURCEQUAL,0.0) );
					ENCHECK( ENsetnodevalue(sources[is].sourceNodeIndex,EN_SOURCETYPE,EN_CONCEN) );
				}
			}
            /* Save results */
            if ( rstep < net->reportStep && wqsteps < net->numSteps ) {

                /* Accumulate time averaged node data in TSO data structures */
                astep = MIN(tstep,net->reportStep - rstep);
                astep=entime-prevEntime;
                ENL_getQualResults(wqsteps,astep,db);
                if ( rstep == 0 ) wqsteps++; /* End of current averaging report interval */
            }

            if(tstep > 0) {
				if ( status = ENstepQ(&tstep) ) {
					if (status < 100)
						wrncnt++;
					else epanetError(status);
				}

				printf("\b\b\b\b\b\b\b");
            }
        } while (entime < stopTime);


        /* Check for errors and issue warnings */
        printf("\n");
        if (wrncnt) {
            printf("\nOne or more warnings occurred during the water quality\n");
            printf("analysis, check MS/EPANET output file\n");
        }
        if ( wqsteps != net->numSteps ) TEVASimError(1,"Inconsistent calculated/actual water quality time steps");

        /* Write scenario results to TSO database */
		indexData=newTEVAIndexData(source->nsources,source->source);
		ERD_writeQualResults(db,indexData);
    } /* End of while */

	ENcloseQ();
	return wrncnt;
}
int run_quality_msx(PERD db, FILE *tsifile, const char *msx_species, PSourceData source, double **initQual)
{
    int  is, wqsteps, wrncnt, status, scenario;
    int  storageMethod=-1, runensemble=0;
    long entime, tstep, rtime, rstep, astep;
	char atime[10];
	PNetInfo net=db->network;
	PNodeInfo nodes=db->nodes;
	PLinkInfo links=db->links;

    printf("Computing network water quality:\n");
    scenario=0;
	while ( ENL_setSource(source, net, tsifile,1) ) /* Load the next scenario define in TSI file */
	{
		PSource sources=source->source;
    	int prevEntime;
		PSourceData indexData;
    	int stopTime=net->simDuration;
        tstep = net->reportStep - net->reportStart; /* initial averaging step when reportstart < reportstep */
        wrncnt = 0;
        wqsteps = 0;
        entime=0;
        scenario++;
        printf("Scenario %05d, injection nodes ",scenario);
        for (is=0; is < source->nsources; is++) printf("%+15s, ",sources[is].sourceNodeID);
        printf(" time");

		ERD_clearQualityData(db);
		ENL_restoreInitQual(db->network,initQual);
        ENCHECK( MSXinit(0) );
        ENL_getQualResults(wqsteps,net->reportStep,db);
		wqsteps++;
        do {
            /* Set source strengths and types - order is important if duplicate source nodes */
			for (is=0; is < source->nsources; is++) {
				if ( entime >= sources[is].sourceStart && entime < sources[is].sourceStop ) { /* Source is on */
					ENCHECK( MSXsetsource(sources[is].sourceNodeIndex,sources[is].speciesIndex+1,sources[is].sourceType,sources[is].sourceStrength,0) );
				} else { /* Source is off */
					ENCHECK( MSXsetsource(sources[is].sourceNodeIndex,sources[is].speciesIndex+1,MSX_NOSOURCE,0,0) );
				}
			}
        	prevEntime=entime;
            if ( status = MSXstep(&entime,&tstep) ) {
                if (status < 100)
                    wrncnt++;
                else epanetError(status);
            }
            printf("%-7s", TEVAclocktime(atime, entime));
			printf("\b\b\b\b\b\b\b");

            /* Compute time of, and step to, next reporting interval boundary */
            if (entime <= net->reportStart) rtime = net->reportStart;
            else if (MOD(entime - net->reportStart,net->reportStep) != 0) {
                rtime = net->reportStart + net->reportStep*(1 + (entime - net->reportStart)/net->reportStep);
            } 
            else rtime = entime;
            rstep = rtime - entime;

            /* Save results */
            if ( rstep < net->reportStep && wqsteps < net->numSteps ) {

                /* Accumulate time averaged node data in TSO data structures */
                astep = MIN(tstep,net->reportStep - rstep);
                astep=entime-prevEntime;
                ENL_getQualResults(wqsteps,astep,db);
                if ( rstep == 0 ) wqsteps++; /* End of current averaging report interval */
            }

		} while (entime < stopTime);


        /* Check for errors and issue warnings */
        printf("\n");
        if (wrncnt) {
            printf("\nOne or more warnings occurred during the water quality\n");
            printf("analysis, check MS/EPANET output file\n");
        }
        if ( wqsteps != net->numSteps ) TEVASimError(1,"Inconsistent calculated/actual water quality time steps");

        /* Write scenario results to TSO database */
		indexData=newTEVAIndexData(source->nsources,source->source);
		ERD_writeQualResults(db,indexData);
    } /* End of while */

	return wrncnt;
}

