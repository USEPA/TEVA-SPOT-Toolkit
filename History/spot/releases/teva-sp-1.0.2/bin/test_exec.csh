
setenv TIME 36000

rm -f core >& /dev/null

##
## TODO - launch parallel jobs differently...
##

$*
