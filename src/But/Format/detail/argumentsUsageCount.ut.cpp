#include "gtest/gtest.h"
#include "argumentsUsageCount.hpp"

using But::Format::Invalid;

namespace
{

constexpr auto argumentsUsageCount(char const* fmt) // helper for a more compact syntax
{
  return But::Format::detail::argumentsUsageCount<100>(fmt);
}

struct ButFormatDetailArgumentsUsageCount: public testing::Test
{ };


TEST_F(ButFormatDetailArgumentsUsageCount, SimpleStrings)
{
  {
    constexpr auto cnt = argumentsUsageCount("");
    EXPECT_EQ( 0u, cnt );
  }
  {
    constexpr auto cnt = argumentsUsageCount("test");
    EXPECT_EQ( 0u, cnt );
  }
  {
    constexpr auto cnt = argumentsUsageCount("teST STRIng");
    EXPECT_EQ( 0u, cnt );
  }
  {
    constexpr auto cnt = argumentsUsageCount("!@#%^%^*(()");
    EXPECT_EQ( 0u, cnt );
  }
  {
    constexpr auto cnt = argumentsUsageCount("test $$ data");
    EXPECT_EQ( 0u, cnt );
  }
}


TEST_F(ButFormatDetailArgumentsUsageCount, OneArgument)
{
  {
    constexpr auto cnt = argumentsUsageCount("$1");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsUsageCount("test $1 data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsUsageCount("test${1}data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsUsageCount("test ${1#ignored text} data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsUsageCount("test ${V1} data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsUsageCount("test ${V1#ignored text} data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsUsageCount("test ${T1} data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsUsageCount("test ${T1#ignored text} data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsUsageCount("test $1\ndata");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsUsageCount("test $1\rdata");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsUsageCount("test $1\tdata");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsUsageCount("test $1 data");
    EXPECT_EQ( 1u, cnt );
  }
}


TEST_F(ButFormatDetailArgumentsUsageCount, ExceptionIsThrownOnInvalidFormat)
{
  EXPECT_THROW( argumentsUsageCount("$1oops"), Invalid ) << "invalid number";
  EXPECT_THROW( argumentsUsageCount("${1"), Invalid ) << "missing closing brace";
  EXPECT_THROW( argumentsUsageCount("${1 x"), Invalid ) << "missing closing brace";
  EXPECT_THROW( argumentsUsageCount("${1# x"), Invalid ) << "missing closing brace";
  EXPECT_THROW( argumentsUsageCount("test$"), Invalid ) << "variable declaration too short";
  EXPECT_THROW( argumentsUsageCount("test $1}"), Invalid ) << "invalid number";
}


TEST_F(ButFormatDetailArgumentsUsageCount, MultipleArguments)
{
  constexpr auto cnt = argumentsUsageCount("test $1 data $3\t$2\n");
  EXPECT_EQ( 3u, cnt );
}


TEST_F(ButFormatDetailArgumentsUsageCount, MultipleArgumentsMultipleStyles)
{
  constexpr auto cnt = argumentsUsageCount("test $1 data ${3} ${V2#xx} ${T4}");
  EXPECT_EQ( 4u, cnt );
}


TEST_F(ButFormatDetailArgumentsUsageCount, RepeatedArguments)
{
  constexpr auto cnt = argumentsUsageCount("test $1 data $1\t$2\n");
  EXPECT_EQ( 3u, cnt );
}


TEST_F(ButFormatDetailArgumentsUsageCount, NonContinuousArguments)
{
  constexpr auto cnt = argumentsUsageCount("test $42 data $1");
  EXPECT_EQ( 2u, cnt );
}

}
