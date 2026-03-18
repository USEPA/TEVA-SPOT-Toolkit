#ifndef _SSS_H_
#define _SSS_H_
#include "teva-assess.h"
#include "ta_input.h"
/*
**-------------------------------------------------------------
** Input data file section headers 
**-------------------------------------------------------------
*/
PInputFileData loadSSSFileData(const char *sssFile);
void processSensorSetData(PInputFileData sssIFD,PMem mem);

#endif
