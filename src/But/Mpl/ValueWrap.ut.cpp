#include <string>
#include <memory>
#include <vector>
#include <gtest/gtest.h>
#include <But/Mpl/ValueWrap.hpp>

namespace
{

struct ButMplValueWrap: public testing::Test
{ };


BUT_MPL_VALUE_WRAP(MyInt, int);

TEST_F(ButMplValueWrap, Integer)
{
  MyInt mi{42};
  EXPECT_EQ( 42, mi.get() );
}


BUT_MPL_VALUE_WRAP(MyStr, std::string);

TEST_F(ButMplValueWrap, String)
{
  MyStr str{"bar"};
  EXPECT_EQ( "bar", str.get() );
}


TEST_F(ButMplValueWrap, ConstCorrectness)
{
  const MyStr str{"foo"};
  EXPECT_EQ( "foo", str.get() );
}


TEST_F(ButMplValueWrap, MovingAndCopying)
{
  EXPECT_TRUE( std::is_copy_constructible<MyStr>::value );
  EXPECT_TRUE( std::is_copy_assignable<MyStr>::value );

  EXPECT_TRUE( std::is_move_constructible<MyStr>::value );
  EXPECT_TRUE( std::is_move_assignable<MyStr>::value );
}


BUT_MPL_VALUE_WRAP(MyPtr, std::unique_ptr<int>);

TEST_F(ButMplValueWrap, RvalueGetOperator)
{
  MyPtr ptr{ std::make_unique<int>(42) };
  std::unique_ptr<int> out{ std::move(ptr).get() };
  ASSERT_TRUE(out);
  EXPECT_EQ(42, *out);
}


TEST_F(ButMplValueWrap, MoveOnlyObject)
{
  EXPECT_FALSE( std::is_copy_constructible<MyPtr>::value );
  EXPECT_FALSE( std::is_copy_assignable<MyPtr>::value );

  EXPECT_TRUE( std::is_move_constructible<MyPtr>::value );
  EXPECT_TRUE( std::is_move_assignable<MyPtr>::value );
}


TEST_F(ButMplValueWrap, ForwardingArgumentsToCtor)
{
  const MyStr str(3, 'X');
  EXPECT_EQ( "XXX", str.get() );
}


BUT_MPL_VALUE_WRAP(MyVec, std::vector<double>);

TEST_F(ButMplValueWrap, InitializerListWorks)
{
  const MyVec vec{3.14, 2.72, 42.0};
  EXPECT_EQ( 3u, vec.get().size() );
}


BUT_MPL_VALUE_WRAP(MyNum, unsigned);

TEST_F(ButMplValueWrap, ComparisonOperatorsAreDefined)
{
  const MyNum i1{1};
  const MyNum i2{2};
  EXPECT_TRUE( i1 == i1 );
  EXPECT_TRUE( i1 != i2 );
  EXPECT_TRUE( i1 <  i2 );
  EXPECT_TRUE( i1 <= i2 );
  EXPECT_TRUE( i2 >  i1 );
  EXPECT_TRUE( i2 >= i1 );
}

}
