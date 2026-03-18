
#include <cxxtest/TestSuite.h>
#include <cxxtest/ValueTraits.h>
#include <cxxtest/StdValueTraits.h>
#include <utilib/Any.h>
#include <iostream>

class TestAny : public CxxTest::TestSuite
{
public:


   /// empty any
   void test_default_constructor()
   {
      utilib::Any a;
      TS_ASSERT(a.empty());
      TS_ASSERT(!a.is_reference());
      TS_ASSERT(!a.is_immutable());
   }
};
