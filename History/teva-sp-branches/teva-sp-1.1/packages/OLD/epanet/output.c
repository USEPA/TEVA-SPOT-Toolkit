/*
*********************************************************************
                                                                   
OUTPUT.C -- Binary File Transfer Routines for EPANET Program                
                                                                    
VERSION:    2.00                                              
DATE:       5/8/00
AUTHOR:     L. Rossman
            US EPA - NRMRL
                                                                    
********************************************************************
*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "text.h"
#include "types.h"
#include "funcs.h"
#define  EXTERN  extern
#include "hash.h"
#include "vars.h"

/* Macro to write x[1] to x[n] to file OutFile: */
#define   FSAVE(n)  (fwrite(x+1,sizeof(float),(n),OutFile))

int  savenetdata()
/*
**---------------------------------------------------------------
**   Input:   none
**   Output:  returns error code
**   Purpose: saves input data in original units to binary
**            output file using fixed-sized (4-byte) records
**---------------------------------------------------------------
*/
{
   int   i,nmax;
   INT4  *ibuf;
   float *x;
   int   errcode = 0;

   /* Allocate buffer arrays */
   nmax = MAX(Nnodes,Nlinks) + 1;
   nmax = MAX(nmax,15);
   ibuf = (INT4 *) calloc(nmax, sizeof(INT4));
   x = (float *) calloc(nmax, sizeof(float));
   ERRCODE(MEMCHECK(ibuf));
   ERRCODE(MEMCHECK(x));

   if (!errcode)
   {
      /* Write integer variables to OutFile */
      ibuf[0] = MAGICNUMBER;
      ibuf[1] = VERSION;
      ibuf[2] = Nnodes;
      ibuf[3] = Ntanks;
      ibuf[4] = Nlinks;
      ibuf[5] = Npumps;
      ibuf[6] = Nvalves;
      ibuf[7] = Qualflag;
      ibuf[8] = TraceNode;
      ibuf[9] = Flowflag;
      ibuf[10] = Pressflag;
      ibuf[11] = Tstatflag;
      ibuf[12] = Rstart;
      ibuf[13] = Rstep;
      ibuf[14] = Dur;
      fwrite(ibuf,sizeof(INT4),15,OutFile);

      /* Write string variables to OutFile */
      fwrite(Title[0],sizeof(char),MAXMSG+1,OutFile);
      fwrite(Title[1],sizeof(char),MAXMSG+1,OutFile);
      fwrite(Title[2],sizeof(char),MAXMSG+1,OutFile);
      fwrite(InpFname,sizeof(char),MAXFNAME+1,OutFile);
      fwrite(Rpt2Fname,sizeof(char),MAXFNAME+1,OutFile);
      fwrite(ChemName,sizeof(char),MAXID+1-16,OutFile);
      fwrite(Field[QUALITY].Units,sizeof(char),MAXID+1-16,OutFile);

      /* Write node ID information to OutFile */
      for (i=1; i<=Nnodes; i++)
         fwrite(Node[i].ID, MAXID+1-16, 1, OutFile);

      /* Write link information to OutFile            */
      /* (Note: first transfer values to buffer array,*/
      /* then fwrite buffer array at offset of 1 )    */
      for (i=1; i<=Nlinks; i++)
         fwrite(Link[i].ID, MAXID+1-16, 1, OutFile);
      for (i=1; i<=Nlinks; i++) ibuf[i] = Link[i].N1;
      fwrite(ibuf+1,sizeof(INT4),Nlinks,OutFile);
      for (i=1; i<=Nlinks; i++) ibuf[i] = Link[i].N2;
      fwrite(ibuf+1,sizeof(INT4),Nlinks,OutFile);
      for (i=1; i<=Nlinks; i++) ibuf[i] = Link[i].Type;
      fwrite(ibuf+1,sizeof(INT4),Nlinks,OutFile);

      /* Write tank information to OutFile.*/
      for (i=1; i<=Ntanks; i++) ibuf[i] = Tank[i].Node;
      fwrite(ibuf+1,sizeof(INT4),Ntanks,OutFile);
      for (i=1; i<=Ntanks; i++) x[i] = Tank[i].A;
      FSAVE(Ntanks);

      /* Save node elevations to OutFile.*/
      for (i=1; i<=Nnodes; i++) x[i] = Node[i].El*Ucf[ELEV];
      FSAVE(Nnodes);

      /* Save link lengths & diameters to OutFile.*/
      for (i=1; i<=Nlinks; i++) x[i] = Link[i].Len*Ucf[ELEV];
      FSAVE(Nlinks);
      for (i=1; i<=Nlinks; i++)
      {
         if (Link[i].Type != PUMP)
            x[i] = Link[i].Diam*Ucf[DIAM];
         else
            x[i] = 0.0;
      }
      if (FSAVE(Nlinks) < (unsigned)Nlinks) errcode = 308;
   }

   /* Free memory used for buffer arrays */
   free(ibuf);
   free(x);
   return(errcode);
}


int  savehyd(long *htime)
/*
**--------------------------------------------------------------
**   Input:   *htime   = current time                             
**   Output:  returns error code
**   Purpose: saves current hydraulic solution to file HydFile    
**            in binary format                                    
**--------------------------------------------------------------
*/
{
   int i;
   INT4 t;
   int errcode = 0;

   /* Save current time (htime) */
   t = *htime;
   fwrite(&t,sizeof(INT4),1,HydFile);

   /* Save current nodal demands (D) */
   fwrite(D+1,sizeof(float),Nnodes,HydFile);

   /* Copy heads (H) to buffer of floats (X) and save buffer */
   for (i=1; i<=Nnodes; i++) X[i] = H[i];
   fwrite(X+1,sizeof(float),Nnodes,HydFile);

   /* Force flow in closed links to be zero then save flows */
   for (i=1; i<=Nlinks; i++)
   {
      if (S[i] <= CLOSED) X[i] = 0.0;
      else X[i] = Q[i];
   }
   fwrite(X+1,sizeof(float),Nlinks,HydFile);

   /* Copy link status to buffer of floats (X) & write buffer */
   for (i=1; i<=Nlinks; i++) X[i] = S[i];
   fwrite(X+1,sizeof(float),Nlinks,HydFile);

   /* Save link settings & check for successful write-to-disk */
   /* (We assume that if any of the previous fwrites failed,  */
   /* then this one will also fail.) */
   if (fwrite(K+1,sizeof(float),Nlinks,HydFile) < (unsigned)Nlinks)
      errcode = 308;
   return(errcode);
}                        /* End of savehyd */


int  savehydstep(long *hydstep)
/*
**--------------------------------------------------------------
**   Input:   *hydstep = next time step                           
**   Output:  returns error code
**   Purpose: saves next hydraulic timestep to file HydFile    
**            in binary format                                    
**--------------------------------------------------------------
*/
{
   INT4 t;
   int errcode = 0;
   t = *hydstep;
   if (fwrite(&t,sizeof(INT4),1,HydFile) < 1) errcode = 308;
   if (t == 0) fputc(EOFMARK, HydFile);
   return(errcode);
}


int  saveenergy()
/*
**--------------------------------------------------------------
**   Input:   none                             
**   Output:  returns error code
**   Purpose: saves energy usage by each pump to OutFile    
**            in binary format                                    
**--------------------------------------------------------------
*/
{
    int   i,j;
    INT4  index;
    float x[6];             /* work array */
    float hdur,             /* total duration in hours */
          t;                /* pumping duration */

    hdur = (double) Dur / 3600.0;
    for (i=1; i<=Npumps; i++)
    {
        if (hdur == 0.0)
        {
            for (j=0; j<5; j++) x[j] = Pump[i].Energy[j];
            x[5] = Pump[i].Energy[5]*24.0;
        }
        else
        {
            t = Pump[i].Energy[0];
            x[0] = t/hdur;
            x[1] = 0.0;
            x[2] = 0.0;
            x[3] = 0.0;
            x[4] = 0.0;
            if (t > 0.0)
            {
                x[1] = Pump[i].Energy[1]/t;
                x[2] = Pump[i].Energy[2]/t;
                x[3] = Pump[i].Energy[3]/t;
            }
            x[4] = Pump[i].Energy[4];
            x[5] = Pump[i].Energy[5]*24.0/hdur;
        }
        x[0] *= 100.0;
        x[1] *= 100.0;
        /* Compute Kw-hr per MilGal (or per cubic meter) */
        if (Unitsflag == SI) x[2] = x[2]/LPSperCFS/3600.*1000;
        else                 x[2] = x[2]/GPMperCFS/60.0*1.0e6;
        for (j=0; j<6; j++) Pump[i].Energy[j] = x[j];
        index = Pump[i].Link;
        if (fwrite(&index,sizeof(INT4),1,OutFile) < 1) return(308);
        if (fwrite(x,sizeof(float), 6, OutFile) < 6) return(308);
    }
    Emax = Emax*Dcost;
    if (fwrite(&Emax, sizeof(float), 1, OutFile) < 1) return(308);
    return(0);
}


int  readhyd(long *hydtime)
/*
**--------------------------------------------------------------
**   Input:   none                                                
**   Output:  *hydtime = time of hydraulic solution               
**   Returns: 1 if successful, 0 if not                    
**   Purpose: reads hydraulic solution from file HydFile               
**                                                              
**   NOTE: A hydraulic solution consists of the current time      
**         (hydtime), nodal demands (D) and heads (H), link
**         flows (Q), link status (S), and link settings (K).                                           
**--------------------------------------------------------------
*/
{
   int   i;
   INT4  t;
   if (fread(&t,sizeof(INT4),1,HydFile) < 1)  return(0);
   *hydtime = t;
   if (fread(D+1,sizeof(float),Nnodes,HydFile) < (unsigned)Nnodes) return(0);
   if (fread(X+1,sizeof(float),Nnodes,HydFile) < (unsigned)Nnodes) return(0);
   for (i=1; i<=Nnodes; i++) H[i] = X[i];
   if (fread(Q+1,sizeof(float),Nlinks,HydFile) < (unsigned)Nlinks) return(0);
   if (fread(X+1,sizeof(float),Nlinks,HydFile) < (unsigned)Nlinks) return(0);
   for (i=1; i<=Nlinks; i++) S[i] = (char) X[i];
   if (fread(K+1,sizeof(float),Nlinks,HydFile) < (unsigned)Nlinks) return(0);
   return(1);
}                        /* End of readhyd */


int  readhydstep(long *hydstep)
/*
**--------------------------------------------------------------
**   Input:   none                                                
**   Output:  *hydstep = next hydraulic time step (sec)
**   Returns: 1 if successful, 0 if not                    
**   Purpose: reads hydraulic time step from file HydFile
**--------------------------------------------------------------
*/
{
   INT4  t;
   if (fread(&t,sizeof(INT4),1,HydFile) < 1)  return(0);
   *hydstep = t;
   return(1);
}                        /* End of readhydstep */


int  saveoutput()
/*
**--------------------------------------------------------------
**   Input:   none                                                
**   Output:  returns error code                                  
**   Purpose: writes simulation results to output file            
**--------------------------------------------------------------
*/
{
   int   j;
   int   errcode = 0;

   /* Write out node results, then link results */
   for (j=DEMAND; j<=QUALITY; j++)  ERRCODE(nodeoutput(j,X,Ucf[j]));
   for (j=FLOW; j<=FRICTION; j++) ERRCODE(linkoutput(j,X,Ucf[j]));
   return(errcode);
}                        /* End of saveoutput */


int  nodeoutput(int j, float *x, float ucf)
/*
**--------------------------------------------------------------
**   Input:   j   = type of node variable                         
**            *x  = buffer for node values                        
**            ucf = units conversion factor                       
**   Output:  returns error code                                  
**   Purpose: writes results for node variable j to output file   
**-----------------------------------------------------------------
*/
{
   int   i;

   /* Load computed results (in proper units) into buffer x */
   switch(j)
   {
       case DEMAND:    for (i=1; i<=Nnodes; i++)
                          x[i] = D[i]*ucf;
                       break;
       case HEAD:      for (i=1; i<=Nnodes; i++)
                          x[i] = H[i]*ucf;
                       break;
       case PRESSURE:  for (i=1; i<=Nnodes; i++)
                          x[i] = (H[i] - Node[i].El)*ucf;
                       break;
       case QUALITY:   for (i=1; i<=Nnodes; i++)
                          x[i] = C[i]*ucf;
   }

   /* Write x[1] to x[Nnodes] to output file */
   if (fwrite(x+1,sizeof(float),Nnodes,TmpOutFile) < (unsigned)Nnodes)
      return(308);
   return(0);
}                        /* End of nodeoutput */


int  linkoutput(int j, float *x, float ucf)
/*
**----------------------------------------------------------------
**   Input:   j   = type of link variable                         
**            *x  = buffer for link values                        
**            ucf = units conversion factor                       
**   Output:  returns error code                                  
**   Purpose: writes results for link variable j to output file
**----------------------------------------------------------------
*/
{
   int i;
   float a,h,q;

   /* Load computed results (in proper units) into buffer x */
   switch(j)
   {
      case FLOW:      for (i=1; i<=Nlinks; i++)
                         x[i] = Q[i]*ucf;
                      break;
      case VELOCITY:  for (i=1; i<=Nlinks; i++)
                      {
                         if (Link[i].Type == PUMP) x[i] = 0.0;
                         else
                         {
                            q = ABS(Q[i]);
                            a = PI*SQR(Link[i].Diam)/4.0;
                            x[i] = q/a*ucf;
                         }
                      }
                      break;
      case HEADLOSS:  for (i=1; i<=Nlinks; i++)
                      {
                         if (S[i] <= CLOSED) x[i] = 0.0;
                         else
                         {
                            h = H[Link[i].N1] - H[Link[i].N2];
                            if (Link[i].Type != PUMP) h = ABS(h);
                            if (Link[i].Type <= PIPE)
                               x[i] = 1000.0*h/Link[i].Len;
                            else x[i] = h*ucf;
                         }
                      }
                      break;
      case LINKQUAL:  for (i=1; i<=Nlinks; i++)
                         x[i] = avgqual(i)*ucf;
                      break;
      case STATUS:    for (i=1; i<=Nlinks; i++)
                         x[i] = S[i];
                      break;
      case SETTING:   for (i=1; i<=Nlinks; i++)
                      {
                         if (K[i] != MISSING)
                             switch (Link[i].Type)
                             {
                               case CV:   
                               case PIPE: x[i] = K[i];
                                          break;
                               case PUMP: x[i] = K[i];
                                          break;
                               case PRV:
                               case PSV:
                               case PBV:  x[i] = K[i]*Ucf[PRESSURE];
                                          break;
                               case FCV:  x[i] = K[i]*Ucf[FLOW];
                                          break;
                               case TCV:  x[i] = K[i];
                                          break;
                               default:   x[i] = 0.0;
                             }
                         else x[i] = 0.0;
                      }
                      break;
      case REACTRATE: /* Overall reaction rate in mass/L/day */
                      if (Qualflag == NONE) memset(x,0,(Nlinks+1 )*sizeof(float));
                      else for (i=1; i<=Nlinks; i++) x[i] = R[i]*ucf;
                      break;
      case FRICTION:   /* f = 2ghd/(Lu^2) where f = friction factor */
                       /* u = velocity, g = grav. accel., h = head  */
                       /*loss, d = diam., & L = pipe length         */
                       for (i=1; i<=Nlinks; i++)
                       {
                          if (Link[i].Type <= PIPE && ABS(Q[i]) > TINY)
                          {
                             h = ABS(H[Link[i].N1] - H[Link[i].N2]);
                             x[i] = 39.725*h*pow(Link[i].Diam,5)/Link[i].Len/SQR(Q[i]);
                          }
                          else x[i] = 0.0;
                       }
                       break;
   }

   /* Write x[1] to x[Nlinks] to output file */
   if (fwrite(x+1,sizeof(float),Nlinks,TmpOutFile) < (unsigned)Nlinks)
      return(308);
   return(0);
}                        /* End of linkoutput */


int  savefinaloutput()
/*
**--------------------------------------------------------------
**  Input:   none                                          
**  Output:  returns error code                                  
**  Purpose: saves time series statistics, reaction rates &
**            epilog to output file.
**--------------------------------------------------------------
*/
{
   int errcode = 0;

/* Save time series statistic if computed */
   if (Tstatflag != SERIES && TmpOutFile != NULL)
   {
      ERRCODE(savetimestat(X,NODEHDR));
      ERRCODE(savetimestat(X,LINKHDR));
      if (!errcode) Nperiods = 1;
      fclose(TmpOutFile);
   }

/* Save avg. reaction rates & file epilog */
   if (OutFile != NULL)
   {
      ERRCODE(savenetreacts(Wbulk,Wwall,Wtank,Wsource));
      ERRCODE(saveepilog());
   }
   return(errcode);
}


int  savetimestat(float *x, char objtype)
/*
**--------------------------------------------------------------
**   Input:   *x  = buffer for node values
**            objtype = NODEHDR (for nodes) or LINKHDR (for links)                                                
**   Output:  returns error code                                  
**   Purpose: computes time series statistic for nodes or links
**            and saves to normal output file.
**
**   NOTE: This routine is dependent on how the output reporting
**         variables were assigned to FieldType in TYPES.H.
**--------------------------------------------------------------
*/
{
   int   n, n1, n2;
   int   i, j,  p, errcode = 0;
   long  startbyte, skipbytes;
   float *stat1, *stat2, xx;

/*
  Compute number of bytes in temp output file to skip over (skipbytes)
  when moving from one time period to the next for a particular variable.
*/
   if (objtype == NODEHDR)
   {
   /*
      For nodes, we start at 0 and skip over node output for all
      node variables minus 1 plus link output for all link variables.
   */
      startbyte = 0;
      skipbytes = (Nnodes*(QUALITY-DEMAND) +
                   Nlinks*(FRICTION-FLOW+1))*sizeof(float);
      n = Nnodes;
      n1 = DEMAND;
      n2 = QUALITY;
   }
   else
   {
   /*
      For links, we start at the end of all node variables and skip
      over node output for all node variables plus link output for
      all link variables minus 1.
   */
      startbyte = Nnodes*(QUALITY-DEMAND+1)*sizeof(float);
      skipbytes = (Nnodes*(QUALITY-DEMAND+1) +
                   Nlinks*(FRICTION-FLOW))*sizeof(float);
      n = Nlinks;
      n1 = FLOW;
      n2 = FRICTION;
   }
   stat1 = (float *) calloc(n+1, sizeof(float));
   stat2 = (float *) calloc(n+1, sizeof(float));
   ERRCODE(MEMCHECK(stat1));
   ERRCODE(MEMCHECK(stat2));

   /* Process each output reporting variable */
   if (!errcode)
   {
      for (j=n1; j<=n2; j++)
      {
   
         /* Initialize stat arrays */
         if (Tstatflag == AVG) memset(stat1, 0, (n+1)*sizeof(float));
         else for (i=1; i<=n; i++)
         {
            stat1[i] = -MISSING;  /* +1E10 */
            stat2[i] =  MISSING;  /* -1E10 */
         }
   
         /* Position temp output file at start of output */
         fseek(TmpOutFile, startbyte + (j-n1)*n*sizeof(float), SEEK_SET);

         /* Process each time period */
         for (p=1; p<=Nperiods; p++)
         {

            /* Get output results for time period & update stats */
            fread(x+1, sizeof(float), n, TmpOutFile);
            for (i=1; i<=n; i++)
            {
               xx = x[i];
               if (objtype == LINKHDR)
               {
                  if (j == FLOW) xx = ABS(xx);
                  if (j == STATUS)
                  {
                     if (xx >= OPEN) xx = 1.0;
                     else            xx = 0.0;
                  }
               }
               if (Tstatflag == AVG)  stat1[i] += xx;
               else
               {
                  stat1[i] = MIN(stat1[i], xx);
                  stat2[i] = MAX(stat2[i], xx);
               }
            }

            /* Advance file to next period */
            if (p < Nperiods) fseek(TmpOutFile, skipbytes, SEEK_CUR);
         }

         /* Compute resultant stat & save to regular output file */
         switch (Tstatflag)
         {
            case AVG:   for (i=1; i<=n; i++) x[i] = stat1[i]/(float)Nperiods;
                        break;
            case MIN:   for (i=1; i<=n; i++) x[i] = stat1[i];
                        break;
            case MAX:   for (i=1; i<=n; i++) x[i] = stat2[i];
                        break;
            case RANGE: for (i=1; i<=n; i++) x[i] = stat2[i] - stat1[i];
                        break;
         }
         if (objtype == LINKHDR && j == STATUS)
         {
            for (i=1; i<=n; i++)
            {
               if (x[i] < 0.5) x[i] = CLOSED;
               else            x[i] = OPEN;
            }
         }
         if (fwrite(x+1, sizeof(float), n, OutFile) < (unsigned) n) errcode = 308;

         /* Update internal output variables where applicable */
         if (objtype == NODEHDR) switch (j)
         {
            case DEMAND:  for (i=1; i<=n; i++) D[i] = x[i]/Ucf[DEMAND];
                          break;   
            case HEAD:    for (i=1; i<=n; i++) H[i] = x[i]/Ucf[HEAD];
                          break;   
            case QUALITY: for (i=1; i<=n; i++) C[i] = x[i]/Ucf[QUALITY];
                          break;
         }
         else if (j == FLOW) for (i=1; i<=n; i++) Q[i] = x[i]/Ucf[FLOW];
      }
   }

   /* Free allocated memory */
   free(stat1);
   free(stat2);
   return(errcode);
}



int  savenetreacts(REAL wbulk, REAL wwall, REAL wtank, REAL wsource)
/*
**-----------------------------------------------------
**  Writes average network-wide reaction rates (in
**  mass/hr) to binary output file.
**-----------------------------------------------------
*/
{
   int errcode = 0;
   float t;
   float w[4];
   if (Dur > 0) t = (float)Dur/3600.;
   else t = 1.;
   w[0] = wbulk/t;
   w[1] = wwall/t;
   w[2] = wtank/t;
   w[3] = wsource/t;
   if (fwrite(w,sizeof(float),4,OutFile) < 4) errcode = 308;
   return(errcode);
}


int  saveepilog()
/*
**-------------------------------------------------
**  Writes Nperiods, Warnflag, & Magic Number to 
**  end of binary output file.
**-------------------------------------------------
*/
{
   int errcode = 0;
   INT4 i;
   i = Nperiods;
   if (fwrite(&i,sizeof(INT4),1,OutFile) < 1) errcode = 308;
   i = Warnflag;
   if (fwrite(&i,sizeof(INT4),1,OutFile) < 1) errcode = 308;
   i = MAGICNUMBER;
   if (fwrite(&i,sizeof(INT4),1,OutFile) < 1) errcode = 308;
   return(errcode);
}


/********************** END OF OUTPUT.C **********************/
