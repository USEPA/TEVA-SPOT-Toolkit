//
// Test the exception manager.
//

#include <utilib/utilib_config.h>
#ifdef UTILIB_HAVE_STD
#include <iostream>
#include <cstdio>
#else
#include <iostream.h>
#include <stdio.h>
#endif

#ifdef UTILIB_HAVE_EXCEPTIONS
#include <stdexcept>
#else
#error "rewrite this file to work without exceptions"
#endif

#include <utilib/exception_mngr.h>

int option=0;

void foo()
{
EXCEPTION_MNGR(std::runtime_error, "This is a test. option=" << option);
}


int main(int argc, char* argv[])
{
if (argc == 1) {
	std::cout << "exception <num>\n";
	std::cout << "    Options:  1 - Throw exception,  2 - Abort,  3 - Exit\n";
   return 0;
   }
sscanf(argv[1],"%d",&option);

switch (option) {
  case 1:
	utilib::exception_mngr::set_mode(utilib::exception_mngr::Standard);
	break;
  case 2:
	utilib::exception_mngr::set_mode(utilib::exception_mngr::Abort);
	break;
  case 3:
	utilib::exception_mngr::set_mode(utilib::exception_mngr::Exit);
	break;
  };

try {
  foo();
} catch (std::runtime_error& foo) {
	std::cout << "Caught Exception: " << foo.what() << std::endl;
	}

return 0;
}
