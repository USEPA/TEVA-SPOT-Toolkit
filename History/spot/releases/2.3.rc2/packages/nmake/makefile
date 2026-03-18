
##
## TODO - 
## how to wildcard the target to anything
## makefile-hia depends on makefile-sim being successful
##

## to build everything (both static and shared), use the command
##    nmake JDK_ROOT="PATH_TO_YOUR_JDK_INSTALL" everything
##

!IFDEF DEBUGGING
DBG=DEBUGFLAG=/Z7
LDBG=LINKDBG=/DEBUG
!ELSE
DBG=DEBUGFLAG=
LDBG=LINKDBG=
!ENDIF
!IFDEF USE_SHARED_FLAGS
SHARED_INFO=USE_SHARED_FLAGS=$(USE_SHARED_FLAGS)
!ELSE
SHARED_INFO=USE_SHARED_FLAGS=yes
!ENDIF
!IFDEF JDK_ROOT
SHARED_INFO=JDK_ROOT="$(JDK_ROOT)" $(SHARED_INFO)
!ELSE
SHARED_INFO=JDK_ROOT=C:\Java\jdk1.6.0 $(SHARED_INFO)
!ENDIF

everything: all dlls

all : sim hia cat spot ufl

ufl:
    $(MAKE) /F makefile-ufl

sim :
    $(MAKE) /F makefile-sim /$(MAKEFLAGS) clean-objs
    $(MAKE) /F makefile-sim /$(MAKEFLAGS) $(DBG) $(LDBG) all

hia :
    $(MAKE) /F makefile-hia /$(MAKEFLAGS) clean-objs
    $(MAKE) /F makefile-hia /$(MAKEFLAGS) $(DBG) $(LDBG) all

cat :
    $(MAKE) /F makefile-cat /$(MAKEFLAGS) clean-objs
    $(MAKE) /F makefile-cat /$(MAKEFLAGS) $(DBG) $(LDBG) all

spot :
    $(MAKE) /F makefile-spot /$(MAKEFLAGS) clean-objs
    $(MAKE) /F makefile-spot /$(MAKEFLAGS) $(DBG) $(LDBG) all

dll : dlls

dlls :
    $(MAKE) /F makefile-sim /$(MAKEFLAGS) $(SHARED_INFO) clean-objs
    $(MAKE) /F makefile-sim /$(MAKEFLAGS) $(SHARED_INFO) dlls
    $(MAKE) /F makefile-hia /$(MAKEFLAGS) $(SHARED_INFO) clean-objs
    $(MAKE) /F makefile-hia /$(MAKEFLAGS) $(SHARED_INFO) dlls
    $(MAKE) /F makefile-cat /$(MAKEFLAGS) $(SHARED_INFO) clean-objs
    $(MAKE) /F makefile-cat /$(MAKEFLAGS) $(SHARED_INFO) dlls

clean:
    $(MAKE) /F makefile-sim /$(MAKEFLAGS) $@
    $(MAKE) /F makefile-hia /$(MAKEFLAGS) $@
    $(MAKE) /F makefile-cat /$(MAKEFLAGS) $@
    $(MAKE) /F makefile-spot /$(MAKEFLAGS) $@

zip:
    mkdir bin
    cd static
    xcopy *.exe ..\bin /e /q /i /y
    xcopy *.dll ..\bin /e /q /i /y
    xcopy *.lib ..\bin /e /q /i /y
    cd ..
    python zip.py tevaspot_bin.zip bin
    rmdir /s /q bin

