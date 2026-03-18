dnl ACRO CommonIO option

AC_DEFUN([ACRO_COMMONIO_OPTION],[
  dnl
  dnl --enable-commonio=yes is the default
  dnl 
AC_ARG_ENABLE(commonio,
[AC_HELP_STRING([--disable-commonio],[Omit the utilib commonio function])],
[ENABLE_COMMONIO=$enableval],
[ENABLE_COMMONIO=yes]
)

if test X${ENABLE_COMMONIO} = Xyes; then
    AC_DEFINE(YES_COMMONIO,,[define whether CommonIO is included in utilib])
fi

])
