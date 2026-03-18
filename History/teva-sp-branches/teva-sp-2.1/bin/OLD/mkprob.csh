##
## mkprob <bindir> <deffile>
##
## Creates a new class instance 
##

set bindir = $1
set defs = ${2}.def

$bindir/fillfile class/ClassOpt.cppt class/ClassOpt.ht $defs
#$bindir/fillfile class/ClassAppInterface.cppt class/ClassAppInterface.ht $defs 
$bindir/fillfile class/ClassDirectApp.cppt class/ClassDirectApp.ht $defs 
#$bindir/fillfile class/ClassMSApp.cppt class/ClassMSApp.ht $defs 
$bindir/fillfile class/ClassMSOpt.cppt class/ClassMSOpt.ht $defs 
#$bindir/fillfile class/ClassMixedApp.cppt class/ClassMixedApp.ht $defs 
$bindir/fillfile class/ClassOpt.cppt class/ClassOpt.ht $defs 
$bindir/fillfile class/ClassOptConstructors.cppt class/ClassOptConstructors.ht $defs 
$bindir/fillfile class/ClassOptProblem.cppt class/ClassOptProblem.ht $defs 
#$bindir/fillfile class/ClassSyscallApp.cppt class/ClassSyscallApp.ht $defs 
#$bindir/fillfile - class/ClassProblemResponseDB.ht $defs 

set class = ${defs:r}
ls ${class}/*.cpp | awk '{printf "%s\\\n", $1;}' >> Makefile.classes


