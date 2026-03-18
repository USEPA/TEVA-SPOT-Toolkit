/*
**********************************************************

      String Constants for EPANET MultiSpecies Extension                            
                                                                     
VERSION:    1.00                                               
DATE:       4/11/05
AUTHOR:     Feng Shang	University of Cincinnati
			James Uber  University of Cincinnati 
			
**********************************************************
*/
/* ------------ Keyword Dictionary ---------- */

#define   w_PARAMETER   "PARA"
#define   w_CONSTANT    "CONS"
#define   w_COEFFICIENT "MTCOEFF"
#define   w_RATE		"RATE"
#define   w_DEF			"DEFI"
#define   w_EQU         "EQUA"

#define   w_MULTI		"MULTISPECIES"

#define   w_CUBICFOOT   "CUBICFOOT"
#define   w_GALLON      "GALLON"
#define   w_LITER       "LITER"
#define   w_CUBICMETER  "CUBICMETER"

#define   w_SECOND      "SECOND"
#define   w_MINUTE      "MINUTE"
#define   w_HOUR        "HOUR"
#define   w_DAY         "DAY"

#define   w_AREA		"AREA"

#define   w_RELTOL      "RELTOL"
#define   w_ABSTOL      "ABSTOL"

/* ---------Input Section Names ---------- */
#define	  s_MSVARIABLES "[MSVA"			
#define   s_MSPIPE		"[MSPI"			
#define   s_MSTANK      "[MSTA"			
#define   s_MSOPTIONS   "[MSOP"


/*------------------- Error Messages --------------------*/
//#define ERR101 "System Error 101: insufficient memory available."
//#define ERR102 "System Error 102: no network data available."
//#define ERR202 "Input Error 202: %s %s contains illegal numeric value."
//#define ERR216 "Input Error 216: %s data specified for undefined Pump %s."
//#define ERR217 "Input Error 217: invalid %s data for Pump %s."
//#define ERR219 "Input Error 219: %s %s illegally connected to a tank."
//#define ERR220 "Input Error 220: %s %s illegally connected to another valve."
//#define ERR222 "Input Error 222: %s %s has same start and end nodes."
//#define ERR301 "File Error 301: identical file names."
//#define ERR302 "File Error 302: cannot open input file."
//#define ERR303 "File Error 303: cannot open report file."
//#define ERR304 "File Error 304: cannot open binary output file."

//#define R_ERR201 "Input Error 201: syntax error in following line of "
//#define R_ERR202 "Input Error 202: illegal numeric value in following line of "
//#define R_ERR203 "Input Error 203: undefined node in following line of "
//#define R_ERR204 "Input Error 204: undefined link in following line of "
//#define R_ERR207 "Input Error 207: attempt to control a CV in following line of "
//#define R_ERR221 "Input Error 221: mis-placed clause in following line of "

/*-------------------- Specific Warning Messages -------------------------*/
#define WARN07  "WARNING: Singular matrix in water quality solver at %s hrs."

#define ERR260  "Input Error 260: Invalid water quality species index."
#define ERR261  "Input Error 261: Invalid water quality species ID."
#define ERR262  "Input Error 262: Invalid water quality parameter index or ID."

