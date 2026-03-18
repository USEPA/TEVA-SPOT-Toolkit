# Microsoft Developer Studio Project File - Name="cglLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=cglLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cglLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cglLib.mak" CFG="cglLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cglLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "cglLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cglLib - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "..\..\osi\include" /I "..\..\cgl\include" /I "..\..\Coin\include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "COIN_USE_CLP" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cglLib - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\..\osi\include" /I "..\..\cgl\include" /I "..\..\Coin\include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "COIN_USE_CLP" /YX /FD /GZ /c
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

# Name "cglLib - Win32 Release"
# Name "cglLib - Win32 Debug"
# Begin Group "Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Cgl\include\CglAllDifferent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglClique.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglDuplicateRow.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglFlowCover.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglMixedIntegerRounding.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglMixedIntegerRounding2.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglPreProcess.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglProcess.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglRedSplit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglStored.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglTwomir.hpp
# End Source File
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

SOURCE=..\..\Coin\include\CoinFactorization.hpp
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

SOURCE=..\..\Coin\include\CoinIndexedVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinMessage.hpp
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

SOURCE=..\..\Osi\include\OsiPresolve.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiRowCut.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiRowCutDebugger.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiSolverInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiSolverParameters.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\Cgl\CglAllDifferent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglClique.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglCliqueHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglCutGenerator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglCutGenerator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglDuplicateRow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglFlowCover.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglFlowCoverTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglGomory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglGomory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglGomoryTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglKnapsackCover.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglKnapsackCover.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglLiftAndProject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglLiftAndProject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglMixedIntegerRounding.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglMixedIntegerRounding2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglOddHole.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglOddHole.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglPreProcess.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglProbing.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglProbing.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglProbingTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglRedSplit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglSimpleRounding.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglSimpleRounding.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglStored.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\CglTwomir.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Sbb\include\SimpleBranchAndBound.hpp
# End Source File
# End Target
# End Project
