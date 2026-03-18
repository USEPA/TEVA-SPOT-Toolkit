# Microsoft Developer Studio Project File - Name="sbbLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=sbbLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sbbLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sbbLib.mak" CFG="sbbLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sbbLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "sbbLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sbbLib - Win32 Release"

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
# ADD CPP /nologo /W3 /GR /GX /O2 /I "..\..\osi\include" /I "..\..\sbb\include" /I "..\..\Coin\include" /I "..\..\cgl\include" /I "..\..\clp\include" /I "..\..\osi\osiClp\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "sbbLib - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I "..\..\osi\include" /I "..\..\sbb\include" /I "..\..\Coin\include" /I "..\..\cgl\include" /I "..\..\clp\include" /I "..\..\osi\osiClp\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
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

# Name "sbbLib - Win32 Release"
# Name "sbbLib - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Cgl\include\CglCutGenerator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglProbing.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpMatrixBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpObjective.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpParameters.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpSimplex.hpp
# End Source File
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

SOURCE=..\..\Coin\include\CoinPragma.hpp
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

SOURCE=..\..\Osi\OsiClp\include\OsiClpSolverInterface.hpp
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

SOURCE=..\..\Osi\include\OsiRowCutDebugger.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiSimplexInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiSolverInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiSolverParameters.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\Presolve.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbCutGenerator.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Sbb\SbbBranchActual.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbBranchActual.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\SbbBranchBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbBranchBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbCompare.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbCompareActual.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbCompareBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\SbbCountRowCut.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbCountRowCut.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\SbbCutGenerator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\SbbHeuristic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbHeuristic.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\SbbMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\SbbModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\SbbNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SbbNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\SbbTree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\Sbb_C_Interface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\Sbb_C_Interface.h
# End Source File
# End Target
# End Project
