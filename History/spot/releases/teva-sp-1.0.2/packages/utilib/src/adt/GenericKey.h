/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file GenericKey.h
 * 
 * \depricated definition of utilib::GenericKey class 
 */

#ifndef utilib_GenericKey_h
#define utilib_GenericKey_h

#include <iostream>

#ifdef ANSI_NAMESPACES
using namespace std;
#endif

namespace utilib {

///
template <class KEY, class DATA>
class GenericKey
{
public:

  /// Return true if \a key equals the current key.
  virtual bool operator==(const GenericKey<KEY,DATA>& key) const
		{return (Key == key.Key);}

  /// Return true if the current key is less than \a key.
  virtual bool operator<(const GenericKey<KEY,DATA>& key) const
		{return (Key < key.Key);}

  /// Return true if the current key is greater than \a key.
  virtual bool operator>(const GenericKey<KEY,DATA>& key) const
		{return (Key > key.Key);}

  /**
   * Compare \a key with the current key, returning a value that is 
   * consistent with the 
   * \if GeneratingLaTeX standard comparison semantics (see Section~\ref{compare} \endif
   * \if GeneratingHTML \ref compare "standard comparison semantics" \endif
   */
  virtual int compare(const GenericKey<KEY,DATA>& val) const
		{return (Key < val.key() ? -1 : Key > val.key() ? 1 : 0);}

  /// Write the key to an output stream.
  virtual void write(ostream& os) const
		{os << Key << " " << Data;}

  /// Read the key from an input stream.
  virtual void read(istream& )
		{}

  /// Return a reference to the data.
  DATA& data()
		{return Data;}

  /// Return a reference to the key.
  KEY&  key()
		{return Key;}

  /// Return a const reference to the key.
  const KEY&  key() const
		{return Key;}

protected:

  /// The key.
  KEY Key;

  /// The data.
  DATA Data;
};


} // namespace utilib

template <class KEY, class DATA>
inline ostream& operator<<(ostream& output, utilib::GenericKey<KEY,DATA>& key)
{ key.write(output); return(output); }

template <class KEY, class DATA>
inline istream& operator>>(istream& input, utilib::GenericKey<KEY,DATA>& key)
{ key.read(input); return(input); }


#endif
