/*
**************************************************************************

MSINPUT.C -- Input data file interpreter for EPANET Multispecies EXtension

VERSION:    1.00
DATE:       4/11/05
AUTHOR:     Feng Shang	University of Cincinnati
			James Uber	University of Cincinnati

This module reads and interprets the input data from file InFile.

The entry points for this module are:
   msnetsize()   -- called from ENopen() in MSEPANET.C
   msallocdata() -- called from ENopen() in MSEPANET.C   
   msgetdata()  --  called from ENopen() in MSEPANET.C

***************************************************************************
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "hash.h"
#include "text.h"
#include "mstext.h"
#include "types.h"
#include "mstypes.h"
#include "funcs.h"
#include "msfuncs.h"
#define  EXTERN  extern
#include "vars.h"
#include "msvars.h"
#include "mathexpr.h"

#define   MAXERRS     10  /* Max. input errors reported        */
#define   MSRELTOL    1.E-3   /* Default numerical integration relative error tolerance */
#define   MSABSTOL    1.E-6   /* Default numerical integration absolute error tolerance */

int    Ntokens,           /* Number of tokens in input line    */
       Ntitle;            /* Number of title lines             */
char   *Tok[MAXTOKS];     /* Array of token strings            */

                          /* Defined in enumstxt.h and msenumstxt.h in EPANET.C */
extern char *SectTxt[];   /* Input section keywords            */
extern char *MSSectTxt[]; 
                          /* Defined in INPUT3.C: */
extern STmplist  *PrevPat;       /* Pointer to pattern list element   */
extern STmplist  *PrevCurve;     /* Pointer to curve list element     */

int  msnetsize()
/*
**--------------------------------------------------------------
**  Input:   none
**  Output:  returns error code
**  Purpose: determines number of system components
**--------------------------------------------------------------
*/
{
   char  line[MAXLINE+1];     /* Line from input data file    */
   char  *tok;                /* First token of line          */
   int   sect,newsect;        /* Input data sections          */
   int   errcode = 0;         /* Error code                   */

/* Initialize network component counts */
   MaxJuncs    = 0;
   MaxTanks    = 0;
   MaxPipes    = 0;
   MaxPumps    = 0;
   MaxValves   = 0;
   MaxControls = 0;
   MaxRules    = 0;
   MaxCurves   = 0;
   MaxSpecies  = 0;
   sect        = -1;

/* Add a default pattern 0 */
   MaxPats = -1;
   addpattern("");

/* Make pass through data file counting number of each component */
   while (fgets(line,MAXLINE,InFile) != NULL)
   {
   /* Skip blank lines & those beginning with a comment */
      tok = strtok(line,SEPSTR);
      if (tok == NULL) continue;
      if (*tok == ';') continue;

   /* Check if line begins with a new section heading */
      if (*tok == '[')
      {
         newsect = findmatch(tok,SectTxt);
         if (newsect >= 0)
         {
            sect = newsect;
            if (sect == _END) break;
            continue;
         }
		 else
		 {
			 newsect = findmatch(tok, MSSectTxt);
			 if (newsect >= 0)
			 {
				 sect = newsect + EPANETSECT;
				 continue;
			 }
			 else continue;
		 }
      }

   /* Add to count of current component */
      switch(sect)
      {
            case _JUNCTIONS:  MaxJuncs++;    break;
            case _RESERVOIRS:
            case _TANKS:      MaxTanks++;    break;
            case _PIPES:      MaxPipes++;    break;
            case _PUMPS:      MaxPumps++;    break;
            case _VALVES:     MaxValves++;   break;
            case _CONTROLS:   MaxControls++; break;
            case _RULES:      addrule(tok);  break; /* See RULES.C */ 
			case _MSVARIABLES: mscount(tok); break;
            case _PATTERNS:   errcode = addpattern(tok);
                              break;
            case _CURVES:     errcode = addcurve(tok);
                              break;
      }
      if (errcode) break;
   }

   MaxNodes = MaxJuncs + MaxTanks;
   MaxLinks = MaxPipes + MaxPumps + MaxValves;
   if (MaxPats < 1) MaxPats = 1;
   if (!errcode)
   {
      if (MaxJuncs < 1) errcode = 223;       /* Not enough nodes */
      else if (MaxTanks == 0) errcode = 224; /* No tanks */
   }
   return(errcode);
}                        /*  End of netsize  */

int  msallocdata()
/*----------------------------------------------------------------
**  Input:   none
**  Output:  none
**  Returns: error code
**  Purpose: allocates memory for multispecies allication
**----------------------------------------------------------------
*/
{
   int i, j, n;
   int errcode = 0;
   float * data;

   /* Allocate memory for reaction parameters */
   n = MaxLinks + 1;
   LinkParameters = (float **) calloc(n, sizeof(float *));
   ERRCODE(MEMCHECK(LinkParameters));
   for ( i = 1; i < n; i++)
   {
	   data = (float *)calloc((MaxParameters+1), sizeof(float));
	   ERRCODE(MEMCHECK(data));
	   LinkParameters[i] = data;
	   for ( j = 1; j < MaxParameters+1; j++)
		   LinkParameters[i][j] = MISSING;				
   }   
 
   n = MaxTanks + 1;
   TankParameters = (float **) calloc(n, sizeof(float *));
   ERRCODE(MEMCHECK(TankParameters));
   for ( i = 1; i < n; i++)
   {
	   data = (float *)calloc((MaxParameters+1), sizeof(float));
	   ERRCODE(MEMCHECK(data));
	   TankParameters[i] = data;
	   for ( j = 1; j < MaxParameters+1; j++)
		   TankParameters[i][j] = MISSING;				
   }
 
/* Allocate memory for multispecies variables*/
   if (!errcode)
   {
	   Specie = (Sspecie *)  calloc(MaxSpecies+1, sizeof(Sspecie));
	   BulkSpecie = (Sspecie *)  calloc(MaxSpecies+1, sizeof(Sspecie));
	   PipeSurfaceSpecie = (Sspecie *)  calloc(MaxSpecies+1, sizeof(Sspecie));
	   TankSurfaceSpecie = (Sspecie *)  calloc(MaxSpecies+1, sizeof(Sspecie));
	   PipeSpecie = (Sspecie *)  calloc(MaxSpecies+1, sizeof(Sspecie));
	   TankSpecie = (Sspecie *)  calloc(MaxSpecies+1, sizeof(Sspecie));
	   StoPS = (int *) calloc(MaxSpecies+1, sizeof(int));
	   StoTS = (int *) calloc(MaxSpecies+1, sizeof(int));
	   PStoS = (int *) calloc(MaxSpecies+1, sizeof(int));
	   TStoS = (int *) calloc(MaxSpecies+1, sizeof(int));	   
	   Parameter = (Spara *) calloc(MaxParameters+1, sizeof(Spara));
	   Coefficient = (Scoeff *) calloc(MaxCoefficients+1, sizeof(Scoeff));
	   Constant = (Sconst *) calloc(MaxConstants+1, sizeof(Sconst));
	   PipeRateList = (SExprlist *)calloc(MaxSpecies+1, sizeof(SExprlist));
	   PipeDefList = (SExprlist *)calloc(MaxSpecies+1, sizeof(SExprlist));
	   PipeEquList = (SExprlist *)calloc(MaxSpecies+1, sizeof(SExprlist));
	   TankRateList = (SExprlist *)calloc(MaxSpecies+1, sizeof(SExprlist));
	   TankDefList = (SExprlist *)calloc(MaxSpecies+1, sizeof(SExprlist));
	   TankEquList = (SExprlist *)calloc(MaxSpecies+1, sizeof(SExprlist));
       PSabstol = (float *)calloc(MaxSpecies+1, sizeof(float));
       TSabstol = (float *)calloc(MaxSpecies+1, sizeof(float));
	   ERRCODE(MEMCHECK(Specie));
	   ERRCODE(MEMCHECK(BulkSpecie));
	   ERRCODE(MEMCHECK(PipeSurfaceSpecie));
	   ERRCODE(MEMCHECK(TankSurfaceSpecie));
	   ERRCODE(MEMCHECK(PipeSpecie));
	   ERRCODE(MEMCHECK(TankSpecie));
	   ERRCODE(MEMCHECK(StoPS));
	   ERRCODE(MEMCHECK(StoTS));
	   ERRCODE(MEMCHECK(PStoS));
	   ERRCODE(MEMCHECK(TStoS));
 	   ERRCODE(MEMCHECK(Parameter));
	   ERRCODE(MEMCHECK(Constant));	  
	   ERRCODE(MEMCHECK(PipeRateList));
	   ERRCODE(MEMCHECK(PipeDefList));
	   ERRCODE(MEMCHECK(PipeEquList));
	   ERRCODE(MEMCHECK(TankRateList));
	   ERRCODE(MEMCHECK(TankDefList));
	   ERRCODE(MEMCHECK(TankEquList));	   
       ERRCODE(MEMCHECK(PSabstol));
       ERRCODE(MEMCHECK(TSabstol));
   }

/* Initialize pointers used for multi initial and sourcequality */
   if (!errcode)
   {
	   n = MaxNodes+1;
	   NodeSource = (Pmsource *)malloc(n*sizeof(Pmsource));
	   ERRCODE(MEMCHECK(NodeSource));
	   for (i = 1; i < n; i++)
		   NodeSource[i] = NULL;
	   NodeInitial = (Pminitial *)malloc(n*sizeof(Pminitial));
	   ERRCODE(MEMCHECK(NodeInitial));
	   for (i = 1; i < n; i++)
		   NodeInitial[i] = NULL;
   }

   return(errcode);
}                                       /* End of msallocdata */

int  msgetdata()
/*
**----------------------------------------------------------------
**  Input:   none
**  Output:  returns error code
**  Purpose: reads in network data from disk file
**----------------------------------------------------------------
*/
{
   int errcode = 0;
   setdefaults();                /* Assign default data values     */
   mssetdefaults();

   initreport();                 /* Initialize reporting options   */
   rewind(InFile);               /* Rewind input file              */
   ERRCODE(msreaddata());        /* Read in network data           */
   if (!errcode) adjustdata();   /* Adjust data for default values */
   if (Nspecies > 0)
	   Qualflag = MULTIS;
   if (!errcode) 
   {
	   initunits();    /* Initialize units on input data */
	   msinitunits();
   }
   ERRCODE(inittanks());         /* Initialize tank volumes        */
   if (!errcode) convertunits(); /* Convert units on input data    */
   if (Qualflag == MULTIS)
	   msconvertunits();			 /* Convert initial quality unit */
   return(errcode);
}                       /*  End of getdata  */

void  mssetdefaults()
/*
**----------------------------------------------------------------
**  Input:   none
**  Output:  none
**  Purpose: assigns default values to global variables for 
             multispecies applications
**----------------------------------------------------------------
*/
{
    int i;

    MSVolumeFlag = LITER;		 /* Reaction expression volume unit*/
    MSTimeFlag = SECOND;		 /* Reaction expression time unit  */
    MSreltol  = MSRELTOL;        /* MS num. integration relative error tolerance */
    for(i=1;i<MaxSpecies+1;i++) { /* MS num. integration absolute error tolerance */
        BulkSpecie[i].abstol = MSABSTOL;
        PipeSurfaceSpecie[i].abstol = MSABSTOL;
        TankSurfaceSpecie[i].abstol = MSABSTOL;
    }
}                       /*  End of mssetdefaults  */


int  msreaddata()
/*
**--------------------------------------------------------------
**  Input:   none
**  Output:  returns error code
**  Purpose: reads contents of input data file
**--------------------------------------------------------------
*/
{
   char  line[MAXLINE+1],     /* Line from input data file       */
         wline[MAXLINE+1];    /* Working copy of input line      */
   int   sect,newsect,        /* Data sections                   */
         errcode = 0,         /* Error code                      */
         inperr,errsum;       /* Error code & total error count  */

/* Allocate input buffer */
   X = (float *) calloc(MAXTOKS, sizeof(float));
   ERRCODE(MEMCHECK(X));

   if (!errcode)
   {

   /* Initialize number of network components */
      Ntitle    = 0;
      Nnodes    = 0;
      Njuncs    = 0;
      Ntanks    = 0;
      Nlinks    = 0;
      Npipes    = 0;
      Npumps    = 0;
      Nvalves   = 0;
      Ncontrols = 0;
      Nrules    = 0;

	  Nspecies = 0;
	  Nbulkspecies = 0;
	  Npipesurfacespecies = 0;
	  Ntanksurfacespecies = 0;
      Nparameters = 0;
	  Ncoefficients = 0;
	  Nconstants = 0;

	  Npiperate = 0;
	  Npipedef = 0;
	  Npipeequ = 0;
	  Ntankrate = 0;
	  Ntankdef = 0;
	  Ntankequ = 0;
		
	  Ncurves   = MaxCurves;
      Npats     = MaxPats;
      PrevPat   = NULL;
      PrevCurve = NULL;
      sect      = -1;
      errsum    = 0;

   /* Read each line from input file. */
      while (fgets(line,MAXLINE,InFile) != NULL)
      {

      /* Make copy of line and scan for tokens */
         strcpy(wline,line);
         Ntokens = gettokens(wline);

       /* Skip blank lines and comments */
         if (Ntokens == 0) continue;
         if (*Tok[0] == ';') continue;

      /* Check if max. length exceeded */
         if (strlen(line) >= MAXLINE)
         {
            sprintf(Msg,ERR214);
            writeline(Msg);
            writeline(line);
            errsum++;
         }

      /* Check if at start of a new input section */
         if (*Tok[0] == '[')
         {
            newsect = findmatch(Tok[0],SectTxt);
            if (newsect >= 0)
            {
               sect = newsect;
               if (sect == _END) break;
               continue;
            }
			else
			{
				newsect = findmatch(Tok[0], MSSectTxt);
			    if (newsect >= 0)
				{
					sect = newsect + EPANETSECT;
					continue;
				}            
				else
				{
					inperrmsg(201,sect,line);
					errsum++;
					break;
				}
			}
         }
      /* Otherwise process next line of input in current section */
         else
         {
            inperr = msnewline(sect,line);
            if (inperr > 0)
            {
               inperrmsg(inperr,sect,line);
               errsum++;
            }
         }

      /* Stop if reach end of file or max. error count */
         if (errsum == MAXERRS) break;
      }   /* End of while */

   /* Check for errors */
      if (errsum > 0)  errcode = 200;
   }

/* Check for unlinked nodes */
   if (!errcode) errcode = unlinked();
/* Get pattern & curve data from temp. lists */
   if (!errcode) errcode = getpatterns();
   if (!errcode) errcode = getcurves();
   if (!errcode) errcode = getpumpparams();
   if (!errcode) msspeciesindex();
   if (!errcode) errcode = parseDAsystem();
   if (!errcode) assignpara();
/* Free input buffer */
   free(X);
   return(errcode);

}                        /*  End of msreaddata  */


int  msnewline(int sect, char *line)
/*
**--------------------------------------------------------------
**  Input:   sect  = current section of input file
**           *line = line read from input file
**  Output:  returns error code or 0 if no error found
**  Purpose: processes a new line of data from input file
**--------------------------------------------------------------
*/
{
   int n;
   switch (sect)
   {
       case _TITLE:       if (Ntitle < 3)
                          {
                             n = strlen(line);
                             if (line[n-1] == 10) line[n-1] = ' ';
                             strncpy(Title[Ntitle],line,MAXMSG);
                             Ntitle++;
                          }
                          return(0);
       case _JUNCTIONS:   return(juncdata());
       case _RESERVOIRS:
       case _TANKS:       return(tankdata());
       case _PIPES:       return(pipedata());
       case _PUMPS:       return(pumpdata());
       case _VALVES:      return(valvedata());
       case _PATTERNS:    return(patterndata());
       case _CURVES:      return(curvedata());
       case _DEMANDS:     return(demanddata());
       case _CONTROLS:    return(controldata());
       case _RULES:       return(ruledata());   /* See RULES.C */
       case _SOURCES:     return(mssourcedata());
       case _EMITTERS:    return(emitterdata());
       case _QUALITY:     return(msqualdata());

	   case _MSVARIABLES: return(msvariabledata());
	   case _MSPIPE:	  return(mspipereactdata());
	   case _MSTANK:      return(mstankreactdata());
       
	   case _STATUS:      return(statusdata());
       case _ROUGHNESS:   return(0);
       case _ENERGY:      return(energydata());
       case _REACTIONS:	  return(msreactdata());
       case _MIXING:      return(mixingdata());
       case _REPORT:      return(reportdata());
       case _TIMES:       return(timedata());
       case _OPTIONS:     return(optiondata());
       case _MSOPTIONS:   return(msoptiondata());

   /* Data in these sections are not used for any computations */
       case _COORDS:      return(0);
       case _LABELS:      return(0);
       case _TAGS:        return(0);
       case _VERTICES:    return(0);
       case _BACKDROP:    return(0);
   }
   return(201);

}                        /* end of msnewline */

int  mssourcedata()
/*
**--------------------------------------------------------------
**  Input:   none                                                
**  Output:  returns error code                                  
**  Purpose: processes water quality source data                   
**  Formats:                                                     
**     [SOURCE]                                                
**        node  sourcetype  quality  specie (pattern)               
**                                                              
**  NOTE: units of mass-based source are mass/min                
**--------------------------------------------------------------
*/
{
   int   j,                  /* Node index    */
		 k,					 /* specie index  */
         n,                  /* # data items  */
         p = 0;              /* Time pattern  */
   char  type = CONCEN;      /* Source type   */
   float c0 = 0;             /* Init. quality */
   STmplist *pat;
   Pmsource mssource;
   n = Ntokens;
   
   if (n < 4) return(201);
   if ((j = findnode(Tok[0])) == 0) return(203);
   if      (match(Tok[1],w_CONCEN))    type = CONCEN;
   else if (match(Tok[1],w_MASS))      type = MASS;
   else if (match(Tok[1],w_SETPOINT))  type = SETPOINT;
   else if (match(Tok[1],w_FLOWPACED)) type = FLOWPACED;
   if (!getfloat(Tok[2],&c0)) return(202);      /* Illegal WQ value */
   if ((k=findspecieID(BulkSpecie, Tok[3], Nbulkspecies)) == 0) return (0);
   if (n > 4)
   {
	   pat = findID(Tok[4],Patlist);
	   if (pat == NULL) return(205);            /* Illegal pattern. */
	   p = pat->i;
   }   
   
   mssource = (struct Smsource *) malloc(sizeof(struct Smsource));
   if (mssource == NULL) return(101);
   mssource->C0 = c0;
   mssource->Pat = p;
   mssource->Type = type;
   mssource->index = k;
   mssource->next = NodeSource[j];
   NodeSource[j] = mssource;
   return(0);		
}                        /* end of mssourcedata */

int  msqualdata()
/*
**--------------------------------------------------------------
**  Input:   none                                                
**  Output:  returns error code                                  
**  Purpose: processes initial water quality data                
**  Formats:
**     [QUALITY]                                               
**		  node   specieID initqual
**--------------------------------------------------------------
*/
{
   int   j,k,n;
   float c0;
   Pminitial init; 

   if (Nnodes == 0) return(208);        /* No nodes defined yet */
   n = Ntokens;
   if (n < 3) 
	   return (0);
   else
   {
	   if ( (j = findnode(Tok[0])) == 0) return(0);
	   k = findspecieID(BulkSpecie, Tok[1], Nbulkspecies);		   
	   if ( k == 0)
	   {
		   if ( j > Njuncs && Tank[j-Njuncs].A > 0)
		   {
			   k = findspecieID(TankSurfaceSpecie, Tok[1], Ntanksurfacespecies);
			   if ( k == 0) return (0);
			   k += Nbulkspecies+Npipesurfacespecies; 
		   }
		   else
		   {
			   k = findspecieID(PipeSurfaceSpecie, Tok[1], Npipesurfacespecies);
			   if ( k == 0) return (0);
			   else k += Nbulkspecies;
		   }
	   }
	   if (!getfloat(Tok[2],&c0)) return(209);
	   init = (Pminitial)malloc(sizeof(struct Sminitial));	 
	   init->C0 = c0;
	   init->index = k;
	   init->next = NodeInitial[j];
	   NodeInitial[j] = init;
	   return (0);
   }    
}                        /* end of msqualdata */

int  msvariabledata()
/*
**--------------------------------------------------------------
**  Input:   none                                                
**  Output:  returns error code                                  
**  Purpose: processes multi species variable data                             
**  Format:                                                      
**    [MSVARIABLES]                                              
**      BULK SpecieID
		PIPE|TANK SpecieID
		PARAMETER ParameterID
		CONSTANT  ConstantID
**--------------------------------------------------------------
*/
{
	int      n, errcode;
	float    value = 0.0;
		
	n = Ntokens;
	if (n < 2) return (0);

	if (match(Tok[0],w_BULK))           /* Bulk Species */
	{
		errcode = addspecieID(BULK,Tok[1], VOLUMEUNIT);	
		return (errcode);
	}
  
	if (match(Tok[0], w_PIPE))
	{
		if ( n == 3 && match(Tok[2],w_AREA))
			errcode = addspecieID(PIPESURFACE,Tok[1], AREAUNIT);
		else
			errcode = addspecieID(PIPESURFACE,Tok[1], VOLUMEUNIT);
		return (errcode);
	}
	if (match(Tok[0], w_TANK))
	{
		if ( n == 3 && match(Tok[2],w_AREA))
  			errcode = addspecieID(TANKSURFACE, Tok[1], AREAUNIT);
		else
			errcode = addspecieID(TANKSURFACE, Tok[1], VOLUMEUNIT);
		return (errcode);
	}
	if (match(Tok[0], w_PARAMETER))
	{
		errcode = addparameterID(Tok[1]);  
		return (errcode);
	}
	if (match(Tok[0], w_COEFFICIENT))
	{
		if (n > 2)
		{
			if(!getfloat(Tok[2],&value)) return (252);
		}
		errcode = addcoefficientID(Tok[1], value);
		return (errcode);
	}
	if (match(Tok[0], w_CONSTANT))
	{
		if (n > 2)
		{
			if(!getfloat(Tok[2],&value)) return (252);
		}
		errcode = addconstantID(Tok[1], value);
		return (errcode);
	}
	if (match(Tok[0], w_UNITS))
	{
		if ( n < 3) return (0);
		else if (match(Tok[1], w_VOLUME))
		{
			if (match(Tok[2], w_CUBICFOOT)) MSVolumeFlag = CUBICFOOT;
			else if (match(Tok[2], w_LITER)) MSVolumeFlag = LITER;
			else if (match(Tok[2], w_CUBICMETER)) MSVolumeFlag = CUBICMETER;
		}
		else if (match(Tok[1], w_TIME))
		{
			if (match(Tok[2], w_SECOND)) MSTimeFlag = SECOND;
			else if (match(Tok[2], w_MINUTE)) MSTimeFlag = MINUTE;
			else if (match(Tok[2], w_HOUR)) MSTimeFlag = HOUR;
			else if (match(Tok[2], w_DAY)) MSTimeFlag = DAY;		
		}	
	}
   return(0);
}                        /* end of MS variable data */


int  mspipereactdata()
/*
**--------------------------------------------------------------
**  Input:   none                                                
**  Output:  returns error code                                  
**  Purpose: processes multi species pipe reaction data                             
**  Format:                                                      
**    [MSPIPE]                                              
**      RATE SpecieID expression
		DEFINITION SpecieID expression
		EQUATION expression
**--------------------------------------------------------------
*/
{
	int      n, errcode;		
	n = Ntokens;
	if (n < 2) return (0);

	if (match(Tok[0],w_RATE))           /* Bulk Species */
	{
		errcode = addpipeexpr(RATE);
		return (errcode);
	}
	if (match(Tok[0], w_DEF))
	{
		errcode = addpipeexpr(DEF);	  
		return (errcode);
	}
	if (match(Tok[0], w_EQU))
	{
  		errcode = addpipeexpr(EQU);	
		return (errcode);
	}
	return (0);
}

int  mstankreactdata()
/*
**--------------------------------------------------------------
**  Input:   none                                                
**  Output:  returns error code                                  
**  Purpose: processes multi species tank reaction data                             
**  Format:                                                      
**    [MSTANK]                                              
**      RATE SpecieID expression
		DEFINITION SpecieID expression
		EQUATION expression
**--------------------------------------------------------------
*/
{
	int      n, errcode;		
	n = Ntokens;
	if (n < 2) return (0);

	if (match(Tok[0],w_RATE))           /* Bulk Species */
	{
		errcode = addtankexpr(RATE);	
		return (errcode);
	}
	if (match(Tok[0], w_DEF))
	{
		errcode = addtankexpr(DEF);	  
		return (errcode);
	}
	if (match(Tok[0], w_EQU))
	{
  		errcode = addtankexpr(EQU);	
		return (errcode);
	}
	return (0);
}

int addpipeexpr(char type)
/*
**--------------------------------------------------------------
**  Input:   expression type (RATE, DEF, EQU)                                                
**  Output:  returns error code                                  
**  Purpose: processes pipe reacion expression                             
**--------------------------------------------------------------
*/
{
	int index1, index2;
	char expr[MAXLINE] = "";
	int i;

	switch (type)
	{
		case RATE:
		{
			index1 = findspecieID(BulkSpecie, Tok[1], Nbulkspecies);
			index2 = findspecieID(PipeSurfaceSpecie, Tok[1], Npipesurfacespecies);
			if (index1 == 0 && index2 == 0)
				return (253);			/* invalid pipe species ID in [MSPIPE]*/
			if (findpipeexpr(Tok[1])) return (255);	/* expression redefined for species Tok[1] */
			Npiperate++;			
			strcpy(PipeRateList[Npiperate].expression, "");
			for (i = 2; i < Ntokens; i++)
				strcat(PipeRateList[Npiperate].expression, Tok[i]);
			strcpy(PipeRateList[Npiperate].ID, Tok[1]);
			break;
		}
		case DEF:
		{
			index1 = findspecieID(BulkSpecie, Tok[1], Nbulkspecies);
			index2 = findspecieID(PipeSurfaceSpecie, Tok[1], Npipesurfacespecies);
			if (index1 == 0 && index2 == 0)
				return (253);			/* invalid pipe species ID in [MSPIPE]*/	
			if (findpipeexpr(Tok[1])) return (255);	/* expression redefined for species Tok[1] */	
			Npipedef++;		
			strcpy(PipeDefList[Npipedef].expression, "");
			for (i = 2; i < Ntokens; i++)
				strcat(PipeDefList[Npipedef].expression, Tok[i]);
			strcpy(PipeDefList[Npipedef].ID, Tok[1]);
			break;
		}
		case EQU:
		{	
			Npipeequ++;
			strcpy(PipeEquList[Npipeequ].expression, "");
			for (i = 1; i < Ntokens; i++)
				strcat(PipeEquList[Npipeequ].expression, Tok[i]);
			strcpy(PipeEquList[Npipeequ].ID, "");
			break;
		}
	}
	return (0);
}

int addtankexpr(char type)
/*
**--------------------------------------------------------------
**  Input:   expression type (RATE, DEF, EQU)                                                
**  Output:  returns error code                                  
**  Purpose: processes pipe reacion expression                             
**--------------------------------------------------------------
*/
{
	int index1, index2;
	char expr[MAXLINE] = "";
	int i;

	switch (type)
	{
		case RATE:
		{
			index1 = findspecieID(BulkSpecie, Tok[1], Nbulkspecies);
			index2 = findspecieID(TankSurfaceSpecie, Tok[1], Ntanksurfacespecies);
			if (index1 == 0 && index2 == 0)
				return (254);			/* invalid tank species ID in [MSTANK] */
			if (findtankexpr(Tok[1])) return (256);	/* expression redefined for species Tok[1] */		
			Ntankrate++;
			strcpy(TankRateList[Ntankrate].expression, "");
			for (i = 2; i < Ntokens; i++)
				strcat(TankRateList[Ntankrate].expression, Tok[i]);
			strcpy(TankRateList[Ntankrate].ID, Tok[1]);
			break;
		}
		case DEF:
		{
			index1 = findspecieID(BulkSpecie, Tok[1], Nbulkspecies);
			index2 = findspecieID(TankSurfaceSpecie, Tok[1], Ntanksurfacespecies);
			if (index1 == 0 && index2 == 0)
				return (254);				/* invalid tank species ID in [MSTANK] */
			if (findtankexpr(Tok[1])) return (256);	/* expression redefined for species Tok[1] */
			Ntankdef++;			
			strcpy(TankDefList[Ntankdef].expression, "");
			for (i = 2; i < Ntokens; i++)
				strcat(TankDefList[Ntankdef].expression, Tok[i]);
			strcpy(TankDefList[Ntankdef].ID, Tok[1]);

			break;
		}
		case EQU:
		{
			Ntankequ++;			
			strcpy(TankEquList[Ntankequ].expression, "");
			for (i = 1; i < Ntokens; i++)
				strcat(TankEquList[Ntankequ].expression, Tok[i]);
			strcpy(TankEquList[Ntankequ].ID, "");

			break;
		}
	}
	return (0);
}

int msreactdata()
/*
**--------------------------------------------------------------
**  Input:   none                                                
**  Output:  returns error code                                  
**  Purpose: processes multispecies reaction coeff. data                      
**  Formats:                                                     
**     [REACTIONS]                     
**        GLOBAL    ParamID				coeff                                  
**        PIPE		ParamID  PipeID		coeff                                  
**        TANK      ParamID  TankID		coeff                          
**--------------------------------------------------------------
*/
{
/* Skip line if insufficient data */
   
   int n, j, k;	
   float y;	
   n = Ntokens;
   if (n < 3) return(0);
   k = findparameterID(Tok[1]);
   if (k == 0)return (0);	
   if (match(Tok[0],w_GLOBAL))                    
   {
      if (!getfloat(Tok[2],&y)) return(213);
	  else
		  Parameter[k].value = y;
      return(0);
   }
   if (match(Tok[0],w_PIPE))               
   {
      if ( (j = findlink(Tok[2])) == 0) return(0);
      if (!getfloat(Tok[3],&y)) return(213);
	  LinkParameters[j][k] = y;
      return(0);
   }
   if (match(Tok[0],w_TANK))                
   {
      if (!getfloat(Tok[3],&y)) return(213);
      if ( (j = findnode(Tok[2])) <= Njuncs) return(0);
      TankParameters[j-Njuncs][k] = y;      
	  return(0);
   }
   return (0);
}					/*End of msreactdata */

int  msoptiondata()
/*
**--------------------------------------------------------------
**  Input:   none                                                
**  Output:  returns error code                                  
**  Purpose: processes multi species option data                             
**  Format:                                                      
**    [MSOPTIONS]                                              
**      ABSTOL SpecieID  value
**      RELTOL value
**--------------------------------------------------------------
*/
{
    int n, k;	
    float y;	
    n = Ntokens;
    if (match(Tok[0],w_ABSTOL))                    
    {
        if (n != 3) return(255);
        if (!getfloat(Tok[2],&y)) return(213);
        if (k=findspecieID(BulkSpecie, Tok[1], Nbulkspecies)) {
            BulkSpecie[k].abstol = y;
            return(0);
        }
        else if (k=findspecieID(PipeSurfaceSpecie, Tok[1], Npipesurfacespecies)) {
            PipeSurfaceSpecie[k].abstol = y;
            return(0);
        }
        else if (k=findspecieID(TankSurfaceSpecie, Tok[1], Ntanksurfacespecies)) {
            TankSurfaceSpecie[k].abstol = y;
            return(0);
        }
        else return(254);
    }
    if (match(Tok[0],w_RELTOL))               
    {
        if (n != 2) return(255);
        if (!getfloat(Tok[1],&y)) return(213);
        MSreltol = y;
        return(0);
    }
    return (255);

}                        /* end of MS options data */


void msspeciesindex()	/*determine species index*/	
/*
**------------------------------------------------------------------------ 
**  Input:   none
**  Output:  none
**  Purpose: determine specie index, pipe specie index, tank specie index
             for different species. 
			 determine index conversion vectors StoPS, StoTS, PStoS, TStoS
             determine absolute tolerance arrays for pipe and tank species
**------------------------------------------------------------------------         
*/
{
	int  i, index, n;

	Npipespecies = Nbulkspecies + Npipesurfacespecies;
	Ntankspecies = Nbulkspecies + Ntanksurfacespecies;

	/*	Species list: bulk species at the beginning, pipe surface species next, 
		and tank surface species last.*/
	for ( i = 1; i < Nbulkspecies+1; i++)
	{
		index = i;
		strcpy(Specie[index].ID, BulkSpecie[i].ID);
		Specie[index].MSQtype = BulkSpecie[i].MSQtype;
        Specie[index].abstol = BulkSpecie[i].abstol;
	}
	for ( i = 1; i < Npipesurfacespecies+1; i++)
	{
		index = i+Nbulkspecies;
		strcpy(Specie[index].ID, PipeSurfaceSpecie[i].ID);
		Specie[index].MSQtype = PipeSurfaceSpecie[i].MSQtype;
        Specie[index].abstol = PipeSurfaceSpecie[i].abstol;
	}
	for ( i = 1; i < Ntanksurfacespecies+1; i++)
	{
		index = i+Nbulkspecies+Npipesurfacespecies;
		strcpy(Specie[index].ID, TankSurfaceSpecie[i].ID);
		Specie[index].MSQtype = TankSurfaceSpecie[i].MSQtype;
        Specie[index].abstol = TankSurfaceSpecie[i].abstol;
	}

	/*	Pipe Species list: rate species at the beginning, definition species next,
		and algebraic species last.*/
	for ( i = 1; i < Npiperate+1; i++)
	{
		index = i;
		strcpy(PipeSpecie[index].ID, PipeRateList[i].ID);
	}
	for ( i = 1; i < Npipedef+1; i++)
	{
		index = i+Npiperate;
		strcpy(PipeSpecie[index].ID, PipeDefList[i].ID);
	}		
	n = Npiperate + Npipedef;
	index = n;
	for ( i = 1; i < Nbulkspecies+1; i++)
	{
		if (!findspecieID(PipeSpecie, BulkSpecie[i].ID, n))
		{
			index++;
			n++;
			strcpy(PipeSpecie[index].ID, BulkSpecie[i].ID);
		}
	}
	for ( i = 1; i < Npipesurfacespecies+1; i++)
	{
		if (!findspecieID(PipeSpecie, PipeSurfaceSpecie[i].ID, n))
		{
			index++;
			n++;
			strcpy(PipeSpecie[index].ID, PipeSurfaceSpecie[i].ID);
		}
	}

	/*	Tank Species list: rate species at the beginning, definition species next,
		and algebraic species last.*/
	for ( i = 1; i < Ntankrate+1; i++)
	{
		index = i;
		strcpy(TankSpecie[index].ID, TankRateList[i].ID);
	}
	for ( i = 1; i < Ntankdef+1; i++)
	{
		index = i+Ntankrate;
		strcpy(TankSpecie[index].ID, TankDefList[i].ID);
	}
	n = Ntankrate + Ntankdef;
	index = n;
	for ( i = 1; i < Nbulkspecies+1; i++)
	{
		if (!findspecieID(TankSpecie, BulkSpecie[i].ID, n))
		{
			index++;
			n++;
			strcpy(TankSpecie[index].ID, BulkSpecie[i].ID);
		}
	}
	for ( i = 1; i < Ntanksurfacespecies+1; i++)
	{
		if (!findspecieID(TankSpecie, TankSurfaceSpecie[i].ID, n))
		{
			index++;
			n++;
			strcpy(TankSpecie[index].ID, TankSurfaceSpecie[i].ID);
		}
	}

	/*Specie index to pipe/tank specie index*/
	for ( i = 1; i < Nspecies+1; i++)
	{
		index = findspecieID(PipeSpecie, Specie[i].ID, Npipespecies);
		StoPS[i] = index;
		index = findspecieID(TankSpecie, Specie[i].ID, Ntankspecies);
		StoTS[i] = index;
	}
	/*Pipe specie index to specie index*/
	for ( i = 1; i < Npipespecies+1; i++)
	{
		index = findspecieID(Specie, PipeSpecie[i].ID, Nspecies);
		PStoS[i] = index;
	}
	/*Tank specie index to specie index*/
	for ( i = 1; i < Ntankspecies+1; i++)
	{
		index = findspecieID(Specie, TankSpecie[i].ID, Nspecies);
		TStoS[i] = index;
	}

    /* Absolute tolerance arrays for pipe and tank species */
    for ( i = 1; i < Npipespecies+1; i++ ) PSabstol[i] = Specie[PStoS[i]].abstol;
    for ( i = 1; i < Ntankspecies+1; i++ ) TSabstol[i] = Specie[TStoS[i]].abstol;
}

int parseDAsystem()
/*
**------------------------------------------------------------------------ 
**  Input:   none
**  Output:  return errcode
**  Purpose: parse the expressions for differential-algebraic equations 
			 that describe the pipe/tank dynamic system and generate 
			 evaluation trees
**------------------------------------------------------------------------         
*/
{
	struct ExprListNode * listnode;
	struct ExprListNode ** plistnode;
	struct ExprListNode * prev;
	int i, j, n, errcode = 0;
	int stacksize = 0;
	PipeDA = (PExprTree *)calloc((Npipespecies+1), sizeof(PExprTree));
	PipeDAList = (PExprList *)calloc((Npipespecies+1), sizeof(PExprList));
	for ( i = 1; i < Npipespecies+1; i++)
	{
		PipeDA[i] = NULL;
		PipeDAList[i] = NULL;
	}
	TankDA = (PExprTree *)calloc((Ntankspecies+1), sizeof(PExprTree));
	TankDAList = (PExprList *)calloc((Ntankspecies+1), sizeof(PExprList));
	for ( i = 1; i < Ntankspecies+1; i++)
	{
		TankDA[i] = NULL;
		TankDAList[i] = NULL;
	}
    ERRCODE(MEMCHECK(PipeDA));
	ERRCODE(MEMCHECK(TankDA));
	ERRCODE(MEMCHECK(PipeDAList));
	ERRCODE(MEMCHECK(TankDAList));
	if (errcode) return (errcode);
	for ( j = 1; j < Npiperate+1; j++)
	{
		i = j;
		PipeDA[i] = mathexpr_create(PipeRateList[j].expression, getPipeSpecieIndex);		
	}
	for ( j = 1; j < Npipedef+1; j++)
	{
		i = j+Npiperate;
		PipeDA[i] = mathexpr_create(PipeDefList[j].expression, getPipeSpecieIndex);
	}
	for ( j = 1; j < Npipeequ+1; j++)
	{
		i = j+Npiperate+Npipedef;
		PipeDA[i] = mathexpr_create(PipeEquList[j].expression, getPipeSpecieIndex);
	}
	for ( j = 1; j < Npiperate+Npipedef+Npipeequ+1; j++)
	{
		plistnode = (struct ExprListNode **)malloc(sizeof(struct ExprListNode *));
		(*plistnode) = NULL;
		traversaltree(PipeDA[j], plistnode);
		listnode = (*plistnode);
		n = 1;
		prev = listnode->prev;
		while (prev != NULL)
		{
			n = n+1;
			listnode = prev;
			prev = listnode->prev;
		}
		PipeDAList[j] = listnode;
		if (n > stacksize) stacksize = n;
	}

	for ( j = 1; j < Ntankrate+1; j++)
	{
		i = j;
		TankDA[i] = mathexpr_create(TankRateList[j].expression, getTankSpecieIndex);
	}
	for ( j = 1; j < Ntankdef+1; j++)
	{
		i = j+Ntankrate;
		TankDA[i] = mathexpr_create(TankDefList[j].expression, getTankSpecieIndex);
	}
	for ( j = 1; j < Ntankequ+1; j++)
	{
		i = j+Ntankrate+Ntankdef;
		TankDA[i] = mathexpr_create(TankEquList[j].expression, getTankSpecieIndex);
	}
	for ( j = 1; j < Ntankrate+Ntankdef+Ntankequ+1; j++)
	{
		plistnode = (struct ExprListNode **)malloc(sizeof(struct ExprListNode *));
		(*plistnode) = NULL;
		traversaltree(TankDA[j], plistnode);
		listnode = (*plistnode);
		n = 1;
		prev = listnode->prev;
		while (prev != NULL)
		{
			n = n+1;
			listnode = prev;
			prev = listnode->prev;
		}
		TankDAList[j] = listnode;
		if ( n > stacksize) stacksize = n;
		free(plistnode);
	}
	buildstack(stacksize);
	return (0);
}

int getPipeSpecieIndex(char* id)
/*
**------------------------------------------------------------------------ 
**  Input:   id: specie id
**  Output:  return pipe specie index, -1 if not a pipe specie
**  Purpose: find pipe specie index  
**------------------------------------------------------------------------         
*/
{
    int i;
    for ( i=1; i< Npipespecies+1; i++ )
    {
        if ( strcmpi(id, PipeSpecie[i].ID) == 0 ) return i;
    }
    return -1;
}

int getTankSpecieIndex(char* id)
/*
**------------------------------------------------------------------------ 
**  Input:   id: specie id
**  Output:  return tank specie index, -1 if not a tank specie
**  Purpose: find tank specie index  
**------------------------------------------------------------------------         
*/
{
    int i;
    for ( i=1; i< Ntankspecies+1; i++ )
    {
        if ( strcmpi(id, TankSpecie[i].ID) == 0 ) return i;
    }
    return -1;
}

int	  addspecieID(char specietype, char * id, char qualitytype)
/*
**---------------------------------------------------------------------- 
**  Input:   specietype: specie type (BULK, PIPESURFACE, or TANKSURFACE)
**           id:         specie ID
**  Output:  returns error code
**  Purpose: add a specie to corresponding specie list
**----------------------------------------------------------------------   
*/
{
	int index;
	index = findspecieID(Specie, id, Nspecies)+findconstantID(id)
		    +findparameterID(id)+findcoefficientID(id);
	/*check if ID has been used*/
	if (index > 0) return (215);
	if (Nspecies == MaxSpecies) return (200);

	switch (specietype)
	{
		case BULK: /*bulk specie*/
		{
			Nspecies ++;
			Nbulkspecies ++; 
			strcpy(BulkSpecie[Nbulkspecies].ID, id);
			break;
		}
		case PIPESURFACE: /*pipe surface specie*/
		{
			Nspecies ++;
			Npipesurfacespecies ++; 
			strcpy(PipeSurfaceSpecie[Npipesurfacespecies].ID, id);
			PipeSurfaceSpecie[Npipesurfacespecies].MSQtype = qualitytype;
			break;
		}
		case TANKSURFACE: /*tank surface specie*/
		{
			Nspecies ++;
			Ntanksurfacespecies ++; 
			strcpy(TankSurfaceSpecie[Ntanksurfacespecies].ID, id);
			TankSurfaceSpecie[Ntanksurfacespecies].MSQtype = qualitytype;
			break;
		}
	}
	return (0);
}

int addcoefficientID(char * id, float value)
/*
**------------------------------------------------------------------------ 
**  Input:   id: mass transfer coefficient ID
**           value: specie diffusivity in water
**  Output:  returns error code
**  Purpose: add a mass transfer coefficient to coefficient list
             mass transfer coefficient = Sherwood*Diffusivity/diameter
**------------------------------------------------------------------------         
*/
{
	int index;
	/*check if ID has been used*/
	index = findspecieID(Specie, id, Nspecies)+findconstantID(id)
		    +findparameterID(id)+findcoefficientID(id);;
	if (index > 0) return (215);
	if (Ncoefficients == MaxCoefficients) return (200);
	Ncoefficients ++;
	strcpy(Coefficient[Ncoefficients].ID, id);
	Coefficient[Ncoefficients].diffusivity = value;
	return (0);
}

int addparameterID(char * id)
/*
**------------------------------------------------------------------------ 
**  Input:   id: parameter ID
**  Output:  returns error code
**  Purpose: add a parameter to parameter lsit
**------------------------------------------------------------------------         
*/
{
	int index;
	/*check if ID has been used*/
	index = findspecieID(Specie, id, Nspecies)+findconstantID(id)
		    +findparameterID(id)+findcoefficientID(id);;
	if (index > 0) return (215);
	if (Nparameters == MaxParameters) return (200);
	Nparameters ++;
	strcpy(Parameter[Nparameters].ID, id);
	return (0);
}

int addconstantID(char * id, float value)
/*
**------------------------------------------------------------------------ 
**  Input:   id: constant ID
             value: constant value
**  Output:  returns error code
**  Purpose: add a constant to constant list 
**------------------------------------------------------------------------         
*/
{
	int index;
	
	/*check if ID has been used*/
	index = findspecieID(Specie, id, Nspecies)+findconstantID(id)
		    +findparameterID(id)+findcoefficientID(id);
	if (index > 0) return (215);
	if (Nconstants == MaxConstants) return (200);
	Nconstants ++;
	strcpy(Constant[Nconstants].ID, id);
	Constant[Nconstants].value = value;
	return (0);
}

int findspecieID(Sspecie * list, char * id, int len)
/*
**------------------------------------------------------------------------ 
**  Input:   list: specie list
			 id:  specie ID
			 len: specie list length
**  Output:  returns specie position in the list
**  Purpose: find specie index corresponding to id
**------------------------------------------------------------------------         
*/
{
	int i;
	for ( i = 1; i < len+1; i++)
	{
		if (strcmpi(list[i].ID, id) == 0)
			return i;
	}
	return 0;
}

int findparameterID(char * id)
/*
**------------------------------------------------------------------------ 
**  Input:   id: parameter ID
**  Output:  returns parameter index
**  Purpose: find parameter index corresponding to id 
**------------------------------------------------------------------------         
*/
{
	int i;
	for ( i = 1; i < Nparameters+1; i++)
	{
		if (strcmpi(Parameter[i].ID, id) == 0)
			return i;
	}
	return 0;
}

int findcoefficientID(char * id)
/*
**------------------------------------------------------------------------ 
**  Input:   id: coefficient ID
**  Output:  returns mass transfer coefficient index
**  Purpose: find mass transfer coefficient index corresponding to id  
**------------------------------------------------------------------------         
*/
{
	int i;
	for ( i = 1; i < Ncoefficients+1; i++)
	{
		if (strcmpi(Coefficient[i].ID, id) == 0)
			return i;
	}
	return 0;
}

int findconstantID(char * id)
/*
**------------------------------------------------------------------------ 
**  Input:   id: constant ID
**  Output:  returns constant index
**  Purpose: find constant index corresponding to id  
**------------------------------------------------------------------------         
*/
{
	int i;
	for ( i = 1; i < Nconstants+1; i++)
	{
		if (strcmpi(Constant[i].ID, id) == 0)
			return i;
	}
	return 0;

}

void assignpara()
/*
**------------------------------------------------------------------------ 
**  Input:   none
**  Output:  none
**  Purpose: assign global parameter value  
**------------------------------------------------------------------------         
*/
{
	int i,k;
	for ( i = 1; i < Nlinks+1; i++)
	{
		for ( k = 1; k < Nparameters+1; k++)
			if (LinkParameters[i][k] == MISSING) LinkParameters[i][k]=Parameter[k].value;
	}

	for ( i = 1; i < Ntanks+1; i++)
	{
		for ( k = 1; k < Nparameters+1; k++)
			if (TankParameters[i][k] == MISSING) TankParameters[i][k]=Parameter[k].value;
	}
}

int findpipeexpr(char * id)
/*
**------------------------------------------------------------------
**  Input:   id = specie ID
**  Output:  returns error code
**  Purpose: check if the definition or rate for a specie is defined
**------------------------------------------------------------------
*/
{
	int i;

    for (i = 1; i < Npiperate; i++)
    {
        if (strcmpi(PipeRateList[i].ID,id) == 0)
        {
           return (1);
        }
    }

    for (i = 1; i < Npipedef; i++)
    {
        if (strcmpi(PipeDefList[i].ID,id) == 0)
        {
           return (1);
        }
    } 

	return (0);
}

int findtankexpr(char * id)
/*
**------------------------------------------------------------------
**  Input:   id = specie ID
**  Output:  returns error code
**  Purpose: check if the definition or rate for a specie is defined
**------------------------------------------------------------------
*/
{
	int i;
    for (i = 1; i < Ntankrate; i++)
    {
        if (strcmpi(TankRateList[i].ID,id) == 0)
        {
           return (1);
        }
    }

    for (i = 1; i < Ntankdef; i++)
    {
        if (strcmpi(TankDefList[i].ID,id) == 0)
        {
           return (1);
        }
    } 

	return (0);
}

void  msinitunits()
/*
**--------------------------------------------------------------
**  Input:   none
**  Output:  none
**  Purpose: determines unit conversion factors
**--------------------------------------------------------------
*/
{
   float  ccf;  /* concentration conversion factor */

   ccf = 1.0/LperFT3;  
   UcfAreaq = 1.0/CM2perFT2;

   Ucf[QUALITY]   = ccf;
   if (MSVolumeFlag == CUBICFOOT)	
   {
	   MSUcf[VOLUME] = 1.0;
	   MSUcf[LENGTH] = 1.0;
	   MSUcfAreaq = 1.0;
   }
   else if (MSVolumeFlag == LITER) 
   {
	   MSUcf[VOLUME] = LperFT3;
	   MSUcf[LENGTH] = MperFT*10.0;
	   MSUcfAreaq = 100.0/CM2perFT2;
   }
   else if (MSVolumeFlag == CUBICMETER)
   {
	   MSUcf[VOLUME] = M3perFT3;
	   MSUcf[LENGTH] = MperFT;
	   MSUcfAreaq = 10000.0/CM2perFT2;
   }

   MSUcf[QUALITY] = 1/MSUcf[VOLUME];	
   
   if (MSTimeFlag == SECOND) MSUcf[TIME] = 1.0;
   else if (MSTimeFlag == MINUTE) MSUcf[TIME] = MINUTEperSEC;
   else if (MSTimeFlag == HOUR)   MSUcf[TIME] = HOURperSEC;   
   else if (MSTimeFlag == DAY)    MSUcf[TIME] = DAYperSEC;   

}                       /*  End of msinitunits  */

void  msconvertunits()
/*
**--------------------------------------------------------------
**  Input:   none
**  Output:  none
**  Purpose: converts units of input data
**--------------------------------------------------------------
*/
{
   int   i;
   Pminitial msinitial;

   /* (WQ source units are converted in MSQUALITY.C */
   for (i=1; i<=Nnodes; i++)
   {
	  for (msinitial = NodeInitial[i]; msinitial != NULL; msinitial = msinitial->next)
	  {
		  if (Specie[msinitial->index].MSQtype == VOLUMEUNIT)
			  msinitial->C0 /= Ucf[QUALITY];
		  else
			  msinitial->C0 /= UcfAreaq;
	  }
   }
}                       /*  End of msconvertunits  */


int mscount(char* tok)
/*
**-------------------------------------------------------------
**  Input:   tok = multi species variable type
**  Output:  returns error code
**  Purpose: count the number of multi species variables
**--------------------------------------------------------------
*/
{
	if (match(tok, w_BULK))  
		MaxSpecies ++;
	else if (match(tok, w_PIPE))
		MaxSpecies ++;
	else if (match(tok, w_TANK))
		MaxSpecies ++;
	else if (match(tok, w_PARAMETER))
		MaxParameters ++;
	else if (match(tok, w_CONSTANT))
		MaxConstants ++;
	else if (match(tok, w_COEFFICIENT))
		MaxCoefficients++;
	return (0);
}

/********************** END OF MSINPUT.C ************************/


