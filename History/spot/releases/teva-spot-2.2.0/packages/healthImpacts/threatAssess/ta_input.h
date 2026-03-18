#ifndef _TA_INPUT_H_
#define _TA_INPUT_H_

#include "tevautil.h"

/* Global Constants */
#define   MAXLINE  2048        /* Max input line chars       */
#define   MAXTOKS  200          /* Max input line tokens      */
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


PInputFileData loadInputFile(const char *inp, char **validKeywords);
void freeInputFileData(PInputFileData *ifd);
PTSO OpenTSOFile(PInputFileData taiIFD,char *tsoKeyword);

#endif
