#!/usr/bin/perl
#

use Cwd;
use Text::ParseWords;

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

@output_file=();
@log_file=();
@seed_list=();
$debug=0;

#
# Save the filename of measurements
#
sub analyzeData {
    push @output_file, $_[0];
    push @log_file, $_[1];
    push @seed_list, $_[2];
}

$description="  <Description>ERROR</Description>\n";

#
# A callback that is used to print measurement information
#
sub printMeasurements {
     print $description;
     $i = 0;
     while ($i <= $#output_file) {
       print "  <Trial id=\"$i\" seed=\"$seed_list[$i]\">\n";

       print "     <Value name=\"LogFile\" type=\"text/plain\">$log_file[$i].xml</Value>\n";
       open(INPUT,$output_file[$i]);
       @lines = <INPUT>;
       $status=0;
       foreach $line (@lines) {
         chomp($line);
         #print "$line $status\n";
         if ($status == 0) {
	    @words = &quotewords('\s+', 0, $line);
	    if ($#words != 2) {
               print "ERROR: bad measurement line: $#words $line $words[0],$words[1],$words[2]\n";
            } else {
              print "     <Value name=\"$words[0]\" type=\"$words[1]\">";
              if ($words[2] eq "<Value>") {
                 $status = 1;
              } else {
		 TestLib::scrub(\$words[2]);
	         print "$words[2]</Value>\n";
	      }
            }
         } elsif ($status == 1) {
            if ($line eq "</Value>") {
               $status = 0;
               print "</Value>\n";
            } else {
	       TestLib::scrub(\$line);
	       print "$line\n";
            }
         }
       }
     close(INPUT);
     print "  </Trial>\n";
     if ($debug == 0) {
        unlink("$output_file[$i]");
        unlink("$log_file[$i]");
     }
     $i = $i + 1;
     }
}


##
## MAIN ROUTINE
##
if (! @ARGV) {
   print "\n";
   print "runexp_expdriver.pl - Generic experiment driver\n";
   print "\n";
   print "usage:\n";
   print "\n";
   print "   runexp_expdriver.pl [options] <expname> <testname> <factor-id> <factor-value> [...]\n";
   print "\n";
   print "options:\n";
   print "\n";
   print "   --script\t\tThe script that will be run to perform the experiment.\n";
   print "\n";
   print "   --debug\t\tPrint debugging information.\n";
   print "\n";
   print "notes:\n";
   print "\n";
   print "   This script is launched by 'runexp' to execute and process a\n";
   print "   single test from an experiment file.  The output of this test\n";
   print "   is stored in the file <expname>-<testname>.out, which can be\n";
   print "   reviewed after the test is performed.  This script generates an\n";
   print "   XML summary that is included in the <expname>.results.xml file\n";
   print "   by 'runexp'.\n";
   print "\n";
   print "   This script is not intended for interactive use.  Consequently,\n";
   print "   the processing of the command-line options is somewhat fragile.\n";
   print "   For example, it depends on the order in which the options are\n";
   print "   processed in this script!\n";
   print "\n";
   print "   This script assumes that the first factor is the test information,\n";
   print "   and the remaining factors specify command-line options.\n";
   exit;
}
#
# Process command line arguments
#
$testoptions = "";
$script="";
if ($ARGV[$ctr] eq "--script") {
   $ctr += 1;
   $script = $ARGV[$ctr];
   $ctr += 1;
}
if ($ARGV[$ctr] eq "--debug") {
   $ctr += 1;
   $debug = 1;
}
$description = TestLib::generate_description($ctr,@ARGV);
$expname = $ARGV[$ctr];
$ctr +=1;
$testname = $ARGV[$ctr];
$ctr +=1;
$ntrials = $ARGV[$ctr];
$factor_start=$ctr+1;
$ctr+=3;
while ($ctr <= $#ARGV) {
  $testoptions .= $ARGV[$ctr] . " ";
  $ctr+=3;
}
if ($debug == 1) {
   print "options: $testoptions\n";
}
#
# Process experiment tag/value pairs
#
%testparams = ();
$auxoptions = TestLib::process_testoptions($testoptions, \%testparams);
if ($debug == 1) {
   print "exp:       $expname\n";
}
#
# Get solver directory
#
$dir = getcwd;
@dirwords = split(/\//,$dir);
#if ($dirwords[1] eq "cygdrive") { $solverdir = `cygpath -m $dir`; }
#else { $solverdir = $dir; }
$solverdir = $dir;
chomp $solverdir;
#
# Setup command line
#
$cmdline = "$solverdir/$script ${expname}_${testname}.in %s %s";
#
# Create input file
#
(open OUTPUT, ">${expname}_${testname}.in")
        || die "ERROR: cannot open file ${expname}_${testname}.in";
print OUTPUT  "_runexp_debug\t${debug}\n";
print OUTPUT  "_experiment_name\t${expname}\n";
print OUTPUT  "_test_name\t${testname}\n";
print OUTPUT  "_test_num_trials\t${ntrials}\n";
$ctr=$factor_start;
$ndx=1;
while ($ctr <= $#ARGV) {
  print OUTPUT  "_factor_" . $ndx . "_name\t$ARGV[$ctr]\n";
  print OUTPUT  "_factor_" . $ndx . "_level\t$ARGV[$ctr+1]\n";
  print OUTPUT  "_factor_" . $ndx . "_value\t\"$ARGV[$ctr+2]\"\n";
  @options = split(/\s+/,$ARGV[$ctr+2]);
  while (@options) {
    local $tmp = shift @options;
    if ($tmp ne "") {
       @pair=split(/=/,$tmp,2);
       if ($#pair == 1) {
          print OUTPUT $pair[0] . "\t" . $pair[1] . "\n";
          }
       }
    }
  $ndx+=1;
  $ctr+=3;
  }
print OUTPUT  "seed\t\$PSEUDORANDOM_SEED\n";
close(OUTPUT);
if ($debug == 1) {
   print "CMDLINE: $cmdline\n";
}
#
# Launch the test with the TestLib driver.
#
TestLib::run_experiments(\*STDOUT, $expname, $testname, $cmdline, $ntrials, \&analyzeData, \&printMeasurements, $auxoptions, \%testparams);
if ($debug == 0) {
   unlink("${expname}_${testname}.in");
}
