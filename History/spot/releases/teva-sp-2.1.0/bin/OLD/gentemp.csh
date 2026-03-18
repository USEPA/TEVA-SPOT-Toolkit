##
## gentemp is a utility for taking a template header and source
## file and generating files that can be used to define "static" instantiations
## of the templates.  gentemp takes a list of arguments and replaces the
## different classes and types with generic descriptors, which can later
## be used with '#defines' to define static templates.
##
## gentemp [-dir directory] <file_c> <file_h> <class1> ... <classn> -types <type1> ...  <typen>
##

if ("$1" == "-dir") then
   shift
   set directory = $1
   shift
else
   set directory = "."
endif

set filec = $1
set outc = ${directory}/${1}t
shift

set fileh = $1
set outh = ${directory}/${1}t
shift


cp $fileh /tmp/mkgen.h.$$
if ("$filec" != "-") then
   cp $filec /tmp/mkgen.c.$$
endif

##
## Remove 'template' declarations
##
sed < /tmp/mkgen.h.$$ >! /tmp/mkgen.$$ -e "\
s/^template <class.*>//\
"
mv -f /tmp/mkgen.$$ /tmp/mkgen.h.$$

if ("$filec" != "-") then
sed < /tmp/mkgen.c.$$ >! /tmp/mkgen.$$ -e "\
s/^template <class.*>//\
"
mv -f /tmp/mkgen.$$ /tmp/mkgen.c.$$
endif

##
## Add HEADERS flag
##
## This assumes that the user wraps the header's contents with a 
## #ifndef,#define,#endif combination.
##
awk 'BEGIN {flag=0} {\
        if ((flag == 0) && \\
            (($1 == "#include") || ($1 == "#define") )) {\
	   print $0;\
	   printf "HEADERS\nTYPEDEFS";\
	   print "";\
	   flag=1;\
	   }\
	else print $0;\
	}' /tmp/mkgen.h.$$ >! /tmp/mkgen.$$
mv /tmp/mkgen.$$ /tmp/mkgen.h.$$


##
## Iterate over class names
##
@ num = 1
while (("$1" != "") && ("$1" != "-types"))
  set name = $1
  shift

sed < /tmp/mkgen.h.$$ >! /tmp/mkgen.$$ -e "\
s/$name/CLASSNAME${num}/g\
s/"'"'"\\CLASSNAME${num}.h"'"'"/"'"'"${name}.h"'"'"/g\
s/"'"'"\(..*\)CLASSNAME${num}.h"'"'"/"'"'"\1${name}.h"'"'"/g\
s/"'"'"CLASSNAME${num}.h"'"'"/CLASSNAMEINC${num}/g\
s/CLASSNAME${num}\(.*\)CLASSNAME${num}\([^#]*\)#/${name}\1${name}\2#/g\
s/CLASSNAME${num}\([^#]*\)#/${name}\1#/g\
s/CLASSNAME${num}<[^>]*>/CLASSNAME$num/g\
s/CLASSNAME${num}_/${name}_/g\
s/_${name}_/_CLASSNAME${num}_/g\
s/define CLASSNAME${num}/define ${name}/g\
s/defined(CLASSNAME${num}T)/defined(${name}T)/g\
"
  mv -f /tmp/mkgen.$$ /tmp/mkgen.h.$$

if ("$filec" != "-") then
sed < /tmp/mkgen.c.$$ >! /tmp/mkgen.$$ -e "\
s/$name/CLASSNAME$num/g\
s/CLASSNAME${num}<[^>]*>/CLASSNAME$num/g\
s/"'"'"\\CLASSNAME${num}.h"'"'"/"'"'"${name}.h"'"'"/g\
s/CLASSNAME${num}_/${name}_/g\
s/_${name}_/_CLASSNAME${num}_/g\
s/"'"'"\(..*\)CLASSNAME${num}.h"'"'"/"'"'"\1${name}.h"'"'"/g\
s/"'"'"CLASSNAME${num}.h"'"'"/CLASSNAMEINC${num}/g\
"
  mv -f /tmp/mkgen.$$ /tmp/mkgen.c.$$
endif
  @ num = $num + 1
end

if ("$1" == "-types") then
   shift
endif

@ num = 1
while ("$1" != "")
  set T1 = $1
  shift

sed < /tmp/mkgen.h.$$ >! /tmp/mkgen.$$ -e "\
s/^$T1 / TYPE$num /g\
s/^$T1\& / TYPE$num \& /g\
s/^$T1\* / TYPE${num}ptr /g\
s/^$T1\*\* / TYPE${num}ptrptr /g\
s/ $T1 / TYPE$num /g\
s/ $T1\& / TYPE$num \& /g\
s/ $T1\* / TYPE${num}ptr /g\
s/ $T1\*\* / TYPE${num}ptrptr /g\
s/($T1\*)/( TYPE${num}ptr )/g\
s/($T1\*\*)/( TYPE${num}ptrptr )/g\
"
  mv -f /tmp/mkgen.$$ /tmp/mkgen.h.$$

if ("$filec" != "-") then
sed < /tmp/mkgen.c.$$ >! /tmp/mkgen.$$ -e "\
s/^$T1 / TYPE$num /g\
s/^$T1\& / TYPE$num \& /g\
s/^$T1\* / TYPE${num}ptr /g\
s/^$T1\*\* / TYPE${num}ptrptr /g\
s/ $T1 / TYPE$num /g\
s/ $T1\& / TYPE$num \& /g\
s/ $T1\* / TYPE${num}ptr /g\
s/ $T1\*\* / TYPE${num}ptrptr /g\
s/($T1\*)/( TYPE${num}ptr )/g\
s/($T1\*\*)/( TYPE${num}ptrptr )/g\
"
  mv -f /tmp/mkgen.$$ /tmp/mkgen.c.$$
endif

  @ num = $num + 1
end

mv -f /tmp/mkgen.h.$$ $outh
if ("$filec" != "-") then
mv -f /tmp/mkgen.c.$$ $outc
endif
