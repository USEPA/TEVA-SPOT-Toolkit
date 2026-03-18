/* System Headers */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>

/* Epanet Programmers Toolkit Header */
#ifdef WIN32
#include "epanet2.h"
#endif

#ifdef __linux__
#include "toolkit.h"
#endif

/* TEVA header */
#include "teva.h"

/* NR header */
#include "nrutil.h"

int main(int argc, char **argv)
/*
**--------------------------------------------------------------
** Module:  TEVA
** Usage:   teva.exe <epanet input> <epanet output> <tsg input> <tsi output> <tso output>
** Input:   EPANET input file, threat simulation generator file
** Output:  program status, epanet output, simulation output database
** Purpose: generate tables of water quality data for specified
**          threat simulations
**---------------------------------------------------------------
*/
{
    int    is,
         hsteps,
         wqsteps,
         wrncnt,
         status,
         version,
         scenario = 0;
    long   rstart,
         rstep,
         entime,
         tstep;
    FILE   *simgen,
         *simin;
    char   atime[10];
    time_t cpustart,
         cpustop;
    double cpuelapsed;
	int nspecies=1;
//  int    *savec;
//    float  **c;
    Source sources[MAXSOURCES];
	PNetInfo net;
	PTSO tso;
	PNodeInfo nodes;
	PLinkInfo links;

     if(argc!=6)
       {
         printf("***Incorrect invocation\n");
         printf("***Correct usage is: teva <epanet-input-file> <epanet-output-file> <tsg-input-file> <tsi-input-file> <tso-output-file>\n");
         return 0;
       }

    /* Open EPANET and process input file */
    ENgetversion(&version);                                                    /* Epanet version */
    printf("T H R E A T  E N S E M B L E  V U L N E R A B I L I T Y  A N A L Y S I S\n");
    printf("                               (T E V A)\n");
    printf("                            Alpha Version 1.3\n\n");
    printf("T E V A is using Epanet version %d\n",version);
    printf("Initializing Epanet \n");
    ENCHECK( (ENopen(argv[1], argv[2], "")) );

    /* Open simulation input and output files */
    FILEOPEN(simgen = fopen(argv[3], "rt"));
    FILEOPEN(simin = fopen(argv[4], "w+t"));

	net = TSO_AllocateNetworkInfo(nspecies);
    /* Memory allocation */
    GetSize(net,&rstart,&rstep);            /* Network size */
	TSO_AllocateNetworkData(&nodes,&links,net);
	InitializeNetwork(nodes,links,net);
	tso = TSO_Create(argv[5],-1,net);

	/* Process INP file to get node coordinates */
    GetNodeXY(nodes, net->nnodes, argv[1]);                                       /* x,y coordinate data */

    /* Process threat simulation generator file and write simulation input file */
    WriteTSI(net->nnodes, sources, simgen, simin);

    /*****************************************************/
    /*             Hydraulic Simulation                  */
    /*****************************************************/

    ENCHECK( ENopenH() );
    ENCHECK( ENinitH(1) );
    hsteps = 0;
    wrncnt = 0;

    printf("Computing network hydraulics at time = ");
    do {
        if ( status = ENrunH(&entime) )
    {
            if (status < 100) wrncnt++;
            else epaneterror(status);
        }

        printf("%-7s", TEVAclocktime(atime, entime));
        if (entime >= rstart && MOD(entime,rstep) == 0) {
            /* Write the node demands */
			GetDemandData(net,nodes,hsteps);
			GetLinkData(net,links,hsteps);
			hsteps++;
        }

        if ( status = ENnextH(&tstep) )
        {
            if (status < 100) wrncnt++;
            else epaneterror(status);
        }
        printf("\b\b\b\b\b\b\b");
    } while (tstep > 0);
    TSO_WritePrologue(tso,net,nodes,links);
    ENcloseH();

    /* Check for errors and issue warnings */
  printf("\n");
    if (wrncnt) {
        printf("One or more warnings occurred during the hydraulic\n");
        printf("analysis.  Check the EPANET output file.\n");
    }
    if ( hsteps != net->nsteps ) nrerror("Inconsistent calculated/actual hydraulic time steps");

    /************************************************************/
    /*             Water Quality Simulations                    */
    /************************************************************/

    /* Initialize EPANET quality simulation */
    time( &cpustart );
    ENCHECK( ENopenQ() );
    printf("Computing network water quality:\n");

    while ( SetSource(sources, &net->nsources, net->nnodes, simin) )       /* Load the next scenario */
    {
        wrncnt = 0;
        wqsteps = 0;
        scenario++;
        printf("Scenario %05d, injection nodes ",scenario);
        for (is=0; is < net->nsources; is++) printf("%+15s, ",sources[is].SourceNodeID);
        printf(" time");

        ENCHECK( ENinitQ(0) );
        do {
            if ( status = ENrunQ(&entime) ) {
                if (status < 100)
                    wrncnt++;
                else epaneterror(status);
            }
            printf("%-7s", TEVAclocktime(atime, entime));

            /* Set source strengths and types - order is important if duplicate source nodes */
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

            /* Save results */
            if (entime >= rstart && MOD(entime,rstep) == 0) {
                GetNodeData(EN_QUALITY,net->nnodes,tso->nodedata);
                for (is=0; is < net->nnodes; is++) nodes[is].c[0][wqsteps]=tso->nodedata[is];
                wqsteps++;
            }

            if ( status = ENnextQ(&tstep) ) {
                if (status < 100)
                    wrncnt++;
                else epaneterror(status);
            }

            printf("\b\b\b\b\b\b\b");
        } while (tstep > 0);

        /* Check for errors and issue warnings */
        printf("\n");
        if (wrncnt) {
            printf("\nOne or more warnings occurred during the water quality\n");
            printf("analysis, check EPANET output file\n");
        }
        if ( wqsteps != net->nsteps ) nrerror("Inconsistent calculated/actual water quality time steps");
		TSO_WriteSimResults(tso,net,sources,nodes);
    } /* End of while */

	ENcloseQ();
    printf("\nClosing EPANET WQ Simulator\n");
    time( &cpustop );
    cpuelapsed = difftime(cpustop,cpustart);
    printf("Elapsed time for quality simulations (seconds): %e\n", cpuelapsed);
    printf("T E V A  Normal  Termination\n");
	// releasea all memoryr allocated
	TSO_ReleaseNetworkData(&nodes,&links,net);
	TSO_ReleaseNetworkInfo(&net);
	// close any files that have been opened
	fclose(simgen);
	fclose(simin);
	TSO_Close(&tso);
	// release EPANET memory
	ENclose();
}


void GetSize(PNetInfo net, long *rstart, long *rstep )
/*
**--------------------------------------------------------------
**  Input:   None.
**
**  Output:  nnodes, nlinks, ntanks, njunctions = network size parameters
**           nsteps = number of reporting time steps
**           rstart, rstep = report start time and time step
**  Purpose: Get network size and simulation parameters.
**--------------------------------------------------------------
*/
{
	long dur;
	int i;

    ENCHECK( ENgetcount(EN_NODECOUNT, &net->nnodes) );                  /* # Nodes */
    ENCHECK( ENgetcount(EN_LINKCOUNT, &net->nlinks) );                  /* # Links */
    ENCHECK( ENgetcount(EN_TANKCOUNT, &net->ntanks) );                  /* # Tanks */
    ENCHECK( ENgettimeparam(EN_REPORTSTEP, rstep) );              /* Report start time */
    ENCHECK( ENgettimeparam(EN_REPORTSTART, rstart) );            /* Report time step */
    ENCHECK( ENgettimeparam(EN_DURATION, &dur) );                 /* Sim duration */
    net->nsteps = 1 + (dur - *rstart)/(*rstep);                       /* # Report steps */
    net->njunctions = net->nnodes - net->ntanks;                                            /* # Junctions */
	for(i=0;i<net->nspecies;i++) {
		//temporoary until multi-species EPANET
		strcpy(net->species[i],"Unknown");
	}
	net->stepsize = (float)(*rstep)/60.0f/60.0f;
    net->fltmax = FLT_MAX;

}

void InitializeNetwork(PNodeInfo nodes, PLinkInfo links, PNetInfo net) {
    int i,fromnode, tonode, nodetype;

    for (i=0; i<net->nnodes; i++) {                                        /* Tank indices */
        ENCHECK( ENgetnodetype(i+1,&nodetype) );
        if ( nodetype == EN_TANK || nodetype == EN_RESERVOIR ) {
            nodes[i].ntype = tank;
        } else {
            nodes[i].ntype = junction;
		}
		nodes[i].x = net->fltmax;
		nodes[i].y = net->fltmax;
    }
    for (i=0; i<net->nnodes; i++) {                                        /* Node IDs */
        memset( nodes[i].id, '\0', MAXCHARID );
        ENCHECK( ENgetnodeid(i+1,nodes[i].id) );
    }
    for (i=0; i<net->nlinks; i++) {                                        /* From link nodes */
        ENCHECK( ENgetlinknodes(i+1,&fromnode,&tonode) );
		links[i].from=fromnode;
		links[i].to=tonode;
    }
}


int SetSource(PSource sources, int *numsources,
                 int nnodes, FILE *simin)
/*
**--------------------------------------------------------------
**  Input:   simin = pointer to simulation input stream
**  Output:  structure sources contains name of Epanet source
**           node ID, source start time (seconds), source stop
**           time (seconds), and source strength for all sources.
**           numsources contains the number of sources.
**           Returns 1 if successful; 0 if not
**  Purpose: Reads one line of threat scenario input file and returns
**           source parameters
**--------------------------------------------------------------
*/
{
    int   i,is;
    long  tokl;
    char  line[MAXLINE+1],     /* Line from input data file       */
            wline[MAXLINE+1];    /* Working copy of input line      */
    int   status = 0,          /* Error code                      */
            ntokens = 0,         /* # tokens in input line          */
            tokspersource = 5,   /* # tokens to describe one source */
            nreadtoks;           /* # of read tokens                */
    char  *tok[MAXTOKS];       /* Array of token strings          */

    /* Read a line from input file. */
    while (fgets(line,MAXLINE,simin) != NULL)
    {

        /* Make copy of line and scan for tokens */
        strcpy(wline,line);
        ntokens = TEVAgettokens(wline,tok);

        /* Skip blank lines and comments */
        if (ntokens == 0) continue;
        if (*tok[0] == ';') continue;

        /* Check if max. length exceeded */
        if (strlen(line) >= MAXLINE) nrerror("TSI file error");

        /* Check for proper number of tokens */
        if ( MOD(ntokens,tokspersource) != 0 ) nrerror("TSI file error");

        /* Otherwise process input line */
        if ( (*numsources = ntokens/tokspersource) > MAXSOURCES) nrerror("TSI file error");          /* Max # of quality sources   */
        for (is = 0; is < *numsources; is++) {
            nreadtoks = is*tokspersource;
            if ( strlen(tok[nreadtoks]) > MAXCHARID ) nrerror("TSI file error");
            ENCHECK( ENgetnodeindex(tok[nreadtoks],&sources[is].SourceNodeIndex) );                    /* Node index of source       */
            strcpy(sources[is].SourceNodeID,tok[nreadtoks]);                                           /* source node ID             */
            if ( !getlong(tok[nreadtoks+1],&tokl) ) nrerror("TSI file error");                         /* start time                 */
            sources[is].SourceType=(int)tokl;
            sources[is].SpeciesIndex=1;  /* Default species index for now */
            if ( !TEVAgetfloat(tok[nreadtoks+2],&sources[is].SourceStrength) ) nrerror("TSI file error");  /* source strength            */
            if ( !getlong(tok[nreadtoks+3],&sources[is].SourceStart) ) nrerror("TSI file error");      /* start time                 */
            if ( !getlong(tok[nreadtoks+4],&sources[is].SourceStop) ) nrerror("TSI file error");       /* stop time                  */
        }
        for (i = 1; i <= nnodes; i++ ) { /* Override ALL *.inp source and WQ information */
            ENCHECK( ENsetnodevalue(i,EN_SOURCETYPE,EN_CONCEN) );  /* CONCEN type                */
            ENCHECK( ENsetnodevalue(i,EN_SOURCEQUAL,0.0) );        /* Zero all sources           */
            ENCHECK( ENsetnodevalue(i,EN_SOURCEPAT,0) );           /* No pattern                 */
            ENCHECK( ENsetnodevalue(i,EN_INITQUAL,0) );            /* Initial quality = 0        */
    }
    return(1);
    }   /* End of while */
  return(0);
}     /* End of SetSource  */


void  GetNodeXY(PNodeInfo nodes, int nnodes, char *epanetfname)
/*
**--------------------------------------------------------------
**  Input:   xnode  =  pointer to float array >= nnodes+1 elements
**           ynode  =  pointer to float array >= nnodes+1 elements
**           nnodes =  int number of network nodes
**           *epanetfname = pointer to epanet input file name
**  Output:  returns xnode,ynode arrays filled with coordinates.
**           if no coordinate found in [COORD] section of data
**           file then associated xnode,ynode are NaN.
**  Purpose: reads Epanet data file to get node coordinate info
**           (no toolkit function for this task).  Modeled
**           closely after readdata() Epanet function.
**--------------------------------------------------------------
*/
{
    char  line[MAXLINE+1],     /* Line from input data file       */
        wline[MAXLINE+1];        /* Working copy of input line      */
    int   sect,                /* Data sections                   */
        ntokens = 0,             /* # tokens in input line          */
        nodeindex;               /* node index                      */
    FILE *EPANETIN;            /* File pointer to epanet input    */
    char *tok[MAXTOKS];        /* Array of token strings          */
    float x,y;                 /* coordinates                     */

    char *secttxt[]         = {"[TITL", "[JUNC", "[RESE",
        "[TANK", "[PIPE", "[PUMP",
        "[VALV", "[CONT", "[RULE",
        "[DEMA", "[SOUR", "[EMIT",
        "[PATT", "[CURV", "[QUAL",
        "[STAT", "[ROUG", "[ENER",
        "[REAC", "[MIXI", "[REPO",
        "[TIME", "[OPTI", "[COOR",
        "[VERT", "[LABE", "[BACK",
        "[TAGS", "[END",  NULL};

    /* Open epanet input file */
    FILEOPEN(EPANETIN = fopen( epanetfname, "rt" ));

    sect = -1;
    /* Read each line from input file. */
    while (fgets(line,MAXLINE,EPANETIN) != NULL)
    {

        /* Make copy of line and scan for tokens */
        strcpy(wline,line);
        ntokens = TEVAgettokens(wline,tok);

        /* Skip blank lines and comments */
        if (ntokens == 0) continue;
        if (*tok[0] == ';') continue;

        /* Check if max. length exceeded */
        if (strlen(line) >= MAXLINE) nrerror("Epanet input file error.");

        /* Check if at start of a new input section */
        if (*tok[0] == '[')
        {
            sect = TEVAfindmatch(tok[0],secttxt);
            if (sect >= 0)
            {
                if (sect == _END) break;
                continue;
            }
            else nrerror("Epanet input file error");
        }

        /* Otherwise process input if in Coords. section */
        else if (sect == _COORDS)
        {
            if ( ntokens != 3 ) nrerror("Epanet input file error");
            ENCHECK( ENgetnodeindex(tok[0],&nodeindex) );
            if ( nodeindex > nnodes ) nrerror("Epanet input file error");
            if ( TEVAgetfloat(tok[1],&x) ) {
                nodes[nodeindex-1].x = x;
            }
            else nrerror("Epanet input file error");
            if ( TEVAgetfloat(tok[2],&y) ) {
                nodes[nodeindex-1].y = y;
            }
            else nrerror("Epanet input file error");
        }
    }   /* End of while */
	fclose(EPANETIN);

}     /* End of GetNodeXY  */



int  TEVAgettokens(char *s, char **tok)
/*
**--------------------------------------------------------------
**  Input:   *s = string to be tokenized
**  Output:  returns number of tokens in s
**  Purpose: scans string for tokens, saving pointers to them
**           in Tok[]
**
** Tokens can be separated by the characters listed in SEPSTR
** (spaces, tabs, newline, carriage return) which is defined
** in TYPES.H. Text between quotes is treated as a single token.
**--------------------------------------------------------------
*/
{
    int  len, m, n;
    char *c;

    /* Begin with no tokens */
    for (n=0; n<MAXTOKS; n++) tok[n] = NULL;
    n = 0;

    /* Truncate s at start of comment */
    c = strchr(s,';');
    if (c) *c = '\0';
    len = strlen(s);

    /* Scan s for tokens until nothing left */
    while (len > 0 && n < MAXTOKS)
    {
        m = strcspn(s,SEPSTR);         /* Find token length */
        len -= m+1;                    /* Update length of s */
        if (m == 0) s++;               /* No token found */
        else
        {
            if (*s == '"')               /* Token begins with quote */
            {
                s++;                       /* Start token after quote */
                m = strcspn(s,"\"\n\r");   /* Find end quote (or EOL) */
            }
            s[m] = '\0';                 /* Null-terminate the token */
            tok[n] = s;                  /* Save pointer to token */
            n++;                         /* Update token count */
            s += m+1;                    /* Begin next token */
        }
    }
    return(n);
}                        /* End of TEVAgettokens */



int  TEVAfindmatch(char *line, char *keyword[])
/*
**--------------------------------------------------------------
**  Input:   *line      = line from input file
**           *keyword[] = list of NULL terminated keywords
**  Output:  returns index of matching keyword or
**           -1 if no match found
**  Purpose: determines which keyword appears on input line
**--------------------------------------------------------------
*/
{
    int i = 0;
    while (keyword[i] != NULL)
    {
        if (TEVAmatch(line,keyword[i])) return(i);
        i++;
    }
    return(-1);
}                        /* end of TEVAfindmatch */



int  TEVAmatch(char *str, char *substr)
/*
**--------------------------------------------------------------
**  Input:   *str    = string being searched
**           *substr = substring being searched for
**  Output:  returns 1 if substr found in str, 0 if not
**  Purpose: sees if substr matches any part of str
**
**      (Not case sensitive)
**--------------------------------------------------------------
*/
{
    int i,j;

    /*** Updated 9/7/00 ***/
    /* Fail if substring is empty */
    if (!substr[0]) return(0);

    /* Skip leading blanks of str. */
    for (i=0; str[i]; i++) {
        if (str[i] != ' ') break;
    }

    /* Check if substr matches remainder of str. */
    for (i=i,j=0; substr[j]; i++,j++) {
        if (!str[i] || UCHAR(str[i]) != UCHAR(substr[j]))
            return(0);
    }
    return(1);
}                        /* end of TEVAmatch */


void GetLinkData(PNetInfo net, PLinkInfo links, int time)
/*
**-----------------------------------------------------------
**  Input:   net = network infoe
**           links = link data structure
**           time = the current timestep
**  Output:  None
**  Purpose: Loads links with hydraulic information
**-----------------------------------------------------------
*/
{
    int i;

    for (i = 1; i <= net->nlinks; i++) {                                      /* Store node values in nodedata */
        ENCHECK( ENgetlinkvalue(i, EN_FLOW, &links[i-1].flow[time]) );
        ENCHECK( ENgetlinkvalue(i, EN_VELOCITY, &links[i-1].velocity[time]) );
    }
}


void  GetDemandData(PNetInfo net, PNodeInfo nodes, int time)
/*
**-----------------------------------------------------------
**  Input:   nodedatatype = integer Epanet node data category
**           simout = pointer to output stream
**           nodedata = pointer to nnodes+1 floats
**  Output:  None
**  Purpose: Writes node data for all nodes to unformatted
**           output stream.
**-----------------------------------------------------------
*/
{
    int i;

    for (i = 1; i <= net->nnodes; i++) {                                      /* Store node values in nodedata */
        ENCHECK( ENgetnodevalue(i, EN_DEMAND, &nodes[i-1].q[time]) );
    }
}


void  GetNodeData(int nodedatatype, int nnodes, float *nodedata)
/*
**-----------------------------------------------------------
**  Input:   nodedatatype = integer Epanet node data category
**           nodedata = pointer to nnodes+1 floats
**  Output:  None
**  Purpose: Stores node data for all nodes in nodedata
**-----------------------------------------------------------
*/
{
    int i;

    for (i = 0; i < nnodes; i++) {                                      /* Store node values in nodedata */
        ENCHECK( ENgetnodevalue(i+1, nodedatatype, &nodedata[i]) );
    }
}


void  WriteTSI(int nnodes, PSource sources, FILE *simgen, FILE *simin)
/*
**--------------------------------------------------------------
**  Input:   simin = pointer to simulation input stream
**           simgen = pointer to simulation generator stream
**  Output:
**           Returns error code, or 0 if successful
**  Purpose: Processes simulation generator file to create
**           threat simulation input (TSI) file.
**--------------------------------------------------------------
*/
{
    char line[MAXLINE+1],     /* Line from input data file       */
        wline[MAXLINE+1];     /* Working copy of input line      */
    int sourcetype,           /* source type                     */
        ntokens = 0,          /* # tokens in input line          */
        tokspersource = 4,    /* # tokens to describe one source */
        numsources;           /* number of sources               */
    float sourcestrength;     /* source strength                 */
    long starttime,           /* start time                      */
        stoptime;             /* stop time                       */
    char *tok[MAXTOKS];       /* Array of token strings          */

    /* Read a line from simulation generator input file. */
    while (fgets(line,MAXLINE,simgen) != NULL)
    {

        /* Make copy of line and scan for tokens */
        strcpy(wline,line);
        ntokens = TEVAgettokens(wline,tok);

        /* Skip blank lines and comments */
        if (ntokens == 0) continue;
        if (*tok[0] == ';') continue;

        /* Check if max. length exceeded */
        if (strlen(line) >= MAXLINE) nrerror("TSG file error");

        /* Otherwise process input line */
        numsources = ntokens - 4;                                                  /* number of sources         */
        if (numsources > MAXSOURCES || numsources < 1) nrerror("TSG file error");
        if ( TEVAmatch(tok[ntokens-4],"CONCEN") ) {sourcetype=EN_CONCEN;}
        else if ( TEVAmatch(tok[ntokens-4],"SETPOINT") ) {sourcetype=EN_SETPOINT;}
        else if ( TEVAmatch(tok[ntokens-4],"FLOWPACED") ) {sourcetype=EN_FLOWPACED;}
        else if ( TEVAmatch(tok[ntokens-4],"MASS") ) {sourcetype=EN_MASS;}
        else nrerror("TSG file error");
        if ( !TEVAgetfloat(tok[ntokens-3],&sourcestrength) ) nrerror("TSG file error");/* source strength            */
        if ( !getlong(tok[ntokens-2],&starttime) )  nrerror("TSG file error");     /* source start time          */
        if ( !getlong(tok[ntokens-1],&stoptime) ) nrerror("TSG file error");       /* source stop time           */

        ExpandGen(nnodes, sourcetype, sourcestrength, starttime, stoptime, numsources, 0, tok, sources, simin);

    }   /* End of while */

    rewind(simin);
}   /* End of WriteTSI  */



void ExpandGen(int nnodes, int sourcetype, float sourcestrength, long starttime, 
               long stoptime, int numsources, int is, char **nodetok, PSource sources, 
               FILE *simin)
/*
**--------------------------------------------------------------
**  Purpose: Recursive processing of TSG file source node tokens
**--------------------------------------------------------------
*/
{
    int i, itok, nodetype;
    float demand;

    if (is < numsources-1) {  /* Working down the stack of node tokens */

        if ( TEVAmatch(nodetok[is],"ALL") ) {
            for (i=1; i<=nnodes; i++) {
                ENCHECK(ENgetnodeid(i,sources[is].SourceNodeID) );
                ENCHECK(ENgetnodetype(i,&nodetype) );
                if (nodetype == EN_JUNCTION) {
                    ExpandGen(nnodes, sourcetype, sourcestrength, starttime, stoptime, numsources, is+1, nodetok, sources, simin);
                }
            }
        }
        else if ( TEVAmatch(nodetok[is],"NZD") ) {
            for (i=1; i<=nnodes; i++) {
                ENCHECK(ENgetnodeid(i,sources[is].SourceNodeID) );
                ENCHECK(ENgetnodetype(i,&nodetype) );
                ENCHECK(ENgetnodevalue(i,EN_BASEDEMAND,&demand) );
                if (nodetype == EN_JUNCTION && demand > 0.0) {
                    ExpandGen(nnodes, sourcetype, sourcestrength, starttime, stoptime, numsources, is+1, nodetok, sources, simin);
                }
            }
        }
        else {
            ENCHECK(ENgetnodeindex(nodetok[is],&i));
            strcpy(sources[is].SourceNodeID,nodetok[is]);
            ExpandGen(nnodes, sourcetype, sourcestrength, starttime, stoptime, numsources, is+1, nodetok, sources, simin);
        }
    }

    else {  /* Last node token */

        if ( TEVAmatch(nodetok[is],"ALL") ) {
            for (i=1; i<=nnodes; i++) {
                ENCHECK(ENgetnodeid(i,sources[is].SourceNodeID) );
                ENCHECK(ENgetnodetype(i,&nodetype) );
                if (nodetype == EN_JUNCTION) {
                    for (itok=0; itok < numsources; itok++) {
                        FILEWRITE(fprintf(simin," %-15s %-15d %-15e %-15d %-15d",sources[itok].SourceNodeID,sourcetype,sourcestrength,starttime,stoptime),0);
                    }
                    fprintf(simin,"\n");
                }
            }
        }
        else if ( TEVAmatch(nodetok[is],"NZD") ) {
            for (i=1; i<=nnodes; i++) {
                ENCHECK(ENgetnodeid(i,sources[is].SourceNodeID) );
                ENCHECK(ENgetnodetype(i,&nodetype) );
                ENCHECK(ENgetnodevalue(i,EN_BASEDEMAND,&demand) );
                if (nodetype == EN_JUNCTION && demand > 0.0) {
                    for (itok=0; itok < numsources; itok++) {
                        FILEWRITE(fprintf(simin," %-15s %-15d %-15e %-15d %-15d",sources[itok].SourceNodeID,sourcetype,sourcestrength,starttime,stoptime),0);
                    }
                    fprintf(simin,"\n");
                }
            }
        }
        else {
            ENCHECK(ENgetnodeindex(nodetok[is],&i));
            strcpy(sources[is].SourceNodeID,nodetok[is]);
            for (itok=0; itok < numsources; itok++) {
                FILEWRITE(fprintf(simin," %-15s %-15d %-15e %-15d %-15d",sources[itok].SourceNodeID,sourcetype,sourcestrength,starttime,stoptime),0);
            }
            fprintf(simin,"\n");
        }
    }
}  /* End of ExpandGen */



void epaneterror(int errnum)
/*
**-----------------------------------------------------------
**  Input:   errnum = epanet error number
**  Purpose: Epanet error handler
**-----------------------------------------------------------
*/
{
      char errmsg[MAXERRMSG];

    ENgeterror(errnum,errmsg,MAXERRMSG);
    fprintf(stderr,"EPAnet run-time error...\n");
    fprintf(stderr,"%s\n",errmsg);
    fprintf(stderr,"...now exiting to system...\n");
    exit(1);
}



int  TEVAgetfloat(char *s, float *y)
/*
**-----------------------------------------------------------
**  Input:   *s = character string
**  Output:  *y = floating point number
**           returns 1 if conversion successful, 0 if not
**  Purpose: converts string to floating point number
**-----------------------------------------------------------
*/
{
    char *endptr;
    *y = (float) strtod(s,&endptr);
    if (*endptr > 0) return(0);
    return(1);
}



int  getlong(char *s, long *y)
/*
**-----------------------------------------------------------
**  Input:   *s = character string
**  Output:  *y = long int number
**           returns 1 if conversion successful, 0 if not
**  Purpose: converts string to long integer (base 10)
**-----------------------------------------------------------
*/
{
    char *endptr;
    *y = strtol(s,&endptr,10);
    if (*endptr > 0) return(0);
    return(1);
}


int Zeros(float *x,int i1, int i2)
/*
**-----------------------------------------------------------
**  Input:   *x = float array; i1 = start index; i2 = end index
**  Output:  1 if x[i1...i2]=0, 0 otherwise.
**  Purpose: Identifies zero-valued subarrays
**-----------------------------------------------------------
*/
{
  int i;
  for(i=i1; i<=i2; i++)
  {
    if(x[i] != 0) return(0);
  }
  return(1);
}

char *TEVAclocktime(char *atime, long time)
{
    int h, m;
    h = time/3600;
    m = (time%3600)/60;
    sprintf(atime, "%01d:%02d", h, m);
    return(atime);
}
