#include <stdlib.h>
#include <string.h>

#include "nrutil.h"
#include "sss.h"
#include "teva-assess.h"
#include "ta_utils.h"
#include "ta_input.h"

enum SensorSetKeywordTypes
{
	_SAMPLETYPE,    /* NodeID|ALL <type of sensor/sample: realtime|composite|filtered> */
	_SAMPLELOC,     /* NodeID|ALL <sensor/sample location code: ignore|existing|potential */
	_DETECTLIMIT,   /* NodeID|ALL <sensor detection limit> */
	_FILTERVOL,     /* NodeID|ALL <volume filtered in sampling period> */
	_SAMPLEFREQ,    /* NodeID|ALL <sample frequency> */
	_SAMPLESTART,   /* NodeID|ALL <first sample time> */
	_SAMPLEDELAY,   /* NodeID|ALL <sample transport/analysis time> */
};
char 
    *sensorSetKeywords[]   = {
    "SAMPLETYPE",      /* realtime, composite, filtered */
    "SAMPLELOC",       /* sample location (node ID) */
    "DETECTLIMIT",     /* detection limit (mg/L or number organisms) */
    "FILTERVOL",       /* volume filtered in one sampling period (liters) */
    "SAMPLEFREQ",      /* sample frequency (hours) */
    "SAMPLESTART",     /* time of first sampling event (hours since sim. start */
    "SAMPLEDELAY",     /* sample transport/analysis time (hours) */
    NULL
};
/*
**-----------------------------------------------------------
**  loadSSSFileData
**  Input:   sss (Sensor Solution Set) data file name
**  Output:  Pointer to input file data contents (for later
**           processing)
**  Purpose: Loads data file contents into memory for later
**           processing
**-----------------------------------------------------------
*/
PInputFileData loadSSSFileData(const char *sssFile)
{
	return loadInputFile(sssFile,sensorSetKeywords);
}
/*
**-----------------------------------------------------------
**  processSensorSetData
**  Input:   Pointer to input file data contents
**  Output:  Updated node/sensor infornmation based on data
**           file contents
**  Purpose: Processes sensor set data and updates sensor info
**-----------------------------------------------------------
*/
void processSensorSetData(PInputFileData sssIFD, PMem mem)
{
	int i;

	for(i=0;i<sssIFD->num;i++)
	{
		int sensorIDX;
		PInputLine il = &sssIFD->inputLines[i];
        /* Process line depending on input keyword */
        int sect = il->idx;
		int ntokens = il->ntokens;
		char **tok=il->tokens;
		PSensor sensor;

        switch( sect )
        {
        case _SAMPLELOC:
            if( ntokens != 8 ) ta_error(1,"Invalid input SAMPLELOC");

            if( (sensorIDX=getnodeindex(mem->net,mem->node,tok[1])) == -1 )
                ta_error(1,"Invalid SAMPLELOC sensor/sample node ID");
            sensor = &mem->node[sensorIDX].sensor;
			sensor->type=existing;

            if(!_stricmp(tok[2],"REALTIME"))
                sensor->sample = realtime;
            else if(!_stricmp(tok[2],"COMPOSITE"))
                sensor->sample = composite;
            else if(!_stricmp(tok[2],"FILTERED"))
                sensor->sample = filtered;
            else ta_error(1,"Invalid input SAMPLELOC (sub-field: sample type)");
            if ( !TAgetfloat(tok[3],&sensor->limit) ) ta_error(1,"Invalid input SAMPLELOC (sub-field: limit)");
            if ( !TAgetfloat(tok[4],&sensor->vol  ) ) ta_error(1,"Invalid input SAMPLELOC (sub-field: vol)");
            if ( !TAgetint  (tok[5],&sensor->freq ) ) ta_error(1,"Invalid input SAMPLELOC (sub-field: freq)");
            if ( !TAgetint  (tok[6],&sensor->start) ) ta_error(1,"Invalid input SAMPLELOC (sub-field: start)");
            if ( !TAgetint  (tok[7],&sensor->delay) ) ta_error(1,"Invalid input SAMPLELOC (sub-field: delay)");

            break;
		default:
            ta_error(1,"Invalid SSS keyword");
        }
    }   /* End of while */


}

