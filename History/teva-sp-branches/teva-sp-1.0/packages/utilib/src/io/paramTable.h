/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file paramTable.h
 *
 * Defines the utilib::parameter class.
 *
 * \author Jonathan Eckstein
 */

#ifndef utilib_paramTable_h
#define utilib_paramTable_h

namespace utilib {

/**
 * \class parameter 
 *
 *  Class that allows reading named numeric parameters from the command line
 *  via -<paramName>=<value> or from a file via -param=<file>.
 *  Note: for now the format of the parameter file is described in 
 *  \p utilib/src/io/paramTable.doc.
 *
 * \author Jonathan Eckstein
 */
class parameter
{
public:

  /// Constructor
  parameter(const char* name_,double minValue_,double value_,double maxValue_);

  /// Destructor
  ~parameter();

  /// Returns the current value of the parameter.
  double operator()() { return value; }

  /**
   * Processes the arguement list.
   * Note: \p minArgs and \p maxArgs refer to the number of 
   * command-line arguments that can \b not be parsed as parameters. If
   * the number of such arguments is outside the range 
   * [\p minArgs, \p maxArgs ], an error is signalled.
   */
  static void readAllSerial(int argc,char** argv,int minArgs = 0,
							int maxArgs = 1);

  /**
   * Processes the arguement list.
   * Note: \p minArgs and \p maxArgs refer to the number of 
   * command-line arguments that can \b not be parsed as parameters. If
   * the number of such arguments is outside the range 
   * [\p minArgs, \p maxArgs ], an error is signalled.
   */
  static void readAllParallel(int argc,char** argv,int minArgs = 0,
							int maxArgs = 1);

  /// Todo
  void changeValue(double newValue);

  /// Return the change counter
  int hasBeenChanged() { return changeCounter > 0; }

  /// Returns the number of parameters
  static int howMany() { return numParameters; }

  /// Local constant
  enum { tableSize=101 };

private:

  ///
  double     value;

  ///
  double     maxValue;

  ///
  double     minValue;

  ///
  const char*      name;

  ///
  int        changeCounter;

  ///
  parameter* next;

  ///
  parameter* prev;

  ///
  static int numParameters;

  ///
  static parameter* hashHeader[tableSize];

  ///
  static unsigned int hash(const char* name_);

  ///
  static parameter* lookup(const char* name_,int hashValue_);

  ///
  static parameter* lookup(const char* name_) 
	{ return lookup(name_,hash(name_)); }

  ///
  static void parseLine(int argc,char** arv,int minArgs = 0,int maxArgs = 1);

  ///
  static void readFromFile(const char* fileName);

  ///
  static double convert(const char* buffer, const char* errorKey);
  
};


///
#define CLASS_PARAMETER(name) \
   static utilib::parameter name

///
#define GLOBAL_PARAMETER(name) \
   extern utilib::parameter name

///
#define CLASS_PARAMETER_DEF(class,name,min,default,max) \
   utilib::parameter class::name(#name,min,default,max)

///
#define GLOBAL_PARAMETER_DEF(name,min,default,max) \
   utilib::parameter name(#name,min,default,max)

} // namespace utilib

#endif
