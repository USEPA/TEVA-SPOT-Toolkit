# Microsoft Developer Studio Project File - Name="osiLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=osiLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "osiLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "osiLib.mak" CFG="osiLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "osiLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "osiLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "osiLib - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "..\..\osi\include" /I "..\..\osi\osiclp\include" /I "..\..\coin\include" /I "..\..\clp\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "osiLib - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\..\osi\include" /I "..\..\osi\osiclp\include" /I "..\..\coin\include" /I "..\..\clp\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "osiLib - Win32 Release"
# Name "osiLib - Win32 Debug"
# Begin Group "Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Coin\include\CoinBuild.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinDistance.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinError.hpp
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

SOURCE=..\..\Coin\include\CoinLpIO.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinMessageHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinModelUseful.hpp
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

SOURCE=..\..\Coin\include\CoinPresolveDoubleton.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolveDual.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolveDupcol.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolveEmpty.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolveFixed.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolveForcing.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolveImpliedFree.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolveIsolated.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolveMatrix.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolvePsdebug.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolveSingleton.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolveSubst.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolveTighten.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolveTripleton.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolveUseless.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinPresolveZeros.hpp
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

SOURCE=..\..\Coin\include\CoinWarmStart.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinWarmStartBasis.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiAuxInfo.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiColCut.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiCollections.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiCuts.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiPresolve.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiRowCutDebugger.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiSimplexInterface.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Osi\OsiAuxInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\OsiColCut.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\OsiCut.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiCut.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\OsiCuts.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\OsiPresolve.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\OsiRowCut.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiRowCut.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\OsiRowCutDebugger.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\OsiSolverBranch.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiSolverBranch.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\OsiSolverInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiSolverInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiSolverParameters.hpp
# End Source File
# End Target
# End Project
