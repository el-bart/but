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

}
