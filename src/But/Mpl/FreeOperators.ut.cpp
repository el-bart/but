#include <initializer_list>
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


struct MyCollection
{
  explicit MyCollection(std::initializer_list<std::string> lst): c_{lst} { }

  auto begin() const { return c_.begin(); }
  auto end() const { return c_.end(); }

  std::vector<std::string> c_;
};

BUT_MPL_FREE_OPERATORS_COLLECTION_COMPARE(MyCollection)


TEST_F(ButMplFreeOperators, OperatorsForCollection)
{
  const MyCollection mc1{ "ala",        "ma", "kota" };
  const MyCollection mc2{ "ala", "nie", "ma", "kota" };
  const MyCollection mc3{ "ala", "nie", "ma" };

  EXPECT_TRUE( mc3 < mc2 );

  EXPECT_TRUE( mc1 <= mc1 );
  EXPECT_TRUE( mc3 <= mc2 );

  EXPECT_TRUE( mc1 >= mc1 );
  EXPECT_TRUE( mc2 >= mc3 );

  EXPECT_TRUE( mc1 == mc1 );
  EXPECT_TRUE( mc1 != mc2 );

  EXPECT_FALSE( mc3 >  mc2 );

  EXPECT_FALSE( mc3 >= mc2 );

  EXPECT_FALSE( mc2 <= mc3 );

  EXPECT_FALSE( mc1 == mc2 );
  EXPECT_FALSE( mc1 != mc1 );
}

}
