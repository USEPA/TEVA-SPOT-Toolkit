#!/usr/bin/perl

#
# See if the first argument is '-I' which specifies an include path
# for the TestLib module
#
$ctr=0;
if ($ARGV[0] eq "-I") {
   unshift @INC, $ARGV[1];
   $ENV{'PATH'} = $ENV{'PATH'} . ":.:" . $ARGV[1];
   $ctr = 2;
   print "HERE\n";
   }
#
# See if the TESTLIBDIR environmental variable is set, which provides
# an include path for the TestLib module
#
if ($ENV{TESTLIBDIR} ne "") {
   unshift @INC, $ENV{TESTLIBDIR};
   $ENV{'PATH'} = $ENV{'PATH'} . ":.:" . $ENV{TESTLIBDIR}
   }
require TestLib;
#
# Setup and run a regression test
#
$ctr+=1;
$benchmark = $ARGV[$ctr] . ".qa";
$cmdline = "./" . $ARGV[$ctr];
TestLib::run_regression_test(\*STDOUT, "utilib", $ARGV[$ctr], $cmdline, $benchmark, "");
