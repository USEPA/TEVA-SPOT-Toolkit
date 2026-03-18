##
## makehdr <name>
##
## Create a global header file.
##

set name = $1
shift
set fname = ${name}.h

\rm -f $fname
cat >! $fname << END_ECHO
/**
 * \file $fname
 *
 * A common header file for the $name library that is automatically
 * generated.
 */

#ifndef ${name}_${name}_h
#define ${name}_${name}_h

END_ECHO


foreach flag (`grep 'COLINY_WITHOUT' Makefile`)
  set tmp = `echo $flag | sed 's#\(.*\)-D\(.*\)#\2#g'`
  echo "#ifndef ${tmp}" >> ${fname}
  echo "#define ${tmp}" >> ${fname}
  echo "#endif" >> ${fname}
end
echo "" >> ${fname}

touch ${fname}.$$

foreach FILE (`find $* -name '*.[hH]' -print | grep -v 'Anne'`)
  #set tcount = `grep 'template <class' $FILE | wc` >& /dev/null
  set tmp = `echo ${FILE} | sed 's#\(.*\)\/\([_a-zA-Z0-9\.\-]*\)\.\([hH]\)#\2.\3#g'`
  echo "#include <${name}/${tmp}>" >> ${fname}.$$
end 

sort -d -f ${fname}.$$ >> ${fname}
\rm -f ${fname}.$$

echo "" >> $fname
echo "#endif" >> $fname

