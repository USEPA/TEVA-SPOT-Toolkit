/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file utilib_dll.h
 *
 * This information is used by MVC++ to create a DLL.
 *
 * \author William E. Hart
 */
/**
 * \def UTILIB_API
 *
 * Macro that specifies the import or export information used for
 * making a compiled symbol externally visible within a DLL.
 */

#ifndef utilib_utilib_dll_h
#define utilib_utilib_dll_h

#if 0
// ifdef _MSC_VER
#ifdef UTILIB_EXPORTS                                                    
#define UTILIB_API __declspec(dllexport)                                        
#else                                                                           
#define UTILIB_API __declspec(dllimport)                                        
#endif

#else
#define UTILIB_API
#endif

#endif
