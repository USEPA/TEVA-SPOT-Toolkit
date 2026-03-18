#include "runif_if.h"
#include "teva-assess.h"
double runif()
{
	return mt_drand();
}

void DLLEXPORT set_seed(unsigned long seed)
{
	mt_seed32(seed);
}
