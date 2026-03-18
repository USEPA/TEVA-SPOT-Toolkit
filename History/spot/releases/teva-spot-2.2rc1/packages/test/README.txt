
This test directory contains python unit tests and exact studies.
The python unit tests all have names like "test_{name}.py" and
the exact studies have "study" in the name.

The python unit tests are designed to test sensor placement
components in isolation.  We would like to add more unit tests.

The exact studies run series of tests varying important parameters 
to increase test coverage.

Also, the "test1" directory contains the original linux shell and
windows batch scripts that set up problems to run with the sp
scrip

The "Robs" directory contains a python script called "compare.py"
which can be used to run different solvers with different parameters.
The results are output to a csv file which can be viewed in a
spreadsheet.  We want to run this more often and look for anomalies
of the type that Rob Janke brought to our attention recently.
By editing the top of the compare.py file, you can change the
values of parameters over which the tests are run.

Before running compare.py or any of the unit tests, run "runtests.py"
at least once.  It creates "path.py" which the other tests need
to find supporting python code.  You can also use runtests.py to
select and run the unit tests.
