##
## Tool used to 'fill' a file with strings that replace other strings
##
## fillfile file_c file_h defs
##

set inc = $1
shift

set inh = $1
shift

set defs = $1
shift

set class = ${defs:r}
set ofileh = `echo ${inh:t} | sed 's/\(.*\)t/\1/'`
set ofileh = ${class}/`echo $ofileh | sed 's/Class/'"$class"'/g'`
if ("$inc" != "-") then
   set ofilec = `echo ${inc:t} | sed 's/\(.*\)t/\1/'`
   set ofilec = ${class}/`echo $ofilec | sed 's/Class/'"$class"'/g'`
else
   set ofilec = ""
endif

##
## This assumes that you're always creating a header file...
##
if (!(-e $ofileh)) then
   echo "Creating "$ofileh
else
   if ("${inc}" == "-") then
      set tmpfile = `\ls -t ${inh} ${ofileh} | head -1`
   else
      set tmpfile = `\ls -t ${inh} ${ofileh} ${ofilec} ${inc} | head -1`
   endif
   if (("$tmpfile" == "${inh}") || ("$tmpfile" == "${inc}")) then
      echo "Creating "$ofileh
   else
      exit
   endif
endif

##
## Setup TMP files
##
set tmpfileh = /tmp/instemp.$$.h
set tmpfilec = /tmp/instemp.$$.cpp
cat $inh >! $ofileh
if ("$inc" != "-") then
   cat $inc >! $ofilec
endif


##
## Replace Names
##
sed < $ofileh >! $tmpfileh -f $defs
mv -f $tmpfileh $ofileh
if ("$inc" != "-") then
   sed < $ofilec >! $tmpfilec -f $defs
   mv -f $tmpfilec $ofilec
endif

cat >! $tmpfileh << END-CAT
//
// WARNING: This file was derived from $inh.
// Any changes made to this file are not propagated, and will be lost!
//
END-CAT
cat $ofileh >> $tmpfileh
cat $tmpfileh | awk 'BEGIN {flag=0; type="";}\
{\
if (($1 == "#start") && (flag == 0) && ($2 != "'"$class"'")) {\
      flag = 1;\
      type=$2;\
      }\
if ((flag == 0) && ($1 != "#start") && ($1 != "#finish"))\
   print $0;\
if (($1 == "#finish") && (flag == 1) && ($2 == type))\
   flag = 0;\
}' >! $ofileh

cat >! $tmpfilec << END-CAT
//
// WARNING: This file was derived from $inc.
// Any changes made to this file are not propagated, and will be lost!
//
END-CAT
if ("$inc" != "-") then
   cat $ofilec >> $tmpfilec
cat $tmpfilec | awk 'BEGIN {flag=0; type="";}\
{\
if (($1 == "#start") && (flag == 0) && ($2 != "'"$class"'")) {\
      flag = 1;\
      type=$2;\
      }\
if ((flag == 0) && ($1 != "#start") && ($1 != "#finish"))\
   print $0;\
if (($1 == "#finish") && (flag == 1) && ($2 == type))\
   flag = 0;\
}' >! $ofilec
endif

\rm -f $tmpfileh $tmpfilec
