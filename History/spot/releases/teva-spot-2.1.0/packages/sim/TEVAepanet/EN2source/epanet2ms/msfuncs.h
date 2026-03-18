/*
**************************************************************************
                                                                   
FUNCS.H -- Function Prototypes for EPANET MultiSpecies Extension                       
                                                                   
VERSION:    1.00
DATE:       4/11/05
AUTHOR:     Feng Shang	University of Cincinnati
            James Uber  University of Cincinnati
                                                                   
**************************************************************************
*/

/* ------- EPANET.C --------------------*/
/*
**  NOTE: The exportable functions that can be called
**        via the DLL are prototyped in TOOLKIT.H.
*/
//void    initpointers(void);               /* Initializes pointers       */
//int     allocdata(void);                  /* Allocates memory           */
//void    freeTmplist(STmplist *);          /* Frees items in linked list */
//void    freeFloatlist(SFloatlist *);      /* Frees list of floats       */
//void    freedata(void);                   /* Frees allocated memory     */
//int     openfiles(char *,char *,char *);  /* Opens input & report files */
//int     openhydfile(void);                /* Opens hydraulics file      */
//int     openoutfile(void);                /* Opens binary output file   */
//int     strcomp(char *, char *);          /* Compares two strings       */
//float   interp(int, float *,              /* Interpolates a data curve  */
//               float *, float);
//int     findnode(char *);                 /* Finds node's index from ID */
//int     findlink(char *);                 /* Finds link's index from ID */
//char*   geterrmsg(int);                   /* Gets text of error message */
//void    errmsg(int);                      /* Reports program error      */
//void    writecon(char *);                 /* Writes text to console     */
//void    writewin(char *);                 /* Passes text to calling app */

/* ------- MSINPUT.C --------------------*/
int     msgetdata(void);                    /* Gets network data          */
void    mssetdefaults(void);                /* Default value for multispecies application */
int     msallocdata(void);					/* Allocates memeory for multispecies application */
int     msnetsize(void);					/* Determines network size    */
int     msnewline(int, char *);				/* Input file line processing */
int     mssourcedata(void);					/* Reads species Source */
int     msqualdata(void);					/* Reads initial Quality */
int     msvariabledata(void);				/* Reads multispecies variables */
int		mscount(char *);					/* Counts multi species variable */
int     msreaddata(void);					/* Reads in network data      */
int     mspipereactdata(void);				/* Reads in pipe reaction dynamics */
int     mstankreactdata(void);				/* Reads in tank reaction dynmaics */
int     addpipeexpr(char);					/* Adds pipe reaction expression */
int     addtankexpr(char);					/* Adds tank reaction expression */
int     msreactdata(void);					/* Reads reaction parameter */
int     msoptiondata(void);                 /* Reads multispecies algorithmic options */
void    msspeciesindex(void);				/* Species indicies arrangement */
int     parseDAsystem(void);				/* Save the mathematical expressions in post fix form */
int     getPipeSpecieIndex(char *);			/* Get pipe species index */
int		getTankSpecieIndex(char*);          /* Get tank species index */
int     addspecieID(char, char *, char);    /* Add to a specie list */
int     addcoefficientID(char *, float);	/* Add to mass transfer coefficient list */
int     addparameterID(char *);				/* Add to reaction parameter list */
int     addconstantID(char *, float);		/* Add to reaction constant list */
int     findspecieID(Sspecie *, char *, int);	/* Find index from a specefic species list */
int     findparameterID(char *);			/* Find index from parameter list */
int     findcoefficientID(char *);			/* Find index from mass transfer coefficient list */
int     findconstantID(char *);				/* Find index from reaction constant list */
void    assignpara(void);					/* Assign parameter global value */     
int     findpipeexpr(char *);				/* Check the existency of species rate or definition (pipe dynamics)*/
int     findtankexpr(char *);				/* Check the existency of species rate or definition (tank dynamics)*/
void    msinitunits(void);					/* Initialize units for multispecies application */
void    msconvertunits(void);				/* Convert initial quality units */
/* ------------- REPORT.C --------------*/

/* ----------- MSQUALITY.C ---------------*/
int		msopenqual(void);				/* Open Multispecies WQ solver */
void	msinitqual();					/* Initialize Multispecies WQ solver */
void	msinitsegs();					/* Initialize multispecies water quality segments */
void	msaddseg(int, float, float *);	/* Add water quality segments */
void	msupdatenodes(long);			/* Node concentration update due to inflow */
void	msupdatetanks(long dt);			/* Update tank concentrations and volume */
void	mssourceinput(long);
void	mstankreact(int, float *, float, long);		/*Update concentrations in tank segment */
void	mstransport(long);				/* Transport species mass under constant hydraulics */
void	masstransfer(int);				/* Calculate mass transfer coefficient under certain hydraulics */
void	msreorientsegs();				/* Re-orient segments of flow reverses */
void	msupdatesegs(long);				/* Update species qualities in water quality segments */
void	msaccumulate(long);				/* Accumulate species mass flow at nodes */				
float	mssourcequal(Pmsource);
int		msgethyd(long *, long *);		/*	Retrieves hydraulic solution and hydraulic
										time step for next hydraulic event */
void	msrelease(long);
int		msstepqual(long *);
void	msupdatesourcenodes(long);
void	mspipereact(int, float *, float, long);  /* Update species qualities in water quality segments */
void    mstankmix1(int, long);              /* Complete mix tank model    */
void    mstankmix2(int, long);              /* 2-compartment tank model   */
void    mstankmix3(int, long);              /* FIFO tank model            */
void    mstankmix4(int, long);              /* LIFO tank model            */
void    combinesegments();
int		msclosequal();
float	msavgqual(int, int);
int		msrunqual(long *);
int		msnextqual(long *);
/* ----------- CHEMSIM.C ---------------*/
void	differentialSolution(int, float *, float, float);
void	algebraicSolution(int, float *);





/* ------------ OUTPUT.C ---------------*/
//int     savenetdata(void);                /* Saves basic data to file   */
//int     nodeoutput(int,float *,float);    /* Saves node results to file */
//int     linkoutput(int,float *,float);    /* Saves link results to file */

/* ------------ INPFILE.C ---------------*/
//int     saveinpfile(char *);             /* Saves network to text file  */
