REM This bat file is an attempt to start MS Visual Studio
REM Version 6 with a workspace and project files for
REM use with COIN. You should use this bat file to 
REM start MS Visual Studio after editing this file as
REM indicated below.
REM 
REM
REM COIN can be used with many optional solvers that 
REM are external to COIN.
REM 1. Environment variables are set in this bat file
REM    to indicate where these optional solvers are 
REM    on your computer.
REM 2. COIN_HAS needs to be set to have names of optional solvers.
REM 3. As source files get added(or deleted), then the
REM    files belonging to projects will need to updated.
REM    There is not an automatic way to keep the MS project
REM    files updated. If a file is missing when building a 
REM    project, then the file may have been removed from COIN.
REM    If when linking there are undefined symbols, then 
REM    there may be new files which need to be added to 
REM    a project.
REM    
REM
REM -------------------------------------------------
REM Set path to solvers.
REM The Open Solver Interface (OSI) can be built to
REM interface with different solvers.  
REM For the solvers you choose to build OSI with
REM Microsoft Development Studio needs to know where
REM these solvers are located on your computer.
REM Set the following variables for the solvers
REM you wish to build OSI with.
REM
REM XxxIncDir is the directory containing the 
REM    include files for solver Xxx.
REM    This symbol is used by project osiXxxLib
REM    and osiUnitTest.
REM XxxLibDir is the directory containing the 
REM    library for solver Xxx.
REM    This symbol is used by project osiUnitTest.
REM XxxLibName is the name of the library for 
REM    solver Xxx.
REM    This symbol is used by project osiUnitTest.
REM
REM Note: It is only necessary to set these 
REM enviromnent variables for solvers that 
REM are not part of COIN. So it it is not necessary
REM to set variables for Clp and Vol solvers.
REM -------------------------------------------------
set OslIncDir=d:\osl\o_osl_v2_h
set OslLibDir=d:\osl\lib
set OslLibName=oslmd6030.lib

set XprIncDir=e:\xpressmp\include
set XprLibDir=e:\xpressmp\lib
set XprLibName=xprs.lib

set GlpkIncDir=d:\glpk-3.2.3\include
set GlpkLibDir=d:\glpk-3.2.3\mds\glpkLib\Release
set GlpkLibName=glpkLib.lib

set DylpIncDir1=d:\dylp\OsiDylp
set DylpIncDir2=d:\dylp\DylpLib
set DylpLibDir1=d:\dylp\mds\OsiDylp\Release
set DylpLibDir2=d:\dylp\mds\DylpLib\Release
set DylpLibNames=osiDylp.lib dylpLib.lib

set SpxIncDir=d:\soplex-1.2.1\src
set SpxLibDir=d:\soplex-1.2.1\mds\soplexLib\Release
set SpxLibName=soplexLib.lib

set CpxIncDir=e:\ILOG\cplex81\include\ilcplex
set CpxLibDir=e:\ILOG\cplex81\lib\msvc6\stat_st
set CpxBinDir=e:\ILOG\cplex81\bin\msvc6
set CpxLibName=cplex81.lib

set FmpIncDir=e:\FortMP\include
set FmpLibDir=e:\FortMP\WIN32
set FmpLibName=fortmp.lib
REM LICFORTMP is directory containing license
set LICFORTMP=e:\fortmp\

REM -------------------------------------------------
REM Set the COIN_HAS to indicate which optional
REM solvers are available.
REM Some choices are:
REM   COIN_HAS_VOL 
REM   COIN_HAS_OSL 
REM   COIN_HAS_XPR 
REM   COIN_HAS_CPX 
REM   COIN_HAS_CPL  
REM   COIN_HAS_GLPK  
REM   COIN_HAS_DYLP
REM   COIN_HAS_SPX
REM   COIN_HAS_FMP
REM
REM For example, the following line
REM would indicate that FortMP, CLP, and OSL are to be used.
REM   set COIN_HAS=COIN_HAS_FMP" /D "COIN_HAS_CLP" /D "COIN_HAS_OSL
REM Notice that there is not a double quote after the last solver
REM and there is not a '/D "' prior to the first solver.
REM
REM The syntax for specifying a single solver is:
REM   set COIN_HAS=COIN_HAS_CLP
REM -------------------------------------------------
set COIN_HAS=COIN_HAS_CLP

REM -------------------------------------------------
REM LIB_NAMES has name of optional solvers libraries
REM -------------------------------------------------
REM set LIB_NAMES=%OslLibName% %XprLibName% %CpxLibName% %GlpkLibName% %DylpLibName% %SpxLibName% %FmpLibName% 
set LIB_NAMES="" 

REM -------------------------------------------------
REM LIB_DIRS has directory location of the optional solver libraries.
REM Using LIB_DIRS has not yet worked.
REM One needs to specify each library directory in the development studio.
REM -------------------------------------------------
set LIB_DIRS=%OslLibDir%" /libpath:"%XprLibDir%" /libpath:"%CpxLibDir%" /libpath:"%GlpkLibDir%" /libpath:"%DylpLibDir1%" /libpath:"%DylpLibDir2%" /libpath:"%FmpLibDir%

REM -------------------------------------------------
REM INCLUDE_DIRS has name of optional solver header file directories
REM INCLUDE_DIRS is not yet used in the development studio.
REM Each include directory is individually specified in the development studio
REM -------------------------------------------------
REM set INCLUCDE_DIRS


REM -------------------------------------------------
REM Set path to solver dll's
REM -------------------------------------------------
REM set PATH=%CpxBinDir%;%FmpLibDir%;%OslLibDir%;%PATH%
set PATH=%FmpLibDir%;%OslLibDir%;%PATH%
echo %PATH%

REM -------------------------------------------------
REM Start MS Visual Studio specifying the workspace
REM file.
REM -------------------------------------------------
Rem vcvars32
"E:\Microsoft Visual Studio\Common\MSDev98\Bin\msdev" D:\COIN\Win\coin.dsw


