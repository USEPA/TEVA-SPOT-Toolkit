##
## Tool used to instantiate a template class
##
## instemp -api <api> -basename <basename> <inst-file> <file_c> <file_h> 
##      <defndirectory> <type1> ... <typen>
##	-classes <class1> <name1> ... <classn> <namen>
##	-declare <decl1> <decl2> ... <decln>
##	-headers <header1> <header2> ... <headern>
##

##
## Figure out api
##
if ("$1" == "-api") then
   shift
   set api = "$1"
   shift
else 
   set api = ""
endif

##
## Figure out base name
##
if ("$1" == "-basename") then
   shift
   set basename = "$1"
   shift
else 
   set basename = ""
endif

set insfile = $1
shift

set inc = $1
shift

set inh = $1
shift

set defndir = $1
shift

set type = ("" "" "" "" "" "" "" "" "" "" "" "")
set typename = ("" "" "" "" "" "" "" "" "" "" "" "")
set typedef = ("" "" "" "" "" "" "" "" "" "" "" "")
set fbase = ${inh:t}
set fbase = ${fbase:r}

##
## Iterate over type names
##
@ ctr = 1
while (("$1" != "") && ("$1" != "-declare") && ("$1" != "-headers") && ("$1" != "-classes"))
  set type[$ctr] = "$1"
  shift

  set typename[$ctr] = `echo "$type[$ctr]" | sed 's/\*/ptr/g'`
  if ("$type[$ctr]" != "$typename[$ctr]") then
     set typedef[$ctr] = "typedef $type[$ctr] $typename[$ctr];"
  endif
  @ ctr = $ctr + 1
end

##
## Determine output files
##
@ i = 2
set pref = `echo "$type[1]" | sed 's/\*/ptr/g'`
while ($i < $ctr)
  set hdr = `echo "$type[$i]" | sed 's/\*/ptr/g'`
  set pref = "${pref}_$hdr"
  @ i = $i + 1
end

if (("$1" != "-headers") && ("$1" != "-classes")) then
   echo "Null headers and classes"
   exit
endif

##
## Create Output Files
##
set directory=${insfile:h}
if ("$directory" == "$insfile") then
   set directory="."
endif

set ttmp = `echo ${inh:t} | sed 's/\([^\.]*\)\.\(.*\)/\1_'${pref}'.\2/'`
set ofileh = ${directory}/${ttmp}
#
# Exit Now If This File Is Up-To-Date
#
if (!(-e $ofileh)) then
   echo "Creating "$ofileh
else 
   set tmpfile = `\ls -t ${defndir}/${inh}t $insfile ${defndir}/template.defns | head -1`
   #\ls -lt ${defndir}/${inh}t $insfile ${defndir}/template.defns
   if ( "$tmpfile" != "${defndir}/template.defns" ) then
      echo "Creating "$ofileh
   else
      exit
   endif
endif

\cp ${defndir}/${inh}t $ofileh
if ("${inc}" != "-") then
   if ("$basename" == "") then
      set ttmp = `echo ${inc:t} | sed 's/\([^\.]*\)\.\(.*\)/\1_'${pref}'.\2/'`
      set ofilec = ${directory}/${ttmp}
   else
      set ofilec = ${directory}/${basename}.cpp
   endif
   \cp ${defndir}/${inc}t $ofilec
endif
   
set tmpfileh = /tmp/instemp.$$.h
set tmpfilec = /tmp/instemp.$$.cpp
 
##
## Replace API
##
if ("$api" != "") then
   sed < $ofileh >! $tmpfileh -e "s/DLL/$api/g"
else
   sed < $ofileh >! $tmpfileh -e "s/DLL_API//g"
endif
mv -f $tmpfileh $ofileh
if ("$inc" != "-") then
   if ("$api" != "") then
      sed < $ofilec >! $tmpfilec -e "s/DLL/$api/g"
   else
      sed < $ofilec >! $tmpfilec -e "s/DLL_API//g"
   endif
   mv -f $tmpfilec $ofilec
endif


##
## Replace Type Names
##
@ num = 1
while ($num < $ctr)
  sed < $ofileh >! $tmpfileh -e "s/TYPE$num/$typename[$num]/g"
  mv -f $tmpfileh $ofileh
  sed < $ofileh -e "\
/TYPEDEFS/a\\
TYPEDEFS\\
\#ifndef $typename[$num]_type\\
\#define $typename[$num]_type\\
$typedef[$num]\\
typedef $typename[$num] * $typename[$num]ptr;\\
typedef $typename[$num] ** $typename[$num]ptrptr;\\
\#endif" >! $tmpfileh
  mv -f $tmpfileh $ofileh
  if ("$inc" != "-") then
     sed < $ofilec >! $tmpfilec -e "s/TYPE$num/$typename[$num]/g"
     mv -f $tmpfilec $ofilec
  endif
  @ num = $num + 1
end
sed < $ofileh >! $tmpfileh -e "s/TYPEDEFS//g"
mv -f $tmpfileh $ofileh

##
## Replace Class Names
##
@ num = 1
if ("$1" == "-classes") then
   shift
   while (("$1" != "") && ("$1" != "-declare") && ("$1" != "-headers"))
     set class = $1
     shift
     sed < $ofileh >! $tmpfileh -e "\
s/CLASSNAME$num/$class/g\
s/CLASSNAMEINC$num/"'"'"${class}.h"'"'"/g"
     mv -f $tmpfileh $ofileh
     if ("$inc" != "-") then
        sed < $ofilec >! $tmpfilec -e "\
s/CLASSNAME$num/$class/g\
s/CLASSNAMEINC$num/"'"'"${class}.h"'"'"/g"
        mv -f $tmpfilec $ofilec
     endif
     @ num = $num + 1
   end
endif

##
## Insert Include Files Into The Template File
##
cat >! $tmpfileh << END-CAT
//
// WARNING: This is a hard-coded template instantiation.
// Any changes made to this file are not propagated to the
// initial template file, and will be lost!
//
END-CAT
cp $tmpfileh $tmpfilec
if ("$inc" != "-") then
   cat $ofilec >> $tmpfilec
   mv -f $tmpfilec $ofilec
endif
cat $ofileh >> $tmpfileh


##
## Add pointer declarations
##
if ("$1" == "-declare") then
   shift
   while (("$1" != "") && ("$1" != "-headers"))
     sed < $tmpfileh -e "\
/HEADERS/a\\
HEADERS\\
class "$1";" >! $ofileh
     mv -f $ofileh $tmpfileh
     shift
   end
endif

##
## Add typedefs to tmpfileh
##
if ("$1" == "-headers") then
   shift
   while ("$1" != "")
     sed < $tmpfileh -e "\
/HEADERS/a\\
HEADERS\\
#include "'"'$1'"'"" >! $ofileh
     mv -f $ofileh $tmpfileh
     shift
   end
endif
sed < $tmpfileh -e "s/HEADERS//g" >! $ofileh

\rm -f $tmpfileh $tmpfilec
