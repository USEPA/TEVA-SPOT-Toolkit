#ifndef config_bool_h
#define config_bool_h

#include <teva_config.h>
#ifdef ACRO_HAVE_STDBOOL_H
#include <stdbool.h>
#else
# if ! ACRO_HAVE__BOOL
#  ifdef __cplusplus
typedef bool _Bool;
#  else
typedef unsigned char _Bool;
#  endif
# endif
# define false 0
# define true 1
# define __bool_true_false_are_defined 1
#endif

#endif

