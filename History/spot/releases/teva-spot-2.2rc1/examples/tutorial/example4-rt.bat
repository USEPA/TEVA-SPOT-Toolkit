@echo off
echo "BAT Script for Example 4"
echo "    Output in example4.out"

tso2Impact ex1 ex1.tso -vc -responseTime 0 > example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 --seed=123 --responseTime 0 > example4.out

tso2Impact ex1 ex1.tso -vc -responseTime 60 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 --seed=123 --responseTime=60 >> example4.out

tso2Impact ex1 ex1.tso -vc -responseTime 300 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 --seed=123 --responseTime=300 >> example4.out

tso2Impact ex1 ex1.tso -vc -responseTime 600 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 --seed=123 --responseTime=600 >> example4.out

tso2Impact ex1 ex1.tso -vc -responseTime 900 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 --seed=123 --responseTime=900 >> example4.out

tso2Impact ex1 ex1.tso -vc -responseTime 1200 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 --seed=123 --responseTime=1200 >> example4.out

tso2Impact ex1 ex1.tso -vc -responseTime 1500 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 --seed=123 --responseTime=1500 >> example4.out

tso2Impact ex1 ex1.tso -vc -responseTime 2000 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 --seed=123 --responseTime=2000 >> example4.out

tso2Impact ex1 ex1.tso -vc -responseTime 2500 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 --seed=123 --responseTime=2500 >> example4.out

tso2Impact ex1 ex1.tso -vc -responseTime 3000 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 --seed=123 --responseTime=3000 >> example4.out

tso2Impact ex1 ex1.tso -vc -responseTime 3500 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 --seed=123 --responseTime=3500 >> example4.out

tso2Impact ex1 ex1.tso -vc -responseTime 4000 >> example4.log
call sp --print-log --network=ex1 --objective=vc --solver=pico --ub=ns,1 --seed=123 --responseTime=4000 >> example4.out

