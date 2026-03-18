# Microsoft Developer Studio Project File - Name="SbbSamplesSample2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SbbSamplesSample2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SbbSamplesSample2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SbbSamplesSample2.mak" CFG="SbbSamplesSample2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SbbSamplesSample2 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "SbbSamplesSample2 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SbbSamplesSample2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /I "..\..\..\sbb\include" /I "$(FmpIncDir)" /I "$(SpxIncDir)" /I "$(OslIncDir)" /I "$(XprIncDir)" /I "$(CpxIncDir)" /I "$(GlpkIncDir)" /I "$(DylpIncDir1)" /I "$(DylpIncDir2)" /I "..\..\osi\include" /I "..\..\osi\osiclp\include" /I "..\..\osi\osicpx\include" /I "..\..\osi\osiDylp\include" /I "..\..\osi\osiGlpk\include" /I "..\..\osi\osiOsl\include" /I "..\..\osi\osiSpx\include" /I "..\..\osi\osiFmp\include" /I "..\..\osi\osiVol\include" /I "..\..\osi\osiXpr\include" /I "..\..\coin\include" /I "..\..\clp\include" /I "..\..\sbb\include" /I "..\..\cgl\include" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "$(COIN_USE)" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 $(LIB_NAMES) kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /libpath:"$(OslLibDir)" /libpath:"$(XprLibDir)" /libpath:"$(CpxLibDir)" /libpath:"$(GlpkLibDir)" /libpath:"$(DylpLibDir1)" /libpath:"$(DylpLibDir2)" /libpath:"$(SpxLibDir)" /libpath:"$(FmpLibDir)"

!ELSEIF  "$(CFG)" == "SbbSamplesSample2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I "$(FmpIncDir)" /I "$(SpxIncDir)" /I "$(OslIncDir)" /I "$(XprIncDir)" /I "$(CpxIncDir)" /I "$(GlpkIncDir)" /I "$(DylpIncDir1)" /I "$(DylpIncDir2)" /I "..\..\osi\include" /I "..\..\osi\osiclp\include" /I "..\..\osi\osicpx\include" /I "..\..\osi\osiDylp\include" /I "..\..\osi\osiGlpk\include" /I "..\..\osi\osiOsl\include" /I "..\..\osi\osiSpx\include" /I "..\..\osi\osiFmp\include" /I "..\..\osi\osiVol\include" /I "..\..\osi\osiXpr\include" /I "..\..\coin\include" /I "..\..\clp\include" /I "..\..\sbb\include" /I "..\..\cgl\include" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "$(COIN_USE)" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 $(LIB_NAMES) kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"$(OslLibDir)" /libpath:"$(XprLibDir)" /libpath:"$(CpxLibDir)" /libpath:"$(GlpkLibDir)" /libpath:"$(DylpLibDir1)" /libpath:"$(DylpLibDir2)" /libpath:"$(SpxLibDir)" /libpath:"$(FmpLibDir)"

!ENDIF 

# Begin Target

# Name "SbbSamplesSample2 - Win32 Release"
# Name "SbbSamplesSample2 - Win32 Debug"
# Begin Group "HeaderFiles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Coin\include\CoinDistance.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinError.hpp
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

SOURCE=..\..\Coin\include\CoinMessageHandler.hpp
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

SOURCE=..\..\Coin\include\CoinShallowPackedVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinSort.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinWarmStart.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinWarmStartBasis.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiColCut.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiCollections.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiCut.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiCuts.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiRowCut.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiSolverInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiSolverParameters.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbBranchActual.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbBranchBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\Samples\SbbBranchUser.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbCompareBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbHeuristic.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\Samples\SbbHeuristicUser.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbNode.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Sbb\Samples\sample2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\Samples\SbbBranchUser.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\Samples\SbbHeuristicUser.cpp
# End Source File
# End Target
# End Project
