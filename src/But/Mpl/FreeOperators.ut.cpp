#include "gtest/gtest.h"
#include "FreeOperators.hpp"

namespace
{

struct MyData
{
  int value_;
};

BUT_MPL_FREE_OPERATORS_COMPARE(MyData, .value_)

struct ButMplFreeOperators: public testing::Test
{
  const MyData d1_{13};
  const MyData d2_{42};
};


TEST_F(ButMplFreeOperators, AllOperatorsAreDefinedCorrectly)
{
  EXPECT_TRUE( d1_ <  d2_ );

  EXPECT_TRUE( d1_ <= d2_ );
  EXPECT_TRUE( d1_ <= d1_ );

  EXPECT_TRUE( d1_ >= d1_ );
  EXPECT_TRUE( d2_ >= d1_ );

  EXPECT_TRUE( d1_ == d1_ );
  EXPECT_TRUE( d1_ != d2_ );

  EXPECT_FALSE( d1_ >  d2_ );

  EXPECT_FALSE( d1_ >= d2_ );

  EXPECT_FALSE( d2_ <= d1_ );

  EXPECT_FALSE( d1_ == d2_ );
  EXPECT_FALSE( d1_ != d1_ );
}

}
