/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

//
// MixedIntVars.cpp
//

#include <utilib/MixedIntVars.h>

using namespace std;

namespace utilib {


MixedIntVars::MixedIntVars(const int numBits, const int numGenInts, 
					const int numDoubles)
	: a(0)
{
construct(numBits, numGenInts, numDoubles);
}


MixedIntVars::MixedIntVars()
	: a(0)
{
construct(0,0,0);
}


void MixedIntVars::resize(const int numBits, const int numGenInts,
                                        const int numDoubles)
{
a->bitVars.resize(numBits);
a->gintVars.resize(numGenInts);
a->doubleVars.resize(numDoubles);

}     


void MixedIntVars::write(ostream& os) const
{
os << a->bitVars << a->gintVars << a->doubleVars;
}


void MixedIntVars::read(istream& is)
{
is >> a->bitVars >> a->gintVars >> a->doubleVars;
}


void MixedIntVars::write(PackBuffer& os) const
{
os << a->bitVars << a->gintVars << a->doubleVars;
}


void MixedIntVars::read(UnPackBuffer& is)
{
is >> a->bitVars >> a->gintVars >> a->doubleVars;
}


void MixedIntVars::construct(const int nbits, const int nints, const int ndlbs)
{
free();
a = new MixedIntVarsRep(nbits, nints, ndlbs);
}

void MixedIntVars::free()
{
if (a) {
   delete a;
   a = 0;
   }
}


int MixedIntVars::compare(const MixedIntVars& array) const
{
int bitval = a->bitVars.compare(array.a->bitVars);
if (bitval != 0) return bitval;

int intval = a->gintVars.compare(array.a->gintVars);
if (intval != 0) return intval;

return a->doubleVars.compare(array.a->doubleVars);
}


bool MixedIntVars::operator==(const MixedIntVars& array) const
{
if (compare(array) == 0)
   return true;
return false;
}


MixedIntVars& MixedIntVars::operator=(const MixedIntVars& array)
{
if (this != &array) {
   free();
   construct(array.numIntegerVars(), array.numGeneralIntVars(), 
	     array.numDoubleVars());
   (*this) << array;
   }
return *this;
}


MixedIntVars& MixedIntVars::operator&=(const MixedIntVars& array)
{
if (array.a == a) return *this;

free();
a = array.a;
a->ref++;
return *this;
}

} // namespace utilib
