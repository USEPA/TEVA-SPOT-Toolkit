@echo off
echo "BAT Script for Example 3 (Number of Sensors vs VC)"
echo "    Output in example3.out"

tso2Impact --vc --minQuality 0 ex1 ex1.erd > example3.log

evalsensor --nodemap=ex1.nodemap none ex1_vc.impact > example3.out

call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 >> example3.out

call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,2 >> example3.out

call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,3 >> example3.out

call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,4 >> example3.out

call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,5 >> example3.out

call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,6 >> example3.out

call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,7 >> example3.out

call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,8 >> example3.out

call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,9 >> example3.out

call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,10 >> example3.out
