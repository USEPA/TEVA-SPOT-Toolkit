#define UCHAR(x) (((x) >= 'a' && (x) <= 'z') ? ((x)&~32) : (x))

/* Function Prototypes */
void ta_error(int exitCode,const char *msg,...);

/* Function Prototypes */
int   TAgettokens(char*, char**);
int   TAfindmatch(char*, char**);
int   TAmatch(char*, char*);
int   TAgetfloat(char*, float*);
int   TAgetlong(char*, long*);
int   TAgetint(char*, int*);

/* not available on unix */
#ifdef __linux__

int _stricmp (char *s, char *t);

#elif ( defined(__WINDOWS__) || defined(WIN32)) && !defined(MINGW)

int strcasecmp (char *s, char *t);

#endif
