/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file exception_mngr.h
 *
 * Defines routines for managing exceptions in various ways: throwing the
 * exception, exiting the algorithm, or aborting.
 *
 * Note that you can disable the throwing of exceptions by adding the
 * following line in your code:
 \verbatim

  utilib::exception_mngr::set_mode(utilib::exception_mngr::Abort);
 \endverbatim
 * This code forces an abort whenever an exception occurs, which creates a
 * core file for debugging on most systems.
 *
 * \author William E. Hart
 */

#ifndef utilib_exception_mngr_h
#define utilib_exception_mngr_h

#include <utilib/utilib_config_bool.h>
#include <utilib/std_headers.h>
#ifdef UTILIB_HAVE_SSTREAM
#include <sstream>
#endif
#ifdef UTILIB_HAVE_MPI
#include <mpi.h>
#endif
#ifdef UTILIB_YES_COMMONIO
#include <utilib/CommonIO.h>
#endif

/// External function that is called before an exception is thrown
extern void exit_fn();

namespace utilib {

using std::ostringstream;
using std::runtime_error;

/**
 * A namespace that encapsulates the enum declaration and global data
 * used for managing exceptions.
 */
namespace exception_mngr {

using std::cerr;
using std::endl;


/** Different modes of operation for the exception manager. */
enum handle_t
  {
  /// Simply throw exceptions
  Standard,
  /// Print an error (using CommonIO) and abort
  Abort,
  /// Print an error (using CommonIO) and exit
  Exit
  };


/** The global mode of the exception manager. */
extern handle_t mode;


/** Set the mode of the exception manager. */
void set_mode(handle_t);


/** A typedef for functions that are called when aborting or exiting on
  * exceptions. */
typedef void (*null_fn_type)(void);


/** The global exit function of the exception manager. */
extern null_fn_type exit_function;


/** Setup a function that is called by \c handle_exception
  * when aborting or exiting.
  */
void set_exit_function(null_fn_type fn);



/** Process an exception based on the exception manager mode. */
template <class ExceptionT>
void handle_exception(ostringstream& msg)
{
ostringstream omsg;

#ifdef UTILIB_HAVE_MPI
int mpiActive=0;
MPI_Initialized(&mpiActive);
if (mpiActive) {
   int rank;
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   omsg << "(PN" << rank << ")  ";
   }
#endif

omsg << msg.str().c_str();

switch (utilib::exception_mngr::mode) {
  case Standard:
        if (exit_function) (*exit_function)();
        throw ExceptionT(omsg.str().c_str());

  case Abort:
        #ifdef UTILIB_HAVE_MPI
	/*
        if (mpiActive)
           MPI_Abort(MPI_COMM_WORLD,1);
	*/
        #endif
        if (exit_function) (*exit_function)();
	#ifdef UTILIB_YES_COMMONIO
	ucerr << omsg.str().c_str() << endl;
	CommonIO::end_all();
	#else
	cerr << omsg.str().c_str() << endl;
	#endif
        abort();
        break;

  case Exit:
        #ifdef UTILIB_HAVE_MPI
        if (mpiActive)
           MPI_Abort(MPI_COMM_WORLD,1);
        #endif
        if (exit_function) (*exit_function)();
	#ifdef UTILIB_YES_COMMONIO
	ucerr << omsg.str().c_str() << endl;
	CommonIO::end_all();
	#else
	cerr << omsg.str().c_str() << endl;
	#endif
        exit(-1);
  }
}


/** This macro is designed to test and handle an exception.
 *
 * @param  exception_test
 *               [in] Test for when an exception has occured.  This can and
 *               should be an expression that may mean something to the user.
 *               The text verbatim of this expression is included in the
 *               formed error string.
 * @param  Exception
 *               [in] This should be the name of an exception class.  The
 *               only requirement for this class is that it have a constructor
 *               that accepts and null terminated C string 
 *               (i.e. <tt>const char*</tt>).
 * @param  msg   [in] This is any expression that can be included in an
 *               output stream operation.  This is useful when building
 *               error messages on the fly.  Note that the code in this
 *               argument only gets evaluated if <tt>exception_test</tt>
 *               evaluates to <tt>true</tt> when an exception must be handled.
 *
 * This macro should be called within source code like a function, 
 * except that a semicolon should not be added after the macro.  For example,
 * suppose that in a piece of code in the file <tt>my_source_file.cpp</tt>
 * that the exception <tt>std::out_of_range</tt> is thrown if <tt>n > 100</tt>.
 * To use the macro, the source code would contain (at line 225
 * for instance):
 \verbatim

  EXCEPTION_TEST( n>100, std::out_of_range , "Error, n = " << n << is bad" )
 \endverbatim
 * When the program runs and with <tt>n = 125 > 100</tt> for instance,
 * the <tt>std::out_of_range</tt> exception would be thrown with the
 * error message:
 \verbatim

 /home/bob/project/src/my_source_file.cpp:225: n > 100: Error, n = 125 is bad
 \endverbatim
 *
 * Note that this macro calls the exception_mngr::handle_exception function,
 * so you can simply break in this function when using a debugger.  See
 * the \c set_mode function for a discussion of how to force exit or aborts
 * when an exception occurs (instead of throwing an exception).
 *
 * \note This macro is adapted from a similar technique developed by Roscoe
 * Bartlett for the rSQP++ software.
 */
#define EXCEPTION_TEST(exception_test,Exception,msg)\
	{\
	const bool test_val = (exception_test);\
	if (test_val) {\
  	   std::ostringstream omsg;\
	   omsg << __FILE__ << ":" << __LINE__ << ": "\
		<< #exception_test << ": " << msg;\
	   utilib::exception_mngr::handle_exception<Exception>(omsg);\
	   }\
	}
	   

/** This macro is designed to handle an exception.
 *
 * @param  Exception
 *               [in] This should be the name of an exception class.  The
 *               only requirement for this class is that it have a constructor
 *               that accepts and null terminated C string 
 *               (i.e. <tt>const char*</tt>).
 * @param  msg   [in] This is any expression that can be included in an
 *               output stream operation.  This is useful when building
 *               error messages on the fly.  Note that the code in this
 *               argument only gets evaluated if <tt>throw_exception_test</tt>
 *               evaluates to <tt>true</tt> when an exception is throw.
 *
 * This macro should be called within source code like a function, 
 * except that a semicolon should not be added after the macro.  For example,
 * suppose that in a piece of code in the file <tt>my_source_file.cpp</tt>
 * that the exception <tt>std::out_of_range</tt> is thrown if <tt>n > 100</tt>.
 * To use the macro, the source code would contain (at line 225
 * for instance):
 \verbatim

  if (n > 100)
    EXCEPTION_MNGR( std::out_of_range , "Error, n = " << n << is bad" )
 \endverbatim
 * When the program runs and with <tt>n = 125 > 100</tt> for instance,
 * the <tt>std::out_of_range</tt> exception would be thrown with the
 * error message:
 \verbatim

 /home/bob/project/src/my_source_file.cpp:225: Error, n = 125 is bad
 \endverbatim
 *
 * Note that this macro calls the exception_mngr::handle_exception function,
 * so you can simply break in this function when using a debugger.  See
 * the \c set_mode function for a discussion of how to force exit or aborts
 * when an exception occurs (instead of throwing an exception).
 */
#define EXCEPTION_MNGR(Exception,msg)\
	{\
		std::ostringstream omsg;\
	omsg << __FILE__ << ":" << __LINE__ << ": " << msg;\
	utilib::exception_mngr::handle_exception<Exception>(omsg);\
	}

/// A macro that performs a catch for all standard exception types
#define STD_CATCH(cmd)\
  catch (const char* str) { std::cerr << "ERROR! Caught exception string: " << str << std::endl << std::flush; cmd ; }\
\
  catch (const std::bad_alloc& err) { std::cerr << "ERROR! Caught `bad_alloc' exception: " << err.what() << std::endl << std::flush; cmd ; }\
\
  catch (const std::bad_exception& err) { std::cerr << "ERROR! Caught `bad_exception' exception: " << err.what() << std::endl << std::flush; cmd ; }\
\
  catch (const std::bad_cast& err) { std::cerr << "ERROR! Caught `bad_cast' exception: " << err.what() << std::endl << std::flush; cmd ; }\
\
  catch (const std::bad_typeid& err) { std::cerr << "ERROR! Caught `bad_typeid' exception: " << err.what() << std::endl << std::flush; cmd ; }\
\
  catch (const std::ios_base::failure& err) { std::cerr << "ERROR! Caught `ios_base::failure' exception: " << err.what() << std::endl << std::flush; cmd ; }\
\
  catch (const std::length_error& err) { std::cerr << "ERROR! Caught `length_error' exception: " << err.what() << std::endl << std::flush; cmd ; }\
\
  catch (const std::domain_error& err) { std::cerr << "ERROR! Caught `domain_error' exception: " << err.what() << std::endl << std::flush; cmd ; }\
\
  catch (const std::out_of_range& err) { std::cerr << "ERROR! Caught `out_of_range' exception: " << err.what() << std::endl << std::flush; cmd ; }\
\
  catch (const std::invalid_argument& err) { std::cerr << "ERROR! Caught `invalid_argument' exception: " << err.what() << std::endl << std::flush; cmd ; }\
\
  catch (const std::logic_error& err) { std::cerr << "ERROR! Caught `logic_error' exception: " << err.what() << std::endl << std::flush; cmd ; }\
\
  catch (const std::range_error& err) { std::cerr << "ERROR! Caught `range_error' exception: " << err.what() << std::endl << std::flush; cmd ; }\
\
  catch (const std::overflow_error& err) { std::cerr << "ERROR! Caught `overflow_error' exception: " << err.what() << std::endl << std::flush; cmd ; }\
\
  catch (const std::underflow_error& err) { std::cerr << "ERROR! Caught `underflow_error' exception: " << err.what() << std::endl << std::flush; cmd ; }\
\
  catch (const std::runtime_error& err) { std::cerr << "ERROR! Caught `runtime_error' exception: " << err.what() << std::endl << std::flush; cmd ; }\
\
  catch (const std::exception& str) { std::cerr << "ERROR! Caught other exception object: " << str.what() << std::endl << std::flush; cmd ; }

} // namespace exception_mngr

} // namespace utilib

#endif
