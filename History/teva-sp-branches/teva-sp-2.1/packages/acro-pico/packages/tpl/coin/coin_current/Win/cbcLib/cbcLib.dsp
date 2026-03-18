# Microsoft Developer Studio Project File - Name="cbcLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=cbcLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cbcLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cbcLib.mak" CFG="cbcLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cbcLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "cbcLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cbcLib - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "..\..\osi\include" /I "..\..\cbc\include" /I "..\..\Coin\include" /I "..\..\cgl\include" /I "..\..\clp\include" /I "..\..\osi\osiClp\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "cbcLib - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\..\osi\include" /I "..\..\cbc\include" /I "..\..\Coin\include" /I "..\..\cgl\include" /I "..\..\clp\include" /I "..\..\osi\osiClp\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
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

# Name "cbcLib - Win32 Release"
# Name "cbcLib - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Cbc\include\CbcBranchCut.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcBranchDynamic.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcCutGenerator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcEventHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcFeasibilityBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcStatistics.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcStrategy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcTree.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcTreeLocal.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglClique.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglCutGenerator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglFlowCover.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglGomory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglKnapsackCover.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglMixedIntegerRounding.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglMixedIntegerRounding2.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglOddHole.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglPreProcess.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cgl\include\CglProbing.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpEventHandler.hpp
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

SOURCE=..\..\Clp\include\ClpPresolve.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpSimplex.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpSimplexOther.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Clp\include\ClpSolve.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\Coin_C_defines.h
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

SOURCE=..\..\Osi\include\OsiAuxInfo.hpp
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

SOURCE=..\..\Osi\include\OsiPresolve.hpp
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

SOURCE=..\..\Osi\include\OsiSolverBranch.hpp
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
# End Group
# Begin Source File

SOURCE=..\..\Cbc\Cbc_C_Interface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\Cbc_C_Interface.h
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcBranchActual.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcBranchActual.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcBranchBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcBranchBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcBranchCut.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcBranchDynamic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcCompare.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcCompareActual.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcCompareActual.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcCompareBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcCountRowCut.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcCountRowCut.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcCutGenerator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcEventHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcHeuristic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcHeuristic.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\include\CbcNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcStatistics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcStrategy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcTree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Cbc\CbcTreeLocal.cpp
# End Source File
# End Target
# End Project
