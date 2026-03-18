#include "tevautil.h"

/* Global Constants */
#define   MAXLINE  10239       /* Max input line chars       */
#define   MAXTOKS  500         /* Max input line tokens      */
#define   SEPSTR    " \t\n\r"  /* Token separator characters */
#define   MAXERRMSG   100      /* Max size of err message    */
int ERRNUM;

/* Globa Conversion macros */
#define FILEOPEN(x) (((x) == NULL) ? (nrerror("Opening files")) : (0))
#define MEMCHECK(x) (((x) == NULL) ? (nrerror("Allocating memory")) : (0))
#define FILEWRITE(x,y) (((x) < (unsigned int)(y)) ? (nrerror("Too few bytes written")) : (0))
#define ENCHECK(x) (((ERRNUM = x) > 0) ? (epaneterror(ERRNUM)) : (0))
#define MIN(x,y) (((x)<=(y)) ? (x) : (y))     /* minimum of x and y    */
#define MAX(x,y) (((x)>=(y)) ? (x) : (y))     /* maximum of x and y    */
#define MOD(x,y) ((x)%(y))                    /* x modulus y           */
#define UCHAR(x) (((x) >= 'a' && (x) <= 'z') ? ((x)&~32) : (x))
                                              /* uppercase char of x   */

/* Global Data Types */
enum SectType    {_TITLE,_JUNCTIONS,_RESERVOIRS,_TANKS,_PIPES,_PUMPS,
                  _VALVES,_CONTROLS,_RULES,_DEMANDS,_SOURCES,_EMITTERS,
                  _PATTERNS,_CURVES,_QUALITY,_STATUS,_ROUGHNESS,_ENERGY,
                  _REACTIONS,_MIXING,_REPORT,_TIMES,_OPTIONS,
                  _COORDS,_VERTICES,_LABELS,_BACKDROP,_TAGS,_END};

/* Function Prototypes */
void  GetSize(PNetInfo, long*, long*);
void  WritePrologue(int, int, int, int, int, long, float*, float*, FILE*);
void  GetNodeXY(PNodeInfo, int, char*);
int   TEVAgettokens(char*, char**);
int   TEVAfindmatch(char*, char**);
int   TEVAmatch(char*, char*);
int   TEVAgetfloat(char*, float*);
int   TEVAgetlong(char*, long*);
void  WriteNodeData(int, int, float*, FILE*);
char  *TEVAclocktime(char*, long);
int   SetSource(PSource, int*, int, FILE*);
void  ExpandGen(int, int, float, long, long, int, int, char**, PSource, FILE*);
void  WriteTSI(int, PSource, FILE*, FILE*);
void  epaneterror(int);
void  GetDemandData(PNetInfo, PNodeInfo, int );
void GetLinkData(PNetInfo , PLinkInfo , int );
void  GetNodeData(int, int, float*);
int   Zeros(float*,int,int);
void InitializeNetwork(PNodeInfo, PLinkInfo, PNetInfo);
