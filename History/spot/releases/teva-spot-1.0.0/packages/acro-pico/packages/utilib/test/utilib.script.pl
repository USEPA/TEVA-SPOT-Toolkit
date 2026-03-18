#!/usr/bin/perl

#
# Read input parameters
#
(open INPUT, "$ARGV[0]") || die "ERROR: cannot open file $ARGV[0]";
@lines = <INPUT>;
close(INPUT);
#
# Find command-line parameter
#
foreach $line (@lines) {
  chomp($line);
  @words=split(/\s+/,$line);
  #print "LINE: $line\n";
  if ($words[0] eq "command") {
     `$words[1] > $ARGV[2] 2>&1`;
     $command=$words[1];
     last;
     }
  }
#
# Generate measurements
#
$diffs = `diff $ARGV[2] ${command}.qa`;
open OUTPUT, ">$ARGV[1]";
print OUTPUT "_exit_status numeric/integer 0\n";
if ($diffs == "") {
   print OUTPUT "diffs numeric/boolean 0\n";
}
else {
   print OUTPUT "diffs numeric/boolean 1\n";
}
print OUTPUT "diff_output text/string <Value>\n";
print OUTPUT "$diffs";
print OUTPUT "</Value>\n";
close(OUTPUT);
