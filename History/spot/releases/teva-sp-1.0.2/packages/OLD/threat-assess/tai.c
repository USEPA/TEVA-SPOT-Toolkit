#include <stdlib.h>
//#include <ctype.h>
//#include <stddef.h>
//#include <stdio.h>
#include <string.h>
//#include <math.h>
//#include <time.h>
//#include <limits.h>
//#include <float.h>

#include "nrutil.h"
#include "tai.h"
#include "teva-assess.h"
#include "ta_utils.h"
#include "ta_input.h"
/*
**-------------------------------------------------------------
** Input data file section headers 
**-------------------------------------------------------------
*/

enum secttype 
{
	_DR_A,          /* Sigmoid dose-response coefficient */
	_DR_M,          /* Sigmoid dose-response coefficient */
	_DR_N,          /* Sigmoid dose-response coefficient */
	_DR_TAU,        /* Sigmoid dose-response coefficient */
	_BODYMASS,      /* Body mass (Kg) */
	_NORMALIZE,     /* YES|NO Normalize dose by body mass */
	_LATENCYTIME,   /* Time from exposure to disease symptoms */
	_FATALITYTIME,  /* Time from symptoms to death */
	_FATALITYRATE,  /* Fraction exposed that die */
	_INGESTIONRATE, /* Volumetric ingestion rate */
	_USAGERATE,     /* Per capita usage rate */
	_TSONAME,       /* Name of TSO file */
	_TAONAME,       /* Name of output file */
	_SAPNAME,       /* Name of sensor parameter output file */
	_SAPVARS,       /* Sensor parameters to output.  Current valid values: MeanInfections  MaxInfections*/
	_DOSETYPE,      /* TOTAL (total mass dose) */
	_INGESTIONTYPE, /* DEMAND (ingestion proportional to demand) */
	_MAXLOC,        /* Maximum no. sample locations */
};

char 
    *secttxt[]   = {
    "DR:A",            /* dose-response 'a' coefficient */
    "DR:M",            /* dose-response 'm' coefficient */
    "DR:N",            /* dose-response 'n' coefficient */
    "DR:TAU",          /* dose-response 'tau' coefficient */
    "BODYMASS",        /* exposed individual body mass (Kg) */
    "NORMALIZE",       /* Response computed using mg/Kg ('yes') or mg ('no') */
    "LATENCYTIME",     /* time from being exposed to exhibiting definitive symptoms (hours) */
    "FATALITYTIME",    /* time from exhibiting symptoms till death or recovery (hours) */
    "FATALITYRATE",    /* Fraction of exposed population that die */
    "INGESTIONRATE",   /* volumetric ingestion rate (liters/day) */
    "USAGERATE",       /* per capita usage rate (flow units/person) */
    "TSONAME",         /* name of TSO file */
    "TAONAME",         /* name of TAO file */
	"SAPNAME",         /* Name of sensor parameter output file */
	"SAPVARS",         /* Sensor parameters to output.  Current valid values: MeanInfections  MaxInfections*/
    "DOSETYPE",        /* 'total' for total ingested mass */
    "INGESTIONTYPE",   /* 'demand' for ingestion probability proportional to node demand */
    "MAXLOC",          /* Maximum no. of locations */
    NULL
};

/*
**-----------------------------------------------------------
**  loadTAIFileData
**  Input:   tai data file name
**  Output:  Pointer to input file data contents (for later
**           processing)
**  Purpose: Loads data file contents into memory for later
**           processing
**-----------------------------------------------------------
*/
PInputFileData loadTAIFileData(const char *fn)
{
	return loadInputFile(fn,secttxt);
}
/*
**-----------------------------------------------------------
**  processSensorSetData
**  Input:   Pointer to input file data contents
**  Output:  Updated assessment parameters based on data
**           file contents
**  Purpose: Processes TAI data and updates parameters
**-----------------------------------------------------------
*/
void processTAIData(PInputFileData taiIFD, PMem mem)
{
    int   
        i;
    int   
        sect = -1,              /* Data sections */
        ntokens = 0;            /* # tokens in input line */
    float 
        x;

    /* data model declaration */

	for(i=0;i<taiIFD->num;i++)
	{
		PInputLine il = &taiIFD->inputLines[i];
        /* Process line depending on input keyword */
        int sect = il->idx;
		int ntokens = il->ntokens;
		char **tok=il->tokens;

        switch( sect )
        {
        case _DR_A:
            if( ntokens != 2 ) ta_error(1,"Invalid input DR:A");
            if ( TAgetfloat(tok[1],&x) ) mem->dr->a=x;
            else ta_error(1,"Invalid input DR:A");
            break;
        case _DR_M:
            if( ntokens != 2 ) ta_error(1,"Invalid input DR:M");
            if ( TAgetfloat(tok[1],&x) ) mem->dr->m=x;
            else ta_error(1,"Invalid input DR:M");
            break;
        case _DR_N:
            if( ntokens != 2 ) ta_error(1,"Invalid input DR:N");
            if ( TAgetfloat(tok[1],&x) ) mem->dr->n=x;
            else ta_error(1,"Invalid input DR:N");
            break;
        case _DR_TAU:
            if( ntokens != 2 ) ta_error(1,"Invalid input DR:TAU");
            if ( TAgetfloat(tok[1],&x) ) mem->dr->tau=x;
            else ta_error(1,"Invalid input DR:TAU");
            break;
        case _BODYMASS:
            if( ntokens != 2 ) ta_error(1,"Invalid input BODYMASS");
            if ( TAgetfloat(tok[1],&x) ) mem->dr->bodymass=x;
            else ta_error(1,"Invalid input BODYMASS");
            break;
        case _NORMALIZE:
            if( ntokens != 2 ) ta_error(1,"Invalid input NORMALIZE");
            if(!_stricmp(tok[1],"YES"))
                mem->dr->normalize = 1;
            else if(!_stricmp(tok[1],"NO"))
                mem->dr->normalize = 0;
            else ta_error(1,"Invalid input NORMALIZE");
            break;
        case _LATENCYTIME:
            if( ntokens != 2 ) ta_error(1,"Invalid input LATENCYTIME");
            if ( TAgetfloat(tok[1],&x) ) mem->dr->nL=ROUND(x/mem->net->info->stepsize);
            else ta_error(1,"Invalid input LATENCYTIME");
            break;
        case _FATALITYTIME:
            if( ntokens != 2 ) ta_error(1,"Invalid input FATALITYTIME");
            if ( TAgetfloat(tok[1],&x) ) mem->dr->nF=ROUND(x/mem->net->info->stepsize);
            else ta_error(1,"Invalid input FATALITYTIME");
            break;
        case _INGESTIONRATE:
            if( ntokens != 2 ) ta_error(1,"Invalid input INGESTIONRATE");
            if ( TAgetfloat(tok[1],&x) ) mem->dr->phi=(float)(x/24.);
            else ta_error(1,"Invalid input INGESTIONRATE");
            break;
        case _FATALITYRATE:
            if( ntokens != 2 ) ta_error(1,"Invalid input FATALITYRATE");
            if ( TAgetfloat(tok[1],&x) ) mem->dr->frate=x;
            else ta_error(1,"Invalid input FATALITYRATE");
            break;
        case _USAGERATE:
            if( ntokens != 2 ) ta_error(1,"Invalid input USAGERATE");
            if ( TAgetfloat(tok[1],&x) ) mem->dr->pcu=x;
            else ta_error(1,"Invalid input USAGERATE");
            break;
        case _DOSETYPE:
            if( ntokens != 2 ) ta_error(1,"Invalid input DOSETYPE");
            if(!_stricmp(tok[1],"TOTAL"))
                mem->dr->dtype = totalmass;
            else ta_error(1,"Invalid input DOSETYPE");
            break;
        case _INGESTIONTYPE:
            if( ntokens != 2 ) ta_error(1,"Invalid input INGESTIONTYPE");
            if(!_stricmp(tok[1],"DEMAND"))
                mem->dr->itype = demand;
            else ta_error(1,"Invalid input INGESTIONTYPE");
            break;
        case _TSONAME:
//            if( ntokens != 2 ) ta_error(1,"Invalid input TSONAME");
//            strcpy(io->tsoname,tok[1]);
            break;
        case _TAONAME:
            if( ntokens != 2 ) ta_error(1,"Invalid input TAONAME");
            strcpy(mem->io->taoname,tok[1]);
            break;
        case _SAPNAME:
            if( ntokens != 2 ) ta_error(1,"Invalid input SPONAME");
            strcpy(mem->sap->fn,tok[1]);
            break;
        case _SAPVARS:
			{
				int n;
	            if( ntokens < 2 ) ta_error(1,"Invalid input SPOVARS");
				for(n=1;n<ntokens;n++) {
					if(_stricmp(tok[n],"MeanInfections")==0)
						mem->sap->meanInfections=1;
					else if(_stricmp(tok[n],"MaxInfections")==0)
						mem->sap->maxInfections=1;
				}
			}
            break;
        default:
            ta_error(1,"Invalid TAI keyword");
        }
    }   /* End of while */

    /* Allocate memory depending on assessment parameters */
    mem->dr->maxsteps = mem->net->info->nsteps + mem->dr->nL + mem->dr->nF;       /* Max number   of time steps for disease response */
    mem->u->dos = vector(0,mem->dr->maxsteps-1);                  /* Basic dose-respone   state variables */
    mem->u->res = vector(0,mem->dr->maxsteps-1);
    mem->u->s = vector(0,mem->dr->maxsteps-1);
    mem->u->i = vector(0,mem->dr->maxsteps-1);
    mem->u->d = vector(0,mem->dr->maxsteps-1);
    mem->u->f = vector(0,mem->dr->maxsteps-1);
    mem->xa->dos = vector(0,mem->dr->maxsteps-1);                 /*  Spatial averages */
    mem->xa->res = vector(0,mem->dr->maxsteps-1);
    mem->xa->s = vector(0,mem->dr->maxsteps-1);
    mem->xa->i = vector(0,mem->dr->maxsteps-1);
    mem->xa->d = vector(0,mem->dr->maxsteps-1);
    mem->xa->f = vector(0,mem->dr->maxsteps-1);
    mem->ta->dos = vector(0,mem->net->info->nnodes-1);                  /* Temporal averages */
    mem->ta->res = vector(0,mem->net->info->nnodes-1);
    mem->ta->s = vector(0,mem->net->info->nnodes-1);
    mem->ta->i = vector(0,mem->net->info->nnodes-1);
    mem->ta->d = vector(0,mem->net->info->nnodes-1);
    mem->ta->f = vector(0,mem->net->info->nnodes-1);
}

