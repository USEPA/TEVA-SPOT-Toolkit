//
// exception_mngr.cpp
//

#include <utilib/utilib_config.h>
#ifdef UTILIB_HAVE_MPI
#include <mpi.h>
#endif

#include <utilib/exception_mngr.h>


void exit_fn() {}

namespace utilib {

namespace exception_mngr {


handle_t mode = Standard;


void set_mode(handle_t mode_) { mode=mode_; }


null_fn_type exit_function = &exit_fn;


void set_exit_function(null_fn_type fn) {exit_function = fn;}


} // exception_mngr namespace

} // utilib namespace
