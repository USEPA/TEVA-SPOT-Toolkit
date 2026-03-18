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

AC_ARG_WITH(mpi-compilers,
AS_HELP_STRING([--with-mpi-compilers=DIR or --with-mpi-compilers=yes],
[use MPI compilers (mpicc, etc.) found in directory DIR, or in your PATH if =yes]),
[
  ENABLE_MPI=yes

  if test X${withval} = Xyes; then
    # Look for MPI C++ compile script/program in user's path, set
    # MPI_CXX to name of program found, else set it to "none".  Could
    # add more paths to check as 4th argument if you wanted to.
    # Calls AC_SUBST for MPI_CXX, etc.


    if test "X${MPI_CXX}" = "Xnone" ; then 
      echo "Looking for an MPI C++ compiler in your path"
      AC_CHECK_PROGS([MY_CXX], [mpic++ mpicxx mpiCC], [none],)
      MPI_CXX=${MY_CXX}
    fi
    if test "X${MPI_CC}" = "Xnone" ; then 
      echo "Looking for an MPI C compiler in your path"
      AC_CHECK_PROGS([MY_CC], [mpicc], [none],)
      MPI_CC=${MY_CC}
    fi
    if test "X${MPI_F77}" = "Xnone" ; then 
      echo "Looking for an optional MPI F77 compiler in your path"
      AC_CHECK_PROGS([MY_F77], [mpif77], [none],)
      MPI_F77=${MY_F77}
    fi
  else
    foundCompiler=no
    if test "X${MPI_CXX}" = "Xnone" ; then
      AC_MSG_CHECKING(MPI C++ compiler in ${withval})
      if test -f ${withval}/mpic++ ; then
        MPI_CXX=mpic++
        foundCompiler=yes
      fi
      if test "X${MPI_CXX}" = "Xnone" && test -f ${withval}/mpicxx ; then
        MPI_CXX=mpicxx
        foundCompiler=yes
      fi
      if test "X${MPI_CXX}" = "Xnone" && test -f ${withval}/mpiCC ; then
        MPI_CXX=mpiCC
        foundCompiler=yes
      fi
      AC_MSG_RESULT([${MPI_CXX}])
    fi

    if test "X${MPI_CC}" = "Xnone" ; then
      AC_MSG_CHECKING(MPI C compiler in ${withval})
      if test -f ${withval}/mpicc ; then
        MPI_CC=mpicc
        foundCompiler=yes
      fi
      AC_MSG_RESULT([${MPI_CC}])
    fi

    if test "X${MPI_F77}" = "Xnone" ; then
      AC_MSG_CHECKING(optional MPI Fortran compiler in ${withval})
      if test -f ${withval}/mpif77 ; then
        MPI_F77=mpif77
        foundCompiler=yes
      fi
      AC_MSG_RESULT([${MPI_F77}])
    fi

    if test $foundCompiler = yes ; then
      PATH=${withval}${PATH_SEPARATOR}${PATH}
    fi
  fi

  if test "X${MPI_CXX}" = "Xnone" ; then
    AC_MSG_ERROR([MPI C++ compiler script/program not found.])
  fi

  if test "X${MPI_CC}" = "Xnone" ; then
    AC_MSG_ERROR([MPI C compiler script/program (mpicc) not found.])
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
