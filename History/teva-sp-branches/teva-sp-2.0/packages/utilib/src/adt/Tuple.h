/*  _________________________________________________________________________
 *
 *  UTILIB: A utility library for developing portable C++ codes.
 *  Copyright (c) 2001, Sandia National Laboratories.
 *  This software is distributed under the GNU Lesser General Public License.
 *  For more information, see the README file in the top UTILIB directory.
 *  _________________________________________________________________________
 */

/**
 * \file Tuple.h
 *
 * Defines the tuple class objects: utilib::Tuple1, utilib::Tuple2, 
 * utilib::Tuple3, utilib::Tuple4, utilib::Tuple5, utilib::Tuple6 
 * and utilib::Tuple7.
 *
 * \note It might be possible to create a truly n-tuple object using 
 * template value parameters (e.g. Tuple<n>), but this does not appear to be
 * widely supported at the moment.
 */

#ifndef utilib_Tuple_h
#define utilib_Tuple_h

#include <utilib/utilib_config_bool.h>
#include <utilib/utilib_config_explicit.h>
#ifdef UTILIB_HAVE_STD
#include <iostream>
#else
#include <iostream.h>
#endif

#include <utilib/BasicArray.h>

namespace utilib {

template <class T1> class Tuple1;
template <class T1, class T2> class Tuple2;
template <class T1, class T2, class T3> class Tuple3;
template <class T1, class T2, class T3, class T4> class Tuple4;
template <class T1, class T2, class T3, class T4, class T5> class Tuple5;
template <class T1, class T2, class T3, class T4, class T5, class T6> class Tuple6;
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7> class Tuple7;



/**
 * Implements a 1-tuple which provides a fixed length list with
 * syntactic differentiation of the elements.
 */
template <class T1> class Tuple1 : public PackObject
{
public:

  friend class BasicArray<Tuple1<T1> >;

  ///
  explicit 
  Tuple1(T1 _val1) 
		: val1(_val1) {num_elements=1;}

  ///
  virtual ~Tuple1() {}

  ///
  bool operator==(const Tuple1<T1>& tuple) const
		{return (tuple.val1 == val1);}

  ///
  bool operator<(const Tuple1<T1>& tuple) const
		{return (val1 < tuple.val1);}

  ///
  T1 val1;

  ///
  void write(std::ostream& os) const
		{
		os << "( " << val1;
		if (num_elements == 1)
		   os << " )";
		}

  ///
  void write(PackBuffer& os) const
		{os << val1;}

  ///
  void read(std::istream& /*is*/) 
		{;}

  ///
  void read(UnPackBuffer& is)
		{is >> val1;}

protected:

  ///
  Tuple1() {num_elements=0;}

  ///
  unsigned int num_elements;
};


/**
 * Implements a 2-tuple which provides a fixed length list with
 * syntactic differentiation of the elements.
 */
template <class T1, class T2>
class Tuple2 : public Tuple1<T1>
{
public:

  friend class BasicArray<Tuple2<T1,T2> >;
  #if !defined(SOLARIS_CC)
  typedef Tuple1<T1> base_t;
  using base_t::num_elements;
  #endif

  ///
  explicit 
  Tuple2(T1 _val1, T2 _val2) 
		: Tuple1<T1>(_val1), val2(_val2) {num_elements=2;}

  ///
  virtual ~Tuple2() {}

  ///
  bool operator==(const Tuple2<T1,T2>& tuple) const
		{return (Tuple1<T1>::operator==(tuple) && 
			(tuple.val2 == val2));}

  ///
  bool operator<(const Tuple2<T1,T2>& tuple) const
		{return (Tuple1<T1>::operator<(tuple) || 
			 (Tuple1<T1>::operator==(tuple) && 
			  (val2 < tuple.val2)));}

  ///
  T2 val2;

  ///
  void write(std::ostream& os) const
		{
		Tuple1<T1>::write(os);
		os << ", " << val2;
		if (num_elements == 2)
		   os << " )";
		}

  ///
  void write(PackBuffer& os) const
		{Tuple1<T1>::write(os);os << val2; }

  ///
  void read(std::istream& /*is*/) 
		{;}

  ///
  void read(UnPackBuffer& is)
		{is >> val2; Tuple1<T1>::read(is);}

protected:

  ///
  Tuple2() {num_elements=0;}
};


/**
 * Implements a 3-tuple which provides a fixed length list with
 * syntactic differentiation of the elements.
 */
template <class T1, class T2, class T3>
class Tuple3 : public Tuple2<T1,T2>
{
public:

  friend class BasicArray<Tuple3<T1,T2,T3> >;
  #if !defined(SOLARIS_CC)
  typedef Tuple2<T1,T2> base_t;
  using base_t::num_elements;
  #endif

  ///
  explicit 
  Tuple3(T1 _val1, T2 _val2, T3 _val3) 
		: Tuple2<T1,T2>(_val1,_val2), val3(_val3) {num_elements=3;}

  ///
  virtual ~Tuple3() {}

  ///
  bool operator==(const Tuple3<T1,T2,T3>& tuple) const
		{return (Tuple2<T1,T2>::operator==(tuple) && 
			(tuple.val3 == val3));}

  ///
  bool operator<(const Tuple3<T1,T2,T3>& tuple) const
		{return (Tuple2<T1,T2>::operator<(tuple) || 
			 (Tuple2<T1,T2>::operator==(tuple) && 
		 	  (val3 < tuple.val3))
			);}

  ///
  T3 val3;

  ///
  void write(std::ostream& os) const
		{
		Tuple2<T1,T2>::write(os);
		os << ", " << val3;
		if (num_elements == 3)
		   os << " )";
		}

  ///
  void write(PackBuffer& os) const
		{Tuple2<T1,T2>::write(os);os << val3; }

  ///
  void read(std::istream& /*is*/) 
		{;}

  ///
  void read(UnPackBuffer& is)
		{is >> val3; Tuple2<T1,T2>::read(is);}

protected:

  ///
  Tuple3() {num_elements=0;}
};


/**
 * Implements a 4-tuple which provides a fixed length list with
 * syntactic differentiation of the elements.
 */
template <class T1, class T2, class T3, class T4>
class Tuple4 : public Tuple3<T1,T2,T3>
{
public:

  friend class BasicArray<Tuple4<T1,T2,T3,T4> >;
  #if !defined(SOLARIS_CC)
  typedef Tuple3<T1,T2,T3> base_t;
  using base_t::num_elements;
  #endif

  ///
  explicit 
  Tuple4(T1 _val1, T2 _val2, T3 _val3, T4 _val4) 
		: Tuple3<T1,T2,T3>(_val1,_val2,_val3), val4(_val4) {num_elements=4;}

  ///
  virtual ~Tuple4() {}

  ///
  bool operator==(const Tuple4<T1,T2,T3,T4>& tuple) const
		{return (Tuple3<T1,T2,T3>::operator==(tuple) && 
			(tuple.val4 == val4));}

  ///
  bool operator<(const Tuple4<T1,T2,T3,T4>& tuple) const
		{return (Tuple3<T1,T2,T3>::operator<(tuple) ||
		         (Tuple3<T1,T2,T3>::operator==(tuple) && 
			  (val4 < tuple.val4)));}

  ///
  T4 val4;

  ///
  void write(std::ostream& os) const
		{
		Tuple3<T1,T2,T3>::write(os);
		os << ", " << val4;
		if (num_elements == 4)
		   os << " )";
		}

  ///
  void write(PackBuffer& os) const
		{Tuple3<T1,T2,T3>::write(os);os << val4; }

  ///
  void read(std::istream& /*is*/) 
		{;}

  ///
  void read(UnPackBuffer& is)
		{is >> val4; Tuple3<T1,T2,T3>::read(is);}

protected:

  ///
  Tuple4() {num_elements=0;}
};


/**
 * Implements a 5-tuple which provides a fixed length list with
 * syntactic differentiation of the elements.
 */
template <class T1, class T2, class T3, class T4, class T5>
class Tuple5 : public Tuple4<T1,T2,T3,T4>
{
public:

  friend class BasicArray<Tuple5<T1,T2,T3,T4,T5> >;
  #if !defined(SOLARIS_CC)
  typedef Tuple4<T1,T2,T3,T4> base_t;
  using base_t::num_elements;
  #endif

  ///
  explicit 
  Tuple5(T1 _val1, T2 _val2, T3 _val3, T4 _val4, T5 _val5) 
		: Tuple4<T1,T2,T3,T4>(_val1,_val2,_val3,_val4), val5(_val5) {num_elements=5;}

  ///
  virtual ~Tuple5() {}

  ///
  bool operator==(const Tuple5<T1,T2,T3,T4,T5>& tuple) const
		{return (Tuple4<T1,T2,T3,T4>::operator==(tuple) && 
			(tuple.val5 == val5));}

  ///
  bool operator<(const Tuple5<T1,T2,T3,T4,T5>& tuple) const
		{return (Tuple4<T1,T2,T3,T4>::operator<(tuple) ||
		         (Tuple4<T1,T2,T3,T4>::operator==(tuple) &&
			  (val5 < tuple.val5)));}

  ///
  T5 val5;

  ///
  void write(std::ostream& os) const
		{
		Tuple4<T1,T2,T3,T4>::write(os);
		os << ", " << val5;
		if (num_elements == 5)
		   os << " )";
		}

  ///
  void write(PackBuffer& os) const
		{Tuple4<T1,T2,T3,T4>::write(os);os << val5; }

  ///
  void read(std::istream& /*is*/) 
		{;}

  ///
  void read(UnPackBuffer& is)
		{is >> val5; Tuple4<T1,T2,T3,T4>::read(is);}

protected:

  ///
  Tuple5() {num_elements=0;}
};


/**
 * Implements a 6-tuple which provides a fixed length list with
 * syntactic differentiation of the elements.
 */
template <class T1, class T2, class T3, class T4, class T5, class T6>
class Tuple6 : public Tuple5<T1,T2,T3,T4,T5>
{
public:

  friend class BasicArray<Tuple6<T1,T2,T3,T4,T5,T6> >;
  #if !defined(SOLARIS_CC)
  typedef Tuple5<T1,T2,T3,T4,T5> base_t;
  using base_t::num_elements;
  #endif

  ///
  explicit 
  Tuple6(T1 _val1, T2 _val2, T3 _val3, T4 _val4, T5 _val5, T6 _val6) 
		: Tuple5<T1,T2,T3,T4,T5>(_val1,_val2,_val3,_val4,_val5),
		  val6(_val6) {num_elements=6;}

  ///
  virtual ~Tuple6() {}

  ///
  bool operator==(const Tuple6<T1,T2,T3,T4,T5,T6>& tuple) const
		{return (Tuple5<T1,T2,T3,T4,T5>::operator==(tuple) && 
			(tuple.val6 == val6));}

  ///
  bool operator<(const Tuple6<T1,T2,T3,T4,T5,T6>& tuple) const
		{return (Tuple5<T1,T2,T3,T4,T5>::operator<(tuple) ||
			 (Tuple5<T1,T2,T3,T4,T5>::operator==(tuple) &&
			  (val6 < tuple.val6)));}

  ///
  T6 val6;

  ///
  void write(std::ostream& os) const
		{
		Tuple5<T1,T2,T3,T4,T5>::write(os);
		os << ", " << val6;
		if (num_elements == 6)
		   os << " )";
		}

  ///
  void write(PackBuffer& os) const
		{Tuple5<T1,T2,T3,T4,T5>::write(os);os << val6; }

  ///
  void read(std::istream& /*is*/) 
		{;}

  ///
  void read(UnPackBuffer& is)
		{is >> val6; Tuple5<T1,T2,T3,T4,T5>::read(is);}

protected:

  ///
  Tuple6() {num_elements=0;}
};


/**
 * Implements a 7-tuple which provides a fixed length list with
 * syntactic differentiation of the elements.
 */
template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
class Tuple7 : public Tuple6<T1,T2,T3,T4,T5,T6>
{
public:

  friend class BasicArray<Tuple7<T1,T2,T3,T4,T5,T6,T7> >;
  #if !defined(SOLARIS_CC)
  typedef Tuple6<T1,T2,T3,T4,T5,T6> base_t;
  using base_t::num_elements;
  #endif

  ///
  explicit 
  Tuple7(T1 _val1, T2 _val2, T3 _val3, T4 _val4, T5 _val5, T6 _val6,
				T7 _val7) 
		: Tuple6<T1,T2,T3,T4,T5,T6>(_val1,_val2,_val3,_val4,_val5,_val6),
		  val7(_val7) {num_elements=7;}

  ///
  virtual ~Tuple7() {}

  ///
  bool operator==(const Tuple7<T1,T2,T3,T4,T5,T6,T7>& tuple) const
		{return (Tuple6<T1,T2,T3,T4,T5,T6>::operator==(tuple) && 
			(tuple.val7 == val7));}

  ///
  bool operator<(const Tuple7<T1,T2,T3,T4,T5,T6,T7>& tuple) const
		{return (Tuple6<T1,T2,T3,T4,T5,T6>::operator<<(tuple) ||
			 (Tuple6<T1,T2,T3,T4,T5,T6>::operator==(tuple) && 
			  (val7 < tuple.val7)));}

  ///
  T7 val7;

  ///
  void write(std::ostream& os) const
		{
		Tuple6<T1,T2,T3,T4,T5,T6>::write(os);
		os << ", " << val7;
		if (num_elements == 7)
		   os << " )";
		}

  ///
  void write(PackBuffer& os) const
		{Tuple6<T1,T2,T3,T4,T5,T6>::write(os);os << val7; }

  ///
  void read(std::istream& /*is*/) 
		{;}

  ///
  void read(UnPackBuffer& is)
		{is >> val7; Tuple6<T1,T2,T3,T4,T5,T6>::read(is);}

protected:

  ///
  Tuple7() {num_elements=0;}
};

} // namespace utilib

#endif
