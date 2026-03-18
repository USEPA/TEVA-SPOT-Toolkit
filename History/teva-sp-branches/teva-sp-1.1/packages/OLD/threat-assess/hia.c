/* System Headers */
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <float.h>

/* TEVA headers */
#include "sss.h"
#include "nrutil.h"


/*
**--------------------------------------------------------------
**  GETNODEINDEX
**  Input:   node id for which node index is needed; list of all
**           node ids
**  Output:  node index that matches provided ID
**  Purpose: matches node ID with list; returns associated
**           0-based index. Returns -1 if not found
**--------------------------------------------------------------
*/
int getnodeindex(PNet net,PNode node,char *nid)
{
    int i;

    for(i=0;i<net->info->nnodes;i++) {
        if(!strcmp(nid,node[i].info->id)) return(i);
    }
    return(-1);
}


/*
**--------------------------------------------------------------
**  ACCUMULATE
**  Input:   scenario state variables
**  Output:  updated accumulation arrays
**  Purpose: accumulates results for ensemble statistics
**--------------------------------------------------------------
*/
void Accumulate(int inod,int nz,PNet net,PDR dr,float pop,PU u,PXA xa,PTA ta,PHIST hist)
{
    int j,maxsteps;
	float *xdos,*xres,*xs,*xi,*xd,*xf;
	float *udos,*ures,*us,*ui,*ud,*uf;

	maxsteps=dr->maxsteps;
    if(pop==0) return;
    if(nz) {  /* Non-zero concentration/dose at node */
		float maxdos=-FLT_MAX,
			maxres=-FLT_MAX,
			maxs=-FLT_MAX,
			maxi=-FLT_MAX,
			maxd=-FLT_MAX,
			maxf=-FLT_MAX;
		xdos = xa->dos;  udos=u->dos;
		xres = xa->res;  ures=u->res;
		xs = xa->s;  us=u->s;
		xi = xa->i;  ui=u->i;
		xd = xa->d;  ud=u->d;
		xf = xa->f;  uf=u->f;
        for(j=0;j<maxsteps;j++) {                     /* Accumulate over time */
			float f;
			f=*udos++; if(f>maxdos) maxdos=f; (*xdos++) += pop*f;
			f=*ures++; if(f>maxres) maxres=f; (*xres++) += pop*f;
			f=*us++; if(f>maxs) maxs=f; (*xs++) += pop*f;
			f=*ui++; if(f>maxi) maxi=f; (*xi++) += pop*f;
			f=*ud++; if(f>maxd) maxd=f; (*xd++) += pop*f;
			f=*uf++; if(f>maxf) maxf=f; (*xf++) += pop*f;
        }
        ta->dos[inod]+=pop*maxdos;             /* Accumulate over nodes */
        ta->res[inod]+=pop*maxres;
        ta->s[inod]+=pop*maxs;
        ta->i[inod]+=pop*maxi;
        ta->d[inod]+=pop*maxd;
        ta->f[inod]+=pop*maxf;
    }
    else {    /* Zero concentration/dose at node */
		xs = xa->s;
        for(j=0;j<maxsteps;j++) {(*xs++)+=pop;}      /* Accumulate over time */
        ta->s[inod]+=pop;                                 /* Accumulate over nodes */
    }
	/** NEW **/
	/* Save histogram data -- for each scenario, fatalities, response, dose */
	hist->testh+=1; /* should be equal to nnodes for each scenario */
	if(nz) {
		hist->fh+=pop*u->f[dr->maxsteps-1];  /* RM 11/9/04 */
		hist->resh+=pop*u->res[dr->maxsteps-1];
		hist->dosh+=pop*u->dos[dr->maxsteps-1]; 
	}
	/** End NEW **/
}

/** NEW **/
void AccumulateFinal(PNet net,PDR dr,PXA totalXA,PXA interimXA)
/*  
**--------------------------------------------------------------
**	Input:	 scenario state variables
**	Output:  updated accumulation arrays
**	Purpose: accumulates results for ensemble statistics
**--------------------------------------------------------------
*/
{
	float *totdos,*totres,*tots,*toti,*totd,*totf;
	float *intdos,*intres,*ints,*inti,*intd,*intf;
	int j,maxsteps=dr->maxsteps;
	/* Sum state variables over all nodes and all scenarios */

	/* Accumulate total population statistics as a function of time */
	totdos = totalXA->dos;  intdos=interimXA->dos;
	totres = totalXA->res;  intres=interimXA->res;
	tots = totalXA->s;  ints=interimXA->s;
	toti = totalXA->i;  inti=interimXA->i;
	totd = totalXA->d;  intd=interimXA->d;
	totf = totalXA->f;  intf=interimXA->f;
	for(j=0;j<maxsteps;j++) {                     /* Accumulate over time */
		(*totdos++) += (*intdos++);
		(*totres++) += (*intres++);
		(*tots++) += (*ints++);
		(*toti++) += (*inti++);
		(*totd++) += (*intd++);
		(*totf++) += (*intf++);
	}
}
/** End NEW **/

/*
**--------------------------------------------------------------
**  FINDMAX
**  Input:   float array
**  Output:  max element
**  Purpose: finds maximum value in array of length n
**--------------------------------------------------------------
*/
float findmax(int n,float* x)
{
    int i;
    float xmax=-FLT_MAX;

    for(i=0;i<n;i++) {
        if(xmax<x[i]) xmax=x[i];
    }
    return(xmax);
}


/*
**--------------------------------------------------------------
**  RESPONSE
**  Input:   cumulative dose and dose-response curve parameters
**  Output:  number of exposed persons over time
**  Purpose: Estimate the time evolution of population exposed,
**           or the population that will become symptomatic.
**--------------------------------------------------------------
*/
void Response(PNet net,PDR dr,PU u)
{
    int i;
    float *dos=u->dos;
    float *res=u->res;

    for(i=0;i<dr->maxsteps;i++) res[i]=DoseResponse(dos[i],dr);
}


/*
**--------------------------------------------------------------
**  INITIALIZE
**  Input:   pointers to arrays
**  Output:  zero'd arrays
**  Purpose: Initialize arrays to zero
**--------------------------------------------------------------
*/
void initialize(PNet net,PDR dr,PXA xa,PTA ta)
{
    int i;

    for(i=0;i<dr->maxsteps;i++) {
        xa->dos[i]=0;
        xa->res[i]=0;
        xa->s[i]=0;
        xa->i[i]=0;
        xa->d[i]=0;
        xa->f[i]=0;
    }
    for(i=0;i<net->info->nnodes;i++) {
        ta->dos[i]=0;
        ta->res[i]=0;
        ta->s[i]=0;
        ta->i[i]=0;
        ta->d[i]=0;
        ta->f[i]=0;
    }
}


/*
** --------------------------------------------------------------------------------
** DOSERESPONSE
** Input :  a   = constant from sigmoid curve fitting
**                  m   = constant
**                  n   = constant
**                  tau = constant
**          dose    = cumulative dose
** Output:  response= returns % response for given dose
** Purpose: Calculates percentage of population that will respond to the given dose
**                  at a given node based on the dose response sigmoid equation
**                  R(dose)=(1+m exp(-dose/tau))/(1+n exp(-dose/tau))
**  -------------------------------------------------------------------------------
*/
float DoseResponse(float dose, PDR dr)
{
    float temp;
//	if(dose > 0)
//		printf("");
    if(dr->normalize) temp=(float)exp(-dose/(dr->tau*dr->bodymass));
    else temp=(float)exp(-dose/dr->tau);
    return( dr->a * (1+dr->m*temp) / (1+dr->n*temp) );
}


/*
**--------------------------------------------------------------
**  DOSE
**  Input:   dose rate variation and concentration time series
**  Output:  expected cumulative dose over time
**  Purpose: Calculate the expected cumulative dose at one node
**--------------------------------------------------------------
*/
void Dose(int dtime,PNet net,PDR dr,PU u,PNode node)
{
    int i;
    float temp;
    float *dos=u->dos;
    float *rho=node->rho;
    float *c=node->info->c[0];

    if(dtime>net->info->nsteps) dtime=net->info->nsteps;
    temp=dr->phi*net->info->stepsize;
    dos[0]=temp*c[0]*rho[0];
    for(i=1;i<dtime;i++) { 
		dos[i]=dos[i-1]+temp*c[i]*rho[i];
//		if(dos[i] > 0)
//			printf("");
	}
    for(i=dtime;i<dr->maxsteps;i++) dos[i]=dos[dtime-1];
}


/*
**--------------------------------------------------------------
**  DISEASE
**  Input:   dose-response time series
**  Output:  Disease dynamics S, I, D, F
**  Purpose: Calculate the dynamics of population health states
**           Susceptible, Infected, Diseased, and Fatality
**--------------------------------------------------------------
*/
void Disease(PNet net,PDR dr,PU u)
{
    int
        i,
        nsteps=net->info->nsteps,
        nL=dr->nL,
        nF=dr->nF;
    float
        sout,
        iout,
        dout,
        frate=dr->frate;
    float
        *res=u->res,
        *S=u->s,
        *I=u->i,
        *D=u->d,
        *F=u->f;

    S[0]=1 - (res[1]-res[0]);
    I[0]=(res[1]-res[0])-(RES(1-nL)-RES(-nL));
    D[0]=(RES(1-nL)-RES(-nL))-(RES(1-nL-nF)-RES(-nL-nF));
    F[0]=(RES(1-nL-nF)-RES(-nL-nF))*frate;
    for(i=0;i<nsteps+nL+nF-1;i++) {
        sout=RES(i+1)-RES(i);
        iout=RES(i+1-nL)-RES(i-nL);
        dout=RES(i+1-nL-nF)-RES(i-nL-nF);
        S[i+1]=S[i] - sout;
        I[i+1]=I[i] + sout - iout;
        D[i+1]=D[i] + iout - dout;
        F[i+1]=F[i] + dout*frate;
    }
}

// This version is from Regan's Modified code
// Jim and Regan will need to identify which one
// of the two to use.
void DiseaseRM(PNet net,PDR dr,PU u)
/*
**--------------------------------------------------------------
**	Input:	 dose-response time series
**	Output:  Disease dynamics S, I, D, F
**	Purpose: Calculate the dynamics of population health states
**           Susceptible, Infected, Diseased, and Fatalities
**--------------------------------------------------------------
*/
{
	UINT
		i;
	UINT
		nL=dr->nL,
		nF=dr->nF;
	float 
		sout,
		iout,
		dout,
		frate=dr->frate;
	float 
		*res=u->res,
		*S=u->s,
		*I=u->i,
		*D=u->d,
		*F=u->f;

	
	/* RM Changed Initial Conditions 10/12/04 */
	/* Removed RES function */
	/* S[0]=1 - (res[1]-res[0]); */
	/* I[0]=(res[1]-res[0])-(RES(1-nL)-RES(-nL)); */
	/* D[0]=(RES(1-nL)-RES(-nL))-(RES(1-nL-nF)-RES(-nL-nF)); */
	/* F[0]=(RES(1-nL-nF)-RES(-nL-nF))*frate; */

		S[0]=1;
		I[0]=0;
		D[0]=0;
		F[0]=0;

	for(i=0;i<nL;i++) {
		sout=res[i+1]-res[i];
		iout=0;
		dout=0;
		S[i+1]=S[i] - sout; 
		I[i+1]=I[i] + sout - iout; 
		D[i+1]=D[i] + iout - dout;
		F[i+1]=F[i] + dout*frate;
	}
	for(i=nL;i<nL+nF;i++) {
		sout=res[i+1]-res[i];
		iout=res[i+1-nL]-res[i-nL];
		dout=0;
		S[i+1]=S[i] - sout; 
		I[i+1]=I[i] + sout - iout;
		D[i+1]=D[i] + iout - dout;
		F[i+1]=F[i] + dout*frate;
	}
	for(i=nL+nF;i<net->info->nsteps+nL+nF-1;i++) {
		sout=res[i+1]-res[i];
		iout=res[i+1-nL]-res[i-nL];
		dout=res[i+1-nL-nF]-res[i-nL-nF];
		S[i+1]=S[i] - sout; 
		I[i+1]=I[i] + sout - iout; 
		D[i+1]=D[i] + iout - dout;
		F[i+1]=F[i] + dout*frate;
	}
}


/*
**--------------------------------------------------------------
**  POPULATION
**  Input:   node demands
**  Output:  node population estimate
**  Purpose: Rough estimate of node population using constant
**           per capita usage factor.
**--------------------------------------------------------------
*/
void Population(PNet net,PDR dr,PNode node)
{
    int i,
        j;
    float qavg;

    for (i=0;i<net->info->nnodes;i++) {
        qavg=0;
        for (j=0;j<net->info->nsteps;j++) qavg+=node[i].info->q[j];
        qavg/=(float)net->info->nsteps;
        node[i].pop=qavg/dr->pcu;
    }
    for (i=0;i<net->info->nnodes;i++) {
        if ((node[i].info->ntype==tank) || node[i].pop<0) node[i].pop=0;
    }
}


/*
**--------------------------------------------------------------
**  INGESTIONRATE
**  Input:   node demands
**  Output:  ingestion rate function
**  Purpose: Estimates the ingestion rate variation by assuming
**           it is proportional to node demand variation
**--------------------------------------------------------------
*/
void IngestionRate(PNet net, PNode node)
{
    int i,
        j;
    float qavg;

    for (i=0;i<net->info->nnodes;i++) {
        qavg=0;
        for (j=0;j<net->info->nsteps;j++) qavg+=node[i].info->q[j];
        qavg/=(float)net->info->nsteps;
        if ((node[i].info->ntype==tank) || qavg<=0) {
            for (j=0;j<net->info->nsteps;j++) node[i].rho[j]=0;
        }
        else {
            for (j=0;j<net->info->nsteps;j++) node[i].rho[j]=node[i].info->q[j]/qavg;
        }
    }
}


/*
**--------------------------------------------------------------
**  DETECT
**  Input:   Concentration time series at one node
**  Output:  1...nsteps; Time step for contaminant detection
**           or INT_MAX if never detected.
**  Purpose: Calculates the time step for contaminant detection,
**           if sensor or sampling were exposed to concentration
**           time series c.
**--------------------------------------------------------------
*/
int Detect(PNet net, PNode node)
{
    int  
        i,
        n,
        nsteps,
        start,
        freq,
        del;
    float 
        lim,
        vol,
        *c;
	PSensor sensor = &node->sensor;
    start=sensor->start;      /* sample start (time index) */
    freq=sensor->freq;        /* sample frequency (number of steps) */
    del=sensor->delay;        /* laboratory delay (number of steps) */
    lim=sensor->limit;        /* concentration detection limit */
    vol=sensor->vol;          /* sample volume */
    nsteps=net->info->nsteps;     /* total number of steps */
    c=node->info->c[0];              /* node concentration time series c[0]...c[nsteps-1] */

    switch( sensor->sample ) /* real-time (instantaneous), composite grab, or filtered sampling */
    {
    case realtime:
        for (i=start;i<=nsteps;i+=freq)
        {if (c[i-1] > lim) return(i+del);}
        break;
    case composite:
        /* First cycle up through start */
        if (cavg(1,start-1,freq,c) > lim) return(start-1+del);
        /* Cycles beginning at start */
        n=start;
        while (n<=nsteps-freq+1) {
            if (cavg(n,n+freq-1,freq,c) > lim) return(n+freq-1+del);
            n+=freq;
        }
        /* Last cycle */
        if (cavg(n,nsteps,freq,c) > lim) return(n+freq-1+del);
        break;
    case filtered:
        /* First cycle up through start */
        if (vol*cavg(1,start-1,freq,c) > lim) return(start-1+del);
        /* Cycles beginning at start */
        n=start;
        while (n<=nsteps-freq+1) {
            if (vol*cavg(n,n+freq-1,freq,c) > lim) return(n+freq-1+del);
            n+=freq;
        }
        /* Last cycle */
        if (vol*cavg(n,nsteps,freq,c) > lim) return(n+freq-1+del);
        break;
    default:
        ta_error(1,"Invalid sensor type in Detect");
    }
    return(INT_MAX);   /* No detection */
}


/*
**--------------------------------------------------------------
**  CAVG
**  Input:   Concentration time series at one node and interval
**  Output:  Average concentration over time interval
**  Purpose: Calculates average concentration
**--------------------------------------------------------------
*/
float cavg(int i1, int i2, int length, float *c)
{
    int i;
    float a;

    a=0;
    for (i=i1;i<=i2;i++) a+=c[i-1];
    a/=(float)length;
    return(a);
}




/*
**-----------------------------------------------------------
**  UCASE
**  Input:   *s = character string
**  Purpose: converts string to upper case
**-----------------------------------------------------------
*/
void Ucase( char *s )
{
    char *ptr;

    for (ptr = s; *ptr; ptr++)
        *ptr = toupper( *ptr );
}

/*
**-----------------------------------------------------------
**  InitializeAssessMemory
**  Input:   pointers to data items
**  Output:  data for health impact assessment
**  Purpose: Initializes memory needed for health impact assessment
**-----------------------------------------------------------
*/
void InitializeAssessMemory(PMem mem,PNetInfo netinfo)
{
    int   
        i,
        nnodes,
        nlinks,
        ntanks,
        njunctions,
        nsteps;
    float 
        stepsize,
        fltmax;

    PIO       io;
    PDR       dr;
    PNode     node;
    PLink     link;
	PSensorParms sap;
    PNet      net;
    PXA       xa;
    PTA       ta;
    PU        u;
    PSource   sources;

	nnodes = netinfo->nnodes;
	nsteps = netinfo->nsteps;
	nlinks = netinfo->nlinks;
	fltmax = netinfo->fltmax;
	njunctions = netinfo->njunctions;
	ntanks = netinfo->ntanks;
	stepsize = netinfo->stepsize;

    /* Allocate memory depending on network size */
    MEMCHECK(io=(PIO)malloc((size_t) sizeof(IO)));                          /* input/output */
    MEMCHECK(dr=(PDR)malloc((size_t) sizeof(DR)));                          /* dose/response */
    MEMCHECK(node = (PNode)malloc((size_t) nnodes*sizeof(Node)));           /* Nodes */
    for(i=0;i<nnodes;i++) {
        node[i].rho = vector(0,nsteps-1);
		node[i].info = &mem->nodeinfo[i];
    }
    MEMCHECK(link = (PLink)malloc((size_t) nlinks*sizeof(Link)));           /* Links */
    for(i=0;i<nlinks;i++) {
        link[i].info = &mem->linkinfo[i];
    }

    MEMCHECK(net = (PNet)malloc((size_t) sizeof(Net)));                     /* Network */
	net->info = netinfo;
    MEMCHECK(xa = (PXA)malloc((size_t) sizeof(XA)));                        /* spatial averages */
    MEMCHECK(ta = (PTA)malloc((size_t) sizeof(TA)));                        /* temporal averages */
    MEMCHECK(u = (PU)malloc((size_t) sizeof(U)));                           /* state variables */
    MEMCHECK(sources = (PSource)malloc((size_t) MAXSOURCES*sizeof(Source)));/* Sources */
    MEMCHECK(sap = (PSensorParms)malloc((size_t) sizeof(SensorParms)));     /* SensorParms */

    /* Default population of data structures */
    dr->dtype=totalmass;
    dr->itype=demand;
    dr->a=0;
    dr->m=0;
    dr->n=0;
    dr->tau=0;
    dr->bodymass=70;
    dr->normalize=1;
    dr->nL=0;
    dr->nF=0;
    dr->phi=(float)(2./24.);
    dr->frate=1;
    dr->protectlimit=0;
    dr->pcu=(float)(200./60./60.); /* gal/person/min */
    for(i=0;i<nnodes;i++) {
        node[i].sensor.type=ignore;
        node[i].pop=0;
        node[i].sensor.sample=realtime;
        node[i].sensor.limit=0;
        node[i].sensor.vol=0;
        node[i].sensor.freq=0;
        node[i].sensor.start=0;
        node[i].sensor.delay=0;
    }
    net->nscenario=0;

    /* Copy addresses to mem for return */
    mem->io=io;
    mem->dr=dr;
    mem->node=node;
    mem->link=link;
    mem->net=net;
    mem->xa=xa;
    mem->ta=ta;
    mem->u=u;
    mem->sources=sources;
	mem->sap=sap;
}
/*
**-----------------------------------------------------------
**  FreeAssessMemory
**  Input:   pointers to data items
**  Output:  data for health impact assessment
**  Purpose: Releases all memory allocated
**-----------------------------------------------------------
*/
void FreeAssessMemory(PMem mem)
{
    int   
        i,
        nnodes,
		nsteps,
        maxsteps;

	nnodes = mem->net->info->nnodes;
	maxsteps = mem->dr->maxsteps;
	nsteps = mem->net->info->nsteps;

    /* Allocate memory depending on network size */
    free(mem->io);
    free(mem->dr);
    for(i=0;i<nnodes;i++) {
        free_vector(mem->node[i].rho,0,nsteps);
    }
    free(mem->node);
    free(mem->link);

    free(mem->net);
   
	
	freeXA(mem->xa,maxsteps);
	free(mem->xa);
	freeTA(mem->ta,nnodes);
	free(mem->ta);
	freeU(mem->u,maxsteps);
	free(mem->u);
    free(mem->sources);
    free(mem->sap);
}

/** NEW **/
/*
**-----------------------------------------------------------
**  initXA
**  Input:   pointer to XA data structure and length of each
**           data item
**  Output:  XA data structure with all elements allocated
**  Purpose: Allocates memory for XA data structure
**-----------------------------------------------------------
*/
void initXA(PXA xa, int len) {
	xa->dos = vector(0,len-1);				/* Spatial averages */
	xa->res = vector(0,len-1);				
	xa->s = vector(0,len-1);
	xa->i = vector(0,len-1);
	xa->d = vector(0,len-1);
	xa->f = vector(0,len-1);
	resetXA(xa,len);
}
/*
**-----------------------------------------------------------
**  resetXA
**  Input:   pointer to XA data structure and length of each
**           data item
**  Output:  XA data structure with all elements reset to 0
**  Purpose: resets memory for XA data structure
**-----------------------------------------------------------
*/
void resetXA(PXA xa, int len) {
	int i;
	for(i=0;i<len;i++) {
		xa->dos[i] = 0;				/* Spatial averages */
		xa->res[i] = 0;				
		xa->s[i] = 0;
		xa->i[i] = 0;
		xa->d[i] = 0;
		xa->f[i] = 0;
	}
}
/*
**-----------------------------------------------------------
**  freeXA
**  Input:   pointer to XA data structure and length of each
**           data item
**  Output:  none
**  Purpose: frees memory previously allocated for XA data
**-----------------------------------------------------------
*/
void freeXA(PXA xa, int len) {
	free_vector(xa->dos,0,len-1);				/* Spatial averages */
	free_vector(xa->res,0,len-1);				
	free_vector(xa->s,  0,len-1);				
	free_vector(xa->i,  0,len-1);				
	free_vector(xa->d,  0,len-1);				
	free_vector(xa->f,  0,len-1);				
}

/*
**-----------------------------------------------------------
**  freeTA
**  Input:   pointer to TA data structure and length of each
**           data item
**  Output:  none
**  Purpose: frees memory previously allocated for TA data
**-----------------------------------------------------------
*/
void freeTA(PTA ta, int len) {
	free_vector(ta->dos,0,len-1);				/* Spatial averages */
	free_vector(ta->res,0,len-1);				
	free_vector(ta->s,  0,len-1);				
	free_vector(ta->i,  0,len-1);				
	free_vector(ta->d,  0,len-1);				
	free_vector(ta->f,  0,len-1);				
}

/*
**-----------------------------------------------------------
**  freeU
**  Input:   pointer to U data structure and length of each
**           data item
**  Output:  none
**  Purpose: frees memory previously allocated for U data
**-----------------------------------------------------------
*/
void freeU(PU u, int len) {
	free_vector(u->dos,0,len-1);				/* Spatial averages */
	free_vector(u->res,0,len-1);				
	free_vector(u->s,  0,len-1);				
	free_vector(u->i,  0,len-1);				
	free_vector(u->d,  0,len-1);				
	free_vector(u->f,  0,len-1);				
}

/*
**-----------------------------------------------------------
**  freeHistogramData
**  Input:   pointer to head of histogram data list
**  Output:  none
**  Purpose: frees memory previously allocated for histogram data
**-----------------------------------------------------------
*/
void freeHistogramData(PHIST hist) {
	PHIST t=hist;
	while(t!=NULL) {
		PHIST next=t->next;
		free(t->simID);
		free(t);
		t=next;
	}
}
/** End NEW **/
