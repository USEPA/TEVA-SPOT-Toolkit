/*
****************************************************************************
                                                                     
TYPES.H -- Global constants and data types for EPANET Multispecies Extension  
                                                                     
VERSION:    1.00                                               
DATE:       4/9/05
AUTHOR:     Feng Shang	University of Cincinnati
			James Uber	University of Cincinnati                                         			
****************************************************************************

/* Various conversion factors */
#define   GperFT3     7.4805
#define   CM2perFT2	  929.0304
#define   DAYperSEC   1.1574E-5
#define   HOURperSEC  2.7778E-4
#define   MINUTEperSEC 0.016667

/*
-----------------------------
   Global Constants
-----------------------------
*/
/*** Updated 6/24/02 ***/
#define   MSCODEVERSION        10010


/*
------------------------------------------------------
   Global Data Structures                             
------------------------------------------------------
*/
struct Exprlist			/*MATHEMATICAL EXPRESSION OBJECT*/
{
	char ID[MAXID+1];
	char expression[MAXLINE+1];
};
typedef struct Exprlist SExprlist;

struct ExprNode	/* Node in a tokenized math expression tree */
{
    		int    opcode;                // operator code
    		int    ivar;                  // variable index
    		double fvalue;                // numerical value
    		struct ExprNode* left;        // left sub-tree of tokenized formula
    		struct ExprNode* right;       // right sub-tree of tokenized formula
};

struct ExprListNode /*Node in a tokenized math postfix list */
{
    		int    opcode;                // operator code
    		int    ivar;                  // variable index
    		double fvalue;                // numerical value
    		struct ExprListNode* next;    // next token
			struct ExprListNode* prev;
};


typedef struct ExprNode ExprTree;
typedef struct ExprNode * PExprTree;

typedef struct ExprListNode ExprList;
typedef struct ExprListNode * PExprList;


typedef struct           /*SPECIES OBJECT*/
{
	char ID[MAXID+1];
	char Units[MAXID+1];
	char MSQtype;   /* surface quality type 0 (VOLUMEUNIT), 1 (AREAUNIT)*/ 
    float abstol;   /* specie-specific absolute tolerance */
}Sspecie;
		
typedef struct		/* REACTION PARAMETER OBJECT */
{
	char  ID[MAXID+1];
	float value;
}Spara;

typedef struct 		/* REACTION CONSTANT OBJECT */
{
	char  ID[MAXID+1];
	float value;
}Sconst;

typedef struct		/*MASS TRANSFER COEFFICIENT OBJECT */
{
	char  ID[MAXID+1];
	float diffusivity;   /* species diffusivity */
}Scoeff;


struct Smsource		/* MULTI SPECIES SOURCE OBJECT */
{
	int    index;	 /* General specie index, but only for bulk species*/
	float  C0;       /* Base concentration/mass  */
   	int    Pat;      /* Pattern index            */
   	float  Smass;    /* Actual mass flow rate    */
   	char   Type;     /* SourceType  */
	struct Smsource * next;		/* Next bulk specie source*/
};
typedef struct Smsource * Pmsource;

struct Sminitial		/* MULSTI SPECIES INITIAL QUALITY OBJECT*/
{
	int index;  	/* General specie index, bulk and pipe surface specie for all nodes, plus tank surface specie for tanks*/
	float C0;		/* Initial concentration */
	struct Sminitial * next;		/* Next specie initial quality */
};
typedef struct Sminitial * Pminitial;


struct  Smsseg            /* PIPE SEGMENT record used */
{                         /*   for WQ routing         */
   float  v;              /* Segment volume      */
   float  *c;           /* Water quality value */
   struct Smsseg *prev;     /* Record for previous segment */
   struct Smsseg *next;	  /* Record for next segment */	
};
typedef struct Smsseg *Pmsseg;    /* Pointer to pipe segment */

/*
----------------------------------------------
   Global Enumeration Variables
----------------------------------------------
*/
#define MULTIS   4				/* Multi-species Quality Analysis Type */
#define EPANETSECT 29           /* Number of sections in original EPANET*/
                               
enum MSSectType  {_MSVARIABLES=29,_MSPIPE=30,_MSTANK=31,_MSOPTIONS=32};

enum MSVolumeType {CUBICFOOT, LITER, CUBICMETER};

enum MSAreaUnitType {SQUAREFOOT, SQUARECENTIMETER, SQUAREMETER};

enum MSTimeType {SECOND, MINUTE, HOUR, DAY};

enum MSQType {VOLUMEUNIT, AREAUNIT};

enum ReactionType     {PIPES, TANKS};

enum PhysicalType   {BULK, PIPESURFACE, TANKSURFACE};    /* specie physical type */

enum DynamicType	{RATE, DEF, EQU};				/* specie dynamic type */
