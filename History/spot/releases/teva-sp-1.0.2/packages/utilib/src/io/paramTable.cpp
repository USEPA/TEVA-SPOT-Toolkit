/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file paramTable.cpp
 *
 * Class that allows reading named numeric parameters from the command line
 * via -<paramName>=<value> or from a file via -param=<file>.
 *
 * \author Jonathan Eckstein
 */

#ifdef NON_ANSI
#include <iostream.h>
#include <fstream.h>
#else
#include <iostream>
#include <fstream>
#endif
#ifndef ANSI_HDRS
#include <string.h>
#include <stdio.h>
#else
#include <cstring>
#include <cstdio>
#endif
#if !defined(WINDOWS)
#include <strings.h>
#include <unistd.h>
#else
#include <direct.h>
#endif

#ifdef USING_MPI
#include <utilib/mpiUtil.h>
#endif
#include <utilib/errmsg.h>
#include <utilib/paramTable.h>
#include <utilib/nicePrint.h>
#include <utilib/CharString.h>
#include <utilib/CommonIO.h>
#include <utilib/comments.h>

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

namespace utilib {


int parameter::numParameters = 0;
parameter* parameter::hashHeader[parameter::tableSize];


parameter::parameter(const char* name_,
		     double minValue_,
		     double value_,
		     double maxValue_) :

		     value(value_),
		     maxValue(maxValue_),
		     minValue(minValue_),
		     name(name_),
		     changeCounter(0)
		     
{
  if (numParameters++ == 0)
    for(int i=0; i<tableSize; i++)
      hashHeader[i] = 0;
  if ((minValue > maxValue) ||
      (value < minValue)    ||
      (value > maxValue))
     EXCEPTION_MNGR(runtime_error, "Inconsistent setup for parameter " << name << " (min=" << minValue << ",default=" << value << ",max=" << maxValue << ")")
  int hashValue = hash(name);
  if (lookup(name,hashValue))
     EXCEPTION_MNGR(runtime_error, "Multiple parameters named " << name)
  prev = 0;
  next = hashHeader[hashValue];
  if (next)
    next->prev = this;
  hashHeader[hashValue] = this;
}


parameter::~parameter()
{
  if (prev)
    prev->next = next;
  else
    hashHeader[hash(name)] = next;
  if (next)
    next->prev = prev;
}


void parameter::changeValue(double newValue)
{
  if (newValue < minValue)
     EXCEPTION_MNGR(runtime_error, "Attempt to set parameter " << name << " to " << newValue << " < " << minValue)
  else if (newValue > maxValue)
     EXCEPTION_MNGR(runtime_error, "Attempt to set parameter " << name << " to " << newValue << " > " << maxValue)
  value = newValue;
  changeCounter++;
}


// Read in information on one processor.

void parameter::parseLine(int argc,char** argv,int minArgs,int maxArgs)
{
  int argCount = 0;

  for(int arg=1; arg<argc; arg++)
    {
      char* c = argv[arg];
      if (*c == '-')                            // Does this look like a param?
	{
	  CharString name(strlen(c++));         // Make buffer for param name
	  {
	    char* n = name.data();
	    while ((*c) && (*c != '='))         // Copy name to separate buffer
	      *(n++) = *(c++);                  // stop at '=' or null
	    *n = '\0';
	  }
	  if ((*c) && !(*(++c)))                    
	     EXCEPTION_MNGR(runtime_error, "A value must be supplied immediately after -" << name << "=")

	  if (strcmp(name.data(),"param") == 0)        // Should we read a file?
	    readFromFile(c);
	  else 
	    {
	      parameter* param = lookup(name.data());  // No, find param name
	      if (param)
		{                               // If found, set value
		  if (*c)
		    param->changeValue(convert(c,name.data()));
		  else
		    param->changeValue(1);      // If no value given, set to 1
		}
	      else
		EXCEPTION_MNGR(runtime_error, "Unknown parameter name -" << name)
	    }						
	}
      else
	argCount++;
    }
  if (argCount < minArgs)
     EXCEPTION_MNGR(runtime_error, "Less than " << minArgs << " command line argument" << plural(minArgs))
  if (argCount > maxArgs)
     EXCEPTION_MNGR(runtime_error, "More than " << maxArgs << " command line argument" << plural(maxArgs) )
}


// To read from a file

void parameter::readFromFile(const char* fileName)
{
  istream* s;
  if (fileName && *fileName)              // Open a file, if specified.
    {
      s = new ifstream(fileName);
      if (!s->good())
#if !defined(_MSC_VER)
	 EXCEPTION_MNGR(runtime_error, "Failed to open file " << fileName << " in directory " << getcwd(0,256))
#else
	 EXCEPTION_MNGR(runtime_error, "Failed to open file " << fileName << " in directory " << _getcwd(0,256))
#endif
    }
  else
    {
      ucout << "Reading parameters from standard input.\n";
      s = &cin;
    }
  
  char buffer[4096];

  while(!(s->eof())) 
    {
      *s >> whitespace;
      if (s->eof())
	break;
      *s >> buffer;
      if (*buffer == '*')
	for(char c='\0'; (c != '\n') && !(s->eof()); s->get(c));
      else 
	{
	  parameter* param = lookup(buffer);
	  if (!param)
	     EXCEPTION_MNGR(runtime_error, "Attempt to set unknown parameter " << buffer)
	  *s >> whitespace >> buffer;
	  if (s->eof())
	     EXCEPTION_MNGR(runtime_error, "Premature EOF in parameter file")
	  param->changeValue(convert(buffer,param->name));
	}
    }
  if (s != &cin)
    delete s;
}
  

// To convert values from string to float.

double parameter::convert(const char* buffer, const char* errorKey)
{
  double value;
#if defined(_MSC_VER)
  if (stricmp(buffer,"true") == 0)
#else
  if (strcasecmp(buffer,"true") == 0)
#endif
    return 1;
#if defined(_MSC_VER)
  else if (stricmp(buffer,"false") == 0)
#else
  else if (strcasecmp(buffer,"false") == 0)
#endif
    return 0;
  else if (!sscanf(buffer,"%lf",&value))
    EXCEPTION_MNGR(runtime_error, "Tried to set " << errorKey << " to non-numeric value '" << buffer << "'")
  return value;
};


// To handle reading in on (possibly) multiple processors.

void parameter::readAllSerial(int argc,char** argv,int minArgs,int maxArgs)
{
parseLine(argc,argv,minArgs,maxArgs);
}

void parameter::readAllParallel(int argc,char** argv,int minArgs,int maxArgs)
{
#ifdef USING_MPI
  if (!uMPI::running())
    {
      parseLine(argc,argv,minArgs,maxArgs);
      return;
    }

  double* buffer  = new double[numParameters];
  int*    ibuffer = new int[numParameters];
  int i = 0;
  int list;
  
  if (uMPI::iDoIO)
    {
      parseLine(argc,argv,minArgs,maxArgs);
      //double* cursor = buffer;
      for(list=0; list<tableSize; list++)
	for(parameter* param=hashHeader[list]; param; param=param->next)
	  {
	    buffer[i]  = param->value;
	    ibuffer[i] = param->changeCounter;
	    i++;
	  }
    }

  uMPI::broadcast(buffer,numParameters,MPI_DOUBLE,uMPI::ioProc);
  uMPI::broadcast(ibuffer,numParameters,MPI_INT,uMPI::ioProc);

  i = 0;
  if (!uMPI::iDoIO)
    for(list=0; list<tableSize; list++)
      for(parameter* param=hashHeader[list]; param; param=param->next)
	{
	  param->value = buffer[i];
	  param->changeCounter = ibuffer[i];
	  i++;
	}
	  
  delete[] buffer;
  delete[] ibuffer;
  
#endif
}

	
	    
	    
	

  
  

// Hash function lifted from Kernighan and Richie.

unsigned int parameter::hash(const char* name_)
{
  unsigned int hashValue = 0;
  while(*name_)
    hashValue = *(name_++) + 31*hashValue;
  return hashValue % (unsigned int) tableSize;
}


parameter* parameter::lookup(const char* name_,int hashValue_)
{
  parameter *ptr;
  for(ptr = hashHeader[hashValue_];
      ptr && strcmp(name_,ptr->name);
      ptr = ptr->next);
  return ptr;
}

} // namespace utilib
