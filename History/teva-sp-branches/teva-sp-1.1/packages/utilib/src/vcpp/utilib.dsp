# Microsoft Developer Studio Project File - Name="utilib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=utilib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "utilib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "utilib.mak" CFG="utilib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "utilib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "utilib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "utilib - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UTILIB_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UTILIB_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UTILIB_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /MDd /W3 /Gm /GX /ZI /Od /I "C:/Program Files/Python/include" /I ".." /I "../adt" /I "../templates" /I "../sys" /I "../io" /I "../ranlib/include" /I "../sort" /I "../math" /D "_CRTDBG_MAP_ALLOC" /D "WIN32" /D "__WIN32__" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UTILIB_EXPORTS" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"C:/Program Files/Python/libs"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "utilib - Win32 Release"
# Name "utilib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "adt"

# PROP Default_Filter "*.h *.c *.cpp"
# Begin Source File

SOURCE=..\adt\AbstractHashTable.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\AbstractHashTable.h
# End Source File
# Begin Source File

SOURCE=..\adt\AbstractHeap.cpp
# End Source File
# Begin Source File

SOURCE=..\adt\AbstractHeap.h
# End Source File
# Begin Source File

SOURCE=..\adt\AbstractSplayTree.cpp
# End Source File
# Begin Source File

SOURCE=..\adt\AbstractSplayTree.h
# End Source File
# Begin Source File

SOURCE=..\adt\Basic2DArray.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\Basic2DArray.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\BasicArray.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\BasicArray.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\BitArray.cpp
# End Source File
# Begin Source File

SOURCE=..\adt\BitArray.h
# End Source File
# Begin Source File

SOURCE=..\adt\CharString.cpp
# End Source File
# Begin Source File

SOURCE=..\adt\CharString.h
# End Source File
# Begin Source File

SOURCE=..\adt\DoubleMatrix.h
# End Source File
# Begin Source File

SOURCE=..\adt\DoubleVector.h
# End Source File
# Begin Source File

SOURCE=..\adt\GenericHashTable.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\GenericHashTable.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\GenericHeap.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\GenericHeap.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\GenericKey.cpp
# End Source File
# Begin Source File

SOURCE=..\adt\GenericKey.h
# End Source File
# Begin Source File

SOURCE=..\adt\GenericSplayTree.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\GenericSplayTree.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\IntKey.h
# End Source File
# Begin Source File

SOURCE=..\adt\IntMatrix.h
# End Source File
# Begin Source File

SOURCE=..\adt\IntVector.h
# End Source File
# Begin Source File

SOURCE=..\adt\LinkedList.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\LinkedList.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\lookupa.cpp
# End Source File
# Begin Source File

SOURCE=..\adt\MixedIntVars.cpp
# End Source File
# Begin Source File

SOURCE=..\adt\MixedIntVars.h
# End Source File
# Begin Source File

SOURCE=..\adt\MultiLL.cpp
# End Source File
# Begin Source File

SOURCE=..\adt\MultiLL.h
# End Source File
# Begin Source File

SOURCE=..\adt\Num2DArray.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\Num2DArray.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\NumArray.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\NumArray.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\OrderedList.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\OrderedList.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\QueueList.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\QueueList.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\RealMatrix.h
# End Source File
# Begin Source File

SOURCE=..\adt\RealVector.h
# End Source File
# Begin Source File

SOURCE=..\adt\Simple2DArray.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\Simple2DArray.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\SimpleArray.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\SimpleArray.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\SimpleHashTable.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\SimpleHashTable.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\SimpleHeap.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\SimpleHeap.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\SimpleSplayTree.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\SimpleSplayTree.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\SparseMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\adt\SparseMatrix.h
# End Source File
# Begin Source File

SOURCE=..\adt\StackArray.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\StackArray.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\StackList.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\StackList.h

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\adt\TwoBitArray.cpp
# End Source File
# Begin Source File

SOURCE=..\adt\TwoBitArray.h
# End Source File
# End Group
# Begin Group "templates"

# PROP Default_Filter "*.h *.c *.cpp"
# Begin Source File

SOURCE=..\templates\Basic2DArray_double.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\Basic2DArray_double.h
# End Source File
# Begin Source File

SOURCE=..\templates\Basic2DArray_int.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\Basic2DArray_int.h
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_AbstractHashTableItemptr.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_AbstractHashTableItemptr.h
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_BitArray.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_BitArray.h
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_char.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_char.h
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_CharString.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_CharString.h
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_double.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_double.h
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_DoubleVector.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_DoubleVector.h
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_int.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_int.h
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_IntVector.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_IntVector.h
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_MPI_Request.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_MPI_Request.h
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_PackBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_PackBuffer.h
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_RealMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_RealMatrix.h
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_RealVector.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\BasicArray_RealVector.h
# End Source File
# Begin Source File

SOURCE=..\templates\Num2DArray_double.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\Num2DArray_double.h
# End Source File
# Begin Source File

SOURCE=..\templates\Num2DArray_int.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\Num2DArray_int.h
# End Source File
# Begin Source File

SOURCE=..\templates\NumArray_double.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\NumArray_double.h
# End Source File
# Begin Source File

SOURCE=..\templates\NumArray_int.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\NumArray_int.h
# End Source File
# Begin Source File

SOURCE=..\templates\OrderedList_voidptr_double.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\OrderedList_voidptr_double.h
# End Source File
# Begin Source File

SOURCE=..\templates\QueueArray_double.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\QueueArray_double.h
# End Source File
# Begin Source File

SOURCE=..\templates\QueueArray_int.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\QueueArray_int.h
# End Source File
# Begin Source File

SOURCE=..\templates\Simple2DArray_double.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\Simple2DArray_double.h
# End Source File
# Begin Source File

SOURCE=..\templates\Simple2DArray_int.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\Simple2DArray_int.h
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_char.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_char.h
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_CharString.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_CharString.h
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_double.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_double.h
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_DoubleVector.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_DoubleVector.h
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_int.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_int.h
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_IntVector.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_IntVector.h
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_RealMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_RealMatrix.h
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_RealVector.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleArray_RealVector.h
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleSplayTree_double.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleSplayTree_double.h
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleSplayTree_int.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\SimpleSplayTree_int.h
# End Source File
# Begin Source File

SOURCE=..\templates\StackArray_double.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\StackArray_double.h
# End Source File
# Begin Source File

SOURCE=..\templates\StackArray_int.cpp
# End Source File
# Begin Source File

SOURCE=..\templates\StackArray_int.h
# End Source File
# End Group
# Begin Group "sys"

# PROP Default_Filter "*.h *.c *.cpp"
# Begin Source File

SOURCE=..\sys\_generic.h
# End Source File
# Begin Source File

SOURCE=..\sys\alloc.c
# End Source File
# Begin Source File

SOURCE=..\sys\alloc.h
# End Source File
# Begin Source File

SOURCE=..\sys\Boolean.h
# End Source File
# Begin Source File

SOURCE=..\sys\cppMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\sys\errmsg.c
# End Source File
# Begin Source File

SOURCE=..\sys\errmsg.h
# End Source File
# Begin Source File

SOURCE=..\sys\general.h
# End Source File
# Begin Source File

SOURCE=..\sys\misc.c
# End Source File
# Begin Source File

SOURCE=..\sys\ncube.c
# End Source File
# Begin Source File

SOURCE=..\sys\paragon.c
# End Source File
# Begin Source File

SOURCE=..\sys\real.h
# End Source File
# Begin Source File

SOURCE=..\sys\seconds.cpp
# End Source File
# Begin Source File

SOURCE=..\sys\stdlibmpi.h
# End Source File
# Begin Source File

SOURCE=..\sys\utilib_dll.h
# End Source File
# End Group
# Begin Group "io"

# PROP Default_Filter "*.h *.c *.cpp"
# Begin Source File

SOURCE=..\io\comments.cpp
# End Source File
# Begin Source File

SOURCE=..\io\comments.h
# End Source File
# Begin Source File

SOURCE=..\io\CommonIO.cpp
# End Source File
# Begin Source File

SOURCE=..\io\CommonIO.h
# End Source File
# Begin Source File

SOURCE=..\io\mpiUtil.cpp
# End Source File
# Begin Source File

SOURCE=..\io\mpiUtil.h
# End Source File
# Begin Source File

SOURCE=..\io\PackBuf.cpp
# End Source File
# Begin Source File

SOURCE=..\io\PackBuf.h
# End Source File
# Begin Source File

SOURCE=..\io\sync_io.cpp
# End Source File
# End Group
# Begin Group "ranlib"

# PROP Default_Filter "*.h *.c *.cpp"
# Begin Source File

SOURCE=..\ranlib\include\Cauchy.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\CRandVar.extern.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\CRandVar.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\CRandVec.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\default_rng.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\DRandVar.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\DUniform.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\random\genbet.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\random\gencau.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\random\genchi.c

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ranlib\random\genexp.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\random\genf.c

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ranlib\random\gengam.c

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ranlib\random\genmn.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\random\gennch.c

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ranlib\random\gennf.c

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ranlib\random\gennor.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\misc\genprm.c

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ranlib\random\genunf.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\rng\global_rng.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\misc\globals.cpp
# End Source File
# Begin Source File

SOURCE=..\ranlib\random\ignbin.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\random\ignpoi.c

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ranlib\misc\LCG.cpp
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\LCG.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\rng\LEC\LEC_gen.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\rng\LEC\LEC_misc.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\rng\LEC\LEC_rng.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\LEC_rng.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\rng\LEC\LEC_seed.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\random\MNormal.cpp
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\MNormal.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\random\MUniform.cpp
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\MUniform.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\Normal.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\misc\ParkMiller.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\misc\PM_LCG.cpp
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\PM_LCG.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\Random.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\RNG.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\random\scauchy.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\random\sexpo.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\random\sgamma.c

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ranlib\random\snorm.c
# End Source File
# Begin Source File

SOURCE=..\ranlib\misc\Triangular.cpp
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\Triangular.h
# End Source File
# Begin Source File

SOURCE=..\ranlib\include\Uniform.h
# End Source File
# End Group
# Begin Group "sort"

# PROP Default_Filter "*.h *.c *.cpp"
# Begin Source File

SOURCE=..\sort\_sort.h
# End Source File
# Begin Source File

SOURCE=..\sort\compare.c
# End Source File
# Begin Source File

SOURCE=..\sort\dotest.c

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\sort\heap_sort.c
# End Source File
# Begin Source File

SOURCE=..\sort\ins_sort.c
# End Source File
# Begin Source File

SOURCE=..\sort\merge_sort.c
# End Source File
# Begin Source File

SOURCE=..\sort\order.c
# End Source File
# Begin Source File

SOURCE=..\sort\order_k.c
# End Source File
# Begin Source File

SOURCE=..\sort\quick_sort.c
# End Source File
# Begin Source File

SOURCE=..\sort\rank.c
# End Source File
# Begin Source File

SOURCE=..\sort\shell_sort.c
# End Source File
# Begin Source File

SOURCE=..\sort\sort.h
# End Source File
# End Group
# Begin Group "math"

# PROP Default_Filter "*.h *.c *.cpp"
# Begin Source File

SOURCE=..\math\_math.h
# End Source File
# Begin Source File

SOURCE=..\math\cholesky.c
# End Source File
# Begin Source File

SOURCE=..\math\cov.c
# End Source File
# Begin Source File

SOURCE=..\math\linpack.h
# End Source File
# Begin Source File

SOURCE=..\math\math.c
# End Source File
# Begin Source File

SOURCE="..\math\median-test.cpp"

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\math\median.cpp
# End Source File
# Begin Source File

SOURCE=..\math\nr.bit.cpp
# End Source File
# Begin Source File

SOURCE=..\math\nr.bit.h
# End Source File
# Begin Source File

SOURCE=..\math\nr.double.cpp
# End Source File
# Begin Source File

SOURCE=..\math\nr.double.h
# End Source File
# Begin Source File

SOURCE=..\math\nr.int.cpp
# End Source File
# Begin Source File

SOURCE=..\math\nr.int.h
# End Source File
# End Group
# Begin Group "swig"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\python\utilib_wrap.cpp

!IF  "$(CFG)" == "utilib - Win32 Release"

!ELSEIF  "$(CFG)" == "utilib - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\utilib.cpp
# End Source File
# Begin Source File

SOURCE=..\utilib.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
