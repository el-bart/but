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
  {
    constexpr auto cnt = argumentsCount("test $$ data");
    EXPECT_EQ( 0u, cnt );
  }
}


TEST_F(ButFormatDetailArgumentsCount, OneArgument)
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
  {
    constexpr auto cnt = argumentsCount("test ${1#ignored text} data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCount("test ${V1} data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCount("test ${V1#ignored text} data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCount("test ${T1} data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCount("test ${T1#ignored text} data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCount("test $1\ndata");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCount("test $1\rdata");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCount("test $1\tdata");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCount("test $1 data");
    EXPECT_EQ( 1u, cnt );
  }
}


TEST_F(ButFormatDetailArgumentsCount, ExceptionIsThrownOnInvalidFormat)
{
  EXPECT_THROW( argumentsCount("$1oops"), Invalid ) << "invalid number";
  EXPECT_THROW( argumentsCount("${1"), Invalid ) << "missing closing brace";
  EXPECT_THROW( argumentsCount("${1 x"), Invalid ) << "missing closing brace";
  EXPECT_THROW( argumentsCount("${1# x"), Invalid ) << "missing closing brace";
  EXPECT_THROW( argumentsCount("test$"), Invalid ) << "variable declaration too short";
  EXPECT_THROW( argumentsCount("test $1}"), Invalid ) << "invalid number";
}


TEST_F(ButFormatDetailArgumentsCount, MultipleArguments)
{
  constexpr auto cnt = argumentsCount("test $1 data $3\t$2\n");
  EXPECT_EQ( 3u, cnt );
}


TEST_F(ButFormatDetailArgumentsCount, MultipleArgumentsMultipleStyles)
{
  constexpr auto cnt = argumentsCount("test $1 data ${3} ${V2#xx} ${T4}");
  EXPECT_EQ( 4u, cnt );
}


TEST_F(ButFormatDetailArgumentsCount, RepeatedArguments)
{
  constexpr auto cnt = argumentsCount("test $1 data $1\t$2\n");
  EXPECT_EQ( 2u, cnt );
}


TEST_F(ButFormatDetailArgumentsCount, NonContinuousArguments)
{
  constexpr auto cnt = argumentsCount("test $42 data $1");
  EXPECT_EQ( 42u, cnt );
}

}
