/*
************************************************************************
            Global Variables for EPANET Multispecies Extension                            
                                                                    
VERSION:    1.00                                               
DATE:       4/9/05
AUTHOR:     Feng Shang	University of Cincinnati
			James Uber  University of Cincinnati                                         
************************************************************************
*/
EXTERN char     MSTimeFlag,				/* Reaction expression time unit */
				MSVolumeFlag,			/* Reaction expression volume unit */
				SingularFlag;			/* Singular matrix flag for WQ solver */

EXTERN int      MaxSpecies,            /* Species count from input file */
				MaxParameters,		   /* Parameter count from input file */
				MaxCoefficients,	   /* Mass transfer coefficient count from input file */
				MaxConstants,		   /* Constant count from input file */
				
				Nspecies,				/* Number of species */
				Nbulkspecies,			/* Number of bulk species */
				Npipesurfacespecies,	/* Numer of pipe surface species */
				Ntanksurfacespecies,	/* Number of tank surface species */
				Npipespecies,			/* Number of pipe species (=Nbulkspecies+Npipesurfacespecies) */
				Ntankspecies,			/* Number of tank species (=Nbulkspecies+Ntanksurfacespecies) */
				Nparameters,			/* Number of parameters */
				Nconstants,				/* Number of constants */
				Ncoefficients,			/* Number of mass transfer coefficients */
				
				Npiperate,				/* Number of pipe rate expression */
				Npipedef,				/* Number of pipe definition expression */
				Npipeequ,				/* Number of pipe equation expression */
				Ntankrate,				/* Number of tank rate expression */
				Ntankdef,				/* Number of tank definition expression */
				Ntankequ;				/* Number of tank equation expression */

				
EXTERN float    MSUcf[MAXVAR];         /* Unit conversion factors for reaction expressions */     
EXTERN float    MSreltol;              /* MS num. integration relative error tolerance */
EXTERN float UcfAreaq;				   /* Unit conversion factor for area quality*/
EXTERN float MSUcfAreaq;
/* Array pointers*/
EXTERN float    *CurrentConcen,		   /* Multi species segment quality */	
				*CurrentPara;		   /* Multi species pipe/tank parameter */	
EXTERN float    CurrentSegDiam;		   /* Water quality segment diameter */
EXTERN float    CurrentSegVelocity;    /* Water quality segment flow velocity */
EXTERN float    CurrentTime;		   /* Water quality simulation time */
EXTERN float    CurrentReynolds;       /* Water quality segment Reynolds number */
EXTERN float    *CurrentMasstracoeff;  /* Water quality segment mass transfer coefficient */ 

EXTERN float	*MSMassReact;		   /* Multi Species Mass Change  */
EXTERN float    **MSC,				   /* Node multi species quality*/
				**MSR,					
				**MSX;                 /* General purpose array for multi species analysis*/

EXTERN int      *StoPS,				   /* Specie index to pipe specie index*/
				*StoTS,				   /* Specie index to tank specie index*/
				*PStoS,				   /* Pipe specie index to specie index*/
				*TStoS;				   /* Tank specie index to specie index*/

EXTERN Sspecie  *Specie;				
EXTERN Sspecie  *BulkSpecie;
EXTERN Sspecie  *PipeSurfaceSpecie;
EXTERN Sspecie  *TankSurfaceSpecie;			   
EXTERN Sspecie  *PipeSpecie;
EXTERN Sspecie  *TankSpecie;
EXTERN Spara    *Parameter;
EXTERN Scoeff   *Coefficient;
EXTERN Sconst   *Constant;

EXTERN float    *PSabstol;             /* Pipe and Tank species absolute error tolerances */
EXTERN float    *TSabstol; 

EXTERN PExprTree * PipeDA;				/*Pipe differential-algebraic system in tree */	
EXTERN PExprTree * TankDA;				/*Tank differential-algebraic system in tree */

EXTERN PExprList * PipeDAList;	/*Pipe differential-algebraic system in list */
EXTERN PExprList * TankDAList;  /*Tank differential-algebraic system in list */

EXTERN SExprlist * PipeRateList, * PipeDefList, * PipeEquList;
EXTERN SExprlist * TankRateList, * TankDefList, * TankEquList;


EXTERN Pmsource *		NodeSource;				/* Node source strength */
EXTERN Pminitial *	    NodeInitial;			/* Node initial quality */
EXTERN float	**      LinkParameters;		    /* Reaction parameters for pipes and tanks*/
EXTERN float    **      TankParameters;