# Microsoft Developer Studio Project File - Name="smiLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=smiLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "smiLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "smiLib.mak" CFG="smiLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "smiLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "smiLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "smiLib - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\Smi\include" /I "..\..\Osi\include" /I "..\..\Coin\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "OSLMSDLL" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "smiLib - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\Smi\include" /I "..\..\Osi\include" /I "..\..\Coin\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "OSLMSDLL" /YX /FD /GZ /c
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

# Name "smiLib - Win32 Release"
# Name "smiLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\Coin\include\CoinFileIO.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Smi\SmiCoreCombineRule.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Smi\SmiScnData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Smi\SmiScnModel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Smi\SmiSmpsIO.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "CoinHeader"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\include\CoinDistance.hpp
# End Source File
# Begin Source File

SOURCE=..\..\include\CoinError.hpp
# End Source File
# Begin Source File

SOURCE=..\..\include\CoinFinite.hpp
# End Source File
# Begin Source File

SOURCE=..\..\include\CoinFloatEqual.hpp
# End Source File
# Begin Source File

SOURCE=..\..\include\CoinHelperFunctions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\include\CoinMessageHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\include\CoinPackedMatrix.hpp
# End Source File
# Begin Source File

SOURCE=..\..\include\CoinPackedVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\include\CoinPackedVectorBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\include\CoinShallowPackedVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\include\CoinSort.hpp
# End Source File
# Begin Source File

SOURCE=..\..\include\OsiCollections.hpp
# End Source File
# Begin Source File

SOURCE=..\..\include\OsiSolverInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\include\OsiSolverParameters.hpp
# End Source File
# End Group
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

SOURCE=..\..\Coin\include\CoinShallowPackedVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Coin\include\CoinSort.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiCollections.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiSolverInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Osi\include\OsiSolverParameters.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Smi\include\SmiCoreCombineRule.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Smi\include\SmiDiscreteDistribution.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Smi\include\SmiLinearData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Smi\include\SmiScenarioTree.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Smi\include\SmiScnData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Smi\include\SmiScnModel.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Smi\include\SmiSmpsIO.hpp
# End Source File
# End Group
# End Target
# End Project
