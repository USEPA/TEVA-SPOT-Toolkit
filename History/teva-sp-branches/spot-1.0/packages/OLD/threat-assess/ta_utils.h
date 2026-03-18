#define UCHAR(x) (((x) >= 'a' && (x) <= 'z') ? ((x)&~32) : (x))

/* Function Prototypes */
void ta_error(int exitCode,const char *msg,...);

/* not available on unix */
#ifdef __linux__

int _stricmp (char *s, char *t);

#endif
