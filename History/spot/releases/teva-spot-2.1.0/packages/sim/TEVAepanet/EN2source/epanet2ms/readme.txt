Files needed to build EPANET Multispecies Extension


1. C Files

MSEPANET.C  -- main module providing supervisory control
INPUT1.C  -- controls processing of input data
INPUT2.C  -- reads data from input file
INPUT3.C  -- parses individual lines of input data
MSINPUT.C -- parses multispecies input data
INPFILE.C -- saves modified input data to a text file
RULES.C   -- implements rule-based control of piping system
HYDRAUL.C -- computes extended period hydraulic behavior
QUALITY.C -- tracks transport & fate of water quality
MSQUALITY.C -- tracks transport & reaction of multispecies water quality
OUTPUT.C  -- handles transfer of data to and from binary files
REPORT.C  -- handles reporting of results to text file
SMATRIX.C -- sparse matrix linear equation solver routines
MEMPOOL.C -- memory allocation routines
HASH.C    -- hash table routines
CHEMSIM.C -- multispecies reaction simulation	 
ODESOLVE.C -- ordinary differential equations solver
NEWTON.C   -- algebraic equations solver
MATHEXPR.C -- evaluate mathematical expressions

2. Header Files

hash.h    -- header file for hash.c

text.h	  -- string constants 
mstext.h

types.h   -- global constants and data types
mstyppes.h

enumstxt.h -- enumerated data types
msenumstxt.h

funcs.h	-- function prototypes	
msfuncs.h 

vars.h  -- global variables
msvars.h

toolkit.h -- DLL function prototypes
mstoolkit.h

mempool.h --header file for mempool.c
newton.h  -- header file for newton.c
odesolve.h -- header file for odesolve.c
mathexpr.h -- header file for mathexpr.c

3. Header file for EPANET Multispecies Toolkit generated

Epanet2ms.h