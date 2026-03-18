# Microsoft Developer Studio Project File - Name="clpUnitTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=clpUnitTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "clpUnitTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "clpUnitTest.mak" CFG="clpUnitTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "clpUnitTest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "clpUnitTest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "clpUnitTest - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "..\..\clp\include" /I "..\..\coin\include" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "COIN_USE_CLP" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "clpUnitTest - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\..\clp\include" /I "..\..\coin\include" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "COIN_USE_CLP" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "clpUnitTest - Win32 Release"
# Name "clpUnitTest - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Clp\Test\CbcOrClpParam.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpDualRowDantzig.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpDualRowPivot.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpDualRowSteepest.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpFactorization.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpInterior.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpLinearObjective.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpMatrixBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpNetworkMatrix.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpNonLinearCost.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpObjective.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpPackedMatrix.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpParameters.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpPlusMinusOneMatrix.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpPresolve.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpPrimalColumnDantzig.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpPrimalColumnPivot.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpPrimalColumnSteepest.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpSimplex.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpSimplexNonlinear.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpSimplexOther.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpSimplexPrimal.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpSolve.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinDistance.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinError.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinFactorization.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinFileIO.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinFinite.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinFloatEqual.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinHelperFunctions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinMessageHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinMpsIO.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPackedMatrix.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPackedVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPackedVectorBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPragma.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolveMatrix.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinShallowPackedVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinSort.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinTime.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\Idiot.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Clp\Test\CbcOrClpParam.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpEventHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\Test\ClpMain.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\CoinPresolveMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\Test\MyEventHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\Test\MyEventHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\Test\MyMessageHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\Test\MyMessageHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\Presolve.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\Test\unitTest.cpp
# End Source File
# End Target
# End Project
