/* TEVA header */

#include "teva.h"

static char *usageString="usage: tevasim [-s StorageMethod] [-v TSOFileVersion] [-tsg tsg-file] [-tsi tsi-file] [-tso tso-file] <epanet-input-file> <epanet-output-file>\n";

int main(int argc, char **argv)
/*
**--------------------------------------------------------------
** Module:  TEVA
** Usage:   tevasim [-s StorageMethod] [-tsg tsg-file] [-tsi tsi-file] [-tso tso-file] <epanet-input-file> <epanet-output-file>
** Input:   EPANET input file, threat simulation generator or input file
** Output:  program status, epanet output, simulation output database
** Purpose: generate tso format database of water quality data for specified
**          threat simulations
**---------------------------------------------------------------
*/
{
    int  ai, is, hsteps, wqsteps, wrncnt, status, version, scenario;
    int  storageMethod=-1, tsofileversion=TSOFILEVERSION, runensemble=0;
    long entime, tstep, rtime, rstep, astep;
    FILE *tsgfile=NULL, *tsifile=NULL;
    char atime[10];
    char *epanetinpfname, *epanetoutfname, *tsgfname=NULL, *tsifname=NULL, *tsofname=NULL;
    time_t cpustart, cpustop;
    double cpuelapsed;
    PSource sources;
	PNetInfo net;
	PTSO tso;
	PNodeInfo nodes;
	PLinkInfo links;

    /* Command Line */
    ai = 1;
    while(ai<argc-1) { /* Process flags */
        if(strncmp(argv[ai],"-s",2)==0) {
            ai++;
            storageMethod = atoi(argv[ai++]);
            if(storageMethod>TSOSTORAGEMETHOD) TEVASimError(1,"Invalid TSO Storage Method Flag\n");
        } else if(strncmp(argv[ai],"-tsi",4)==0) {
            ai++;
            tsifname=argv[ai++];
            tsifile = fopen(tsifname, "r+t");
            if(!tsifile) TEVAFILEOPEN(tsifile = fopen(tsifname, "w+t"));
            runensemble=1;
        } else if(strncmp(argv[ai],"-tsg",4)==0) {
            ai++;
            tsgfname=argv[ai++];
            TEVAFILEOPEN(tsgfile = fopen(tsgfname, "rt"));
            runensemble=1;
        } else if(strncmp(argv[ai],"-tso",4)==0) {
            ai++;
            tsofname=argv[ai++];
        } else if(strncmp(argv[ai],"-v",2)==0) {
            ai++;
            tsofileversion=atoi(argv[ai++]);
            if(tsofileversion<1 || tsofileversion>TSOFILEVERSION) TEVASimError(1,"Invalid TSO File Version Flag\n");
        } else break;
    }
    if(argc-ai != 2) { 
        printf(usageString); 
        exit(9); 
    }
    epanetinpfname=argv[ai++];
    epanetoutfname=argv[ai];

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

    /* (Optionally) Process threat simulation generator file and write simulation input file */
    if(tsgfile) {
        if(!tsifile) { /* temporary tsi file */
            if( ( tsifname = tmpnam( NULL ) ) != NULL )
                TEVAFILEOPEN(tsifile = fopen(tsifname, "w+t"));
            else
                TEVASimError(1,"Can not create temporary TSI file\n");
        }
        ENTU_WriteTSI(net, sources, tsgfile, tsifile);
    }

    /*****************************************************/
    /*             Hydraulic Simulation                  */
    /*****************************************************/
    tstep = net->reportstep - net->reportstart; /* initial averaging step when reportstart < reportstep */
    hsteps = 0;
    wrncnt = 0;
    ENCHECK( ENopenH() );
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
        tstep = net->reportstep - net->reportstart; /* initial averaging step when reportstart < reportstep */
        wrncnt = 0;
        wqsteps = 0;
        scenario++;
        printf("Scenario %05d, injection nodes ",scenario);
        for (is=0; is < net->nsources; is++) printf("%+15s, ",sources[is].SourceNodeID);
        printf(" time");

		ENTU_ClearNodeQData(nodes,net);

        ENCHECK( ENinitQ(0) );
        do {
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
                ENTU_GetNodeQData(nodes,net,wqsteps,astep);
                ENTU_GetLinkQData(links,net,wqsteps,astep);
                if ( rstep == 0 ) wqsteps++; /* End of current averaging report interval */
            }

            if ( status = ENnextQ(&tstep) ) {
                if (status < 100)
                    wrncnt++;
                else ENTU_EpanetError(status);
            }

            printf("\b\b\b\b\b\b\b");
        } while (tstep > 0);


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
    return(0);
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

