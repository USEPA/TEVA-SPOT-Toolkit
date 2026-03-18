##
## gentemps creates the template setup files using multiple calls to
## gentemp.
##
## gentemp [-temp <template-name>] [template-file-defns]
##

#
# Process Inputs
#
set file = "$0"
set filedir = ${file:h}
if ("$1" == "-temp") then
   shift
   set tempname=$1
   set tempname=${tempname:t}
   shift
else
   set tempname=""
endif
if ("$1" == "") then
   set defnfile = "template.defns"
else
   set defnfile = "$1"
endif

if (!(-e $defnfile)) then
   echo "Template definitions file "$defnfile" is missing."
   echo "Aborting template setup."
   exit -1
endif

set directory = ${defnfile:h}
if ("$directory" == "$defnfile") then
   set directory == ""
else
   set directory = "-dir "$directory
endif

#
# Create executable file
#
awk 'BEGIN {status=0;}\
{\
if ($1 != "##") {\
if (status==0) {\
   if ($1 == "FILE")\
      printf "";\
   else\
      status=1;\
   }\
else {\
   if (status == 1) {\
      if (("'"$tempname"'" == "") || ("'"$tempname"'" == $1))\
         { printf "%s/gentemp %s ", "'"$filedir"'", "'"$directory"'"; status=3; }\
      else \
         { status=6; }\
      }\
   else if (status == 3)\
      { printf "%s ", $0; status=4; }\
   else if (status == 4)\
      { printf "%s ", $0; status=5; }\
   else if (status == 5)\
      { printf "-types %s\n", $0; status=6; }\
   else if (status == 6) {\
      if ($1 == "#")\
         status=1;\
      }\
   }\
}\
}' $defnfile >! /tmp/gentemps.$$

chmod 700 /tmp/gentemps.$$
#cat /tmp/gentemps.$$
/tmp/gentemps.$$
\rm /tmp/gentemps.$$
