#!/usr/bin/perl
#
# William Hart (wehart@sandia.gov)
# Sandia National Labs
# March, 2006
#
# This configuration file defines the software projects that are managed
# by the DB Server.
#
# Note: these configuration options define operations that will be executed
# 	at the db-server directory.
#

$val = 0;

%db_config = (
   #
   # The Acro optimization library
   #
   'acro' => {
	name => 'Acro',
	dir => 'data/acro',
	maillist => 'acro-regression@software.sandia.gov',
	update => 'cvs -Q update data/acro',
	checkout => 'cvs -Q checkout -d data/acro acro/data',
	remove => 'cvs remove',
	commit => 'cvs commit -m "Files removed by db-daemon"',
	},
   #
   # The TEVA-SP library
   #
   'tevasp' => {
	name => 'TEVA-SP',
   	dir => 'data/tevasp',
	maillist => 'teva-tests@listserv.uc.edu wehart@sandia.gov jberry@sandia.gov jwatson@sandia.gov',
	update => 'cvs -Q update data/tevasp',
	checkout => 'cvs -Q checkout -d data/tevasp projects/teva-sp/data',
	remove => 'cvs remove',
	commit => 'cvs commit -m "Files removed by db-daemon"'
	},
);

1;
