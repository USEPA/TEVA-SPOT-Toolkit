#!/usr/bin/env perl

$re = '^(.*?)(\\lstinputlisting)(?:\[([^\]]*)\])?\{([^}]+)\}'.
      '(\s*\%\s*anchor\s*=\s*)(\S+)(.*)';

unless ( @ARGV )
  { die "Usage: $0 target.tex ...\n"; }

foreach $f ( @ARGV )
  {
  unless ( -f $f and -r $f and -w $f )
    {
    print "File '$f' does not appear to be a valid target.  Skipping.\n";
    next;
    }
  print "Updating $f\n";
  &update_file($f);
  }

exit 0;


sub update_file($)
  {
  my($fname) = @_;

  open FILE, "<$fname" or die "Error opening TeX file $fname\n";

  my(@lines) = ();
  foreach $line ( <FILE> )
    {
    unless ( $line =~ /$re/so )
      { push @lines, $line; }
    else
      {
      print "Updating reference: $line";
      my($front) = $1;
      my($cmd)   = $2;
      my($args)  = $3;
      my($file)  = $4;
      my($key   )= $5;
      my($anchor)= $6;
      my($tail)  = $7;

      my($firstline) = -1;
      my($lastline) = -1;
      open REF, "<$file" or die("Error opening referenced file $file ".
                                "while processing $fname\n");
      my($lineno) = 0;
      # scan file for the anchor tags (start = '@anchor:', end = '@:anchor')
      while ( $_ = <REF> )
        {
        ++$lineno;
        if ( /\@${anchor}:/ )
          {
          if ( $firstline != -1 )
            { die("Error: multiple starting tags found for anchor $anchor ".
                  " in file $file while processing $fname"); }
          $firstline = $lineno+1;
          }
        if ( /\@:${anchor}/ )
          {
          if ( $lastline != -1 )
            { die("Error: multiple ending tags found for anchor $anchor ".
                  " in file $file while processing $fname"); }
          $lastline = $lineno-1;
          }
        }
      close REF;

      if ( $firstline == -1 )
        { $args =~ s/firstline=\d+//; }
      elsif ( not ( $args =~ s/firstline=\d+/firstline=$firstline/ ) )
        {
        if ( $args )
          { $args .= ','; }
        $args .= "firstline=$firstline";
        }
    
      if ( $lastline == -1 )
        { $args =~ s/lastline=\d+//; }
      elsif ( not ( $args =~ s/lastline=\d+/lastline=$lastline/ ) )
        {
        if ( $args )
          { $args .= ','; }
        $args .= "lastline=$lastline";
        }
      
      # misc cleanup
      while ( $args =~ s/,,/,/ ) {}
      $args =~ s/^,//;
      $args =~ s/,$//;

      # form the modified line
      push @lines, "$head\\${cmd}[$args]{$file}$key$anchor$tail"
      }
    }

  close FILE;
  open FILE, ">$fname" or die "Error overwriting file $fname\n";
  print FILE join '', @lines;
  close FILE;
  }
