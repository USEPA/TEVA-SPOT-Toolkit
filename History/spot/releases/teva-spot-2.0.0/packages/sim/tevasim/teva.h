/* System Headers */
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>

/* MS/Epanet header */
#ifdef WIN32
#ifdef MSEPANET  /* Multi-species or Normal Epanet header */
#include "epanet2ms.h"
#else
#include "epanet2.h"
#endif
#endif

#ifdef __linux__
#ifdef MSEPANET  /* Multi-species or Normal Epanet header */
#include "toolkitms.h"
#else
#include "toolkit.h"
#endif
#endif

/* TEVAUtils headers */
#include "tevautil.h"
#include "enutil.h"

#define TSMAJORVERSION  1
#define TSMINORVERSION  2

/* Global Conversion macros */
#define TEVAFILEOPEN(x) (((x) == NULL) ? (TEVASimError(1,"Opening files")) : (0))
#define MEMCHECK(x) (((x) == NULL) ? (TEVASimError(1,"Allocating memory")) : (0))
#define MIN(x,y) (((x)<=(y)) ? (x) : (y))     /* minimum of x and y    */
#define MAX(x,y) (((x)>=(y)) ? (x) : (y))     /* maximum of x and y    */
#define MOD(x,y) ((x)%(y))                    /* x modulus y           */

/* Function Prototypes */
void TEVASimError(int exitCode,const char *errmsg, ...);
char  *TEVAclocktime(char*, long);
