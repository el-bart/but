#include "gtest/gtest.h"
#include "FreeOperators.hpp"

namespace
{

struct ButMplFreeOperators: public testing::Test
{ };


struct MyData
{
  int value_;
};

BUT_MPL_FREE_OPERATORS_COMPARE(MyData, .value_)


TEST_F(ButMplFreeOperators, OperatorsForRegularClass)
{
  const MyData d1{13};
  const MyData d2{42};

  EXPECT_TRUE( d1 <  d2 );

  EXPECT_TRUE( d1 <= d2 );
  EXPECT_TRUE( d1 <= d1 );

  EXPECT_TRUE( d1 >= d1 );
  EXPECT_TRUE( d2 >= d1 );

  EXPECT_TRUE( d1 == d1 );
  EXPECT_TRUE( d1 != d2 );

  EXPECT_FALSE( d1 >  d2 );

  EXPECT_FALSE( d1 >= d2 );

  EXPECT_FALSE( d2 <= d1 );

  EXPECT_FALSE( d1 == d2 );
  EXPECT_FALSE( d1 != d1 );
}


template<typename T>
struct TmpData
{
  typename T::value_type t_;
};

//BUT_MPL_FREE_OPERATORS_COMPARE_SWO( TmpData, .t_.size() )


TEST_F(ButMplFreeOperators, TemplateClassIsHandled)
{
//  const MixData md1{"alice", 3};
//  const MixData md2{"alice", 4};
//  const MixData md3{"bob",   1};
}

}
