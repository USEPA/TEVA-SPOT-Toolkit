#!/usr/bin/perl -w

my ($line, @block, $bl);
my $rx='^(\s*#\s*include\s*\<\s*)c(.*)(\>.*)';

while ($line = <>) {
  if ($line =~ /ANSI_HDRS/) {           # already done
    print $line;
    while (<>) {
      print;
    }
  } elsif ($line =~ /$rx/) {
    @block = ($line);
    while ( $line = <>) {
      last if (! ($line =~ /$rx/));
      push @block, ($line);
    }

    print "#if defined(ANSI_HDRS)\n";

    foreach $bl (@block) {
      print $bl;
    }

    print "#else\n";

    foreach $bl (@block) {
      $bl =~ s/$rx/$1$2.h$3/;
      print $bl;
    }
    print "#endif\n";

    print $line;
  } else {
    print $line;
  }
}
