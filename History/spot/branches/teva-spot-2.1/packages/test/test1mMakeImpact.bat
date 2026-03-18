rem  The lagrangian fails after this setup:
rem
rem if exist test1b.tso goto IMPACT
rem echo "Creating bio tso file..."
rem %bindir%\tevasim --tsg test1b.tsg --tsi test1b.tsi --tso test1b.tso Net3.inp Net3.out
rem 
rem if exist test1c.tso goto IMPACT
rem 
rem echo "Creating bio tso file..."
rem %bindir%\tevasim --tsg test1c.tsg --tsi test1c.tsi --tso test1c.tso Net3.inp Net3.out
rem 
rem :IMPACT
rem 
rem echo "Creating impact file..."
rem %bindir%\tso2Impact --pe test1 test1b.tso test1c.tso bio.tai chem.tai

if not exist test1.tso then
   %bindir%/tevasim --tsg test1.tsg --tsi test1.tsi --tso test1.tso Net3.inp Net3.out
endif

%bindir%/tso2Impact --mc --vc --ec --pe --epanetin Net3.inp test1 test1.tso chem.tai
