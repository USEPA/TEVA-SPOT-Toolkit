/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// Test the exception manager.
//

#include <utilib/std_headers.h>
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
