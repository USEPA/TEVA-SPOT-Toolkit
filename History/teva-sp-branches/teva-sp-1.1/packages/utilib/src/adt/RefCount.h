/**
 * \file RefCount.h
 *
 * \depricated
 */

#ifndef utilib_RefCount_h
#define utilib_RefCount_h

namespace utilib {

/**
 * \enum EnumDataOwned
 *
 * Ownership categories for objects with reference counts.
 */
#ifndef _ENUMDATAOWNED_
#define _ENUMDATAOWNED_
enum EnumDataOwned
{
  DataNotOwned=0,       /**< Data owned by some other object */
  DataOwned=1,          /**< Memory allocated by object itself */
  AcquireOwnership=1,   /**< Synonym for DataOwned */
  AssumeOwnership=2     /**< We own it now, but it comes from elsewhere */
                        /**< Once the object has been made this is      */
                        /**< identical to DataOwned                     */
};
#endif


///
template <class Type>
class RefCount
{
public:

  ///
  RefCount(Type* type_, EnumDataOwned own_=DataNotOwned)
        {Data = type_; own = own_; nref=1;}

  ///
  ~RefCount()
        {decrement();}

  ///
  void set_data(Type* data_, EnumDataOwned own_=DataNotOwned)
        {
        if (own)
           delete Data;
        Data = data_;
        own = own_;
        }

  ///
  Type* data()
        {return Data;}

  ///
  const Type* data() const
        {return Data;}

  ///
  void increment()
        {nref++;}

  ///
  operator bool()
        {return (nref > 0);}

  ///
  bool decrement()
        {
        nref--;
        if (own && (nref == 0))
           delete Data;
        return (nref == 0);
        }

protected:

  ///
  Type* Data;

  ///
  EnumDataOwned own;

  ///
  int nref;
};


} // namespace utilib

#endif
