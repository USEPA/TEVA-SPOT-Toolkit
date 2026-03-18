##
## doinstemp initializes the creation of a single template.
##
## instemp <ins-file> <defn-file> <template-directory> <template-name> 
##		<type1> [<type2> ...] -api <API>
##		-basename <name>
##		-declare <type1> [<type2> ...]
##		-headers <header1> [<header2> ...]
##

#echo "PARAMS $*"

#
# Process Inputs
#
set file="$0"
set filedir=${file:h}

set cmdline = "$argv[4-$#argv]"
set instfile = $1
shift
set defnfile = $1
shift
set directory = $1
shift
set tempname = $1
shift

set type = ("" "" "" "" "" "" "" "" "" "" "" "")
set typename = ("" "" "" "" "" "" "" "" "" "" "" "")
##
## Iterate over type names
##
@ ctr=1
while (("$1" != "") && ("$1" != "-api") && ("$1" != "-basename") && ("$1" != "-declare") && ("$1" != "-headers"))
  set type[$ctr] = "$1"
  set typename[$ctr] = `echo "$type[$ctr]" | sed 's/\*/ptr/g'`
  shift
  @ ctr = $ctr + 1
end

##
## Figure out API
##
if (("$1" != "") && ("$1" == "-api")) then
   shift
   set api = $1
   shift
else
   set api = ""
endif


##
## Figure out base name
##
if (("$1" != "") && ("$1" == "-basename")) then
   shift
   set basename = $1
   shift
else
   set basename = ""
endif


set decls = ("" "" "" "" "" "" "" "" "" "" "" "")
##
## Iterate over declaration names
##
if ("$1" == "-declare") then
  shift
endif
@ ctr = 1
while (("$1" != "") && ("$1" != "-headers"))
  set decls[$ctr] = "$1"
  shift
  @ ctr = $ctr + 1
end
endif
 

set headers = ("" "" "" "" "" "" "" "" "" "" "" "")
##
## Iterate over headers names
##
if ("$1" == "-headers") then
  shift
endif
@ ctr = 1
while ("$1" != "")
  set headers[$ctr] = "$1"
  shift
  @ ctr = $ctr + 1
end

set defndir=${defnfile:h}
if ("$defndir" == "$defnfile") then
   set defndir="."
endif
 

##
## Process the definition file to create the template instance
##
@ template_flag = $?TEMPLATE_DEFN_FILES
if ($template_flag == 0) then
   set TEMPLATE_DEFN_FILES = ""
endif
set awkexec = `which nawk | grep nawk`
if (("$awkexec[1]" == "no") || ("$awkexec[1]" == "nawk:") || ("$awkexec[1]" == "which:")) then
   set awkexec = `which awk | grep awk`
endif
cat >! /tmp/doinstemp.awk.$$ << END-AWK
BEGIN {
status=1000;
}
{
if (status == 1000) {
   split(truetypes,truetype);
   nvars = split(types,type);
   nheaders = split(headers,header);
   ndecls = split(decls,decl);
   status = 0;
   }
if (\$1 != "##") {
if (status==0) {
    if ("$TEMPLATE_DEFN_FILES" != "") {
      printf "%s %s %s $directory %s ", file, instfile, "$TEMPLATE_DEFN_FILES", tempname;
      for (i=1; i<=nvars; i++)
        printf "'%s' ", truetype[i];
      if (api != "") {
         printf "-api %s ", api;
         }
      if (basename != "") {
         printf "-basename %s ", basename;
         }
      if (ndecls>0) printf "-declare ";
      for (i=1; i<=ndecls; i++)
        printf "%s ", decl[i];
      if (nheaders > 0) printf "-headers ";
      for (i=1; i<=nheaders; i++)
        printf "%s ", header[i];
      printf "\n",tmp;
      }
   if (\$1 == "FILE") {
      printf "%s %s %s $directory %s ", file, instfile, \$2, tempname;
      for (i=1; i<=nvars; i++)
        printf "'%s' ", truetype[i];
      if (api != "") {
         printf "-api %s ", api;
         }
      if (basename != "") {
         printf "-basename %s ", basename;
         }
      if (ndecls>0) printf "-declare ";
      for (i=1; i<=ndecls; i++)
        printf "%s ", decl[i];
      if (nheaders > 0) printf "-headers ";
      for (i=1; i<=nheaders; i++)
        printf "%s ", header[i];
      printf "\n",tmp;
      status=100;
      }
   else
      status=1;
   }
else {
   if (status == 1) {
      if (tempname == \$1)
         { status=3; }
      else 
         { status=100; }
      }

   else if (status == 3)
      {
      printf "%s/instemp ", filedir;
      if (api != "")
         printf "-api %s ", api;
      if (basename != "")
         printf "-basename %s ", basename;
      printf "%s %s %s ",instfile, \$0, defndir;
      for (i=1; i<=nvars; i++)
        printf """'""%s""'"" ", truetype[i];
      status=4;
      }

   else if (status == 4)
      {
      numclasses = split(\$0,classes);
      status=5;
      }

   else if (status == 5)
      { status=6; }

   else if (status == 6) {
      printf "-classes "
      for (i=1; i<=numclasses; i++) {
        ntypes = split(\$i,instype,"_");
        tmp = sprintf("%s_%%s%%s",classes[i]);
        for (j=1; j<ntypes; j++) {
          tmp = sprintf(tmp,type[instype[j]],"_%s%s");
          }
        tmp = sprintf(tmp,type[instype[ntypes]],"");
        printf "%s ",tmp;
        }
      if (ndecls>0) printf "-declare ";
      for (i=1; i<=ndecls; i++)
        printf "%s ", decl[i];
      if (nheaders > 0) printf "-headers ";
      for (i=1; i<=nheaders; i++)
        printf "%s ", header[i];
      printf "\n",tmp;
      status=10;
      }

   else if (status == 10) {
      if (\$1 == "#")
         status=1;
      else {
         printf "%s $instfile $defnfile $directory %s ", file, \$1
         for (i=2; i<=NF; i++)
           printf "'%s' ", truetype[\$i];
         if (api != "")
            printf "-api %s ", api;
         if (ndecls>0) printf "-declare ";
         for (i=1; i<=ndecls; i++)
           printf "%s ", decl[i];
         if (nheaders > 0) printf "-headers ";
         for (i=1; i<=nheaders; i++)
           printf "%s ", header[i];
         printf "\n";
         }
      }
   else if (status == 100) {
      if (\$1 == "#")
         status=1;
      }
   }
}
}
END-AWK

$awkexec -f /tmp/doinstemp.awk.$$ -v defndir="$defndir" -v instfile="$instfile" -v file=$file -v filedir=$filedir -v truetypes="$type" -v types="$typename" -v api="$api" -v basename="$basename" -v headers="$headers" -v decls="$decls" -v cmdline="$cmdline" -v tempname=$tempname $defnfile >! /tmp/doinstemps.$$ 
\rm /tmp/doinstemp.awk.$$

setenv TEMPLATE_DEFN_FILES ""
chmod 700 /tmp/doinstemps.$$
#cat /tmp/doinstemps.$$
/tmp/doinstemps.$$
\rm /tmp/doinstemps.$$
