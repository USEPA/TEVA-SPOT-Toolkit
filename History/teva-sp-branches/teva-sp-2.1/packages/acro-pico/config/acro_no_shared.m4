dnl
dnl The only part of Acro that can build a shared library is the
dnl utilib package.  The rest has not been updated to use libtool.
dnl
dnl If the --enable-shared option is chosen, we will pass it along
dnl to the utilib package, but we also force --enable-static, so that
dnl the packages of Acro that use utilib will have a static libutilib
dnl available to them.
dnl
dnl UTILIB's default is to build a static library.
dnl

AC_DEFUN([ACRO_NO_SHARED],

[AC_ARG_ENABLE(shared,
  AS_HELP_STRING([--enable-shared],[Build a shared UTILIB library]), 
  [ENABLE_SHARED=$enableval], 
  [ENABLE_SHARED=no])
]

[AC_ARG_ENABLE(static,
  AS_HELP_STRING([--enable-static],[Build static libraries (assumed, required)]), 
  [ENABLE_STATIC=$enableval], 
  [ENABLE_STATIC=yes])
]

if test X${ENABLE_SHARED} = Xyes ; then
    AC_MSG_WARN([--enable-shared: Only UTILIB will build a shared library.])
fi

if test X${ENABLE_STATIC} = Xno ; then
    echo ">>>> You have chosen to disable the build of static libraries."
    echo ">>>> Some Acro packages can only build static libraries."
    AC_MSG_ERROR([Rerun configure without disabling static libraries.])
fi

)
