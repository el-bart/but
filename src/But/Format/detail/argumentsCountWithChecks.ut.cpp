#include "gtest/gtest.h"
#include "argumentsCountWithChecks.hpp"

using But::Format::Invalid;

namespace
{

constexpr auto argumentsCountWithChecks(char const* fmt) // helper for a more compact syntax
{
  return But::Format::detail::argumentsCountWithChecks<100>(fmt);
}

struct ButFormatDetailArgumentsCountWithChecks: public testing::Test
{ };


TEST_F(ButFormatDetailArgumentsCountWithChecks, SimpleStrings)
{
  {
    constexpr auto cnt = argumentsCountWithChecks("");
    EXPECT_EQ( 0u, cnt );
  }
  {
    constexpr auto cnt = argumentsCountWithChecks("test");
    EXPECT_EQ( 0u, cnt );
  }
  {
    constexpr auto cnt = argumentsCountWithChecks("teST STRIng");
    EXPECT_EQ( 0u, cnt );
  }
  {
    constexpr auto cnt = argumentsCountWithChecks("!@#%^%^*(()");
    EXPECT_EQ( 0u, cnt );
  }
  {
    constexpr auto cnt = argumentsCountWithChecks("test $$ data");
    EXPECT_EQ( 0u, cnt );
  }
}


TEST_F(ButFormatDetailArgumentsCountWithChecks, OneArgument)
{
  {
    constexpr auto cnt = argumentsCountWithChecks("$0");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCountWithChecks("test $0 data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCountWithChecks("test${0}data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCountWithChecks("test ${0#ignored text} data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCountWithChecks("test ${V0} data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCountWithChecks("test ${V0#ignored text} data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCountWithChecks("test ${T0} data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCountWithChecks("test ${T0#ignored text} data");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCountWithChecks("test $0\ndata");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCountWithChecks("test $0\rdata");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCountWithChecks("test $0\tdata");
    EXPECT_EQ( 1u, cnt );
  }
  {
    constexpr auto cnt = argumentsCountWithChecks("test $0 data");
    EXPECT_EQ( 1u, cnt );
  }
}


TEST_F(ButFormatDetailArgumentsCountWithChecks, ExceptionIsThrownOnInvalidFormat)
{
  EXPECT_THROW( argumentsCountWithChecks("$ 0"), Invalid ) << "missing number";
  EXPECT_THROW( argumentsCountWithChecks("$0oops"), Invalid ) << "invalid number";
  EXPECT_THROW( argumentsCountWithChecks("${0"), Invalid ) << "missing closing brace";
  EXPECT_THROW( argumentsCountWithChecks("${0 x"), Invalid ) << "missing closing brace";
  EXPECT_THROW( argumentsCountWithChecks("${0# x"), Invalid ) << "missing closing brace";
  EXPECT_THROW( argumentsCountWithChecks("test$"), Invalid ) << "variable declaration too short";
  EXPECT_THROW( argumentsCountWithChecks("test $0}"), Invalid ) << "invalid number";
}


TEST_F(ButFormatDetailArgumentsCountWithChecks, MultipleArguments)
{
  constexpr auto cnt = argumentsCountWithChecks("test $0 data $2\t$1\n");
  EXPECT_EQ( 3u, cnt );
}


TEST_F(ButFormatDetailArgumentsCountWithChecks, MultipleArgumentsMultipleStyles)
{
  constexpr auto cnt = argumentsCountWithChecks("test $0 data ${2} ${V1#xx} ${T3}");
  EXPECT_EQ( 4u, cnt );
}


TEST_F(ButFormatDetailArgumentsCountWithChecks, RepeatedArguments)
{
  constexpr auto cnt = argumentsCountWithChecks("test $0 data $0\t$1\n");
  EXPECT_EQ( 2u, cnt );
}


TEST_F(ButFormatDetailArgumentsCountWithChecks, NonContinuousArguments)
{
  EXPECT_THROW( argumentsCountWithChecks("test $42 data $1"), Invalid );
}


TEST_F(ButFormatDetailArgumentsCountWithChecks, RuntimeArgumentsCheck)
{
  {
    const auto cnt = argumentsCountWithChecks("test $0 data $2\t$1\n");
    EXPECT_EQ( 3u, cnt );
  }
  {
    const auto cnt = argumentsCountWithChecks("test $0 data ${2} ${V1#xx} ${T3}");
    EXPECT_EQ( 4u, cnt );
  }
  {
    const auto cnt = argumentsCountWithChecks("test $0 data $0\t$1\n");
    EXPECT_EQ( 2u, cnt );
  }
  EXPECT_THROW( argumentsCountWithChecks("test $42 data $1"), Invalid );
}

}
