#include <stdlib.h>
#include <string.h>

#include "ta_utils.h"
#include "ta_input.h"

/*
**-----------------------------------------------------------
**  loadInputFile
**  Input:   filename containing data to load and valid
**           keywords for the file
**  Output:  Pointer to input file data contents (for later
**           processing)
**  Purpose: Loads data file contents into memory for later
**           processing
**-----------------------------------------------------------
*/
PInputFileData loadInputFile(const char *fn, char **validKeywords)
{
    char  
        line[MAXLINE+1];        /* Line from input data file */
    int   
        sect = -1,              /* Data sections */
        ntokens = 0;            /* # tokens in input line */
    char *tok[MAXTOKS];         /* Array of token strings */
	FILE *inp;
	PInputFileData ifd;

	if((inp=fopen(fn,"r"))==NULL) {
		ta_error(1,"Unable to open input file: &s\n",fn);
	}
	ifd = (PInputFileData)calloc(1,sizeof(InputFileData));

    while (fgets(line,MAXLINE,inp) != NULL)
    {
		char **tokens;
		char *wline = (char *)calloc(strlen(line)+1,sizeof(char));
		PInputLine iline;
		int i;
        /* Make copy of line and scan for tokens */
        strcpy(wline,line);
        ntokens = TAgettokens(wline,tok);

        /* Skip blank lines and comments */
        if (ntokens == 0 || *tok[0] == ';') {
			free(wline);
			continue;
		}

        /* Check if max. length exceeded */
        if (strlen(line) >= MAXLINE) {ta_error(1,"Max input line length exceeded");}

		ifd->num++;
		ifd->inputLines = (InputLine*)realloc(ifd->inputLines,ifd->num*sizeof(InputLine));
		iline = &ifd->inputLines[ifd->num-1];
		iline->line = wline;
		tokens = (char **)calloc(ntokens,sizeof(char *));
		iline->ntokens = ntokens;
		iline->tokens = tokens;
		for(i=0;i<ntokens;i++) tokens[i] = tok[i];
		iline->keyword = tokens[0];
		iline->idx = TAfindmatch(iline->keyword,validKeywords);
        if(iline->idx == -1) {
			ta_error(1,"Unknown keyword: %s\n",iline->keyword);
		}

    }   /* End of while */
	fclose(inp);
	return ifd;
}

/*
**-----------------------------------------------------------
**  freeInputFileData
**  Input:   Pointer to input file data contents 
**  Output:  none
**  Purpose: releases all memory previously allocated to hold
**           input file contents
**-----------------------------------------------------------
*/
void freeInputFileData(PInputFileData *ifd) {
	int i;

	for(i=0;i<(*ifd)->num;i++) {
		PInputLine il = &(*ifd)->inputLines[i];
		free(il->tokens);
		free(il->line);
	}
	free((*ifd)->inputLines);
	free(*ifd);
	*ifd=NULL;

}
/*
**--------------------------------------------------------------
**  TAFINDMATCH
**  Input:   *line= line from input file
**           *keyword[] = list of NULL terminated keywords
**  Output:  returns index of matching keyword or
**           -1 if no match found
**  Purpose: determines which keyword appears on input line
**--------------------------------------------------------------
*/
int  TAfindmatch(char *line, char *keyword[])
{
    int i = 0;
    while (keyword[i] != NULL)
    {
        if (TAmatch(line,keyword[i])) return(i);
        i++;
    }
    return(-1);
}


/*
**--------------------------------------------------------------
**  TAMATCH
**  Input:   *str = string being    searched
**           *substr = substring being searched for
**  Output:  returns 1 if substr found in str, 0 if not
**  Purpose: sees if substr matches any part of str
**
**          (Not case sensitive)
**--------------------------------------------------------------
*/
int  TAmatch(char *str, char *substr)
{
    int i,j;

    /*** Updated 9/7/00 ***/
    /* Fail if substring is empty */
    if (!substr[0]) return(0);

    /* Skip leading blanks of str. */
    for (i=0; str[i]; i++) {
        if (str[i] != ' ') break;
    }

    /* Check if substr matches remainder of str. */
    for (i=i,j=0; substr[j]; i++,j++) {
        if (!str[i] || UCHAR(str[i]) != UCHAR(substr[j]))
            return(0);
    }
    return(1);
}
/*
**--------------------------------------------------------------
**  TAGETTOKENS
**  Input:   *s = string to be tokenized
**  Output:  returns number of tokens in s
**  Purpose: scans string for tokens, saving pointers to them
**           in Tok[]
**
** Tokens can be separated by the characters listed in SEPSTR
** (spaces, tabs, newline, carriage return) which is defined
** in TYPES.H. Text between quotes is treated as a single token.
**--------------------------------------------------------------
*/
int  TAgettokens(char *s, char **tok)
{
    int  len, m, n;
    char *c;

    /* Begin with no tokens */
    for (n=0; n<MAXTOKS; n++) tok[n] = NULL;
    n = 0;

    /* Truncate s at start of comment */
    c = strchr(s,';');
    if (c) *c = '\0';
    len = strlen(s);

    /* Scan s for tokens until nothing left */
    while (len > 0 && n < MAXTOKS)
    {
        m = strcspn(s,SEPSTR);                  /* Find token length */
        len -= m+1;                                         /* Update length of s */
        if (m == 0) s++;                                /* No token found */
        else
        {
            if (*s == '"')               /* Token begins with quote */
            {
                s++;                                            /* Start token after quote */
                m = strcspn(s,"\"\n\r");    /* Find end quote (or EOL) */
            }
            s[m] = '\0';                                 /* Null-terminate the token */
            tok[n] = s;                                  /* Save pointer to token */
            n++;                                                 /* Update token count */
            s += m+1;                                    /* Begin next token */
        }
    }
    return(n);
}


/*
**-----------------------------------------------------------
**  TAGETFLOAT
**  Input:   *s = character string
**  Output:  *y = floating point number
**                   returns 1 if conversion successful, 0 if not
**  Purpose: converts string to floating point number
**-----------------------------------------------------------
*/
int  TAgetfloat(char *s, float *y)
{
    char *endptr;
    *y = (float) strtod(s,&endptr);
    if (*endptr > 0) return(0);
    return(1);
}


/*
**-----------------------------------------------------------
**  TAGETLONG
**  Input:   *s = character string
**  Output:  *y = long int number
**                   returns 1 if conversion successful, 0 if not
**  Purpose: converts string to long integer (base 10)
**-----------------------------------------------------------
*/
int  TAgetlong(char *s, long *y)
{
    char *endptr;
    *y = strtol(s,&endptr,10);
    if (*endptr > 0) return(0);
    return(1);
}


/*
**-----------------------------------------------------------
**  TAGETINT
**  Input:   *s = character string
**  Output:  *y = int number
**                   returns 1 if conversion successful, 0 if not
**  Purpose: converts string to integer (base 10)
**-----------------------------------------------------------
*/
int  TAgetint(char *s, int *y)
{
    char *endptr;
    *y = (int)strtol(s,&endptr,10);
    if (*endptr > 0) return(0);
    return(1);
}

/*
**-----------------------------------------------------------
**  OpenTSOFile
**  Input:   Pointer to input file data contents containing
**           TSO file information and the keyword that
**           contains the TSO file information
**  Output:  Pointer to the TSO data structure from the TSO API
**  Purpose: Opens the tso database specified in the input
**           file data
**-----------------------------------------------------------
*/
PTSO OpenTSOFile(PInputFileData ifd,char *tsoKeyword)
{
    char  
	    tsoname[MAXFNAME+1],
		tsopattern[MAXFNAME+1],
		*patt;
	char tsoversion=-1,
		tsostoragemethod=-1;
    int   
        sect = -1,              /* Data sections */
        ntokens = 0;            /* # tokens in input line */
	PTSO tso;
	int i;

    /* Get TSO file name */
	for(i=0;i<ifd->num;i++)
    {
		PInputLine il = &ifd->inputLines[i];
        /* Process line depending on input keyword */
        int sect = il->idx;
		int ntokens = il->ntokens;
		char **tok=il->tokens;
		if(_stricmp(tsoKeyword,il->keyword)==0) {
			// ntokens = 2: single filename w/o verson & storage method
			// ntokens = 3: multi-part file w/o verson & storage method
			// ntokens = 4: single filename w verson & storage method
			// ntokens = 5: multi-part file w verson & storage method
			if(ntokens < 2 || ntokens > 5)
				ta_error(1,"Invalid intput TSONAME\n");
			if(ntokens == 2 || ntokens == 4) {
				strcpy(tsoname,tok[1]);
				patt = NULL;
			} else {
				strcpy(tsoname,tok[1]);
				strcpy(tsopattern,tok[2]);
				patt = tsopattern;
			}
			if(ntokens ==4) {
				tsoversion = (char)atoi(tok[2]);
				tsostoragemethod = (char)atoi(tok[3]);
			} else if (ntokens==5) {
				tsoversion = (char)atoi(tok[3]);
				tsostoragemethod = (char)atoi(tok[4]);
			}
		}
    }   /* End of while */

	if(tsoversion == -1 && tsostoragemethod == -1) {
		tso = TSO_Open(tsoname,patt);
	} else if (tsoversion != -1 && tsostoragemethod != -1) {
		tso = TSO_OpenNoHeader(tsoname,patt,tsoversion,tsostoragemethod);
	} else {
		ta_error(1,"Either both version & storage method must be specified or neither");
	}
	return tso;
}

