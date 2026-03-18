#!/usr/bin/perl 

# Stefan Chakerian (schake@sandia.gov)
# Sandia National Labs
# Jul, 2005

# convert regression test data to Dart2 format

use Data::Dumper;

use XML::Simple;
use URI::Escape;

my $xmlout;
my $out;
my $xml;
my $j;

my $f = shift;

sub datestr;	# sub at bottom, converts times/dates

$xml = XMLin("$f", forcearray => [ 'Test' ] );

# print Dumper($xml);

      my $track = "-Nightly";
      my $site;
      my $buildname= "$xml->{Key}->{Software}";
# $numtests saves tests, because accesses can create x->{Test}[0]
      my $numtests = $#{$xml->{Test}};
      my $buildstamp = $xml->{Test}[0]->{Date};

      my $x;
      ($x,$x,$site,$x) = split /-/, $xml->{Key}->{BuildName}, 4;
      $site .= " $xml->{Key}->{Site}";

      my $datestring;
      datestr($xml->{Test}[0]->{StartTime},\$datestring,\$buildstamp);
      
# add track to $buildstamp
      $buildstamp .= $track;

      $out = "<Site BuildName=\"$buildname\"\n"
	. "\tName=\"$site\"\n"
	. "\tBuildStamp=\"$buildstamp\"\n"
	. "\tGenerator=\"db_daemon\">\n";
      $out .= "  <Testing>\n";
      $out .= "  <StartDateTime>$datestring</StartDateTime>\n";

      for $j (0 .. $numtests) {
	my ($status,$path,$name,$fullname,$fullcommandline,$result);
	$status = ($xml->{Test}[$j]{IntegrityStatus} eq "Pass")
		? "passed" : "failed";
	$path = $xml->{Key}{Software};
	$path =~ s,-,/,g;
	$path = './' . $path;
	$name = $xml->{Test}[$j]{Name};
	$fullname = $path . "/" . $name;
	$fullcommandline = "/dev/null";

# forget xmlout... just construct it.
# This was elegant, but dart can't read it :(
#
#       $dart->{Test} = {
#	  Status => "$status",
#	  Path => "$path",
#	  Name => "$name",
#	  FullName => "$fullname",
#	  FullCommandLine => "$fullcommandline",
#	  Results => {
#	  	NamedMeasurement => [
#		  {
#		    name => "Execution Time",
#		    type => "numeric/double",
#		    Value => "$xml->{Test}[$j]{RunTime}{content}",
#		  },
#		  {
#		    name => "ExecutionStatus",
#		    type => "text/string",
#		    Value => "$xml->{Test}[$j]{ExecutionStatus}",
#		  }
#		],
#		Measurement => {
#		  Value => "(null)",
#		}
#	    },
#	};
#	$xmlout = XMLout($dart , KeepRoot => 1, NoAttr => 1);
#	$xmlout =~ s/<Test>/<Test Status=\"$status\">/;	# put Status as attr
#	$xmlout =~ s/(^|\n)/\n\t/g;	# nice indentation
#	$out .= $xmlout;

	$result = "  <Test Status=\"$status\">\n";
        $result .= "    <Path>$path</Path>\n";
        $result .= "    <Name>$name</Name>\n";
        $result .= "    <FullName>$fullname</FullName>\n";
        $result .= "    <FullCommandLine>$fullname</FullCommandLine>\n";
        $result .= "    <Results>\n";
	$result .= "\t  "
	  . "<NamedMeasurement name=\"Execution Time\" type=\"numeric/double\">"
	  . "<Value>$xml->{Test}[$j]{RunTime}{content}</Value>"
	  . "</NamedMeasurement>\n";
	$result .= "\t  "
	  . "<NamedMeasurement name=\"Execution Status\" type=\"numeric/double\">"
	  . "<Value>$xml->{Test}[$j]{ExecutionStatus}</Value>"
	  . "</NamedMeasurement>\n";

# Deal with extra <Measurements>

	for my $key (keys %{$xml->{Test}[$j]{Measurements}{Value}}) {
	  my $type = $xml->{Test}[$j]{Measurements}{Value}{$key}{type};
	  my $content = $xml->{Test}[$j]{Measurements}{Value}{$key}{content};
	  $type =~ s/int\b/integer/;
	  $result .= "\t  "
	  . "<NamedMeasurement name=\"$key\" type=\"$type\">"
	  . "<Value>$content</Value>"
	  . "</NamedMeasurement>\n";
	}
# STDOUT here (which isn't currently recorded)
	$result .= "\t  "
	  . "<Measurement><Value>(null)</Value></Measurement>\n";
        $result .= "    </Results>\n";
	$result .= "  </Test>\n";

        $out .= $result;
      }

      $out .= "  </Testing>\n";
      $out .= "</Site>\n";

    print "$out\n";


# convert buildstamp to "date" for Start/EndDateTime labels
use Time::Local;
sub datestr {
  my $time = shift;
  my $datestr_r = shift;
  my $buildstamp_r = shift;
  my ($hr,$min,$mday,$mon,$yr);

  $hr = sprintf "%02d", int $time;
  $min = int (($time - $hr) * 60);  # convert back to mins
  ($yr, $mon, $mday) = split /\//,$$buildstamp_r;
  if (!$mon) {
    $$datestr_r = "??? datetime missing";
    return;
  }
  $mon--;			# month is 0-11
  $hr %= 24;			# make sure 0-23
  $$buildstamp_r =~ s,/,,g;
  $$buildstamp_r .= sprintf "-%02d%02d", $hr, $min;

  $$datestr_r = localtime(timelocal(0, $min, $hr, $mday, $mon, $yr));
}
