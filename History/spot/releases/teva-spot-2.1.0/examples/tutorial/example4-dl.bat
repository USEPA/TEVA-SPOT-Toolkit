@echo off
echo "BAT Script for Example 4 (Detection Limit)"
echo "    Output in example4.out"

tso2Impact ex1 ex1.tso -vc -minQuality 0 > example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 > example4.out

tso2Impact ex1 ex1.tso -vc -minQuality 1.0 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 >> example4.out

tso2Impact ex1 ex1.tso -vc -minQuality 10.0 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 >> example4.out

tso2Impact ex1 ex1.tso -vc -minQuality 100.0 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 >> example4.out

tso2Impact ex1 ex1.tso -vc -minQuality 1000.0 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 >> example4.out

tso2Impact ex1 ex1.tso -vc -minQuality 10000.0 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 >> example4.out

tso2Impact ex1 ex1.tso -vc -minQuality 100000.0 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 >> example4.out

tso2Impact ex1 ex1.tso -vc -minQuality 1000000.0 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 >> example4.out
