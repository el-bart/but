#include "gtest/gtest.h"
#include "format.hpp"

namespace
{

struct ButFormat: public testing::Test
{ };


TEST_F(ButFormat, SampleStrings)
{
  {
    constexpr auto fmt = BUT_FORMAT("");
    EXPECT_EQ( 0u, fmt.expectedArguments() );
    EXPECT_EQ( "", fmt.format() );
  }
  {
    constexpr auto fmt = BUT_FORMAT("some test data");
    EXPECT_EQ( 0u, fmt.expectedArguments() );
    EXPECT_EQ( "some test data", fmt.format() );
  }
}


/*
TEST_F(ButFormat, OneArgument)
{
  constexpr auto fmt = BUT_FORMAT("try $1 out");
  EXPECT_EQ( 1u, fmt.expectedArguments() );
  EXPECT_EQ( "try it out", fmt.format("it") );
}


TEST_F(ButFormat, MultipleArguments)
{
  constexpr auto fmt = BUT_FORMAT("$2 $1");
  EXPECT_EQ( 2u, fmt.expectedArguments() );
  EXPECT_EQ( "bar foo", fmt.format("foo", "bar") );
  EXPECT_EQ( "answer 42", fmt.format(42, "answer") );
}


TEST_F(ButFormat, RepeatedArguments)
{
  {
    constexpr auto fmt = BUT_FORMAT("$1 $2 $1");
    EXPECT_EQ( 2u, fmt.expectedArguments() );
    EXPECT_EQ( "1 2 1", fmt.format(1,2) );
  }
}


TEST_F(ButFormat, MultipleArgumentsMultipleStyles)
{
  {
    constexpr auto fmt = BUT_FORMAT("${1} $1");
    EXPECT_EQ( 2u, fmt.expectedArguments() );
    EXPECT_EQ( "x x", fmt.format("x") );
  }
  {
    constexpr auto fmt = BUT_FORMAT("${V1}=${T1}");
    EXPECT_EQ( 2u, fmt.expectedArguments() );
    EXPECT_EQ( "int=42", fmt.format(42) );
  }
  {
    constexpr auto fmt = BUT_FORMAT("${1#info} ${V1#text} ${T1#foo}");
    EXPECT_EQ( 3u, fmt.expectedArguments() );
    EXPECT_EQ( "x x std::string", fmt.format("x") );
  }
}
*/

}
