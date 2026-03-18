##
## A simple script that looks for a config.status file and 
## 'reruns' it if it exists.
##

(cd bin; ./mkscripts)
if ( -f config.status ) then
   if ( -f configure ) then
      #
      # In the root directory, we want to run the main configure 
      # command rather than bin/configure, since the main configure command
      # does some additional stuff after bin/configure is finished.
      #
      sed '\
s%--norecursion%%g\
s%/bin/configure%/configure%g' config.status >! /tmp/reconfigure.$$
   else
      #
      # In some generic directory
      #
      sed '1,$ s/--norecursion//g' config.status >! /tmp/reconfigure.$$
   endif
   chmod 775 /tmp/reconfigure.$$
   /tmp/reconfigure.$$
   \rm -f /tmp/reconfigure.$$
else
   echo "Error - no config.status exists"
endif
