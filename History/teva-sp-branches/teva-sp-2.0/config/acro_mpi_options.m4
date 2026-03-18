dnl ACRO MPI options

AC_DEFUN([ACRO_MPI_OPTIONS],[
  dnl
  dnl Command-line options for setting up MPI
  dnl 
AC_ARG_ENABLE(mpi,
[AC_HELP_STRING([--enable-mpi],[Enable MPI support])],
[ENABLE_MPI=$enableval],
[ENABLE_MPI=no]
)

AC_ARG_WITH(mpi-compilers,
[AC_HELP_STRING([--with-mpi-compilers=DIR],
[use MPI compilers mpicc, mpif77, and mpicxx (or mpiCC) in the specified path or in the default path if no path is specified. Automatically enables MPI])],
[
  ENABLE_MPI=yes
  if test X${withval} = Xyes; then
    # Check for mpicxx, if it does not exist, use mpiCC instead.
    AC_CHECK_PROG(MPI_CXX, mpicxx, mpicxx, mpiCC)
    MPI_CC=mpicc
    MPI_F77=mpif77
  else
    MPI_TEMP_CXX=${withval}/mpicxx
    if test -f ${MPI_TEMP_CXX}; then
      MPI_CXX=${MPI_TEMP_CXX}
    else
      MPI_CXX=${withval}/mpiCC
    fi
    MPI_CC=${withval}/mpicc
    MPI_F77=${withval}/mpif77
  fi
]
)

AC_ARG_WITH(mpi,
[AC_HELP_STRING([--with-mpi=MPIROOT],[use MPI root directory. Automatically enables MPI.])],
[
  # This if statement is necessary (for now) to make this compatible with DAKOTA
  if test X${withval} != X; then
     ENABLE_MPI=yes
     MPI_DIR=${withval}
     AC_MSG_CHECKING(MPI directory)
     AC_MSG_RESULT([${MPI_DIR}])
  fi
]
)

AC_ARG_WITH(mpi-libs,
[AC_HELP_STRING([--with-mpi-libs="LIBS"],[MPI libraries @<:@default "-lmpi"@:>@])],
[
  ENABLE_MPI=yes
  MPI_LIBS=${withval}
  AC_MSG_CHECKING(user-defined MPI libraries)
  AC_MSG_RESULT([${MPI_LIBS}])
]
)

AC_ARG_WITH(mpe-libs,
[AC_HELP_STRING([--with-mpe-libs="LIBS"],[MPE libraries @<:@default "-lampe -llmpe -ltmpe -lmpe"@:>@])],
[
  ENABLE_MPE=yes
  MPE_LIBS=${withval}
  AC_MSG_CHECKING(user-defined MPE libraries)
  AC_MSG_RESULT([${MPE_LIBS}])
],
[ ENABLE_MPE=no]
)

AC_ARG_WITH(mpi-incdir,
[AC_HELP_STRING([--with-mpi-incdir=DIR],[MPI include directory @<:@default MPIROOT/include@:>@])],
[
  ENABLE_MPI=yes
  MPI_INC=${withval}
  AC_MSG_CHECKING(user-defined MPI includes)
  AC_MSG_RESULT([${MPI_INC}])
]
)

AC_ARG_WITH(mpi-libdir,
[AC_HELP_STRING([--with-mpi-libdir=DIR],[MPI library directory @<:@default MPIROOT/lib@:>@])],
[
  ENABLE_MPI=yes
  MPI_LIBDIR=${withval}
  AC_MSG_CHECKING(user-defined MPI library directory)
  AC_MSG_RESULT([${MPI_LIBDIR}])
]
)

AC_ARG_WITH(mpi-cppflags,
[AC_HELP_STRING([--with-mpi-cppflags=flags],[Extra preprocessor flags for MPI])],
[
  ENABLE_MPI=yes
  MPI_CPPFLAGS=${withval}
  AC_MSG_CHECKING(user-defined MPI preprocessor flags)
  AC_MSG_RESULT([${MPI_CPPFLAGS}])
]
)

AC_ARG_WITH(mpi-ldflags,
[AC_HELP_STRING([--with-mpi-ldflags=flags],[Extra link flags for MPI])],
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
