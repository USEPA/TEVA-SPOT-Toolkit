// This is third-party software that is distributed with the TEVA-SPOT Toolkit.
// Usage of this code is covered by an ATT Commercial License.

#ifndef REDEFS_H
#define REDEFS_H

#include <teva_config.h>
#include <math.h>

#ifndef TEVA_SPOT_HAVE_RINT
#define rint(x) (floor((x)+.05))
#endif

#ifndef TEVA_SPOT_HAVE_SRANDOM
#define srandom(x) (srand(x))
#define random() (rand())
#endif

#ifndef TEVA_SPOT_HAVE_LOG2
#define log2(x) (log(x) / log((double)2))
#endif

#endif
