#
# Script to be processed by autoconf to produce a configure script
# and config.h header file.
#
###############################################################
# boilerplate
###############################################################
#
AC_PREREQ(2.59)
AC_INIT([teva-sp], [2.0], [teva-sp-help@sandia.gov])
#
# Turn off features that only maintainers would use
AM_MAINTAINER_MODE
#
# A file that should be present in the source directory
AC_CONFIG_SRCDIR([packages/tevalibs/TEVAUtils/tso.c])
#
# The directory containing our M4 macros
AC_CONFIG_MACRO_DIR([config])
AC_CONFIG_AUX_DIR([config])

###############################################################
# Platform ID

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

ACRO_PLATFORM

#
###############################################################
# Create this header file which will help configure our Makefiles
# and source files at build time.
#
AC_CONFIG_HEADERS([config.h])
AX_PREFIX_CONFIG_H([teva_config.h])

#
# automake initialization
#
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

###############################################################
# Configure command line options
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Compiler options

ACRO_COMPILER_OPTIONS

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Build options

# TODO: determine when to turn on the -DDLL flag in tevalibs. 

ACRO_TEST_OPTION
AM_CONDITIONAL(BUILD_EXECUTABLES, true)

# We only build modules on 32 bit linux

AC_MSG_CHECKING(whether to build modules)

case ${build_cpu} in
  *64)
    cpu_64=yes
    ;;
  *)
    cpu_64=no
    ;; 
esac 

case ${build_os} in
  linux*)
    linux_os=yes
    ;;
  *)
    linux_os=no
    ;;
esac

if test X${cpu_64} = Xno && test X${linux_os} = Xyes ; then
  AM_CONDITIONAL(BUILD_MODULES, true)
  AC_MSG_RESULT(yes)  
else
  AM_CONDITIONAL(BUILD_MODULES, false)
  AC_MSG_RESULT(no)  
fi

TEVA_ENABLE_BUILD(genattack, GENATTACK, yes)

TEVA_ENABLE_BUILD(impact2Dynamic, IMPACT2DYNAMIC, yes)

TEVA_ENABLE_BUILD(tso2Impact, TSO2IMPACT, yes)

TEVA_ENABLE_BUILD(evalsensor, EVALSENSOR, yes)

TEVA_ENABLE_BUILD(setupIPData, SETUPIPDATA, yes)

TEVA_ENABLE_BUILD(SNLRound, SNLROUND, yes)

TEVA_ENABLE_BUILD(epanet, EPANET, yes)

TEVA_ENABLE_BUILD(tso2text, TSO2TEXT, yes)

TEVA_ENABLE_BUILD(threat_assess, THREAT_ASSESS, yes)

TEVA_ENABLE_BUILD(threat_sim, THREAT_SIM, yes)

TEVA_ENABLE_BUILD(grasp, GRASP, yes)

TEVA_ENABLE_BUILD(pmgen, PMGEN, yes)

TEVA_ENABLE_BUILD(impact2pmm, IMPACT2PMM, yes)

TEVA_ENABLE_BUILD(randomsample, RANDOMSAMPLE, yes)

TEVA_ENABLE_BUILD(sideconstraints, SIDECONSTRAINTS, yes)

TEVA_ENABLE_BUILD(imperfect, IMPERFECT, yes)

#TODO - implement this
#TEVA_ENABLE_BUILD(teva-tests, TEVA_TESTS, yes)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# --with-compiler, only used for mingw

ACRO_COMPILER_OPTION

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# options specifying MPI

ACRO_MPI_OPTIONS

# ------------------------------------------------------------------------
# Checks for programs
# ------------------------------------------------------------------------

ACRO_MPI_COMPILERS

temp_fflags=${FFLAGS}
temp_cflags=${CFLAGS}
temp_cxxflags=${CXXFLAGS}

# AC_PROG_CC: If environment variable CC is set, that becomes
#  the compiler to use, otherwise search for compilers in list.
#  (There are sensible defaults to search for if the list is
#  empty.)  This macro sets the variable $ac_cv_prog_cc_stdc to
#  'yes' or 'no' if the compiler is ANSI or not.  If compiler is
#  GNU C, shell variable GCC is set to 'yes'.  CFLAGS is set
#  to "-g -O2" if not already set.

if test -z "${MPI_CC}" ; then
  AC_PROG_CC(cc gcc)
else
  CC=${MPI_CC}
fi

#
# AC_PROG_CXX: If environment variable CXX or CCC is set, this
#  becomes the compiler to use, otherwise this macro searches
#  the list of compilers provided to it.  If compiler is
#  GNU C++, this macro sets the shell variable GXX to 'yes'.
#  This macro sets CXXFLAGS to "-g -O2" if it was not already set.
#
if test -z "${MPI_CXX}" ; then
  if test X${build_os} = Xcygwin ; then
    cpp_compilers="g++ c++ gcc cxx"
  else
    cpp_compilers="CC g++ c++ gcc cxx"
  fi

  AC_PROG_CXX(${cpp_compilers})
else
  CXX=${MPI_CXX}
fi

if test -z "${MPI_F77}" ; then
  AC_PROG_F77(f77 g77 f90 xlf90 ifort)
else
  F77=${MPI_F77}
fi

AC_PROG_CXXCPP
AC_PROG_LN_S
AM_PROG_AS

CFLAGS=${temp_cflags}
CXXFLAGS=${temp_cxxflags}
FFLAGS=${temp_fflags}

###############################################################
# Platform-specific compiler options

ACRO_PLATFORM_FLAGS


###############################################################
# Setup compiler flags
#    this macro depends on values previously set in
#       acro_compiler_options
#       acro_platform_flags

ACRO_COMPILER_FLAGS

###############################################################
# Validate MPI builds

ACRO_MPI_TESTS

###############################################################
#
# TODO: We need to consider SPECIAL_AR

EXPORT_FLAGS="export CC=\"${CC}\" CXX=\"${CXX}\" F77=\"${F77}\" CPPFLAGS=\"${CPPFLAGS}\" CFLAGS=\"${CFLAGS}\" CXXFLAGS=\"${CXXFLAGS}\" FFLAGS=\"${FFLAGS}\" LDFLAGS=\"${LDFLAGS}\" LIBS=\"${LIBS}\""

AC_SUBST([EXPORT_FLAGS])

export CC=${CC}
export CXX=${CXX}
export F77=${F77}
export CPPFLAGS=${CPPFLAGS}
export CFLAGS=${CFLAGS}
export CXXFLAGS=${CXXFLAGS}
export FFLAGS=${FFLAGS}
export LDFLAGS=${LDFLAGS}
export LIBS=${LIBS}

AC_MSG_NOTICE(CC=${CC})
AC_MSG_NOTICE(CXX=${CXX})
AC_MSG_NOTICE(F77=${F77})
AC_MSG_NOTICE(CPPFLAGS=${CPPFLAGS})
AC_MSG_NOTICE(CFLAGS=${CFLAGS})
AC_MSG_NOTICE(CXXFLAGS=${CXXFLAGS})
AC_MSG_NOTICE(FFLAGS=${FFLAGS})
AC_MSG_NOTICE(LDFLAGS=${LDFLAGS})
AC_MSG_NOTICE(LIBS=${LIBS})

# TODO: Find copies of AX_CXXFLAGS_*_OPTION macros so we can
# test that the chosen flags work.
# (AX_CFLAGS_*_OPTION macros are in the autoconf macro archive)

# ------------------------------------------------------------------------
# Libtool (shared library tool) initialization
# Place this after definition of CC, CFLAGS, etc. because it
# uses those.
#
# AM_PROG_LIBTOOL defines --enable-shared, --disable-shared,
# --enable-static and --disable-static.
#
# Shared libraries are enabled by default if the platform supports
# them.  To change this default, include macro AC_DISABLE_SHARED.
# There is also an AC_DISABLE_STATIC.
# Some people like to disable shared while they are developing, to
# speed up builds.
# ------------------------------------------------------------------------

AC_LIBTOOL_DLOPEN
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL
AC_SUBST([LIBTOOL_DEPS])

AM_CONDITIONAL([BUILD_SHARED], [test X${enable_shared} = Xyes])
AM_CONDITIONAL([BUILD_STATIC], [test X${enable_static} = Xyes])

###############################################################
# Checks for libraries
#

###############################################################
# checks for headers
#

AC_HEADER_STDC

if test "X$ac_cv_header_stdc" = "Xyes"; then
  AM_CONDITIONAL(HAVE_ANSI_C_HEADERS, true)
else
  AM_CONDITIONAL(HAVE_ANSI_C_HEADERS, false)
fi

#
###############################################################
# checks for typedefs, structures and compiler behavior
#

#
###############################################################
# checks for compiler behavior
#
# AC_CXX_* are checks for C++ behavior
#
  
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
# checks for functions

###############################################################
# more checks

#
# We have the usual problems treating gmp as a nested
# subpackage with AC_CONFIG_SUBDIRS, so, if it appears
# it has not been configured, we set a variable causing
# the makefile to descend into the directory and configure it.
#
if test -f packages/gmp-4.1.4/config.status ; then
  AC_MSG_WARN([Skipping configuration of gmp because it's already configured.])
  AC_MSG_WARN([Delete config.status in the gmp directory if you want to force a reconfigure.])
  AM_CONDITIONAL(GMP_NOT_CONFIGURED, false)
else
  AM_CONDITIONAL(GMP_NOT_CONFIGURED, true)
fi

if test -f configure.ac ; then
  AM_CONDITIONAL(OUT_OF_SOURCE_BUILD, false)
else
  AM_CONDITIONAL(OUT_OF_SOURCE_BUILD, true)
fi

#
###############################################################
# Output of autoconf/automake
#

# If configure has not been run in the utilib directory,
# add it to the list of subpackages to be configured.

if test -f packages/utilib/config.status ; then
  AC_MSG_WARN([Skipping configuration of utilib because it's already configured.])
  AC_MSG_WARN([Delete config.status in the utilib directory if you want to force a reconfigure.])
else
  AC_CONFIG_SUBDIRS([packages/utilib])
fi

AC_SUBST([ac_configure_args])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([packages/Makefile])
AC_CONFIG_FILES([packages/exact/Makefile])
AC_CONFIG_FILES([packages/exact/src/Makefile])
AC_CONFIG_FILES([packages/exact/test/Makefile])
AC_CONFIG_FILES([packages/tevalibs/Makefile])
AC_CONFIG_FILES([packages/sp/Makefile])
AC_CONFIG_FILES([packages/sp/src/tso2ImpactModule/Makefile])
AC_CONFIG_FILES([packages/sp/src/tso2Impact/Makefile])
AC_CONFIG_FILES([packages/sp/src/rounding/Makefile])
AC_CONFIG_FILES([packages/sp/src/impact2Dynamic/Makefile])
AC_CONFIG_FILES([packages/sp/src/grasp/Makefile])

AC_OUTPUT
