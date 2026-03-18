dnl ACRO platform flags

AC_DEFUN([ACRO_PLATFORM_FLAGS],[
  dnl
  dnl Determine special platform preprocessor and compiler flags
  dnl TODO: Create tests for NO_MEMBER_TEMPLATE_FRIENDS

case $host_os in

  # IBM test
  rs6000-ibm-aix3.2.* | rs6000-ibm-aix4.* | powerpc-ibm-aix4.*)

    case $CXX in
      xlC* ) SPECIAL_CXXFLAGS="-qnolm -qrtti";;
    esac
  ;;

  # SGI/IRIX test
  mips-sgi-irix* )

    if test "${CXX}" = "CC"; then
     SPECIAL_CXXFLAGS="-LANG:std -LANG:ansi-for-init-scope=ON -64 -DSGI64"
     F77_MISC=f77
     FFLAGS_MISC="-64"
     SPECIAL_AR="CC -ar -W,l -W,s -o"
    fi

  ;;

  solaris* )

    if test "${CXX}" = "CC"; then
       SPECIAL_AR="CC -xar -o";
       SPECIAL_CXXFLAGS="-DSOLARIS_CC -DACRO_NO_MEMBER_TEMPLATE_FRIENDS -DUTILIB_NO_MEMBER_TEMPLATE_FRIENDS"
       SPECIAL_OPT=`echo "${WITH_OPT}" | sed 's/-O\([[0-9]]*\)/-xO\1/g'`;
    fi

  ;;

  cygwin*)

    if test X${WITH_MINGW} = Xyes ; then
       SPECIAL_CFLAGS="-mno-cygwin"
       SPECIAL_CPPFLAGS="-mno-cygwin"
       SPECIAL_CXXFLAGS="-mno-cygwin"
       SPECIAL_LDFLAGS="-mno-cygwin"
    fi

  ;;

esac

])
