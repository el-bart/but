#include <memory>
#include <string>
#include <type_traits>
#include "gtest/gtest.h"
#include "Optional.hpp"

using But::Optional;
using But::makeOptional;

namespace
{

struct ButOptional: public testing::Test
{ };


TEST_F(ButOptional, GettingValue)
{
  Optional<std::string> opt{"test"};
  EXPECT_EQ( "test", opt.get() );
}


TEST_F(ButOptional, ConstCorrectness)
{
  const Optional<std::string> opt{"test"};
  EXPECT_EQ( "test", opt.get() );
}


TEST_F(ButOptional, DefaultConstructor)
{
  const Optional<int> opt{};
  EXPECT_FALSE(opt);
}


TEST_F(ButOptional, Copyable)
{
  Optional<std::string> o1{"one"};
  Optional<std::string> o2 = o1;
  EXPECT_EQ( "one", o1.get() );
  EXPECT_EQ( "one", o2.get() );
}


using Ptr = std::unique_ptr<std::string>;

TEST_F(ButOptional, MoveConstructible)
{
  Optional<Ptr> p1{ std::make_unique<std::string>("ok") };
  EXPECT_TRUE(p1);
  Optional<Ptr> p2{ std::move(p1) };
  EXPECT_FALSE(p1);
  ASSERT_TRUE(p2);
  EXPECT_EQ( "ok", *p2.get() );
}


TEST_F(ButOptional, MoveAssignable)
{
  Optional<Ptr> p1{ std::make_unique<std::string>("ok") };
  EXPECT_TRUE(p1);
  Optional<Ptr> p2;
  EXPECT_FALSE(p2);

  p2 = std::move(p1);
  EXPECT_FALSE(p1);
  ASSERT_TRUE(p2);
  EXPECT_EQ( "ok", *p2.get() );
}


TEST_F(ButOptional, SelfMoveAssignmentIsIgnored)
{
  Optional<Ptr> p{ std::make_unique<std::string>("ok") };
  auto& ref = p;
  p = std::move(ref);
  EXPECT_TRUE(p);
  EXPECT_EQ( "ok", *p.get() );
}


TEST_F(ButOptional, Resetable)
{
  Optional<std::string> opt{"is set"};
  ASSERT_TRUE(opt);
  EXPECT_EQ( "is set", opt.get() );
  opt.reset();
  EXPECT_FALSE(opt);
}


TEST_F(ButOptional, ExplicitlyBoolConvertible)
{
  const Optional<int> opt{42};
  EXPECT_TRUE( static_cast<bool>(opt) );
}


TEST_F(ButOptional, ArrowOperator)
{
  Optional<std::string> opt{"test"};
  EXPECT_FALSE( opt->empty() );
  opt->clear();
  EXPECT_TRUE( opt->empty() );
}


TEST_F(ButOptional, ConstArrowOperator)
{
  const Optional<std::string> opt{"test"};
  EXPECT_EQ( 4u, opt->size() );
}


TEST_F(ButOptional, HelperMakeFunction)
{
  auto opt = makeOptional<std::string>("test");
  ASSERT_TRUE(opt);
  EXPECT_EQ( "test", opt.get() );
}


TEST_F(ButOptional, ValueType)
{
  EXPECT_TRUE( (std::is_same<std::string, Optional<std::string>::value_type>::value) );
}


TEST_F(ButOptional, AsteriskOperator)
{
  auto opt = makeOptional<std::string>("data");
  *opt = "other";
  EXPECT_EQ( "other", *opt );
}


TEST_F(ButOptional, ConstAsteriskOperator)
{
  const auto opt = makeOptional<std::string>("data");
  EXPECT_EQ( "data", *opt );
}


TEST_F(ButOptional, ConstructibleFromNamedObject)
{
  const auto str = std::string{"test"};
  Optional<std::string> os{str};
  EXPECT_EQ(str, *os);
}

}
