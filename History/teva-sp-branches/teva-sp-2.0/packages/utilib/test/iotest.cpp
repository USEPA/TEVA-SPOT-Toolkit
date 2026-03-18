//
// CommonIO Test
//

#include <utilib/utilib_config.h>
#ifdef UTILIB_YES_COMMONIO 

#ifdef WARNINGS
#undef WARNINGS
#endif
#ifdef UTILIB_VALIDATING
#undef UTILIB_VALIDATING
#endif

#ifndef UTILIB_HAVE_STD
#include <fstream.h>
#include <stdio.h>
#include <stdlib.h>
#else
#include <fstream>
#include <cstdio>
#include <cstdlib>
#endif

#ifdef UTILIB_HAVE_NAMESPACES
using namespace std;
#endif

#include <utilib/exception_mngr.h>
#include <utilib/CommonIO.h>
#include <utilib/mpiUtil.h>

using namespace utilib;

class A : public CommonIO {
public:
A() : a(0) {}
void foo() {DEBUGPR(1,ucout << "A IS HERE" << endl;)}
int a;
};

class B : public A {
public:
B() : b(1) {}
void foob() {DEBUGPR(2,ucout << "B IS HERE\n\n" << endl;)}
int b;
};

void foo()
{
ucout << "Beginning CommonIO block." << endl;
CommonIO::begin();
A a;
B b;

DEBUGPRX(0,&(global_CommonIO()), "Debug value 0 works" << endl;)

ucout << "TEST 1" << endl;
a.foo();
ucout << endl << Flush;

ucout << "TEST 2" << endl;
b.foob();
ucout << endl << Flush;

ucout << "TEST 3" << endl;
a.debug = 1;
a.foo();
ucout << endl << Flush;

ucout << "TEST 4" << endl;
b.debug = 1;
b.foo();
b.foob();
ucout << endl << Flush;

ucout << "TEST 5" << endl;
b.debug = 2;
b.foo();
b.foob();
ucout << endl << Flush;

ucout << "TEST FINAL" << endl;
CommonIO::end();
ucout << "Ending CommonIO block." << endl;
}




int main(int argc, char* argv[])
{
utilib::exception_mngr::set_mode(utilib::exception_mngr::Abort);

try {
#ifdef UTILIB_HAVE_MPI
uMPI::init(&argc, &argv);
if (CommonIO::numProcs() > 1) {
   CommonIO::setIORank(-1);
   ucout << "Rank: " << CommonIO::rank() << " IOProc: " << uMPI::ioProc << endl;
   }
#endif

CommonIO::begin();
ucout << "ucout_flag=" << (CommonIO::io_mapping || CommonIO::common_cout) <<  endl;
ucout << "----------------------------------------------------------" << endl;
ucout << "FOO -- default settings" << endl << Flush;
foo();
ucout << endl;
ucout << "----------------------------------------------------------" << endl;
CommonIO::end();


ucout << "----------------------------------------------------------" << endl;
ucout << "FOO -- output to a file iotest.temp.*" << endl << Flush;
char tmp[256];
sprintf(tmp,"iotest.temp.%d",CommonIO::rank());
ofstream ofstr(tmp);
CommonIO::set_streams(&ofstr,&ofstr,&stdcin);
foo();
CommonIO::reset();
ofstr.close();
ucout << endl;
ucout << "----------------------------------------------------------" << endl;


ucout << "----------------------------------------------------------" << endl;
ucout << "FOO -- tagged output with numDigits=5" << endl << Flush;
CommonIO::begin_tagging(5);
foo();
CommonIO::begin_tagging(0);
ucout << endl;
ucout << "----------------------------------------------------------" << endl;


ucout << "----------------------------------------------------------" << endl;
ucout << "FOO -- default settings" << endl << Flush;
foo();
ucout << endl;
ucout << "----------------------------------------------------------" << endl;


ucout << "----------------------------------------------------------" << endl;
ucout << "FOO -- buffered output" << endl << Flush;
CommonIO::begin_buffered();
foo();
CommonIO::end();
ucout << endl;
ucout << "----------------------------------------------------------" << endl;


ucout << "----------------------------------------------------------" << endl;
ucout << "FOO -- default settings" << endl << Flush;
foo();
ucout << endl;
ucout << "----------------------------------------------------------" << endl;


ucout << "----------------------------------------------------------" << endl;
ucout << "FOO -- buffered output to a file with tagging" << endl << Flush;
CommonIO::begin_buffered();
CommonIO::begin_tagging(5);
sprintf(tmp,"iotest.temp2.%d",CommonIO::rank());
ofstr.open(tmp);
CommonIO::set_streams(&ofstr,&ofstr,&stdcin);
foo();
CommonIO::end_tagging();
CommonIO::end();
CommonIO::reset();
ofstr.close();
ucout << endl;
ucout << "----------------------------------------------------------" << endl;


ucout << "Closing MPI" << endl << Flush;
#ifdef UTILIB_HAVE_MPI
uMPI::done();
#endif
ucout << "Ending IO" << endl << Flush;
 }
catch (const std::exception& err) {
   cout << "ERROR: " << err.what() << endl;
}
   
}

#else

#include <iostream>

int main()
{
std::cout << "No testing done since CommonIO is not configured!\n";
return 1;
}
#endif
