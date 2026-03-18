/*
*******************************************************************************
                                                                      
MSQUALITY.C -- Multi Species Water Quality Simulator for EPANET Program         
                                                                      

  The entry points for this module are:
    msopenqual()   -- called from ENopenQ() in EPANET.C
    msinitqual()   -- called from ENinitQ() in EPANET.C
    msrunqual()    -- called from ENrunQ() in EPANET.C
    msnextqual()   -- called from ENnextQ() in EPANET.C
    msstepqual()   -- called from ENstepQ() in EPANET.C
    msclosequal()  -- called from ENcloseQ() in EPANET.C
                                                                      
  Calls are made to:
    AllocInit()
    Alloc()
    AllocFree()   
  in MEMPOOL.C to utilize a memory pool to prevent excessive malloc'ing  
  when constantly creating and destroying pipe sub-segments during    
  the water quality transport calculations.

  Calls are also made to:
    readhyd()
    readhydstep()
    savenetdata()
    saveoutput()
    savefinaloutput()
  in OUTPUT.C to retrieve hydraulic results and save all results.

******************************************************************************* 
*/

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
#include "mempool.h"
#include "odesolve.h"
#include "newton.h"
/*
** Macros to identify upstream & downstream nodes of a link
** under the current flow and to compute link volume
*/
#define   UP_NODE(x)   ( (FlowDir[(x)]=='+') ? Link[(x)].N1 : Link[(x)].N2 )
#define   DOWN_NODE(x) ( (FlowDir[(x)]=='+') ? Link[(x)].N2 : Link[(x)].N1 )
#define   LINKVOL(k)   ( 0.785398*Link[(k)].Len*SQR(Link[(k)].Diam) )

Pmsseg		FreeSeg;              /* Pointer to unused segment               */
Pmsseg      *FirstSeg,            /* First (downstream) segment in each pipe */
			*LastSeg;             /* Last (upstream) segment in each pipe    */
char      *FlowDir;             /* Flow direction for each pipe            */
float     *VolIn;               /* Total volume inflow to node             */
float     *Cin;					/* Tank inflow water qualities             */
float     *TempMsc;				/* Temporary storage for species qualities */	

float	  **  TempSurfaceSpecie;    /* Surface  specie qualities in 
									   to-be-generated last segments */

float     **MSMassIn;           /* Total mass inflow to node               */
float     Sc;                   /* Schmidt Number                          */
char      Reactflag;            /* Reaction indicator                      */
char      OutOfMemory;          /* Out of memory indicator                 */

int  msopenqual()
/*
**--------------------------------------------------------------
**   Input:   none     
**   Output:  returns error code                                          
**   Purpose: opens Multi Species WQ solver system 
**--------------------------------------------------------------
*/
{
   int errcode = 0;
   int n,i;

   SingularFlag = FALSE;
 
   /* Allocate memory pool for WQ segments */
   OutOfMemory = FALSE;
   if (AllocInit() == NULL) errcode = 101;

   /* Allocate scratch array & reaction rate array*/
   n = MAX((Nnodes+1),(Nlinks+1));
   X  = (float *) calloc(MAX((Nnodes+1),(Nlinks+1)),sizeof(float));
   R  = (float *) calloc((Nlinks+1), sizeof(float));  
   ERRCODE(MEMCHECK(R));
   ERRCODE(MEMCHECK(X));
   MSX  = (float **) calloc(n,sizeof(float *));
   MSR  = (float **) calloc(Nlinks+1, sizeof(float *));
   MSC  = (float **) calloc(Nnodes+1, sizeof(float *));
   CurrentMasstracoeff = (float *) calloc(Ncoefficients+1, sizeof(float));
   TempMsc = (float *)calloc(Nspecies+1, sizeof(float));
   Cin  = (float *)calloc(Nspecies+1, sizeof(float));
   MSMassReact = (float *)calloc(Nspecies+1, sizeof(float));
   ERRCODE(MEMCHECK(MSX));
   ERRCODE(MEMCHECK(MSR));
   ERRCODE(MEMCHECK(MSC));
   ERRCODE(MEMCHECK(TempMsc));
   ERRCODE(MEMCHECK(Cin));
   ERRCODE(MEMCHECK(MSMassReact));
   ERRCODE(MEMCHECK(CurrentMasstracoeff));
   for ( i = 1; i < n; i++)
   {
		MSX[i] = (float *)calloc(Nspecies+1, sizeof(float));
		ERRCODE(MEMCHECK(MSX[i]));
   }
   for ( i = 1; i < Nlinks+1; i++)
   {  
		MSR[i] = (float *)calloc(Nspecies+1, sizeof(float));
		ERRCODE(MEMCHECK(MSR[i]));
		if (!errcode) memset(MSR[i], 0, (Npipespecies+1)*sizeof(float));
   }
   for ( i = 1; i < Nnodes+1; i++)
   {
		MSC[i] = (float *)calloc(Nspecies+1, sizeof(float));
		ERRCODE(MEMCHECK(MSC[i]));
		if (!errcode) memset(MSC[i], 0, (Nspecies+1)*sizeof(float));
   }

   /* Allocate memory for WQ solver */
   n        = Nlinks+Ntanks+1;
   FirstSeg = (Pmsseg *) calloc(n, sizeof(Pmsseg));
   LastSeg  = (Pmsseg *) calloc(n, sizeof(Pmsseg));
   FlowDir  = (char *) calloc(n, sizeof(char));
   TempSurfaceSpecie = (float **)calloc(n, sizeof(float *));
   ERRCODE(MEMCHECK(FirstSeg));  
   ERRCODE(MEMCHECK(LastSeg));
   ERRCODE(MEMCHECK(FlowDir));
   ERRCODE(MEMCHECK(TempSurfaceSpecie));
   for ( i = 1; i < n; i++)
   {
	   TempSurfaceSpecie[i] = (float *)calloc(Npipesurfacespecies+1, sizeof(float));
	   ERRCODE(MEMCHECK(TempSurfaceSpecie[i])); 
   }
   n        = Nnodes+1;
   VolIn    = (float *) calloc(n, sizeof(float));
   MSMassIn   = (float **) calloc(n, sizeof(float *));
   ERRCODE(MEMCHECK(VolIn));
   ERRCODE(MEMCHECK(MSMassIn));
   for (i = 1; i < n; i++)
   {
	   MSMassIn[i] = (float *)calloc(Nbulkspecies+1, sizeof(float));
	   ERRCODE(MEMCHECK(MSMassIn[i]));	
   }
   
   ERRCODE(odesolve_open(Nspecies));
   ERRCODE(newton_open(Nspecies));
   
   return(errcode);
}

void  msinitqual()
/*
**--------------------------------------------------------------
**   Input:   none     
**   Output:  none                                          
**   Purpose: re-initializes WQ solver system 
**--------------------------------------------------------------
*/
{
   int i, j;
   Pminitial msI;
   Pmsource  msS;
   /* Initialize quality, tank volumes, & source mass flows */
   for ( i = 1; i < Nnodes+1; i++)
   {
       memset(MSC[i], 0, (Nspecies+1)*sizeof(float));
   }
   for (i=1; i<=Nnodes; i++) 
   {
	   for (msI = NodeInitial[i]; msI != NULL; msI = msI->next)
	   {
		   j = msI->index;
		   MSC[i][j] = msI->C0;
	   }
   }

   for (i=1; i<=Ntanks; i++) Tank[i].V = Tank[i].V0;
   for (i=1; i<=Nnodes; i++)
   {
	   for (msS = NodeSource[i]; msS != NULL; msS = msS->next)
	   {
		   msS->Smass = 0;	
	   }
   }
   /* Set WQ parameters */
   Reactflag = 0;
   if (Qualflag != NONE)
   {
      FreeSeg = NULL;
      AllocReset(); 
   }

   for ( i = 1; i < Nspecies+1; i++)
	   MSMassReact[i] = 0.0;


   /* Re-position hydraulics file */
   fseek(HydFile,HydOffset,SEEK_SET);

   /* Set elapsed times to zero */
   Htime = 0;
   Qtime = 0;
   Rtime = Rstart;
   Nperiods = 0;
}


int msrunqual(long *t)
/*
**--------------------------------------------------------------
**   Input:   none     
**   Output:  t = pointer to current simulation time (sec)
**   Returns: error code                                          
**   Purpose: retrieves hydraulics for next hydraulic time step
**            (at time *t) and saves current results to file
**--------------------------------------------------------------
*/
{
   long    hydtime;       /* Hydraulic solution time */
   long    hydstep;       /* Hydraulic time step     */
   int     errcode = 0;

   /* Update reported simulation time */
   *t = Qtime;
   /* Read hydraulic solution from hydraulics file */
   if (Qtime == Htime)
   {
      errcode = msgethyd(&hydtime, &hydstep);
      Htime = hydtime + hydstep;
   }

   return(errcode);
}


int msnextqual(long *tstep)
/*
**--------------------------------------------------------------
**   Input:   none     
**   Output:  tstep = pointer to time step (sec)
**   Returns: error code                                          
**   Purpose: updates WQ conditions until next hydraulic 
**            solution occurs (after *tstep secs.)
**--------------------------------------------------------------
*/
{
   long    hydstep;       /* Hydraulic solution time step */
   int     errcode = 0;

   /* Determine time step */
   *tstep = 0;
   hydstep = Htime - Qtime;
   /* Perform water quality routing over this time step */

   if (Qualflag != NONE && hydstep > 0) mstransport(hydstep);

   /* Update current time */
   if (OutOfMemory) errcode = 101;
 
   if (SingularFlag == TRUE)
   {
	   sprintf(Msg,WARN07,clocktime(Atime,Qtime));
	   if (Messageflag) writeline(Msg);
	   SingularFlag = FALSE;
   }
  
   if (!errcode) *tstep = hydstep;
   Qtime += hydstep;

   combinesegments();   //combine pipe segments if water qualities are close enough
   /* Save final output if no more time steps */
   if (!errcode && Saveflag && *tstep == 0) errcode = savefinaloutput();
   return(errcode);
}


int msstepqual(long *tleft)
/*
**--------------------------------------------------------------
**   Input:   none     
**   Output:  tleft = pointer to time left in simulation
**   Returns: error code                                          
**   Purpose: updates WQ conditions over a single WQ time step
**--------------------------------------------------------------
*/
{  long dt, hstep, t, tstep;
   int  errcode = 0;
   tstep = Qstep;
   CurrentTime = Qtime;
   do
   {
      dt = tstep;
      hstep = Htime - Qtime;
      if (hstep < dt)
      {
         dt = hstep;
         if (Qualflag != NONE) mstransport(dt);
         Qtime += dt;
         errcode = msrunqual(&t);
         Qtime = t;
      }
      else
      {
         if (Qualflag != NONE) mstransport(dt);
         Qtime += dt;
      }
      tstep -= dt;
      if (OutOfMemory) errcode = 101;
   }  while (!errcode && tstep > 0);
   *tleft = Dur - Qtime;
//   if (!errcode && Saveflag && *tleft == 0) errcode = savefinaloutput();
   return(errcode);
}

int msclosequal()
/*
**--------------------------------------------------------------
**   Input:   none     
**   Output:  returns error code                                          
**   Purpose: closes WQ solver system 
**--------------------------------------------------------------
*/
{
   int errcode = 0;
   int i, n;
   
   Pmsource ms, nextms;
   Pminitial mi, nextmi;

   odesolve_close();
   newton_close(Nspecies);
   AllocFreePool();   /* Free memory pool */
   free(FirstSeg);
   free(LastSeg);
   free(FlowDir);
   free(Cin);
   FlowDir = NULL;
   free(VolIn);
   VolIn = NULL;
   free(X);
   free(R);

   n = Nlinks+Ntanks+1;
   for ( i = 1; i < n; i++)
   {
	   free(TempSurfaceSpecie[i]);
	   TempSurfaceSpecie[i] = NULL;
   }
   free(TempSurfaceSpecie);
   TempSurfaceSpecie = NULL;

   n = Nlinks+1;
   for ( i = 1; i < n; i++)
   {
	   free(MSR[i]);
	   free(LinkParameters[i]);
	   MSR[i] = NULL;
	   LinkParameters[i] = NULL;
   }   
   free(LinkParameters);
   LinkParameters = NULL;
   free(MSR);
   MSR = NULL;


   n = Ntanks+1;
   for ( i = 1; i < n; i++)
   {
	   free(TankParameters[i]);
	   TankParameters[i] = NULL;
   }
   free(TankParameters);
   TankParameters = NULL;
   
   n = MAX((Nnodes+1),(Nlinks+1));
   for ( i = 1; i < n; i++)
   {
	   free(MSX[i]);
	   MSX[i] = NULL;
   }
   free(MSX);
   MSX = NULL;

   n = Nnodes+1;
   for ( i = 1; i < n; i++)
   {
	   free(MSMassIn[i]);
	   free(MSC[i]);
	   MSC[i] = NULL;
	   MSMassIn[i] = NULL;
	   ms = NodeSource[i];
	   while (ms != NULL)
	   {
		   nextms = ms->next;
		   free(ms);
		   ms = nextms;
	   }
	   mi = NodeInitial[i];
	   while (mi != NULL)
	   {
		   nextmi = mi->next;
		   free(mi);
		   mi = nextmi;
	   }
   }
   free(NodeSource);
   free(NodeInitial);
   free(MSMassIn);
   free(MSC);
   MSMassIn = NULL;
   MSC = NULL;
   
   if (CurrentMasstracoeff != NULL)
	   free(CurrentMasstracoeff);

   free(TempMsc);
   TempMsc = NULL;
   free(MSMassReact);
   MSMassReact = NULL;
   return(errcode);
}


void combinesegments()
/*
**--------------------------------------------------------------------
**   Input:   none     
**   Output:  none                                         
**   Purpose: combine adjacent pipe segments if water qualities
			  are close enough (percentage difference less than MSreltol) 
**--------------------------------------------------------------------
*/
{
	int k, flag, index;
	Pmsseg seg, pseg, ppseg;
	float v;

	for ( k = 1; k <= Nlinks; k++)
	{
		seg = FirstSeg[k]; /*strating from first segment*/
		while (seg != NULL)
		{
			flag = 0;
			pseg = seg->prev;
			if ( pseg != NULL)
			{
				for ( index = 1; index <= Npipespecies; index++)
				{
					if (flag == 1) break;
					if (ABS(seg->c[index]-pseg->c[index])>MSreltol*seg->c[index])
						flag = 1;
				}
				if (flag == 0)	/* combine seg and its previous segment */
				{
					v = seg->v+pseg->v; /*sum of volumes*/
					for (index = 1; index <= Npipespecies; index++)
					{
						/* average water qualities */
						seg->c[index] = (seg->c[index]*seg->v+pseg->c[index]*pseg->v)/v;
					}
					seg->v = v;
					ppseg = pseg->prev;
					seg->prev = ppseg;
					if (ppseg == NULL)
						LastSeg[k] = seg;
					else
						ppseg->next = seg;
					
					/* put segment pseg in memory pool*/
					pseg->prev = FreeSeg;
					pseg->next = NULL;
					FreeSeg = pseg;
				}
				else
					seg = pseg;
			}
			else
				seg = NULL;
		}
	}
}

int  msgethyd(long *hydtime, long *hydstep)
/*
**-----------------------------------------------------------
**   Input:   none     
**   Output:  hydtime = pointer to hydraulic solution time
**            hydstep = pointer to hydraulic time step
**   Returns: error code                                          
**   Purpose: retrieves hydraulic solution and hydraulic
**            time step for next hydraulic event
**
**   NOTE: when this function is called, WQ results have
**         already been updated to the point in time when
**         the next hydraulic event occurs.
**-----------------------------------------------------------
*/
{
   int errcode = 0;
   /* Read hydraulic results from file */
   if (!readhyd(hydtime)) return(307);
   if (!readhydstep(hydstep)) return(307);
   Htime = *hydtime;

   /* Save current results to output file */
   if (Htime >= Rtime)
   {
      if (Saveflag)
      {
		 errcode = saveoutput();
         Nperiods++;
      }
      Rtime += Rstep;
   }

   /* If simulating WQ: */
   if (Qualflag != NONE && Qtime < Dur)
   {
      if (Qtime == 0) msinitsegs();
      else            msreorientsegs();
   }
   return(errcode);
}

void  mstransport(long tstep)
/*
**--------------------------------------------------------------
**   Input:   tstep = length of current time step     
**   Output:  none
**   Purpose: transports species mass through pipe network        
**            under a period of constant hydraulic conditions.        
**--------------------------------------------------------------
*/
{
   long   qtime, dt;
   /* Repeat until elapsed time equals hydraulic time step */
   qtime = 0;
   while (!OutOfMemory && qtime < tstep)
   {									/* Qstep is quality time step */
      dt = MIN(Qstep,tstep-qtime);		/* Current time step */
	  CurrentTime = (Qtime+qtime)*MSUcf[TIME];
      qtime += dt;						/* Update elapsed time */
	  msupdatesegs(dt);					/* Update quality in inner link segs */
 	  msaccumulate(dt);                 /* Accumulate flow at nodes */
      msupdatenodes(dt);                /* Update nodal quality */
      mssourceinput(dt);                /* Compute inputs from sources */
      msrelease(dt);					/* Release new nodal flows */
   }
   msupdatesourcenodes(tstep);          /* Update quality at source nodes */
}


void  msinitsegs()
/*
**--------------------------------------------------------------
**   Input:   none     
**   Output:  none
**   Purpose: initializes water quality segments                      
**--------------------------------------------------------------
*/
{
   int     i,j,k,index;
   float   *msc,v;
   /* Examine each link */
   for (k=1; k<=Nlinks; k++)
   {

      /* Establish flow direction */
      FlowDir[k] = '+';
      if (Q[k] < 0.) FlowDir[k] = '-';

      /* Set segs to zero */
      LastSeg[k] = NULL;
      FirstSeg[k] = NULL;

      /* Find quality of downstream node */
      j = DOWN_NODE(k);
	  msc = MSC[j];

      /* Fill link with single segment with this quality */
      msaddseg(k,LINKVOL(k),msc);
	  /* Initialize pipe suface species*/
	  if (LastSeg[k] != NULL)
	  {
		  for ( i = 1; i < Npipesurfacespecies+1; i++)
		  {
			  index = StoPS[i+Nbulkspecies];
			  if ( j <= Njuncs)LastSeg[k]->c[index] = MSC[j][i+Nbulkspecies];
		  }
	  }
   }

   /* Initialize segments in tanks that use them */
   for (j=1; j<=Ntanks; j++)
   {

      /* Skip reservoirs */
      if (Tank[j].A == 0.0) continue;

      /* Tank segment pointers are stored after those for links */
      k = Nlinks + j;
      msc = MSC[Tank[j].Node];
      LastSeg[k] = NULL;
      FirstSeg[k] = NULL;

      /* Add 2 segments for 2-compartment model */
      if (Tank[j].MixModel == MIX2)
      {
         v = MAX(0,Tank[j].V-Tank[j].V1max);
         msaddseg(k,v,msc);
         v = Tank[j].V - v;
         msaddseg(k,v,msc);
      }
      /* Add one segment for FIFO & LIFO models or
	     Add one segment to completely mixed tank */
      else
      {
         v = Tank[j].V;
         msaddseg(k,v,msc);
      }
   }
}


void  msreorientsegs()
/*
**--------------------------------------------------------------
**   Input:   none     
**   Output:  none
**   Purpose: re-orients segments (if flow reverses)                  
**--------------------------------------------------------------
*/
{
   Pmsseg   seg, nseg, pseg;
   int    k;
   char   newdir;

   /* Examine each link */
   for (k=1; k<=Nlinks; k++)
   {

      /* Find new flow direction */
      newdir = '+';
      if (Q[k] == 0.0)     newdir = FlowDir[k];
      else if (Q[k] < 0.0) newdir = '-';

      /* If direction changes, then reverse order of segments */
      /* (first to last) and save new direction */
      if (newdir != FlowDir[k])
      {
         seg = FirstSeg[k]; 
         FirstSeg[k] = LastSeg[k];
         LastSeg[k] = seg;
         pseg = NULL;
         while (seg != NULL)
         {
            nseg = seg->prev;
			seg->next = nseg;
            seg->prev = pseg;
            pseg = seg;
            seg = nseg;
         }
         FlowDir[k] = newdir;
      }
   }
}


void  msupdatesegs(long dt)
/*
**-------------------------------------------------------------
**   Input:   t = time from last WQ segment update     
**   Output:  none
**   Purpose: reacts material in pipe segments up to time t               
**-------------------------------------------------------------
*/
{
   int    k, i, index;
   Pmsseg   seg, seg2;
   float  rsum, vsum, vstart, vend, vposition;
   float  vin, v;

   /* Examine each link in network */
   for (k=1; k<=Nlinks; k++)
   {

      /* Skip zero-length links (pumps & valves) */
      rsum = 0.0;
      vsum = 0.0;
      if (Link[k].Len == 0.0) continue;
      CurrentSegDiam = Link[k].Diam*MSUcf[LENGTH];  
      /* Examine each segment of the link */
	  masstransfer(k);	/*mass transfer coefficients for current pipe segment*/
      seg = FirstSeg[k];
      while (seg != NULL)
      {
            /* React segment over time dt */
            mspipereact(k,seg->c,seg->v,dt);
            seg = seg->prev;
      }

	  /*Update surface species quality change because of segment movement*/
	  v = LINKVOL(k);
	  seg = LastSeg[k];
	  vin = ABS(Q[k])*dt;
	  vsum = seg->v;

	  /*Determines the surface specie concentration in to-be-generated LastSegs*/
	  memset(TempSurfaceSpecie[k], 0, (Npipesurfacespecies+1)*sizeof(float));
	  if (vin > v)
		  vin = v;
	  while ( vsum < vin )
	  {
		  for (i = 1; i < Npipesurfacespecies+1; i++)
		  {
			  index = StoPS[i+Nbulkspecies];
			  TempSurfaceSpecie[k][i] += seg->v*seg->c[index];  
		  }
		  seg = seg->next;
		  if (seg != NULL)
			  vsum += seg->v;
		  else
			  vsum = v;
		  if (vsum > v) vsum = v;
	  }
	  for (i = 1; i < Npipesurfacespecies+1; i++)
	  {
		  index = StoPS[i+Nbulkspecies];
		  if (seg != NULL)
			  TempSurfaceSpecie[k][i] += (seg->v-(vsum - vin))*seg->c[index];
		  TempSurfaceSpecie[k][i] = TempSurfaceSpecie[k][i]/MIN(v,vin);			  
	  }
		
	  /*Update surface specie concentrations in pipe segments*/
	  /*Update starts from current last segments and proceeds downstream*/
	  
	  vstart = vin;  /*future start position of the last segment*/

	  for(seg = LastSeg[k]; seg != NULL; seg = seg->next)
	  {
		  for (i = 1; i < Npipesurfacespecies+1; i++)
		  {
			  index = StoPS[i+Nbulkspecies];
			  TempMsc[index] = 0;
		  }
		  vend = vstart + seg->v;  /*future end position of the segment*/
		  if (vend > v) vend = v;
		  vsum = 0;

		  vposition = vstart; 

		  /*volume to be taken by the segment after segment movement is vend-vstart*/
		  for (seg2 = LastSeg[k]; seg2 != NULL; seg2 = seg2->next)
		  {
			  vsum += seg2->v;
			  if (vsum >= vstart && vsum <= vend)
			  {
				  for (i = 1; i < Npipesurfacespecies+1; i++)
				  {
					  index = StoPS[i+Nbulkspecies];
					  TempMsc[index] += (vsum-vposition)*seg2->c[index]; /*pipe specie mass*/
				  } 
				  vposition = vsum;
			  }
			  if (vsum >= vend) break;
		  }
		  for (i = 1; i < Npipesurfacespecies+1; i++) /**/
		  {
			  index = StoPS[i+Nbulkspecies];
			  if (seg2 != NULL)
				  TempMsc[index] += (vend-vposition)*seg2->c[index]; /*pipe specie mass*/
			  seg->c[index] = TempMsc[index]/(vend-vstart); /*segment pipe surface specie concentration after segment movement*/
			  if (seg->c[index] < 0)
				  seg->c[index] = 0.0;
		  }	
		  vstart = vend;
		  if (vstart == v) break;	/*end of the pipe*/		  
	  }
   }
}


void  msremovesegs(int k)
/*
**-------------------------------------------------------------
**   Input:   k = link index     
**   Output:  none
**   Purpose: removes all segments in link k                                 
**-------------------------------------------------------------
*/
{
    Pmsseg seg;
    seg = FirstSeg[k];
    while (seg != NULL)
    {
        FirstSeg[k] = seg->prev;
        seg->prev = FreeSeg;
		seg->next = NULL;
        FreeSeg = seg;
        seg = FirstSeg[k];
    }
    LastSeg[k] = NULL;
}


void  msaddseg(int k, float v, float *msc)
/*
**-------------------------------------------------------------
**   Input:   k = link segment
**            v = segment volume
**            c = segment quality
**   Output:  none
**   Purpose: adds a segment to start of link k (i.e., upstream
**            of current last segment).
**-------------------------------------------------------------
*/
{
    Pmsseg seg;
	int i, j;
    float * segc;
    if (FreeSeg != NULL)
    {
       seg = FreeSeg;
       FreeSeg = seg->prev;
    }
    else
    {
        seg = (struct Smsseg *) Alloc(sizeof(struct Smsseg));
        segc = (float *)Alloc((Nspecies+1)*sizeof(float));
		seg->c = segc;
		if ((seg == NULL) || (segc == NULL))
        {
           OutOfMemory = TRUE;
           return;
        }     
    }
    seg->v = v;
    seg->prev = NULL;
	seg->next = NULL;

	if ( k < Nlinks + 1)
	{
		for ( i = 1; i < Nspecies+1; i++)
		{
			j = StoPS[i];
			if (j > 0)
				seg->c[j] = msc[i]; /*pipe specie segment quality*/
		}
	}
	else
	{
		for ( i = 1; i < Nspecies+1; i++)
		{
			j = StoTS[i];			/*tank specie segment quality*/
			if ( j > 0)
				seg->c[j] = msc[i];
		}
	}
		
    if (FirstSeg[k] == NULL) FirstSeg[k] = seg;
    if (LastSeg[k] != NULL) 
	{
		LastSeg[k]->prev = seg;//->prev : upstream
		seg ->next = LastSeg[k]; //->next: downstream
	}
    LastSeg[k] = seg;
}


void msaccumulate(long dt)
/*
**-------------------------------------------------------------
**   Input:   dt = current WQ time step
**   Output:  none
**   Purpose: accumulates mass flow at nodes 
**-------------------------------------------------------------
*/
{
   int    i,j,k,l;
   int    specieindex;
   float  *cseg,v,vseg;
   Pmsseg   seg;

   /* Re-set memory used to accumulate mass & volume */
   memset(VolIn,0,(Nnodes+1)*sizeof(float));
   for ( i = 1; i < Nnodes+1; i++)
   {
	   memset(MSMassIn[i],0,(Nbulkspecies+1)*sizeof(float));
	   memset(MSX[i], 0, (Nspecies+1)*sizeof(float));
   }
   /* Compute average conc. of segments adjacent to each node */
   /* (For use if there is no transport through the node) */
   for (k=1; k<=Nlinks; k++)
   {
      j = DOWN_NODE(k);             /* Downstream node */
      if (FirstSeg[k] != NULL)      /* Accumulate concentrations */
      {
	     for(l = 1; l < Nbulkspecies+1; l++)
		 {
	        specieindex = StoPS[l];
			MSMassIn[j][l] += FirstSeg[k]->c[specieindex];
		 }
         VolIn[j]++;
      }
      j = UP_NODE(k);              /* Upstream node */
      if (LastSeg[k] != NULL)      /* Accumulate concentrations */
      {
		 for(l = 1; l < Nbulkspecies+1; l++)
		 {
	        specieindex = StoPS[l];
			MSMassIn[j][l] += LastSeg[k]->c[specieindex];
		 }
         VolIn[j]++;
      }
   }
   for (k=1; k<=Nnodes; k++)
	   for ( l = 1; l < Nbulkspecies+1; l++)
		   if (VolIn[k] > 0.0) MSX[k][l] = MSMassIn[k][l]/VolIn[k];

   /* Move mass from first segment of each pipe into downstream node */
   memset(VolIn,0,(Nnodes+1)*sizeof(float));
   for ( i = 1; i < Nnodes+1; i++)
	   memset(MSMassIn[i],0,(Nbulkspecies+1)*sizeof(float));

   for (k=1; k<=Nlinks; k++)
   {
      i = UP_NODE(k);               /* Upstream node */
      j = DOWN_NODE(k);             /* Downstream node */
      v = ABS(Q[k])*dt;             /* Flow volume */
         
      /* If link volume < flow volume, then transport upstream    */
      /* quality to downstream node and remove all link segments. */
      if (LINKVOL(k) < v)
      {
         VolIn[j] += v;
         seg = FirstSeg[k];
         cseg = MSC[i];
         if (seg != NULL) cseg = seg->c;
		 for (l = 1; l < Nbulkspecies+1; l++)
		 {
            specieindex = StoPS[l];
			MSMassIn[j][l] += v*cseg[specieindex];
		 }
         msremovesegs(k);
      }

      /* Otherwise remove flow volume from leading segments */
      /* and accumulate flow mass at downstream node        */
      else while (v > 0.0)
      {
         /* Identify leading segment in pipe */
         seg = FirstSeg[k];
         if (seg == NULL) break;

         /* Volume transported from this segment is */
         /* minimum of flow volume & segment volume */
         /* (unless leading segment is also last segment) */
         vseg = seg->v;
         vseg = MIN(vseg,v);
         if (seg == LastSeg[k]) vseg = v;

         /* Update volume & mass entering downstream node  */
         cseg = seg->c;
         VolIn[j] += vseg;
		 for ( l = 1; l < Nbulkspecies+1; l++)
		 {
            specieindex = StoPS[l]; 
			MSMassIn[j][l] += vseg*cseg[specieindex];
		 }

         /* Reduce flow volume by amount transported */
         v -= vseg;

         /* If all of segment's volume was transferred, then */
         /* replace leading segment with the one behind it   */
         /* (Note that the current seg is recycled for later use.) */
         if (v >= 0.0 && vseg >= seg->v)
         {
            FirstSeg[k] = seg->prev;
            if (FirstSeg[k] == NULL) LastSeg[k] = NULL;
			else FirstSeg[k]->next = NULL;
            seg->prev = FreeSeg;
			seg->next = NULL;
            FreeSeg = seg;
         }

         /* Otherwise reduce segment's volume */
         else
         {
            seg->v -= vseg;
         }
      }     /* End while */
   }        /* Next link */
}


void msupdatenodes(long dt)
/*
**---------------------------------------------------------------------------
**   Input:   dt = current WQ time step     
**   Output:  none
**   Purpose: updates concentration at all nodes to mixture of accumulated
**            inflow from connecting pipes.
**
**  Note:     Does not account for source flow effects. MSX[i] contains
**            average concen. of segments adjacent to node i, used in case
**            there was no inflow into i.
**---------------------------------------------------------------------------
*/
{
   int i, j, k;

   /* Update junction quality */
   for (i = 1; i < Njuncs+1; i++)
   {
      if (D[i] < 0.0) VolIn[i] -= D[i]*dt;
      if (VolIn[i] > 0.0)
	  {
		  for ( j = 1; j < Nbulkspecies+1; j++)
			  MSC[i][j] = MSMassIn[i][j]/VolIn[i];
	  }
      else    
	  {
		  for ( j = 1; j < Nbulkspecies+1; j++)
			  MSC[i][j] = MSX[i][j];
	  }

	  for ( k = 1; k < Npipespecies+1; k++)
		  MSC[i][k] = MSC[i][k]*MSUcf[QUALITY];
	  /* Algebraic equations solver*/
	  algebraicSolution(PIPES, MSC[i]);
	  for ( k = 1; k < Npipespecies+1; k++)
		  MSC[i][k] = MSC[i][k]/MSUcf[QUALITY];
   }

   /* Update tank quality */
   msupdatetanks(dt);

}


void mssourceinput(long dt)
/*
**---------------------------------------------------------------------
**   Input:   dt = current WQ time step     
**   Output:  none
**   Purpose: computes contribution (if any) of mass additions from WQ
**            sources at each node.
**---------------------------------------------------------------------
*/
{
   int   i,j,n, specieindex;
   float massadded, s, volout;
   float qout, qcutoff;
   Pmsource mssource;

//   if (Qualflag != MULTI) return;

   /* Establish a flow cutoff which indicates no outflow from a node */
   qcutoff = 10.0*TINY;

   /* Zero-out the work array MSX */
   for ( i = 1; i < Nnodes+1; i++)
	   memset(MSX[i],0,(Nbulkspecies+1)*sizeof(float));

   /* Consider each node */
   for (n=1; n < Nnodes+1; n++)
   {
	   /*Consider each specie source*/
	   for (mssource = NodeSource[n]; mssource != NULL; mssource = mssource->next)
	   {
		   if (mssource->C0 ==0) continue;
		   specieindex = mssource->index;
		   
		   /* Find total flow volume leaving node */
		   if (n <= Njuncs) volout = VolIn[n];  /* Junctions */
		   else volout = VolIn[n] - D[n]*dt;    /* Tanks */
		   qout = volout / (float) dt;
		   /* Evaluate source input only if node outflow > cutoff flow */
		   if (qout > qcutoff)
		   {
			   /* Mass added depends on type of source */
			   s = mssourcequal(mssource);
			   switch(mssource->Type)
			   {
				   /* Concen. Source: */
                   /* Mass added = source concen. * -(demand) */
                   case CONCEN:

                   /* Only add source mass if demand is negative */
                   if (D[n] < 0.0)
				   {
					   massadded = -s*D[n]*dt;

                       /* If node is a tank then set concen. to 0. */
                       /* (It will be re-set to true value in msupdatesourcenodes()) */
                       if (n > Njuncs) MSC[n][specieindex] = 0.0;
				   }
                   else massadded = 0.0;
                   break;
				   /* Mass Inflow Booster Source: */
                   case MASS:
				   massadded = s*dt;
                   break;
					   
				   /* Setpoint Booster Source: */
                   /* Mass added is difference between source */
                   /* & node concen. times outflow volume  */
                   case SETPOINT:
                   if (s > MSC[n][specieindex]) massadded = (s-MSC[n][specieindex])*volout;
                   else massadded = 0.0;
                   break;

                  /* Flow-Paced Booster Source: */
                  /* Mass added = source concen. times outflow volume */
                  case FLOWPACED:
                  massadded = s*volout;
                  break;
			   }

			   /* Source concen. contribution = (mass added / outflow volume) */
			   MSX[n][specieindex] = massadded/volout;

			   /* Update total mass added for time period & simulation */
			   mssource->Smass += massadded;
			 //  if (Htime >= Rstart) Wsource += massadded;
		   }
	   }
   }

   /* Add mass inflows from reservoirs to Wsource*/
   if (Htime >= Rstart)
   {
      for (j=1; j<=Ntanks; j++)
      {
         if (Tank[j].A == 0.0)
         {
            n = Njuncs + j;
            volout = VolIn[n] - D[n]*dt;
//            if (volout > 0.0) Wsource += volout*C[n];
         }
      }
   }
}

void msrelease(long dt)
/*
**---------------------------------------------------------
**   Input:   dt = current WQ time step
**   Output:  none
**   Purpose: creates new segments in outflow links from nodes.
**---------------------------------------------------------
*/
{
   int    i,k,n, index;
   int    flag;
   float  q,v,cscal;
   Pmsseg   seg;

   /* Examine each link */
   for (k=1; k<=Nlinks; k++)
   {

      /* Ignore links with no flow */
      if (Q[k] == 0.0) continue;

      /* Find flow volume released to link from upstream node */
      /* (NOTE: Flow volume is allowed to be > link volume.) */
      n = UP_NODE(k);
      q = ABS(Q[k]);
      v = q*dt;

      /* Include source contribution in quality released from node. */
	  for ( i = 1; i < Nbulkspecies+1; i++)
	  {
		  TempMsc[i] = MSC[n][i] + MSX[n][i];
	  }

	  flag = 0;	
      /* If link has a last seg, check if its quality     */
      /* differs from that of the flow released from node.*/
      /* This difference is measured relative to a weight */
      /* that is the maximum of the current segment conc. */
      /* and a component-specific absolute error tolerance*/
      if ( (seg = LastSeg[k]) != NULL)
      {
		  for ( i = 1; i < Nbulkspecies+1; i++) /*bulk species*/
		  {
			  if (flag == 1) break;
			  index = StoPS[i];
              cscal = fabs(seg->c[index]);
              if(cscal < PSabstol[index]) cscal = PSabstol[index];
			  if (fabs( TempMsc[i] - seg->c[index] ) > MSreltol*cscal) flag = 1;
		  }
		  for ( i = 1; i < Npipesurfacespecies+1; i++) /*pipe surface species*/
		  {
			  if (flag == 1) break;
			  index = StoPS[i+Nbulkspecies];
              cscal = fabs(seg->c[index]);
              if(cscal < PSabstol[index]) cscal = PSabstol[index];
			  if (fabs(seg->c[index]-TempSurfaceSpecie[k][i]) > MSreltol*cscal)
				  flag = 1;
		  }
		  if (flag == 1) /*difference in one or more specie concentrations is large enough*/
		  {
			  msaddseg(k,v,TempMsc);
			  
			  /* Update surface species qualities in upstream segments. */
			  for ( i = 1; i < Npipesurfacespecies+1; i++)
			  {
				  index = StoPS[i+Nbulkspecies];
				  LastSeg[k]->c[index] = TempSurfaceSpecie[k][i];
			  }
		  }
		  else
			  seg->v += v;
      }
      /* If link has no segs then add a new one. */
      else 
	  {
		  msaddseg(k,LINKVOL(k),TempMsc);

		  /* Update surface species qualities in newly generated upstream segments. */
	  	  for ( i = 1; i < Npipesurfacespecies+1; i++)
		  {
			  index = StoPS[i+Nbulkspecies];
			  LastSeg[k]->c[index] = TempSurfaceSpecie[k][i];
		  }
	  }
   }//end links
}

void masstransfer(int k) /*mass transfer coefficient for pipe k*/
/*
**---------------------------------------------------
**   Input:   k = link index     
**   Output:  none
**   Purpose: updates specie mass transfer coefficients 
**            under current hydraulic condition
**---------------------------------------------------
*/
{
   float a,d,u,y,re,sh,sc, diffu;
   int i;

   d = Link[k].Diam;                    /* Pipe diameter, ft */
/* Compute Reynolds No. */
   a = PI*d*d/4.0;
   u = ABS(Q[k])/a;
   CurrentSegVelocity = u*MSUcf[LENGTH]/MSUcf[TIME];
   re = u*d/Viscos;
   CurrentReynolds = re;

   for ( i = 1; i < Ncoefficients+1; i++)
   {
	   diffu = Coefficient[i].diffusivity;
	   sc = Viscos/diffu;
	   /* Ignore mass transfer if Schmidt No. is 0 */
	   if (sc == 0.0)
	   {
		   CurrentMasstracoeff[i] = 0;
		   break;
	   }
	   /* Compute Sherwood No. for stagnant flow  */
	   /* (mass transfer coeff. = Diffus./radius) */
	   if (re < 1.0) sh = 2.0;
	   /* Compute Sherwood No. for turbulent flow *
	   /* using the Notter-Sleicher formula.      */
	   else if (re >= 2300.0)
		   sh = 0.0149*pow(re,0.88)*pow(sc,0.333);
	   /* Compute Sherwood No. for laminar flow *
	   /* using Graetz solution formula.        */
	   else
	   {
		   y = d/Link[k].Len*re*sc;
		   sh = 3.65+0.0668*y/(1.0+0.04*pow(y,0.667));
	   }
	   /* Compute mass transfer coeff. (in ft/sec) */
	   CurrentMasstracoeff[i] = sh*diffu/d;
	   CurrentMasstracoeff[i] = CurrentMasstracoeff[i]/(MSUcf[TIME]*MSUcf[LENGTH]*MSUcf[LENGTH]);
   }
}

void  msupdatesourcenodes(long dt)
/*
**---------------------------------------------------
**   Input:   dt = current WQ time step     
**   Output:  none
**   Purpose: updates quality at source nodes.
**            (MSX[n] = concen. added by source at node n)
**---------------------------------------------------
*/
{
   int i,j,k,n;
   Pmsource mssource;

//   if (Qualflag != CHEM) return;

   /* Examine each WQ source node */
   for (n=1; n<=Nnodes; n++)
   {
      /*Consider each specie source*/ 
      for (mssource = NodeSource[n]; mssource != NULL; mssource=mssource->next)
	  {
		  j = mssource->index;
		  /* Add source to current node concen. */
		  MSC[n][j] += MSX[n][j];
		  /* For tanks, node concen. = internal concen. */
		  if (n > Njuncs)
		  {
			  i = n - Njuncs;
//			  if (Tank[i].A > 0.0) MSC[n][j] = Tank[i].msC[j];
		  }
		  /* Normalize mass added at source to time step */
		  mssource->Smass /= (float)dt;	  
	  }
	  /* Algebraic species qualities update */
	  if (NodeSource[n] != NULL)
	  {
		  if ( n > Njuncs)
		  {
			  for ( k = 1; k < Ntankspecies+1; k++)
				  MSC[n][k] = MSC[n][k]*MSUcf[QUALITY];
			  algebraicSolution(TANKS, MSC[n]);
			  for ( k = 1; k < Ntankspecies+1; k++)
				  MSC[n][k] = MSC[n][k]/MSUcf[QUALITY];
		  }
		  else
		  {
			  for ( k = 1; k < Npipespecies+1; k++)
				  MSC[n][k] = MSC[n][k]*MSUcf[QUALITY];
			  algebraicSolution(PIPES, MSC[n]);
			  for ( k = 1; k < Npipespecies+1; k++)
				  MSC[n][k] = MSC[n][k]/MSUcf[QUALITY];
		  }
	  }
   }
}


void  msupdatetanks(long dt)
/*
**---------------------------------------------------
**   Input:   dt = current WQ time step     
**   Output:  none
**   Purpose: updates tank volumes & concentrations            
**---------------------------------------------------
*/
{
    int   i;

   /* Examine each reservoir & tank */
   for (i=1; i<=Ntanks; i++)
   {
	   CurrentSegDiam = sqrt(Tank[i].A/PI)/6.0;
	   CurrentSegVelocity = 0.0;
	   CurrentReynolds = 0.0;

      /* Update tank WQ based on mixing model */
      if (Tank[i].A > 0)
	  {
		  switch(Tank[i].MixModel)
		  {
			  //	case MIX2: tankmix2(i,dt); break;
			  //         case FIFO: tankmix3(i,dt); break;
			  //         case LIFO: tankmix4(i,dt); break;
				default:   mstankmix1(i,dt); break;
		  }
      }
   }
}


void  mstankmix1(int i, long dt)
/*
**---------------------------------------------
**   Input:   i = tank index
**            dt = current WQ time step     
**   Output:  none
**   Purpose: complete mix tank model                     
**---------------------------------------------
*/
{
    int   n, j, k, index;
	Pmsseg seg;

	k = Nlinks + i;
	seg = LastSeg[k];
    memset(Cin, 0, (Nspecies+1)*sizeof(float));

    n = Tank[i].Node;
    if (VolIn[n] > 0.0) 
	{
		for ( j = 1; j < Nbulkspecies+1; j++)
			Cin[j] = MSMassIn[n][j]/VolIn[n];
	}

	/* Update tank segment qualities*/
    if (Tank[i].V > 0.0)mstankreact(i, seg->c, seg->v, dt);
	for ( j = 1; j < Ntankspecies+1; j++)
	{
		index = TStoS[j];
		MSC[i+Njuncs][j] = seg->c[j];	
	}

	/* Consider incoming flow */
	for ( j = 1; j < Nbulkspecies+1; j++) /*for bulk species*/
	{
		MSC[i+Njuncs][j] += ((Cin[j] - MSC[i+Njuncs][j])*VolIn[n]/Tank[i].V);
	}

	for ( j = 1; j < Ntankspecies+1; j++)
	{
		index = TStoS[j];
		seg->c[j] = MSC[i+Njuncs][index];
	}
	

	for ( j = 1; j < Ntankspecies+1; j++)
	{
		index = TStoS[j];
		if (Specie[index].MSQtype == VOLUMEUNIT)
			MSC[i][j] = MSC[i][j]*MSUcf[QUALITY];
		else
			MSC[i][j] = MSC[i][j]*MSUcfAreaq;
	}
	/* Equllibrium*/
	algebraicSolution(TANKS, MSC[i]);
	for ( j = 1; j < Ntankspecies+1; j++)
	{
		index = TStoS[j];
		if (Specie[index].MSQtype == VOLUMEUNIT)
			MSC[i][j] = MSC[i][j]/MSUcf[QUALITY];
		else 
			MSC[i][j] = MSC[i][j]/MSUcfAreaq;
	}


   /* Update tank volume */
    Tank[i].V += D[n]*dt;
	seg->v = Tank[i].V;
}

void  mstankmix2(int i, long dt)
/*
**------------------------------------------------
**   Input:   i = tank index
**            dt = current WQ time step     
**   Output:  none
**   Purpose: 2-compartment tank model                      
**            (seg1 = mixing zone,
**             seg2 = ambient zone)      
**------------------------------------------------
*/
{
    int     j, k,n;
    long    tstep, tstar;
    float   cin,        /* Inflow quality */
            qin,        /* Inflow rate */
            qout,       /* Outflow rate */
            qnet;       /* Net flow rate */
   Pmsseg     seg1,seg2;  /* Compartment segments */

   /* Identify segments for each compartment */
   k = Nlinks + i;
   seg1 = LastSeg[k];
   seg2 = FirstSeg[k];
   if (seg1 == NULL || seg2 == NULL) return;

   /* Find inflows & outflows */
   n = Tank[i].Node;
   qnet = D[n];
   qin = VolIn[n]/(float)dt;
   qout = qin - qnet;
   
   if (qin > 0.0) 
   {
	   for ( j = 1; j < Nbulkspecies+1; j++)
		   Cin[j] = MSMassIn[n][j]/VolIn[n];
   }
   else           
   {
	   for ( j = 1; j < Nbulkspecies+1; j++)
		   Cin[j] = 0;
   }   
   Tank[i].V += qnet*dt;

   /* Case of no net volume change */
//   if (ABS(qnet) < TINY)
//   {
//      seg1->c = tankreact(seg1->c,seg1->v,Tank[i].Kb,dt);
//      seg2->c = tankreact(seg2->c,seg2->v,Tank[i].Kb,dt);
//   }

   /* Case of net filling (qnet > 0) */
//   else if (qnet > 0.0)
//   {

      /* Case where ambient zone empty & mixing zone filling */
//      if (seg2->v <= 0.0)
//      {
//         tstar = (long) ((Tank[i].V1max-(seg1->v))/qnet);
//         tstep = MIN(dt, tstar);
//         if (seg1->v > 0.0)
//            seg1->c = tankreact(seg1->c,seg1->v,Tank[i].Kb,tstep) +
//                       qin*tstep*(cin-(seg1->c))/(seg1->v);
//         else seg1->c = cin;
//         seg1->c = MAX(0.0, seg1->c);
//         seg1->v += qnet*tstep;
//         seg2->c = 0.0;
//         dt -= tstep;
//      }

      /* Case where mixing zone full & ambient zone filling */
//      if (dt > 1)
//      {
//         seg1->c = tankreact(seg1->c,seg1->v,Tank[i].Kb,dt) +
//                     qin*dt*(cin-(seg1->c))/(seg1->v);
//         seg1->c = MAX(0.0, seg1->c);
//         if (seg2->v <= 0.0) seg2->c = seg1->c;
//         else
//            seg2->c = tankreact(seg2->c,seg2->v,Tank[i].Kb,dt) +
//                       qnet*dt*((seg1->c)-(seg2->c))/(seg2->v);
//         seg2->c = MAX(0.0, seg2->c);
//         seg2->v += qnet*dt;
//       }
//   }

   /* Case of net emptying (qnet < 0) */
//   else if (qnet < 0.0 && seg1->v > 0.0)
//   {

      /* Case where mixing zone full & ambient zone draining */
//      if ((seg2->v) > 0.0)
//      {
//         tstar = (long)(seg2->v/-qnet);
//         tstep = MIN(dt, tstar);
//         seg1->c = tankreact(seg1->c,seg1->v,Tank[i].Kb,tstep) +
//                    (qin*cin - qnet*(seg2->c) - qout*(seg1->c))*
//                    tstep/(seg1->v);
//         seg2->c = tankreact(seg2->c,seg2->v,Tank[i].Kb,tstep);
//         seg2->v += qnet*tstep;
//         seg1->c = MAX(0.0, seg1->c);
//         seg2->c = MAX(0.0, seg2->c);
//         seg2->v = MAX(0.0, seg2->v);
//         dt -= tstep;
//      }

      /* Case where ambient zone empty & mixing zone draining */
//      if (dt > 1)
//      {
//         seg1->c = tankreact(seg1->c,seg1->v,Tank[i].Kb,dt) +
//                    qin*dt*(cin-(seg1->c))/(seg1->v);
//         seg1->v += qnet*dt;
//         seg1->c = MAX(0.0, seg1->c);
//         seg1->v = MAX(0.0, seg1->v);
//         seg2->c = 0.0;
//      }
//   }

   /* Use quality of mixed compartment (seg1) to */
   /* represent quality of tank since this is where */
   /* outflow begins to flow from */
//   Tank[i].C = seg1->c;
//   C[n] = Tank[i].C;
}


void  mstankmix3(int i, long dt)
/*
**----------------------------------------------------------
**   Input:   i = tank index
**            dt = current WQ time step     
**   Output:  none
**   Purpose: First-In-First-Out (FIFO) tank model                    
**----------------------------------------------------------
*/
{
   int   k,n;
   float vin,vnet,vout,vseg;
   float cin,vsum,csum;
   Pmsseg  seg;

   k = Nlinks + i;
   if (LastSeg[k] == NULL || FirstSeg[k] == NULL) return;

   /* React contents of each compartment */
//   if (Reactflag)
//   {
//      seg = FirstSeg[k];
//      while (seg != NULL)
//      {
//         seg->c = tankreact(seg->c,seg->v,Tank[i].Kb,dt);
//         seg = seg->prev;
//      }
//   }

   /* Find inflows & outflows */
/*
   n = Tank[i].Node;
   vnet = D[n]*dt;
   vin = VolIn[n];
   vout = vin - vnet;
   if (vin > 0.0) cin = MSMassIn[n]/VolIn[n];
   else           cin = 0.0;
   Tank[i].V += vnet;

   /* Withdraw flow from first segment */
/*
   vsum = 0.0;
   csum = 0.0;
   while (vout > 0.0)
   {
      seg = FirstSeg[k];
      if (seg == NULL) break;
      vseg = seg->v;           /* Flow volume from leading seg */
/*      vseg = MIN(vseg,vout);
      if (seg == LastSeg[k]) vseg = vout;
      vsum += vseg;
      csum += (seg->c)*vseg;
      vout -= vseg;            /* Remaining flow volume */
/*      if (vout >= 0.0 && vseg >= seg->v)  /* Seg used up */
//      {
//         FirstSeg[k] = seg->prev;
/*         if (FirstSeg[k] == NULL) LastSeg[k] = NULL;
         seg->prev = FreeSeg;
         FreeSeg = seg;
      }
      else                /* Remaining volume in segment */
/*      {
         seg->v -= vseg;
      }
   }

   /* Use quality withdrawn from 1st segment */
   /* to represent overall quality of tank */
/*   if (vsum > 0.0) Tank[i].C = csum/vsum;
   else            Tank[i].C = FirstSeg[k]->c;
   C[n] = Tank[i].C;

   /* Add new last segment for new flow entering tank */
/*   if (vin > 0.0)
   {
      if ( (seg = LastSeg[k]) != NULL)
      {
         /* Quality is the same, so just add flow volume to last seg */
/*         if (ABS(seg->c - cin) < Ctol) seg->v += vin;

         /* Otherwise add a new seg to tank */
/*         else addseg(k,vin,cin);
      }

      /* If no segs left then add a new one. */
/*      else addseg(k,vin,cin);
   }
*/
}   


void  mstankmix4(int i, long dt)
/*
**----------------------------------------------------------
**   Input:   i = tank index
**            dt = current WQ time step     
**   Output:  none
**   Purpose: Last In-First Out (LIFO) tank model                     
**----------------------------------------------------------
*/
{
   int   k, n;
   float vin, vnet, cin, vsum, csum, vseg;
   Pmsseg  seg, tmpseg;

   k = Nlinks + i;
   if (LastSeg[k] == NULL || FirstSeg[k] == NULL) return;

   /* React contents of each compartment */
/*
   if (Reactflag)
   {
      seg = LastSeg[k];
      while (seg != NULL)
      {
         seg->c = tankreact(seg->c,seg->v,Tank[i].Kb,dt);
         seg = seg->prev;
      }
   }

   /* Find inflows & outflows */
/*
   n = Tank[i].Node;
   vnet = D[n]*dt;
   vin = VolIn[n];
   if (vin > 0.0) cin = MSMassIn[n]/VolIn[n];
   else           cin = 0.0;
   Tank[i].V += vnet;
   Tank[i].C = LastSeg[k]->c;

   /* If tank filling, then create new last seg */ 
/*
   if (vnet > 0.0)
   {
      if ( (seg = LastSeg[k]) != NULL)
      {
         /* Quality is the same, so just add flow volume to last seg */
//         if (ABS(seg->c - cin) < Ctol) seg->v += vnet;

         /* Otherwise add a new last seg to tank */
         /* which points to old last seg */ 
/*         else
         {
            tmpseg = seg;
            LastSeg[k] = NULL;
            addseg(k,vnet,cin);
            LastSeg[k]->prev = tmpseg;
         }
      }

      /* If no segs left then add a new one. */
 //     else addseg(k,vnet,cin);

      /* Update reported tank quality */
 /*     Tank[i].C = LastSeg[k]->c;
   }

   /* If net emptying then remove last segments until vnet consumed */
/*   else if (vnet < 0.0)
   {
      vsum = 0.0;
      csum = 0.0;
      vnet = -vnet;
      while (vnet > 0.0)
      {
         seg = LastSeg[k];
         if (seg == NULL) break;
         vseg = seg->v;
         vseg = MIN(vseg,vnet);
         if (seg == FirstSeg[k]) vseg = vnet;
         vsum += vseg;
         csum += (seg->c)*vseg;
         vnet -= vseg;
         if (vnet >= 0.0 && vseg >= seg->v)  /* Seg used up */
/*         {
            LastSeg[k] = seg->prev;
            if (LastSeg[k] == NULL) FirstSeg[k] = NULL;
            seg->prev = FreeSeg;
            FreeSeg = seg;
         }
         else                /* Remaining volume in segment */
/*         {
            seg->v -= vseg;
         }
      }
      /* Reported tank quality is mixture of flow released and any inflow */
/*      Tank[i].C = (csum + MSMassIn[n])/(vsum + vin);
   }
   C[n] = Tank[i].C;
   */
}         


float  mssourcequal(Pmsource mssource)
/*
**-------------------------------------------------------------------
**   Input:   j = source index
**   Output:  returns source WQ values
**   Purpose: determines source concentrations in current time period  
**-------------------------------------------------------------------
*/
{
   int   i;
   long  k;
   float c;

   /* Get source concentration (or mass flow) in original units */
   c = mssource->C0;

   /* Convert mass flow rate from min. to sec. */
   /* and convert concen. from liters to cubic feet */
   if (mssource->Type == MASS) c /= 60.0;
   else c /= Ucf[QUALITY];

   /* Apply time pattern if assigned */
   i = mssource->Pat;
   if (i == 0) return(c);
   k = ((Qtime+Pstart)/Pstep) % (long)Pattern[i].Length;
   return(c*Pattern[i].F[k]);
}


float  msavgqual(int k, int sindex)
/*
**----------------------------------------------------------------
**   Input:   k = link index
			  s = specie index
**   Output:  returns WQ value
**   Purpose: computes average quality in link k for specie sindex                      
**----------------------------------------------------------------
*/
{
	float  vsum = 0.0, msum = 0.0;
	Pmsseg   seg;
	int index;
	index = StoPS[sindex];
	if (Qualflag == NONE) return(0.);
	
	seg = FirstSeg[k];
	while (seg != NULL)
	{
       vsum += seg->v;	/* Segment volume sum */
       msum += (seg->c[index])*(seg->v); /* Segment mass sum */	
       seg = seg->prev;
	}
	if (vsum > 0.0) 
		return(msum/vsum); /* Segment average qualities */
	else		/* Link with no water */ 
		return( (MSC[Link[k].N1][sindex] + MSC[Link[k].N2][sindex])/2. );	
}


void  mspipereact(int k, float *msc, float v, long dt)
/*
**------------------------------------------------------------
**   Input:   k = link index
**            msc = current WQ values in segment
**            v = segment volume
**            dt = time step
**   Output:  msc = new WQ values
**   Purpose: computes new quality in a pipe segment after
**            reaction occurs              
**------------------------------------------------------------
*/
{
	
   int i, index;
   float dtfloat;

   dtfloat = (float) dt;
   CurrentPara = LinkParameters[k];

   for ( i = 1; i < Npipespecies+1; i++)
   {
	   index = PStoS[i];
	   TempMsc[i] = msc[i];
	   if (Specie[index].MSQtype == VOLUMEUNIT)
		   msc[i] = msc[i]*MSUcf[QUALITY];
	   else
		   msc[i] = msc[i]*MSUcfAreaq;
   }

   v = v*MSUcf[VOLUME];
   dtfloat = dtfloat*MSUcf[TIME];
   /* Algebraic equations solver*/
   algebraicSolution(PIPES, msc);
   /* Differential equations solver*/
   differentialSolution(PIPES, msc, v, dtfloat);


  for ( i = 1; i < Npipespecies+1; i++)
  {
	   index = PStoS[i];
	   if (Specie[index].MSQtype == VOLUMEUNIT)
		   msc[i] = msc[i]/MSUcf[QUALITY];
	   else
		   msc[i] = msc[i]/MSUcfAreaq;
  }
   v = v/MSUcf[VOLUME];
   dt = dt/MSUcf[TIME];

   /* Update cumulative mass reacted */
   if (Htime >= Rstart)
   {
		for (i = 1; i < Npipespecies+1; i++)
		{
			index = PStoS[i];
			if (Specie[index].MSQtype == VOLUMEUNIT)
				MSMassReact[index] += (msc[i]-TempMsc[i])*v;
			else
				MSMassReact[index] += (msc[i]-TempMsc[i])*v/(0.25*Link[k].Diam);
		}
   }   
}


void  mstankreact(int k, float * msc, float v, long dt)
/*
**-------------------------------------------------------
**   Input:   msc = current WQ in tank
**            v = tank volume
**            k = tank index
**            dt = time step
**   Output:  returns new WQ value
**   Purpose: computes new quality in a tank after
**            reaction occurs
**-------------------------------------------------------
*/
{	
   int i, index;
   float dtfloat;

   dtfloat = (float) dt;
   CurrentPara = TankParameters[k];

   for ( i = 1; i < Ntankspecies+1; i++)
   {
	   index = TStoS[i];
	   TempMsc[i] = msc[i];
	   if (Specie[index].MSQtype == VOLUMEUNIT)
		   msc[i] = msc[i]*MSUcf[QUALITY];
	   else
		   msc[i] = msc[i]*MSUcfAreaq;
   }
   v = v*MSUcf[VOLUME];
   dtfloat = dtfloat*MSUcf[TIME];

   algebraicSolution(TANKS, msc);
   differentialSolution(TANKS, msc, v, dtfloat);
   for ( i = 1; i < Ntankspecies+1; i++)
   {
	   index = TStoS[i];
	   if (Specie[index].MSQtype == VOLUMEUNIT)
		   msc[i] = msc[i]/MSUcf[QUALITY];
	   else 
		   msc[i] = msc[i]/MSUcfAreaq;
   }
   v = v/MSUcf[VOLUME];
   dtfloat = dtfloat/MSUcf[TIME];

   /* Update cumulative mass reacted */
   if (Htime >= Rstart)
   {
		for (i = 1; i < Ntankspecies+1; i++)
		{
			index = TStoS[i];
			if (Specie[index].MSQtype == VOLUMEUNIT)
				MSMassReact[index] += (msc[i]-TempMsc[i])*v;
			else
				MSMassReact[index] += (msc[i]-TempMsc[i])*v/(0.5*sqrt(Tank[k].A/PI));
		}
   }   

}
/************************* End of MSQUALITY.C ***************************/
