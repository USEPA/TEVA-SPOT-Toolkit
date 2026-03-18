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
/*
 * Modified 2010 Sandia National Laboratories
 * Modified to run flushing commands if decfname is provided and present
 */
/* TEVA header */

#include "teva.h"

void run_flushsim(int tsofileversion, char* tsgfname, char* tsifname, char* tsofname, char* epanetinpfname, char* epanetoutfname, char* decfname)
{
	// original tevasim variables
    int  is, hsteps, wqsteps, wrncnt, status, version, scenario;
    int  storageMethod=-1, runensemble=0;
    long entime, tstep, rtime, rstep, astep, pstep, simtime;
    char atime[10];
    time_t cpustart, cpustop;
    double cpuelapsed;
    PSource sources;
	PNetInfo net;
	PTSO tso;
	PNodeInfo nodes;
	PLinkInfo links;
	FILE *tsgfile=NULL, *tsifile=NULL;

	// flushing variables
	int cidx, nidx, pidx, patLen, ndPatIdx, nPatInSim, period, ival, nodeID, pipeID;
	int runflushing=0, prnDebugFile=0;
	int nFlushNodes, nClosePipes, flushLen, responseDelay, pipeDelay, detectTime, newPeriod;
	float cvtime, ovtime, val, baseDem, flushrate;
	float* patvals;
	char flushPatName[] = "FLUSH-PAT-0000";
	int *flushNodes, *closePipes;
    FILE *decfile=NULL;

    if (tsgfname) {
       TEVAFILEOPEN(tsgfile = fopen(tsgfname, "r"));
       runensemble=1;
       }
    if (tsifname) {
       tsifile = fopen(tsifname, "r+t");
       if (!tsifile)
          TEVAFILEOPEN(tsifile = fopen(tsifname, "w+t"));
       runensemble=1;
    }
	if (decfname) {
		decfile = fopen(decfname, "r");
		if (!decfile)
			TEVAFILEOPEN(decfile = fopen(decfname, "r"));
		runflushing=1;
	}

    /* Open EPANET/MSEPANET and process input file */
    printf("T H R E A T  E N S E M B L E  V U L N E R A B I L I T Y  A N A L Y S I S\n");
    printf("                               (T E V A)\n");
    printf("                    T H R E A T  S I M U L A T O R\n");
    printf("                              Version %d.%d\n\n",TSMAJORVERSION,TSMINORVERSION);
    ENCHECK( (ENopen(epanetinpfname, epanetoutfname, "")) );
    ENgetversion(&version);   /* MS/Epanet version */
#ifdef MSEPANET
    printf("T E V A is using MSEpanet release %d\n",version);
    printf("Initializing MSEpanet \n");
#else
    printf("T E V A is using Epanet release %d\n",version);
    printf("Initializing Epanet \n");
#endif

    /* Initialize TSO structures and create new TSO file */
    if(!tsofname) { /* temporary tso file */
        if( ( tsofname = tmpnam( NULL ) ) == NULL )
            TEVASimError(1,"Can not create temporary TSO file\n");
    }
    ENTU_Initialize(epanetinpfname, tsofname, storageMethod, tsofileversion, &tso, &net, &nodes, &links, &sources);

    /*****************************************************/
    /* Process demands and demand patterns to create     */
    /* proper demand patterns and agerage demands for    */
    /* use in demand-based population and ingestion.     */
    /*****************************************************/
    ENTU_CreateDemandPatterns(net,nodes);
    /* (Optionally) Process threat simulation generator file and write simulation input file */
    if(tsgfile) {
        if(!tsifile) { /* temporary tsi file */
            if( ( tsifname = tmpnam( NULL ) ) != NULL ) {
                TEVAFILEOPEN(tsifile = fopen(tsifname, "w+t"));
                }
            else {
                TEVASimError(1,"Can not create temporary TSI file\n");
                }
        }
        ENTU_WriteTSI(net, nodes, sources, tsgfile, tsifile);
    }
    /*****************************************************/
    /*             Hydraulic Simulation                  */
    /*****************************************************/
    tstep = net->reportstep - net->reportstart; /* initial averaging step when reportstart < reportstep */
    hsteps = 0;
    wrncnt = 0;
    ENCHECK( ENopenH() );
    /*****************************************************/
    /*             Change Flushing Patterns              */
    /*             and Close Pipes                       */
    /* -dbhart, 2010-Jan                                 */
    /*****************************************************/
	if (runflushing) {
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
			printf("ERROR\n");
			printf("Insufficient memory to load decision variables for\n");
			printf("analysis. Check your system memory.\n");
			return;
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
			printf("ERROR\n");
			printf("Insufficient memory to load decision variables for\n");
			printf("analysis. Check your system memory.\n");
			return;
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
				else ENTU_EpanetError(status);
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
	}
    /*****************************************************/
    /*             Hydraulic Simulation (ctd.)           */
    /*****************************************************/
    ENCHECK( ENinitH(1) );
    printf("Computing network hydraulics at time = ");
    do {
        if ( status = ENrunH(&entime) )
        {
            if (status < 100) wrncnt++;
            else ENTU_EpanetError(status);
        }

        /* Compute time of, and step to, next reporting interval boundary */
        if (entime <= net->reportstart) rtime = net->reportstart;
        else if (MOD(entime - net->reportstart,net->reportstep) != 0) {
            rtime = net->reportstart + net->reportstep*(1 + (entime - net->reportstart)/net->reportstep);
        } 
        else rtime = entime;
        rstep = rtime - entime;

        printf("%-7s", TEVAclocktime(atime, entime));
        if (rstep < net->reportstep && hsteps < net->nsteps ) {
            /* Accumulate time averaged node and link data in TSO data structures */
            astep = MIN(tstep,net->reportstep - rstep);
			ENTU_GetNodeHData(nodes,net,hsteps,astep);
			ENTU_GetLinkHData(links,net,hsteps,astep);
            if ( rstep == 0 ) hsteps++; /* End of current averaging report interval */
        }

        if ( status = ENnextH(&tstep) )
        {
            if (status < 100) wrncnt++;
            else ENTU_EpanetError(status);
        }
        printf("\b\b\b\b\b\b\b");
    } while (tstep > 0);
    ENcloseH();

    /* Check for hydraulic simulation errors and issue warnings */
    printf("\n");
    if (wrncnt) {
        printf("One or more warnings occurred during the hydraulic\n");
        printf("analysis.  Check the EPANET output file.\n");
    }
    if ( hsteps != net->nsteps ) TEVASimError(1,"Inconsistent calculated/actual hydraulic time steps");

    /* Write TSO Prologue                                                              */
    /* It only involves hydraulic information now stored in nodes and links structures */
    TSO_WritePrologue(tso,net,nodes,links);

    /* That's it if no -tsi or -tsg flag */
    time( &cpustart );
    if(!runensemble) goto stop;

    /************************************************************/
    /*             Water Quality Simulations                    */
    /************************************************************/
    ENCHECK( ENopenQ() );
    printf("Computing network water quality:\n");
    scenario=0;
    while ( ENTU_SetSource(sources, net, tsifile) ) /* Load the next scenario define in TSI file */
    {
    	int prevEntime;
    	int stopTime=net->simduration;
        tstep = net->reportstep - net->reportstart; /* initial averaging step when reportstart < reportstep */
        wrncnt = 0;
        wqsteps = 0;
        entime=0;
        scenario++;
        printf("Scenario %05d, injection nodes ",scenario);
        for (is=0; is < net->nsources; is++) printf("%+15s, ",sources[is].SourceNodeID);
        printf(" time");

		ENTU_ClearNodeQData(nodes,net);

        ENCHECK( ENinitQ(0) );
        do {
        	prevEntime=entime;
            if ( status = ENrunQ(&entime) ) {
                if (status < 100)
                    wrncnt++;
                else ENTU_EpanetError(status);
            }
            printf("%-7s", TEVAclocktime(atime, entime));

            /* Compute time of, and step to, next reporting interval boundary */
            if (entime <= net->reportstart) rtime = net->reportstart;
            else if (MOD(entime - net->reportstart,net->reportstep) != 0) {
                rtime = net->reportstart + net->reportstep*(1 + (entime - net->reportstart)/net->reportstep);
            } 
            else rtime = entime;
            rstep = rtime - entime;

            /* Set source strengths and types - order is important if duplicate source nodes */
#ifdef MSEPANET
            if( net->qualcode==EN_MULTIS ) {
                for (is=0; is < net->nsources; is++) {
                    if ( entime >= sources[is].SourceStart && entime < sources[is].SourceStop ) { /* Source is on */
                        ENCHECK( ENMSsetnodequality(sources[is].SourceNodeIndex,sources[is].SpeciesIndex,EN_SOURCEQUAL,sources[is].SourceStrength) );
                        ENCHECK( ENMSsetnodequality(sources[is].SourceNodeIndex,sources[is].SpeciesIndex,EN_SOURCETYPE,(float)sources[is].SourceType) );
                    }
                    else { /* Source is off */
                        ENCHECK( ENMSsetnodequality(sources[is].SourceNodeIndex,sources[is].SpeciesIndex,EN_SOURCEQUAL,0.0) );
                        ENCHECK( ENMSsetnodequality(sources[is].SourceNodeIndex,sources[is].SpeciesIndex,EN_SOURCETYPE,EN_CONCEN) );
                    }
                }
            } else {
                for (is=0; is < net->nsources; is++) {
                    if ( entime >= sources[is].SourceStart && entime < sources[is].SourceStop ) { /* Source is on */
                        ENCHECK( ENsetnodevalue(sources[is].SourceNodeIndex,EN_SOURCEQUAL,sources[is].SourceStrength) );
                        ENCHECK( ENsetnodevalue(sources[is].SourceNodeIndex,EN_SOURCETYPE,(float)sources[is].SourceType) );
                    }
                    else { /* Source is off */
                        ENCHECK( ENsetnodevalue(sources[is].SourceNodeIndex,EN_SOURCEQUAL,0.0) );
                        ENCHECK( ENsetnodevalue(sources[is].SourceNodeIndex,EN_SOURCETYPE,EN_CONCEN) );
                    }
                }
            }
#else
            for (is=0; is < net->nsources; is++) {
                if ( entime >= sources[is].SourceStart && entime < sources[is].SourceStop ) { /* Source is on */
                    ENCHECK( ENsetnodevalue(sources[is].SourceNodeIndex,EN_SOURCEQUAL,sources[is].SourceStrength) );
                    ENCHECK( ENsetnodevalue(sources[is].SourceNodeIndex,EN_SOURCETYPE,(float)sources[is].SourceType) );
                }
                else { /* Source is off */
                    ENCHECK( ENsetnodevalue(sources[is].SourceNodeIndex,EN_SOURCEQUAL,0.0) );
                    ENCHECK( ENsetnodevalue(sources[is].SourceNodeIndex,EN_SOURCETYPE,EN_CONCEN) );
                }
            }
#endif

            /* Save results */
            if ( rstep < net->reportstep && wqsteps < net->nsteps ) {

                /* Accumulate time averaged node data in TSO data structures */
                astep = MIN(tstep,net->reportstep - rstep);
                astep=entime-prevEntime;
                ENTU_GetNodeQData(nodes,net,wqsteps,astep);
                ENTU_GetLinkQData(links,net,wqsteps,astep);
                if ( rstep == 0 ) wqsteps++; /* End of current averaging report interval */
            }

            if(tstep > 0) {
				if ( status = ENstepQ(&tstep) ) {
					if (status < 100)
						wrncnt++;
					else ENTU_EpanetError(status);
				}
	//            if ( status = ENnextQ(&tstep) ) {
	//                if (status < 100)
	//                    wrncnt++;
	//                else ENTU_EpanetError(status);
	//            }

				printf("\b\b\b\b\b\b\b");
            }
        } while (entime < stopTime);


        /* Check for errors and issue warnings */
        printf("\n");
        if (wrncnt) {
            printf("\nOne or more warnings occurred during the water quality\n");
            printf("analysis, check MS/EPANET output file\n");
        }
        if ( wqsteps != net->nsteps ) TEVASimError(1,"Inconsistent calculated/actual water quality time steps");

        /* Write scenario results to TSO database */
		TSO_WriteSimResults(tso,net,sources,nodes);
    } /* End of while */

	ENcloseQ();

stop:
    time( &cpustop );
    cpuelapsed = difftime(cpustop,cpustart);
    printf("Elapsed time for quality simulations (seconds): %e\n", cpuelapsed);
    printf("T E V A  Normal  Termination\n");

	/* release all memory allocated */
	TSO_ReleaseNetworkData(&nodes,&links,net);
    free(sources); /* I'd rather this be part of ReleaseNetworkData... */
	TSO_ReleaseNetworkInfo(&net);

	/* close any files that have been opened */
	if(tsgfile) fclose(tsgfile);
	if(tsifile) fclose(tsifile);
	TSO_Close(&tso);

	/* release EPANET memory */
	ENclose();
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

