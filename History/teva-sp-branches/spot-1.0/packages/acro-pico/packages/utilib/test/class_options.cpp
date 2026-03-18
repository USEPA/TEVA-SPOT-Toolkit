/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// ParameterSet test
//

#include <utilib/std_headers.h>
#include <utilib/stl_auxillary.h>
#include <utilib/ParameterSet.h>
#include <utilib/mpiUtil.h>

#ifdef UTILIB_HAVE_NAMESPACES
using namespace std;
using namespace utilib;
#endif

class A : public ParameterSet
{
public:

  A()
	{
	ParameterLowerBound<int> nonnegative_check(0);
	create_parameter("var1",var1,"","","",nonnegative_check);
	create_parameter("variable4",var4,"","","",ParameterBounds<double>(-10.0,10.0));
	create_parameter("var2",var2,"","","");
	create_parameter("var3",var3,"","","");
	create_parameter("var5",var5,"","","");
	var1=0;
	var2=true;
	var3="str";
	var4=0.0;
	}

  int var1;
  bool var2;
  std::string var3;
  double var4;
  std::vector<int> var5;

};


int main(int argc, char* argv[])
{
#ifdef UTILIB_HAVE_MPI
	uMPI::init(&argc,&argv);
#endif

utilib::exception_mngr::set_mode(utilib::exception_mngr::Abort);
utilib::exception_mngr::set_exit_function(&exit_fn);

ifstream ifstr("class_options.in");
ParameterSet params;
A a;
A b;

try {
  if (argc > 1) {
     a.process_parameters(argc,argv);
     a.write_parameters(cout,true);
     cerr << "argc: " << argc << " argv[1]: " << argv[1] << endl;
     }

  a.var5.resize(5);
  a.var5 << 3;

  a.set_parameter("variable4",-10.0);
  a.alias_parameter("variable4","double");
  a.write_parameters(cout,true);
  cout << a;


  a.read_parameter_values(ifstr,"TERMINATOR");
  a.write_parameters(cout,true);
  cout << a;

  PackBuffer pack;
  pack << a;
  UnPackBuffer unpack(pack);
  b.write_parameters(cout,true);
  cout << b;
  unpack >> b;
  cout << b;
  }
catch (std::exception& err) {
  cerr << "Caught unexpected exception: " << err.what() << endl;
  exit(1);
  }


try {
  ifstr >> a;
  }
catch (std::exception& err) {
  cerr << err.what() << endl;
  }
cout << a;

/// Test out a copy constructor
{
A c(a);
}

}
