dnl ACRO compiler option

AC_DEFUN([ACRO_COMPILER_OPTION],[
  dnl
  dnl Can be used to specify specific characteristics about the compiler.
  dnl 
AC_ARG_WITH(compiler,
[AC_HELP_STRING([--with-compiler],[Use --with-compiler=mingw to build mingw executables and libraries])],
[WITH_COMPILER=$withval],
[WITH_COMPILER=notset]
)
if test X${WITH_COMPILER} = Xmingw && test $build_os != cygwin ; then
  AC_MSG_ERROR([mingw code can only be built on a cygwin system])
fi

if test X${WITH_COMPILER} != Xmingw && test X${WITH_COMPILER} != Xnotset ; then
  AC_MSG_ERROR([Sorry, --with-compiler feature is only implemented for mingw right now])
fi


])
