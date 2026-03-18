#include "runif_if.h"

double runif()
{
	return mt_drand();
}

void set_seed(unsigned long seed)
{
	mt_seed32(seed);
}
