dnl ACRO MPI options

AC_DEFUN([ACRO_MPI_OPTIONS],[
  dnl
  dnl Command-line options for setting up MPI
  dnl 
AC_ARG_ENABLE(mpi,
AS_HELP_STRING([--enable-mpi],[Enable MPI support]),
[ENABLE_MPI=$enableval],
[ENABLE_MPI=no]
)

MPI_CXX=none
MPI_CC=none
MPI_F77=none

AC_ARG_WITH(mpi-cxx,
  AS_HELP_STRING([--with-mpi-cxx=<path>],
  [use the specified MPI C++ compiler]),
[
  ENABLE_MPI=yes
  AC_MSG_CHECKING(MPI C++ compiler)
  if test -f ${withval} ; then
    MPI_CXX=${withval}
  fi
  AC_MSG_RESULT([${MPI_CXX}])
  if test "X${MPI_CXX}" = "Xnone" ; then
    AC_MSG_ERROR([MPI C++ compiler script/program not found.])
  fi
]
)

AC_ARG_WITH(mpi-cc,
  AS_HELP_STRING([--with-mpi-cc=<path>],
  [use the specified MPI C compiler]),
[
  ENABLE_MPI=yes
  AC_MSG_CHECKING(MPI C compiler)
  if test -f ${withval} ; then
    MPI_CC=${withval}
  fi
  AC_MSG_RESULT([${MPI_CC}])
  if test "X${MPI_CC}" = "Xnone" ; then
    AC_MSG_ERROR([MPI C compiler script/program not found.])
  fi
]
)

AC_ARG_WITH(mpi-f77,
  AS_HELP_STRING([--with-mpi-f77=<path>],
  [use the specified MPI Fortran compiler]),
[
  ENABLE_MPI=yes
  AC_MSG_CHECKING(MPI Fortran compiler)
  if test -f ${withval} ; then
    MPI_F77=${withval}
  fi
  AC_MSG_RESULT([${MPI_F77}])
  if test "X${MPI_F77}" = "Xnone" ; then
    AC_MSG_ERROR([MPI Fortran compiler script/program not found.])
  fi
]
)

AC_ARG_WITH(mpi-compilers,
AS_HELP_STRING([--with-mpi-compilers=DIR or --with-mpi-compilers=yes],
[use MPI compilers (mpicc, etc.) found in directory DIR, or in PATH if =yes]),
[
  ENABLE_MPI=yes

  if test X${withval} = Xyes; then
    # Look for MPI C++ compile script/program in user's path, set
    # MPI_CXX to name of program found, else set it to "none".  Could
    # add more paths to check as 4th argument if you wanted to.
    # Calls AC_SUBST for MPI_CXX, etc.

    if test "X${MPI_CXX}" = "Xnone" ; then 
      AC_CHECK_PROGS([MY_CXX], [mpic++ mpicxx mpiCC], [none],)
      MPI_CXX=${MY_CXX}
    fi
    if test "X${MPI_CC}" = "Xnone" ; then 
      AC_CHECK_PROGS([MY_CC], [mpicc], [none],)
      MPI_CC=${MY_CC}
    fi
    if test "X${MPI_F77}" = "Xnone" ; then 
      AC_CHECK_PROGS([MY_F77], [mpif77], [none],)
      MPI_F77=${MY_F77}
    fi
  else
    if test "X${MPI_CXX}" = "Xnone" ; then
      AC_MSG_CHECKING(MPI C++ compiler)
      if test -f ${withval}/mpic++ ; then
        MPI_CXX=${withval}/mpic++
      fi
      if test "X${MPI_CXX}" = "Xnone" && test -f ${withval}/mpicxx ; then
        MPI_CXX=${withval}/mpicxx
      fi
      if test "X${MPI_CXX}" = "Xnone" && test -f ${withval}/mpiCC ; then
        MPI_CXX=${withval}/mpiCC
      fi
      AC_MSG_RESULT([${MPI_CXX}])
    fi

    if test "X${MPI_CC}" = "Xnone" ; then
      AC_MSG_CHECKING(MPI C compiler)
      if test -f ${withval}/mpicc ; then
        MPI_CC=${withval}/mpicc
      else
        MPI_CC=none
      fi
      AC_MSG_RESULT([${MPI_CC}])
    fi

    if test "X${MPI_F77}" = "Xnone" ; then
      AC_MSG_CHECKING(MPI Fortran compiler)
      if test -f ${withval}/mpif77 ; then
        MPI_F77=${withval}/mpif77
      else
        MPI_F77=none
      fi
      AC_MSG_RESULT([${MPI_F77}])
    fi
  fi

  if test "X${MPI_CXX}" = "Xnone" ; then
    AC_MSG_ERROR([MPI C++ compiler script/program not found.])
  fi

  if test "X${MPI_CC}" = "Xnone" ; then
    AC_MSG_ERROR([MPI C compiler script/program (mpicc) not found.])
  fi

  if test "X${MPI_F77}" = "Xnone" ; then
    AC_MSG_ERROR([MPI Fortran compiler script/program (mpif77) not found.])
  fi
]
)

if test "X${MPI_CXX}" = "Xnone" ; then
  if test "X${MPI_CC}" = "Xnone" ; then : ; else
    AC_MSG_ERROR([No C++ compiler: try --with-mpi-compilers= or --with-mpi-cxx=])
  fi
else
  if test "X${MPI_CC}" = "Xnone" ; then
    AC_MSG_ERROR([No C compiler: try --with-mpi-compilers= or --with-mpi-cc=])
  fi
fi


AC_ARG_WITH(mpi,
AS_HELP_STRING([--with-mpi=MPIROOT],[use MPI root directory. Automatically enables MPI.]),
[
  # This if statement is necessary (for now) to make this compatible with DAKOTA
  if test X${withval} = X; then :; else
     ENABLE_MPI=yes
     MPI_DIR=${withval}
     AC_MSG_CHECKING(MPI directory)
     AC_MSG_RESULT([${MPI_DIR}])
  fi
]
)

AC_ARG_WITH(mpi-libs,
AS_HELP_STRING([--with-mpi-libs="LIBS"],[MPI libraries @<:@default "-lmpi"@:>@]),
[
  ENABLE_MPI=yes
  MPI_LIBS=${withval}
  AC_MSG_CHECKING(user-defined MPI libraries)
  AC_MSG_RESULT([${MPI_LIBS}])
]
)

AC_ARG_WITH(mpe-libs,
AS_HELP_STRING([--with-mpe-libs="LIBS"],[MPE libraries @<:@default "-lampe -llmpe -ltmpe -lmpe"@:>@]),
[
  ENABLE_MPE=yes
  MPE_LIBS=${withval}
  AC_MSG_CHECKING(user-defined MPE libraries)
  AC_MSG_RESULT([${MPE_LIBS}])
],
[ ENABLE_MPE=no]
)

AC_ARG_WITH(mpi-incdir,
AS_HELP_STRING([--with-mpi-incdir=DIR],[MPI include directory @<:@default MPIROOT/include@:>@]),
[
  ENABLE_MPI=yes
  MPI_INC=${withval}
  AC_MSG_CHECKING(user-defined MPI includes)
  AC_MSG_RESULT([${MPI_INC}])
]
)

AC_ARG_WITH(mpi-libdir,
AS_HELP_STRING([--with-mpi-libdir=DIR],[MPI library directory @<:@default MPIROOT/lib@:>@]),
[
  ENABLE_MPI=yes
  MPI_LIBDIR=${withval}
  AC_MSG_CHECKING(user-defined MPI library directory)
  AC_MSG_RESULT([${MPI_LIBDIR}])
]
)

AC_ARG_WITH(mpi-cppflags,
AS_HELP_STRING([--with-mpi-cppflags=flags],[Extra preprocessor flags for MPI]),
[
  ENABLE_MPI=yes
  MPI_CPPFLAGS=${withval}
  AC_MSG_CHECKING(user-defined MPI preprocessor flags)
  AC_MSG_RESULT([${MPI_CPPFLAGS}])
]
)

AC_ARG_WITH(mpi-ldflags,
AS_HELP_STRING([--with-mpi-ldflags=flags],[Extra link flags for MPI]),
[
  ENABLE_MPI=yes
  MPI_LDFLAGS=${withval}
  AC_MSG_CHECKING(user-defined MPI link flags)
  AC_MSG_RESULT([${MPI_LDFLAGS}])
]
)

AC_MSG_CHECKING(whether we are using MPI)
AC_MSG_RESULT([${ENABLE_MPI}])

])
