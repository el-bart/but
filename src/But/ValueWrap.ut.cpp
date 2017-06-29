#include <string>
#include <memory>
#include <vector>
#include "gtest/gtest.h"
#include "ValueWrap.hpp"

namespace
{

struct ButValueWrap: public testing::Test
{ };


BUT_VALUE_WRAP(MyInt, int);

TEST_F(ButValueWrap, Integer)
{
  MyInt mi{42};
  EXPECT_EQ( 42, mi.get() );
}


BUT_VALUE_WRAP(MyStr, std::string);

TEST_F(ButValueWrap, String)
{
  MyStr str{"bar"};
  EXPECT_EQ( "bar", str.get() );
}


TEST_F(ButValueWrap, ConstCorrectness)
{
  const MyStr str{"foo"};
  EXPECT_EQ( "foo", str.get() );
}


TEST_F(ButValueWrap, MovingAndCopying)
{
  EXPECT_TRUE( std::is_copy_constructible<MyStr>::value );
  EXPECT_TRUE( std::is_copy_assignable<MyStr>::value );

  EXPECT_TRUE( std::is_move_constructible<MyStr>::value );
  EXPECT_TRUE( std::is_move_assignable<MyStr>::value );
}


BUT_VALUE_WRAP(MyPtr, std::unique_ptr<int>);

TEST_F(ButValueWrap, RvalueGetOperator)
{
  MyPtr ptr{ std::make_unique<int>(42) };
  std::unique_ptr<int> out{ std::move(ptr).get() };
  ASSERT_TRUE(out);
  EXPECT_EQ(42, *out);
}


TEST_F(ButValueWrap, MoveOnlyObject)
{
  EXPECT_FALSE( std::is_copy_constructible<MyPtr>::value );
  EXPECT_FALSE( std::is_copy_assignable<MyPtr>::value );

  EXPECT_TRUE( std::is_move_constructible<MyPtr>::value );
  EXPECT_TRUE( std::is_move_assignable<MyPtr>::value );
}


TEST_F(ButValueWrap, ForwardingArgumentsToCtor)
{
  const MyStr str(3, 'X');
  EXPECT_EQ( "XXX", str.get() );
}


BUT_VALUE_WRAP(MyVec, std::vector<double>);

TEST_F(ButValueWrap, InitializerListWorks)
{
  const MyVec vec{3.14, 2.72, 42.0};
  EXPECT_EQ( 3u, vec.get().size() );
}

}
