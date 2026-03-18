AC_DEFUN([ACRO_CPLEX_OPTIONS], [

AC_ARG_WITH(cplex-includes,
        AS_HELP_STRING([--with-cplex-incdir=<include-dir>], [path to CPLEX header files]))
AC_ARG_WITH(cplex-libs,
        AS_HELP_STRING([--with-cplex-lib=<library>], [Full path to cplex library]))

case $with_cplex_incdir in
        yes | "" | no) ;;
        *) CPLEX_INCLUDE_DIR="$with_cplex_incdir" ;;
esac
case $with_cplex_lib in
        yes | "" | no) ;;
        *) CPLEX_LIB="$with_cplex_lib" ;;
esac

##
## TODO:
## Find the name of a function that always exists in a cplex library,
## and try to link a simple program using that function here.
##
## AC_MSG_CHECKING(for valid CPLEX library)
## AC_MSG_RESULT(yes)
##

if test "X${CPLEX_LIB}" = "X" ; then
  CPLEX_INCLUDE_DIR=""
  CPLEX_LIB=""
  AM_CONDITIONAL(BUILD_CPLEX, false)
else
  AC_MSG_WARN(Proceeding to configure with $CPLEX_LIBS but we don't know if it's valid.)
  AM_CONDITIONAL(BUILD_CPLEX, true)
  AC_DEFINE(HAVE_CPLEX,1,[Define if you have CPLEX library.])
fi

AC_SUBST(CPLEX_INCLUDE_DIR)
AC_SUBST(CPLEX_LIB)

])
