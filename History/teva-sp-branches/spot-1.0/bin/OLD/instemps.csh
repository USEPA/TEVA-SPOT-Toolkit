##
## instemps creates the template instances using multiple calls to
## instemp.
##
## instemps [template-file-defns] [template-file-instances]
##
## BUG:  These scripts core dump when the '-f' flag is used on pueblo!?!
##

#
# Process Inputs
#
set file = $0
set filedir = ${file:h}

if ("$1" == "") then
   set defnfile = "template.defns"
else
   set defnfile = "$1"
   shift
endif

if ("$1" == "") then
   set instfile = "template.insts"
else
   set instfile = "$1"
endif

if (!(-e $defnfile)) then
   echo "Template definition file "$defnfile" is missing."
   echo "Aborting template instantiation."
   exit -1
endif

if (!(-e $instfile)) then
   echo "Template instances file "$instfile" is missing."
   echo "Aborting template instantiation."
   exit -1
endif

set directory = ${instfile:h}
if ("$directory" == "$instfile") then
   set directory = "."
else
   set directory = $directory
endif

#
# Create executable file
#
awk '{\
if ($1 != "##") {\
if ($1 == "FILE")\
   printf "%s %s\n", "'"$file"'", $2;\
else if ($1 != "")\
   printf "%s/doinstemp %s %s %s %s\n", "'"$filedir"'", "'"$instfile"'", "'"$defnfile"'", "'"$directory"'", $0;\
}\
}' $instfile >! /tmp/instemps.$$

chmod 700 /tmp/instemps.$$
#cat /tmp/instemps.$$
#cp /tmp/instemps.$$ doit
/tmp/instemps.$$
\rm /tmp/instemps.$$

touch $instfile
sleep 1
touch $defnfile

##
## Create Makefile.srcs
##
if (!(-e Makefile-unique)) then
   echo "$$" >! Makefile-unique
endif
set appendix = `cat Makefile-unique`

@ i=1
\rm -f tmp*.cpp 
echo -n "SRCS=" >! Makefile-srcs
foreach FILE (*.cpp)
  set fname = "tmp${i}-${appendix}.cpp"
  ln -s ${FILE} ${fname}
  echo "\" >> Makefile-srcs
  echo -n "  ${fname}" >> Makefile-srcs
  @ i = $i + 1
end
echo " " >> Makefile-srcs
