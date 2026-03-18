#ifndef _TAI_H_
#define _TAI_H_

#include "ta_input.h"
#include "teva-assess.h"

PInputFileData loadTAIFileData(const char *taiFile);
void loadPopulationFile(char *fn,PopulationDataPtr pd);
#endif
