#ifndef _TA_INPUT_H_
#define _TA_INPUT_H_

#include "tevautil.h"

/* Global Constants */
#define   MAXLINE  1023        /* Max input line chars       */
#define   MAXTOKS  40          /* Max input line tokens      */
#define   SEPSTR    " \t\n\r"  /* Token separator characters */

typedef struct
{
	char *line;
	char *keyword;
	int idx;
	int ntokens;
	char **tokens;
	int used;
}InputLine, *PInputLine;

typedef struct
{
	int num;
	PInputLine inputLines;
} InputFileData, *PInputFileData;


/* Function Prototypes */
int   TAgettokens(char*, char**);
int   TAfindmatch(char*, char**);
int   TAmatch(char*, char*);
int   TAgetfloat(char*, float*);
int   TAgetlong(char*, long*);
int   TAgetint(char*, int*);

PInputFileData loadInputFile(const char *inp, char **validKeywords);
void freeInputFileData(PInputFileData *ifd);
PTSO OpenTSOFile(PInputFileData taiIFD,char *tsoKeyword);

#endif
