#
# TODO: Setup ACRO_BUILD_DATE macro (e.g. 20050221)
#


###############################################################
# boilerplate

AC_PREREQ(2.59)
AC_INIT([acro], [VOTD], [acro-help@software.sandia.gov])

rm -f packages/dummy/Makefile.in
if test -d packages ; then : ; else mkdir packages ; fi
if test -d packages/dummy ; then : ; else mkdir packages/dummy ; fi
touch packages/dummy/Makefile.in

###############################################################
# A file or directory that should be found here, so we know
# we are in the right place.

AC_CONFIG_SRCDIR([config/siteconfig.pl])


###############################################################
# Location of our m4 macros.  These test for the features
# we are interested.  This doesn't actually work.  Documentation
# says it will in the future.  We need to define
# ACLOCAL_AMFLAGS in Makefile.am instead.  autoconf looks there
# for some reason.

AC_CONFIG_MACRO_DIR([config])
AC_CONFIG_AUX_DIR([config])


###############################################################
# Create this header file which will help configure our Makefiles
# and source files at build time.

AC_CONFIG_HEADERS([include/config.h])
AX_PREFIX_CONFIG_H([include/acro_config.h])
AH_BOTTOM(
#ifndef ACRO_HAVE_EXPLICIT
#ifndef explicit
#define explicit
#endif
#endif
#ifdef ACRO_USING_UTILIB
#include <utilib/utilib_config.h>
#endif
)


###############################################################
# Setup platform information and initialize automake

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([foreign -Wall -Werror filename-length-max=256 tar-ustar])
#AM_INIT_AUTOMAKE([1.9 foreign -Wall -Werror tar-ustar])


###############################################################
# Platform ID

ACRO_PLATFORM


###############################################################
# Configure command line options
# Default compilation flags: -O2 plus maybe some arch/compiler
# special flags

# Various options to override compiler flags
ACRO_COMPILER_OPTIONS
# Control whether UTILIB CommonIO mapping is used
ACRO_COMMONIO_OPTION
# Control whether UTILIB memory debugging utilities are used
ACRO_MEMDEBUG_OPTION
# Control whether UTILIB checksum is used
ACRO_CHECKSUM_OPTION
# Control whether executables are built
ACRO_EXECUTABLES_OPTION
# Control the type of compiler used
ACRO_COMPILER_OPTION
# Control wether test codes are built
ACRO_TEST_OPTION
# Control wether code validation is used
ACRO_VALIDATING_OPTION
# Various options to setup the use of MPI
ACRO_MPI_OPTIONS
# Ensure they did not say --enable-shared, which is unsupported
ACRO_NO_SHARED


###############################################################
# Checks for programs

temp_fflags=${FFLAGS}
temp_cflags=${CFLAGS}
temp_cxxflags=${CXXFLAGS}

#
# Even if we have decided we are going to use mpicc, etc, we
# still need to run AC_PROG_CC, etc.  They set up variables
# (like $OBJEXT) that are used by subsequent macros.
#

#
# Don't supply a full path name to AC_PROG_CC, etc.
# AC_PROG_CC(/usr/bin/gcc) fails but AC_PROG_CC(gcc) works 
# if /usr/bin is in the PATH.
#

# AC_PROG_CC: If environment variable CC is set, that becomes
#  the compiler to use, else it searches for compilers in list.
#  (There are sensible defaults to search for if the list is
#  empty.)  This macro sets the variable $ac_cv_prog_cc_stdc to
#  'yes' or 'no' if the compiler is ANSI or not.  If compiler is
#  GNU C, shell variable GCC is set to 'yes'.  CFLAGS is set
#  to "-g -O3" if not already set.

if test "X${MPI_CC}" = "Xnone" ; then
  if test X${USERDEF_CC} = Xnotset ; then
    if test X$with_vendor_compilers = Xyes ; then
      c_compilers="icc pgcc xlc cc gcc"
    else
      c_compilers="cc gcc"
    fi
  else
    c_compilers="${USERDEF_CC}"
  fi
else
  c_compilers="${MPI_CC}"
fi

AC_PROG_CC(${c_compilers})

# AC_PROG_CXX: If environment variable CXX or CCC is set, this
#  becomes the compiler to use, otherwise this macro searches
#  the list of compilers provided to it.  If compiler is
#  GNU C++, this macro sets the shell variable GXX to 'yes'.
#  This macro sets CXXFLAGS to "-g -O3" if it was not already set.

if test "X${MPI_CXX}" = "Xnone" ; then 
  if test X${USERDEF_CXX} = Xnotset ; then
    if test X${build_os} = Xcygwin ; then
      cpp_compilers="g++"
    else
      if test X$with_vendor_compilers = Xyes ; then
        cpp_compilers="icpc pgCC xlC c++ g++"
      else
        cpp_compilers="c++ g++"
      fi
    fi
  else
    cpp_compilers="${USERDEF_CXX}"
  fi
else
  cpp_compilers="${MPI_CXX}"
fi

AC_PROG_CXX(${cpp_compilers})

if test "X${MPI_F77}" = "Xnone" ; then 
  if test X${USERDEF_F77} = Xnotset ; then
    if test X$with_vendor_compilers = Xyes ; then
      f77_compilers="ifort pgf77 xlf90 gfortran g77 f77 f90"
    else
      f77_compilers="gfortran g77 f77 f90 ifort pgf77 xlf90"
    fi
  else
    f77_compilers="${USERDEF_F77}"
  fi
else
  f77_compilers="${MPI_F77}"
fi

AC_PROG_F77(${f77_compilers})

if test "X${F77}" = "X" ; then
  ac_cv_have_fortran=no
  coliny_warn="coliny will be built without blas or lapack, because these require fortran"
  gnlp_warn="gnlp will be built without npsol, blas, or lapack, because they require fortran"
  pico_warn="pico will be built without blas or lapack, because they require fortran"
  #
  # appspack has 1 fortran source file, but it looks like it is never built
  #
else
  ac_cv_have_fortran=yes
fi

AC_PROG_CXXCPP
AC_PROG_RANLIB
AC_PROG_LN_S
AM_PROG_AS

FFLAGS=${temp_fflags}
CFLAGS=${temp_cflags}
CXXFLAGS=${temp_cxxflags}


###############################################################
# Platform-specific compiler options

# Setup CXX flags
AC_CXX_COMPILER_VENDOR([CXX_VENDOR])
AC_CXX_CPPFLAGS_STD_LANG([CXXFLAGS])

ACRO_PLATFORM_FLAGS

AM_CONDITIONAL(HAVE_GNU_COMPILER,false)
AM_CONDITIONAL(HAVE_PGI_COMPILER,false)

if test X$ac_cv_cxx_compiler_vendor = Xpgi ; then
  AM_CONDITIONAL(HAVE_PGI_COMPILER,true)
else
  if test X$ac_cv_cxx_compiler_vendor = Xgnu ; then
    AM_CONDITIONAL(HAVE_GNU_COMPILER,true)
  fi
fi

###############################################################
# Setup compiler flags
#    this macro depends on values previously set in
#       acro_compiler_options
#       acro_platform_flags

ACRO_COMPILER_FLAGS

###############################################################
# checks for headers

AC_HEADER_STDC

if test "X$ac_cv_header_stdc" = "Xyes"; then
  AM_CONDITIONAL(HAVE_ANSI_C_HEADERS, true)
else
  AM_CONDITIONAL(HAVE_ANSI_C_HEADERS, false)
fi

AC_CHECK_HEADERS([values.h strings.h unistd.h])

###############################################################
# Checks for libraries

if test "X${ac_cv_have_fortran}" = "Xyes" ; then
  # Find F77 link flags
  AC_F77_LIBRARY_LDFLAGS

  ACX_BLAS(,[AC_MSG_WARN([No BLAS --- either because it is disabled or could not be found])])

  ACX_LAPACK(,[AC_MSG_WARN([No LAPACK --- either because it is disabled or could not be found])])

  # Define F77_FUNC - used for correctly munging Fortran function calls
  AC_F77_WRAPPERS

  ACX_NPSOL(,[AC_MSG_WARN([No NPSOL --- either because it is disabled or could not be found])])
else
  BLAS_LIBS=""
  acx_blas_ok=no
  AC_SUBST(BLAS_LIBS)

  LAPACK_LIBS=""
  acx_lapack_ok=no
  AC_SUBST(LAPACK_LIBS)

  NPSOL_LIBS=""
  acx_npsol_ok=no
  AC_SUBST(NPSOL_LIBS)
fi

ACRO_CPLEX_OPTIONS

###############################################################
# Validate MPI builds

ACRO_MPI_TESTS

###############################################################
# checks for typedefs, structures and compiler behavior

AC_TYPE_SIZE_T

if test "X${ac_cv_type_size}" = "Xyes" ; then
  AM_CONDITIONAL(HAVE_SIZE_T, true)
else
  AM_CONDITIONAL(HAVE_SIZE_T, false)
fi


###############################################################
# checks for compiler behavior
#
# AC_CXX_* are checks for C++ behavior
  
# Defines HAVE_NAMESPACES if we can use them
#   TODO - find out if the means std namespace only, or any namespace
#          is it possible we can declare namespaces, but there's no std?
#          Modifying sources to use HAVE_NAMESPACES is not complete
AC_CXX_NAMESPACES

if test X${ac_cv_cxx_namespaces} = Xyes ; then
  AM_CONDITIONAL(HAVE_NAMESPACES, true)
else
  AM_CONDITIONAL(HAVE_NAMESPACES, false)
fi
  
# WEH - turned off because these are not portable
# Defines HAVE_TEMPLATES_AS_TEMPLATE_ARGUMENTS if we can pass a template
# argument in for a template parameter
#AC_CXX_TEMPLATES_AS_TEMPLATE_ARGUMENTS

# Defines HAVE_EXCEPTIONS if we can use throw/catch
AC_CXX_EXCEPTIONS
  
# Defines HAVE_STD if we can include ISO C++ headers
# (iostream, map, iomanip and cmath)
AC_CXX_HAVE_STD

if test X${ac_cv_cxx_have_std} = Xyes ; then
  AM_CONDITIONAL(HAVE_ANSI_CPP_HEADERS, true)
else
  AM_CONDITIONAL(HAVE_ANSI_CPP_HEADERS, false)
fi
  
# Define HAVE_SSTREAM if we have stringstreams
AC_CXX_HAVE_SSTREAM
  
# Define HAVE_EXPLICIT if we can use the explicit keyword
AC_CXX_EXPLICIT
  
# Define HAVE_MEMBER_TEMPLATES if class member functions can be templated
AC_CXX_MEMBER_TEMPLATES

AC_CXX_HAVE_SSTREAM


###############################################################
# Platform-specific C++ compiler settings

case "$ac_cv_cxx_compiler_vendor" in
    gnu)
        AX_CXXFLAGS_GCC_OPTION([-fpermissive])
    ;;
    compaq)
        AX_CXXFLAGS_OSF_OPTION([-tlocal -distinguish_nested_enums])
    ;;
    sgi)
        AX_CXXFLAGS_IRIX_OPTION([-ptused])
    ;;
    *)
    ;;
esac


###############################################################
# checks for functions

AC_CHECK_FUNCS([getrusage],
  [AM_CONDITIONAL(HAVE_GETRUSAGE, true)],
  [AM_CONDITIONAL(HAVE_GETRUSAGE, false)])

AC_CHECK_FUNCS([strerror],
  [AM_CONDITIONAL(HAVE_STRERROR, true)],
  [AM_CONDITIONAL(HAVE_STRERROR, false)])

# Find DLOPEN (defines DLOPEN_LIBS to contain the libraries needed to link)
ACX_FUNC_DLOPEN


###############################################################
# more checks

AC_C_BIGENDIAN()

if test X${ac_cv_c_bigendian} = Xyes ; then
  AM_CONDITIONAL(WORDS_BIGENDIAN, true)
else
  AM_CONDITIONAL(WORDS_BIGENDIAN, false)
fi

###############################################################
# We are all done with actions which modify the compilers
# and build flags.  Add them to ac_configure_args so subpackages
# will get them.
#
AC_MSG_NOTICE(CC=${CC})
AC_MSG_NOTICE(CXX=${CXX})
AC_MSG_NOTICE(F77=${F77})
AC_MSG_NOTICE(CPPFLAGS=${CPPFLAGS})
AC_MSG_NOTICE(CFLAGS=${CFLAGS})
AC_MSG_NOTICE(CXXFLAGS=${CXXFLAGS})
AC_MSG_NOTICE(FFLAGS=${FFLAGS})
AC_MSG_NOTICE(LDFLAGS=${LDFLAGS})
AC_MSG_NOTICE(LIBS=${LIBS})

ac_configure_args="$ac_configure_args 'CC=${CC}' 'CXX=${CXX}' 'F77=${F77}'"
ac_configure_args="$ac_configure_args 'CPPFLAGS=${CPPFLAGS}' 'FLAGS=${CFLAGS}'"
ac_configure_args="$ac_configure_args 'CFLAGS=${CFLAGS}' 'FFLAGS=${FFLAGS}'"
ac_configure_args="$ac_configure_args 'CXXFLAGS=${CXXFLAGS}'"
ac_configure_args="$ac_configure_args 'LDFLAGS=${LDFLAGS}' 'LIBS=${LIBS}'"

###############################################################
# Options related to including packages and reconfiguring
# autoconf'd subpackages.

# snl-user
if test -d ${srcdir}/snl-user ; then
      AC_CONFIG_FILES([snl-user/dummy/Makefile:snl-user/dummy/Makefile.in:packages/dummy/Makefile.in])
fi

# utilib
ACRO_ARG_WITH_PACKAGE(utilib, UTILIB, utilib,
       ${srcdir}/packages/utilib/Makefile.am, yes)
ACRO_ARG_ENABLE_PACKAGE_RECONFIG(utilib, packages/utilib/config.status, yes)
if test X${ac_cv_reconfigure_utilib} = Xyes ; then
   if test -d ${srcdir}/packages/utilib ; then
      AC_CONFIG_SUBDIRS([packages/utilib])
   fi
fi

# 3po
ACRO_ARG_WITH_PACKAGE(3po, THREEPO, 3po,
       ${srcdir}/packages/tpl/3po/cobyla2c/cobyla.c, yes)
if test X${ac_cv_with_3po} = Xyes ; then
   if test -d ${srcdir}/packages/tpl/3po ; then
      AC_CONFIG_FILES([packages/tpl/3po/Makefile:packages/tpl/3po/Makefile.in:packages/dummy/Makefile.in])
   fi
fi

# ampl
ACRO_ARG_WITH_PACKAGE(ampl, AMPL, ampl,
       ${srcdir}/packages/tpl/ampl/ampl/conval.c, yes)
if test X${ac_cv_with_ampl} = Xyes ; then
   if test -d ${srcdir}/packages/tpl/ampl ; then
      AC_CONFIG_FILES([packages/tpl/ampl/Makefile:packages/tpl/ampl/Makefile.in:packages/dummy/Makefile.in])
   fi
fi

# appspack
ACRO_ARG_WITH_PACKAGE(appspack, APPSPACK, appspack,
       ${srcdir}/packages/appspack/src/APPSPACK_Vector.cpp, yes)
ACRO_ARG_ENABLE_PACKAGE_RECONFIG(appspack, packages/appspack/config.status, yes)
if test X${ac_cv_reconfigure_appspack} = Xyes ; then
  if test -d ${srcdir}/packages/appspack ; then
     AC_CONFIG_SUBDIRS([packages/appspack])
  fi
fi

# coin
ACRO_ARG_WITH_PACKAGE(coin, COIN, coin,
       ${srcdir}/packages/tpl/coin/COIN_2006Sep06/Osi/src/OsiRowCut.cpp, yes)
if test X${ac_cv_with_coin} = Xyes ; then
   if test -d ${srcdir}/packages/tpl/coin ; then
      #AC_CONFIG_SUBDIRS([packages/tpl/coin/COIN_2006Sep06])
      AC_CONFIG_FILES([packages/tpl/coin/Makefile:packages/tpl/coin/Makefile.in:packages/dummy/Makefile.in])
   fi
fi

# colin
ACRO_ARG_WITH_PACKAGE(colin, COLIN, colin,
       ${srcdir}/packages/colin/colin/real.h, yes)
if test X${ac_cv_with_colin} = Xyes ; then
   if test -d ${srcdir}/packages/colin ; then
      AC_CONFIG_FILES([packages/colin/Makefile:packages/colin/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/colin/doc/Makefile:packages/colin/doc/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/colin/colin/Makefile:packages/colin/colin/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/colin/test/Makefile:packages/colin/test/Makefile.in:packages/dummy/Makefile.in])
   fi
fi

# coliny
ACRO_ARG_WITH_PACKAGE(coliny, COLINY, coliny,
       ${srcdir}/packages/coliny/src/lib/PatternSearch.cpp, yes)
if test X${ac_cv_with_coliny} = Xyes ; then
   if test -d ${srcdir}/packages/coliny ; then
      AC_CONFIG_FILES([packages/coliny/Makefile:packages/coliny/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/coliny/doc/Makefile:packages/coliny/doc/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/coliny/src/Makefile:packages/coliny/src/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/coliny/coliny/Makefile:packages/coliny/coliny/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/coliny/test/Makefile:packages/coliny/test/Makefile.in:packages/dummy/Makefile.in])
   fi
   if test "X${ac_cv_have_fortran}"="Xno" ; then
     AC_MSG_WARN([$coliny_warn])
   fi
fi

# dscpack
ACRO_ARG_WITH_PACKAGE(dscpack, DSCPACK, dscpack,
       ${srcdir}/packages/tpl/dscpack/Makefile.am, yes)
if test X${ENABLE_MPI} = Xyes ; then
   if test X${ac_cv_with_dscpack} = Xyes ; then
      if test -d ${srcdir}/packages/tpl/dscpack ; then
         AC_CONFIG_FILES([packages/tpl/dscpack/Makefile:packages/tpl/dscpack/Makefile.in:packages/dummy/Makefile.in])
      fi
   fi
else
   DSCPACK_DIR=""
   BUILD_DSCPACK_TRUE='#'
   BUILD_DSCPACK_FALSE=

fi

# exact
ACRO_ARG_WITH_PACKAGE(exact, EXACT, exact,
       ${srcdir}/packages/exact/src/doe/complete_doe.cpp, yes)
if test X${ac_cv_with_exact} = Xyes ; then
   if test -d ${srcdir}/packages/exact ; then
      AC_CONFIG_FILES([packages/exact/Makefile:packages/exact/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/exact/test/Makefile:packages/exact/test/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/exact/src/Makefile:packages/exact/src/Makefile.in:packages/dummy/Makefile.in])
   fi
fi

# filib
ACRO_ARG_WITH_PACKAGE(filib, FILIB, filib,
       ${srcdir}/packages/tpl/filib/filib-coco/bsd/strtod.h, yes)
if test X${ac_cv_with_filib} = Xyes ; then
   if test -d ${srcdir}/packages/tpl/filib ; then
      AC_CONFIG_FILES([packages/tpl/filib/Makefile:packages/tpl/filib/Makefile.in:packages/dummy/Makefile.in])
   fi
fi

# glpk
ACRO_ARG_WITH_PACKAGE(glpk, GLPK, glpk,
       ${srcdir}/packages/tpl/glpk/glpk/ChangeLog, yes)
ACRO_ARG_ENABLE_PACKAGE_RECONFIG(glpk,
     packages/tpl/glpk/glpk/config.status, no)
if test X${ac_cv_with_glpk} = Xyes ; then
   if test -d ${srcdir}/packages/tpl/glpk ; then
      if test X${ac_cv_reconfigure_glpk} = Xyes ; then
        AC_CONFIG_SUBDIRS([packages/tpl/glpk/glpk])
      fi
      AC_CONFIG_FILES([packages/tpl/glpk/Makefile:packages/tpl/glpk/Makefile.in:packages/dummy/Makefile.in])
   fi
fi

# gnlp
ACRO_ARG_WITH_PACKAGE(gnlp, GNLP, gnlp,
       ${srcdir}/packages/gnlp/src/gnlp/gnlpdag.cpp, yes)
if test X${ac_cv_with_gnlp} = Xyes ; then
   if test -d ${srcdir}/packages/gnlp ; then
      AC_CONFIG_FILES([packages/gnlp/Makefile:packages/gnlp/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/gnlp/doc/Makefile:packages/gnlp/doc/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/gnlp/src/Makefile:packages/gnlp/src/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/gnlp/src/headers/Makefile:packages/gnlp/src/headers/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/gnlp/test/Makefile:packages/gnlp/test/Makefile.in:packages/dummy/Makefile.in])
   fi
   if test "X${ac_cv_have_fortran}"="Xno" ; then
     AC_MSG_WARN([$gnlp_warn])
   fi
fi

# ipopt
ACRO_ARG_WITH_PACKAGE(ipopt, IPOPT, ipopt,
       ${srcdir}/packages/tpl/ipopt/ipopt/ChangeLog, yes)
ACRO_ARG_ENABLE_PACKAGE_RECONFIG(ipopt, packages/tpl/ipopt/config.status, no)
if test X${ac_cv_reconfigure_ipopt} = Xyes ; then
  if test -d ${srcdir}/packages/tpl ; then
     AC_CONFIG_SUBDIRS([packages/tpl/ipopt])
  fi
fi

# mtl
ACRO_ARG_WITH_PACKAGE(mtl, MTL, mtl,
       ${srcdir}/packages/tpl/mtl/mtl/Makefile.am, yes)
  #
  # WEH - reconfiguration doesn't really make sense here
  #
  #AM_CONDITIONAL(RECONFIGURE_MTL, false)
  #ACRO_ARG_ENABLE_PACKAGE_RECONFIG(mtl, packages/tpl/mtl/mtl/config.status, no)
  #if test X${ac_cv_reconfigure_mtl} = Xyes ; then
  #  AM_CONDITIONAL(RECONFIGURE_MTL, true)
  #fi
  #
  # We don't AC_CONFIG_SUBDIRS because of errors.  We configure
  # instead from the makefile. 
  #
if test X${ac_cv_with_mtl} = Xyes ; then
   if test -d ${srcdir}/packages/tpl/mtl ; then
      AC_CONFIG_FILES([packages/tpl/mtl/Makefile:packages/tpl/mtl/Makefile.in:packages/dummy/Makefile.in])
   fi
fi

# OPT++
if test X$ac_cv_cxx_compiler_vendor = Xpgi ; then 
  AC_MSG_NOTICE([Omitting Opt++ build because it is not yet ported to PGI])
  AM_CONDITIONAL(BUILD_OPTPP,false)
else
  ACRO_ARG_WITH_PACKAGE(optpp, OPTPP, optpp,
         ${srcdir}/packages/optpp/Makefile.am, yes)
  ACRO_ARG_ENABLE_PACKAGE_RECONFIG(optpp, packages/optpp/config.status, yes)
  if test X${ac_cv_reconfigure_optpp} = Xyes ; then
     if test -d ${srcdir}/packages/optpp ; then
        AC_CONFIG_SUBDIRS([packages/optpp])
     fi
  fi
fi

# parpcx
ACRO_ARG_WITH_PACKAGE(parpcx, PARPCX, parpcx,
       ${srcdir}/packages/parpcx/src/main/io.cc, no)
if test X${ac_cv_with_parpcx} = Xyes ; then
   if test -d ${srcdir}/packages/parpcx ; then
      AC_CONFIG_FILES([packages/parpcx/Makefile:packages/parpcx/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/parpcx/src/Makefile:packages/parpcx/src/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/parpcx/doc/Makefile:packages/parpcx/doc/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/parpcx/src/headers/Makefile:packages/parpcx/src/headers/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/parpcx/src/pcx/Makefile:packages/parpcx/src/pcx/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/parpcx/test/Makefile:packages/parpcx/test/Makefile.in:packages/dummy/Makefile.in])
   fi
fi

# pebbl
ACRO_ARG_WITH_PACKAGE(pebbl, PEBBL, pebbl,
       ${srcdir}/packages/pebbl/src/bb/pebbl/branching.h, yes)
if test X${ac_cv_with_pebbl} = Xyes ; then
   if test -d ${srcdir}/packages/pebbl ; then
      AC_CONFIG_FILES([packages/pebbl/Makefile:packages/pebbl/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/pebbl/doc/Makefile:packages/pebbl/doc/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/pebbl/src/Makefile:packages/pebbl/src/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/pebbl/src/headers/Makefile:packages/pebbl/src/headers/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/pebbl/test/Makefile:packages/pebbl/test/Makefile.in:packages/dummy/Makefile.in])
   fi
fi

# pico
AM_CONDITIONAL(PICO_TEST_DIR_COMPILES, false)
ACRO_ARG_WITH_PACKAGE(pico, PICO, pico,
       ${srcdir}/packages/pico/src/milp/pico/parMilp.h, yes)
if test X${ac_cv_with_pico} = Xyes ; then
   if test -d ${srcdir}/packages/pico ; then
      AC_CONFIG_FILES([packages/pico/Makefile:packages/pico/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/pico/doc/Makefile:packages/pico/doc/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/pico/src/Makefile:packages/pico/src/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/pico/src/headers/Makefile:packages/pico/src/headers/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/pico/test/Makefile:packages/pico/test/Makefile.in:packages/dummy/Makefile.in])
   fi
   if test "X${ac_cv_have_fortran}"="Xno" ; then
     AC_MSG_WARN([$pico_warn])
   fi
fi

# soplex
ACRO_ARG_WITH_PACKAGE(soplex, SOPLEX, soplex,
       ${srcdir}/packages/tpl/soplex/SOPLEX_1_2_1/README, yes)
if test X${ac_cv_with_soplex} = Xyes ; then
   if test -d ${srcdir}/packages/tpl/soplex ; then
      AC_CONFIG_FILES([packages/tpl/soplex/Makefile:packages/tpl/soplex/Makefile.in:packages/dummy/Makefile.in])
   fi
fi

# tmf
ACRO_ARG_WITH_PACKAGE(tmf, TMF, tmf,
       ${srcdir}/packages/tmf/src/dummy.cpp, yes)
if test X${ac_cv_with_tmf} = Xyes ; then
   if test -d ${srcdir}/packages/tmf ; then
      AC_CONFIG_FILES([packages/tmf/Makefile:packages/tmf/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/tmf/doc/Makefile:packages/tmf/doc/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/tmf/src/Makefile:packages/tmf/src/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/tmf/src/headers/Makefile:packages/tmf/src/headers/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/tmf/test/Makefile:packages/tmf/test/Makefile.in:packages/dummy/Makefile.in])
   fi
fi

# tracecache
ACRO_ARG_WITH_PACKAGE(tracecache, TRACECACHE, tracecache,
       ${srcdir}/packages/tracecache/src/dummy.cpp, yes)
if test X${ac_cv_with_tracecache} = Xyes ; then
   if test -d ${srcdir}/packages/tracecache ; then
      AC_CONFIG_FILES([packages/tracecache/Makefile:packages/tracecache/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/tracecache/doc/Makefile:packages/tracecache/doc/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/tracecache/src/Makefile:packages/tracecache/src/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/tracecache/src/headers/Makefile:packages/tracecache/src/headers/Makefile.in:packages/dummy/Makefile.in])
      AC_CONFIG_FILES([packages/tracecache/test/Makefile:packages/tracecache/test/Makefile.in:packages/dummy/Makefile.in])
   fi
fi

# trilinos
ACRO_ARG_WITH_PACKAGE(trilinos, TRILINOS, trilinos,
       ${srcdir}/packages/tpl/trilinos/trilinos_6_0_17/CHANGELOG, no)
if test X${ac_cv_with_trilinos} = Xyes ; then
   if test -d ${srcdir}/packages/tpl/trilinos ; then
      AC_CONFIG_FILES([packages/tpl/trilinos/Makefile:packages/tpl/trilinos/Makefile.in:packages/dummy/Makefile.in])
      #AC_CONFIG_SUBDIRS([packages/tpl/trilinos/trilinos_6_0])
   fi
fi

AC_CONFIG_FILES([packages/dummy/Makefile])

# ------------------------------------------------------------------------
# Other package options
# ------------------------------------------------------------------------

#
# --enable-clp=yes is the default
#
AC_ARG_ENABLE(clp,
AS_HELP_STRING([--disable-clp],[Turn off the CLP LP solver]),
[ENABLE_CLP=$enableval],
[ENABLE_CLP=yes]
) 

if test X${ENABLE_CLP} = Xyes ; then
  AC_DEFINE(USING_CLP,1,[define that clp is being used])
fi

#
## TODO: are we using this anywhere?
# --enable-cobyla=yes is the default
#
AC_ARG_ENABLE(cobyla,
AS_HELP_STRING([--disable-cobyla],[Turn off the Cobyla solver]),
[ENABLE_COBYLA=$enableval],
[ENABLE_COBYLA=yes]
) 

if test X${ENABLE_COBYLA} = Xyes ; then
  AC_DEFINE(USING_COBYLA,1,[define that cobyla is being used])
fi

#
# --enable-plgo=yes is the default
#
AC_ARG_ENABLE(plgo,
AS_HELP_STRING([--disable-plgo],[Turn off the PLGO solver]),
[ENABLE_PLGO=$enableval],
[ENABLE_PLGO=yes]
) 

if test X${ENABLE_PLGO} = Xyes ; then
  AC_DEFINE(USING_PLGO,1,[define that plgo is being used])
fi

#
# --with-soplex-1-2-1 will build the old version of soplex instead
#
AC_ARG_WITH(soplex-1-2-1,
AS_HELP_STRING([--with-soplex-1-2-1],[Build this older revision of soplex instead of the current one]),
[WITH_OLD_SOPLEX=$withval],
[WITH_OLD_SOPLEX=no]
) 

AM_CONDITIONAL(BUILD_SOPLEX_1_2_1, true)
#if test X${WITH_OLD_SOPLEX} = Xyes ; then
#  AM_CONDITIONAL(BUILD_SOPLEX_1_2_1, true)
#else
#  AM_CONDITIONAL(BUILD_SOPLEX_1_2_1, false)
#fi

#AM_CONDITIONAL(BUILD_COIN_20050626, false)
#AM_CONDITIONAL(BUILD_COIN_20060202, false)



###############################################################
# Output of autoconf/automake

AC_CONFIG_FILES([Makefile] [doc/Makefile] [packages/Makefile] [user/dummy/Makefile] [packages/tpl/Makefile])

AC_SUBST([ac_configure_args])

AC_OUTPUT

echo
echo "---"
echo "--- Thank you for downloading Acro. Please register your"
echo "--- copy of Acro by subscribing to our announce list at"
echo "--- http://software.sandia.gov/mailman/listinfo/acro-announce"
echo "---"
echo "--- For more information, visit the Acro web pages at"
echo "--- http://software.sandia.gov/Acro/"
echo "---"
echo
