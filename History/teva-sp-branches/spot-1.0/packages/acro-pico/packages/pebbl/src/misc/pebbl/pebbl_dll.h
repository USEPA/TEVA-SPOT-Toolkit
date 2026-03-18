/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */
/**
 * \file pebbl_dll.h
 *
 * This information is used by MVC++ to create a DLL
 */

#ifndef pebbl_pebbl_dll_h
#define pebbl_pebbl_dll_h

#include <acro_config.h>
#ifdef _MSC_VER
#ifdef PEBBL_EXPORTS
#define PEBBL_API __declspec(dllexport)
#else
#define PEBBL_API __declspec(dllimport)
#endif

#else
#define PEBBL_API
#endif

#endif

