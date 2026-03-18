/*  _________________________________________________________________________
 *
 *  PICO: A C++ library of scalable branch-and-bound and related methods.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top PICO directory.
 *  _________________________________________________________________________
 */

/**
 * \file MILPSymbInfo.h
 *
 * Defines the pico::MILPSymbInfo class.
 */

#ifndef pico_MILPSymbInfo_h
#define pico_MILPSymbInfo_h

#include <acro_config.h>
#include <utilib/QueueList.h>
#include <utilib/BasicArray.h>
#include <utilib/HashedSet.h>
#include <pico/ILPHashItem.h>

namespace pico {

using utilib::QueueList;
using utilib::HashedSet;

/**
 * The class that processes the symbolic information for the rows/columns of
 * a MILP.  Can be used to generate an AppInfo class with mapped symbols.
 */
class MILPSymbInfo
{
public:

  ///
  MILPSymbInfo();

  ///
  virtual ~MILPSymbInfo();

  ///
  void preprocess_ILP(CharString& probname,
			BasicArray<CharString>& row_names,
                        BasicArray<CharString>& col_names,
			CharString& mapfile,
                        CharString& ofile,
			const bool write_flag=false);

  ///
  virtual void add_valid_tuple(QueueList<CharString> ) {}

  /// Read in a valfile
  void read_valfile(CharString& valfile);

  ///
  void write(PackBuffer& pack) const;

  ///
  void read(UnPackBuffer& unpack);

protected:

  ///
  void split_args(const CharString& name, QueueList<CharString>& name_list);

  ///
  void extract_all_names(const CharString& name,
                                        QueueList<CharString>& name_list);

  ///
  void setup_ilp_symbols(BasicArray<CharString>& names,
                        HashedSet<MILPVarMap>& index_ht,
			const int rc_flag);

  /// Init mapfile structures using a *.val file stream
  void create_mapfile(std::istream& ifstr);

  /// Init mapfile using a row/column file stream (and write it out)
  void create_mapfile(std::ostream& ofstr, 
                        BasicArray<CharString>& names,
                        const int write_flag);

  /// Read in a mapfile
  void read_mapfile(CharString& mapfile);

  ///
  void add_set(const bool flag=false);

  ///
  BasicArray<CharString>                 set_names;

  ///
  BasicArray<ILPArgVal>			  sets;

  ///
  BasicArray<ILPArgList>	          set_types;

  ///
  unsigned int num_sets;

  ///
  HashedSet<ILPSymbItem>            hashset;

  ///
  HashedSet<MILPVarMap>             row_index_ht;

  ///
  HashedSet<MILPVarMap>             col_index_ht;

  ///
  HashedSet<MILPVarMap>             other_index_ht;

private:

  ///
  CharString lname;

};


} // namespace pico

#endif
