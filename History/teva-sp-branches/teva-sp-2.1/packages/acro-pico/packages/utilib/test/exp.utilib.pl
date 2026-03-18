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
   }
#
# See if the TESTLIBDIR environmental variable is set, which provides
# an include path for the TestLib module
#
$pwd = `pwd`;
chomp($pwd);
if ($ENV{TESTLIBDIR} ne "") {
   $tmp = $ENV{TESTLIBDIR};
   @words = split /[ ]+/, $tmp, $#tmp;
   foreach $word (@words) {
     unshift @INC, $word;
     }
   }
require TestLib;

#
# A callback that is used to print measurement information
#
$description = "ERROR";
sub printMeasurements {
    print $description;
}

sub analyzeData {
}

#
# Setup and run a regression test
#
if (! @ARGV) {
   print "\n";
   print "exp.utilib.pl - UTILIB experiment driver (for regression tests)\n";
   print "\n";
   print "usage:\n";
   print "\n";
   print "   exp.utilib.pl <expname> <testname> <factor-id> <factor-value>\n";
   print "\n";
   print "notes:\n";
   print "   This script is launched by 'runexp' to execute and process a\n";
   print "   single test from an experiment file.  The output of this test\n";
   print "   is stored in the file <expname>-<testname>.out, which can be\n";
   print "   reviewed after the test is performed.  This script generates an\n";
   print "   XML summary that is included in the <expname>.results.xml file\n";
   print "   by 'runexp'.\n";
   print "\n";
   print "   This script is not intended for interactive use.  Consequently,\n";
   print "   the processing of the command-line options is somewhat fragile.\n";
   print "   For example, this script does not support command-line options\n";
   print "   and it assumes that only a single factor is used that is the\n";
   print "   regression test name.\n";
   print "\n";
   exit;
}
$debug = 0;
if ($ARGV[$ctr] eq "--debug") {
   $ctr += 1;
   $debug = 1;
}
$description = TestLib::generate_description($ctr,@ARGV);
$cmdline = "./" . $ARGV[$ctr+5];
if ($debug == 1) {
   print "COMMAND LINE: $cmdline\n";
}
$auxoptions=$ARGV[$ctr+5];
%testparams = ();
#print "A $description\n";
#print "B $ARGV[$ctr]\n";
#print "C $ARGV[$ctr+1]\n";
#print "D $auxoptions\n";
TestLib::run_experiments(\*STDOUT, $ARGV[$ctr], $ARGV[$ctr+1], $cmdline, 1, \&analyzeData, \&printMeasurements, $auxoptions, \%testparams);
