#include "gtest/gtest.h"
#include "argumentsCount.hpp"

using But::Format::Invalid;
using But::Format::detail::argumentsCount;

namespace
{

struct ButFormatDetailArgumentsCount: public testing::Test
{ };


TEST_F(ButFormatDetailArgumentsCount, SimpleStrings)
{
  {
    constexpr auto cnt = argumentsCount("");
    EXPECT_EQ( 0u, cnt );
  }
  {
    constexpr auto cnt = argumentsCount("test");
    EXPECT_EQ( 0u, cnt );
  }
  {
    constexpr auto cnt = argumentsCount("teST STRIng");
    EXPECT_EQ( 0u, cnt );
  }
  {
    constexpr auto cnt = argumentsCount("!@#%^%^*(()");
    EXPECT_EQ( 0u, cnt );
  }
}


TEST_F(ButFormatDetailArgumentsCount, MacroOneArgument)
{
  {
    constexpr auto cnt = argumentsCount("$1");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCount("test $1 data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCount("test${1}data");
    EXPECT_EQ( 1u, cnt );
  }
  EXPECT_THROW( argumentsCount("$1oops"), Invalid );
}


TEST_F(ButFormatDetailArgumentsCount, MacroMultipleArguments)
{
  // TODO
}


TEST_F(ButFormatDetailArgumentsCount, MacroMultipleArgumentsMultipleStyles)
{
  // TODO
}

}
