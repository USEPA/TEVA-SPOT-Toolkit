/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file ParameterList.h
 * 
 * Defines the utilib::ParameterList class.
 */

#ifndef utilib_ParameterList_h
#define utilib_ParameterList_h

#include <iostream>
#include <utilib/PackBuf.h>
#include <utilib/Parameter.h>
#include <utilib/ParameterSet.h>
#include <list>
#include <string>
#include <vector>
#include <map>
#if defined(USING_MPI)
#include <utilib/mpiUtil.h>
#endif

namespace utilib {

class ParameterSet;


/**
 * Defines an auxillary class that can be used to define a list of 
 * parameters that will be fed into a ParameterSet object.  This class
 * offers additional functionality beyond a simple list of pairs of
 * strings.  Specifically, it can process command-line arguments and
 * files of parameters.
 *
 * Parameter values can be initialized with command-line arguments using the 
 * ParameterList::process_parameters method.  This method supports
 * the use of the GNU style uses parameter keywords preceded by two hyphens
 * rather than keyword letters. This style is extensible to contexts in
 * which there are two many parameters to use single-letter parameters. This 
 * style is easier to read than the alphabet soup of older styles, and it can be
 * combined with single-letter parameters (for commonly used parameters). 
 * A parameter argument (if any) can be separated by either whitespace or a 
 * single .=. (equal sign) character:
 *
 *    program --param1 paramval --param2=paramval
 *
 */
class ParameterList
{
  friend class ParameterSet;

public:

  /// Constructor
  ParameterList() {}

  /// Destructor
  ~ParameterList() {}

#if defined(USING_MPI)
  ///
  void synchronize(int root=0, MPI_Comm comm = MPI_COMM_WORLD);
#endif

  ///
  void read_parameters(istream& is, const string& terminator);

  ///
  void read_parameters(istream& is)
		{read_parameters(is,"");}

  ///
  void write_parameters(ostream& os) const;

  ///
  void write_parameters(PackBuffer& os) const;

  ///
  void read_parameters(UnPackBuffer& is);

  ///
  void process_parameters(int& argc, char**& argv, 
			unsigned int min_num_required_args);

  ///
  void process_parameters(const std::string& filename);

  ///
  size_type size()
		{return parameters.size();}

  ///
  void add_parameter(const std::string& label, const std::string& value);

  ///
  void register_parameters(ParameterSet& pset);

  ///
  template <class Type>
  void register_parameters();

  ///
  void write_registered_parameters(ostream& os, bool verbose=true,
                                        const char* seperator="\n") const
		{registered_params.write_parameters(os,verbose,seperator);}

  ///
  void write_unused_parameters(ostream& os);

  ///
  int unused();

protected:

  ///
  class ParameterListItem {
    public:
    ///
    ParameterListItem() : used(false) {}
    ///
    string label;
    ///
    string value;
    ///
    bool   used;
    };

  ///
  std::list<ParameterListItem> parameters;

  ///
  ParameterListItem tmp_param;

  ///
  ParameterSet registered_params;

};


template <class Type>
inline void ParameterList::register_parameters()
{
Type tmp;
register_parameters(tmp);
}

} // namespace utilib

#endif
