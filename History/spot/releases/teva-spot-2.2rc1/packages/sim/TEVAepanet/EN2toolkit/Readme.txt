Contents of TOOLKIT.ZIP
=======================

EPANET is a program that analyzes the hydraulic and water quality behavior of water
distribution systems. The EPANET Programmer's Toolkit is a dynamic link library (DLL) of
functions that allows developers to customize EPANET's computational engine for their 
own specific needs. The functions can be incorporated into 32-bit Windows applications 
written in C/C++, Delphi Pascal, Visual Basic, or any other language that can call
functions within a Windows DLL. The Toolkit DLL file is named EPANET2.DLL and is
distributed with EPANET. The Toolkit comes with several different header files, function 
definition files, and .lib files that simplify the task of interfacing it with C/C++, 
Delphi, and Visual Basic code.

This archive contains the following files:
  TOOLKIT.HLP   -- Windows Help file that describes how to use the Toolkit
  EPANET2.DLL   -- the Toolkit function library
  EPANET2.H     -- header file for using the Toolkit with C/C++
  EPANET2BC.LIB -- LIB file for using the Toolkit with Borland C/C++
  EPANET2VC.LIB -- LIB file for using the Toolkit with Microsoft Visual C++
  EPANET2.PAS   -- import unit for using the Toolkit with Delphi (Pascal)
  EPANET2.BAS   -- declarations module for using the Toolkit with Visual Basic
  README.TXT    -- this file

NOTE: Function names, arguments, and defined constants may have changed from
      those used in previous versions of the Toolkit. Please review the function
      definitions in the TOOLKIT.HLP help file before using the current EPANET2.DLL
      function library with previously written code.
