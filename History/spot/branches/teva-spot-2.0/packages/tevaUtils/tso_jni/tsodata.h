
#ifndef _TSODATA_H_
#define _TSODATA_H_

typedef struct TSOData {
	jclass readerClass;
	jclass netInfoClass;
	jclass nodeInfoClass;
	jclass linkInfoClass;
	jclass sourceClass;
	jclass dataIndexClass;
	jclass compressedTSOClass;

	jclass stringClass;
	jclass floatArrayClass;

	jmethodID netInfoConstructor;
	jmethodID nodeInfoConstructor;
	jmethodID linkInfoConstructor;
	jmethodID sourceConstructor;
	jmethodID dataIndexConstructor;
	jmethodID compressedTSOConstructor;

	jfieldID readerSources;

	jfieldID nodeInfoQ;
	jfieldID nodeInfoC;
	jfieldID nodeInfoNZ; // one for each species

	jfieldID linkInfoVelocity;
	jfieldID linkInfoFlow;
    jfieldID linkInfoVX;
    jfieldID linkInfoVY;
} TSOData, *TSODataPtr;


#endif

