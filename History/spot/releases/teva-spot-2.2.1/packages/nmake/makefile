
##
## TODO - 
## how to wildcard the target to anything
## makefile-hia depends on makefile-sim being successful
##
## The object files need be deleted before building, because
## objects built for shared and static may be different
##
## Shared build is not all working yet - TODO

SHARED_INFO="JDK_ROOT=C:\Java\jdk1.6.0 USE_SHARED_FLAGS=yes"

all : sim hia cat spot

sim :
    $(MAKE) /F makefile-sim /$(MAKEFLAGS) clean-objs
    $(MAKE) /F makefile-sim /$(MAKEFLAGS) all

hia :
    $(MAKE) /F makefile-hia /$(MAKEFLAGS) clean-objs
    $(MAKE) /F makefile-hia /$(MAKEFLAGS) all

cat :
    $(MAKE) /F makefile-cat /$(MAKEFLAGS) clean-objs
    $(MAKE) /F makefile-cat /$(MAKEFLAGS) all

spot :
    $(MAKE) /F makefile-spot /$(MAKEFLAGS) clean-objs
    $(MAKE) /F makefile-spot /$(MAKEFLAGS) all

dll : dlls

dlls :
    $(MAKE) /F makefile-sim /$(MAKEFLAGS) $(SHARED_INFO) clean-objs
    $(MAKE) /F makefile-sim /$(MAKEFLAGS) $(SHARED_INFO) dlls
    $(MAKE) /F makefile-hia /$(MAKEFLAGS) $(SHARED_INFO) clean-objs
    $(MAKE) /F makefile-hia /$(MAKEFLAGS) $(SHARED_INFO) dlls
    $(MAKE) /F makefile-cat /$(MAKEFLAGS) $(SHARED_INFO) clean-objs
    $(MAKE) /F makefile-cat /$(MAKEFLAGS) $(SHARED_INFO) dlls
    $(MAKE) /F makefile-spot /$(MAKEFLAGS) $(SHARED_INFO) clean-objs
    $(MAKE) /F makefile-spot /$(MAKEFLAGS) $(SHARED_INFO) dlls

clean:
    $(MAKE) /F makefile-sim /$(MAKEFLAGS) $@
    $(MAKE) /F makefile-hia /$(MAKEFLAGS) $@
    $(MAKE) /F makefile-cat /$(MAKEFLAGS) $@
    $(MAKE) /F makefile-spot /$(MAKEFLAGS) $@
