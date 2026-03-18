
! A trivial example of a .spc file for dylp. This tells dylp to always work
! with the full constraint system. Generally this is more efficient if an lp
! will only be solved once.

! svn/cvs: $Id: greenbeb.spc,v 1.1.1.1 2006/09/06 16:16:44 wehart Exp $

lpcontrol fullsys true ;

! Information print for current phase.

lpprint major 1 ;
