/**
 * \file SampleGenerators.h
 *
 * Classes that define generic wrappers for sample generators:
 * utilib::SampleGenerator and utilib::UniformSampleGenerator
 */

#ifndef utilib_SampleGenerators_h
#define utilib_SampleGenerators_h

#include <utilib/AnyRNG.h>
#include <utilib/Uniform.h>
#include <utilib/BasicArray.h>


namespace utilib {

/** 
 *  The base class for sample generator objects
 */
template <class DomainT>
class SampleGenerator
{
public:

  /// Reset the sample generator
  virtual void reset()
		{}

  /// Generate a new point
  virtual void new_point(DomainT& ) = 0;

  /// Set a random number generator
  virtual void set_rng(AnyRNG& ) = 0;

};


/** 
 * Sample generator objects that generate uniform samples
 */
template <class DomainT>
class UniformSampleGenerator
{
public:

  /// Reset the sample generator
  void reset() {}

  /// Generate a new point
  void new_point(DomainT& )
	{ EXCEPTION_MNGR(runtime_error, "UniformSampleGenerator undefined for this datatype") }

  /// Set a random number generator
  void set_rng(AnyRNG& ) {}

};


/** 
 * Sample generator objects that generate uniform samples
 */
template <>
class UniformSampleGenerator<BasicArray<double> >
{
public:

  /// Reset the sample generator
  void reset();

  /// Generate a new point
  void new_point(BasicArray<double>& pt);

  /// Set a random number generator
  void set_rng(AnyRNG& rng)
	{ urnd.generator(&rng); }

  /// The lower bounds
  BasicArray<double> l_bound;

  /// The upper bounds
  BasicArray<double> u_bound;

  /// A uniform random number generator
  Uniform urnd;
};


inline void UniformSampleGenerator<BasicArray<double> >::reset()
{
if (l_bound.size() == 0)
   EXCEPTION_MNGR(runtime_error, "UniformSampleGenerator<BasicArray<double> - No lower bounds provided")

if (u_bound.size() == 0)
   EXCEPTION_MNGR(runtime_error, "UniformSampleGenerator<BasicArray<double> - No upper bounds provided")

if (l_bound.size() != u_bound.size())
   EXCEPTION_MNGR(runtime_error, "UniformSampleGenerator<BasicArray<double> - Inconsistent lower and upper bounds: " << l_bound.size() << " " << u_bound.size())
}


inline void UniformSampleGenerator<BasicArray<double> >::new_point(BasicArray<double>& pt)
{
pt.resize(l_bound.size());

for (size_type i=0; i<l_bound.size(); i++)
  pt[i] = (u_bound[i] - l_bound[i])*urnd() + l_bound[i];
}


} // namespace utilib

#endif
