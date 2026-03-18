#ifndef NAMED_DATA_JNI_H
#define NAMED_DATA_JNI_H

#include <jni.h>

typedef struct NamedDataData {
	jclass namedDataClass;
	jclass stringClass;

	jclass intArrayClass;
	jclass floatArrayClass;
	jclass doubleArrayClass;

	// miscellaneous methods
	jmethodID ndAddVariableMethod;

	// get methods
	jmethodID ndGetIntMethod;
	jmethodID ndGetIntArrayMethod;
	jmethodID ndGetIntArrayAtIndexMethod;

	jmethodID ndGetByteMethod;
	jmethodID ndGetByteArrayMethod;
	jmethodID ndGetByteArrayAtIndexMethod;

	jmethodID ndGetFloatMethod;
	jmethodID ndGetFloatArrayMethod;
	jmethodID ndGetFloatArrayAtIndexMethod;

	jmethodID ndGetDoubleMethod;
	jmethodID ndGetDoubleArrayMethod;
	jmethodID ndGetDoubleArrayAtIndexMethod;

	jmethodID ndGetStringMethod;
	jmethodID ndGetStringArrayMethod;

	jmethodID ndGetNamedDataMethod;

	// set methods
	jmethodID ndSetIntMethod;
	jmethodID ndSetIntArrayMethod;
	jmethodID ndSetIntArray2DMethod;
	jmethodID ndSetIntArrayAtIndexMethod;

	jmethodID ndSetByteMethod;
	jmethodID ndSetByteArrayMethod;
	jmethodID ndSetByteArrayAtIndexMethod;

	jmethodID ndSetFloatMethod;
	jmethodID ndSetFloatArrayMethod;
	jmethodID ndSetFloatArray2DMethod;
	jmethodID ndSetFloatArrayAtIndexMethod;

	jmethodID ndSetDoubleMethod;
	jmethodID ndSetDoubleArrayMethod;
	jmethodID ndSetDoubleArray2DMethod;
	jmethodID ndSetDoubleArrayAtIndexMethod;

	jmethodID ndSetStringMethod;
	jmethodID ndSetStringArrayMethod;

	jmethodID ndSetNamedDataMethod;

} NamedData, *NamedDataPtr;

#endif

