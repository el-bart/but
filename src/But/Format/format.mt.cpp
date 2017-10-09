#include "gtest/gtest.h"
#include "format.hpp"

using But::Format::Invalid;

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


TEST_F(ButFormat, OneArgument)
{
  constexpr auto fmt = BUT_FORMAT("try $0 out");
  EXPECT_EQ( 1u, fmt.expectedArguments() );
  EXPECT_EQ( "try it out", fmt.format("it") );
}


TEST_F(ButFormat, MultipleArguments)
{
  constexpr auto fmt = BUT_FORMAT("$1 $0");
  EXPECT_EQ( 2u, fmt.expectedArguments() );
  EXPECT_EQ( "bar foo", fmt.format("foo", "bar") );
  EXPECT_EQ( "answer 42", fmt.format(42, "answer") );
}


TEST_F(ButFormat, RepeatedArguments)
{
  constexpr auto fmt = BUT_FORMAT("$0 $1 $0");
  EXPECT_EQ( 2u, fmt.expectedArguments() );
  EXPECT_EQ( "1 2 1", fmt.format(1,2) );
}


TEST_F(ButFormat, MultipleArgumentsMultipleStyles)
{
  {
    constexpr auto fmt = BUT_FORMAT("!$0!")
    EXPECT_EQ( 1u, fmt.expectedArguments() );
    EXPECT_EQ( "!ok!", fmt.format("ok") );
  }
  {
    constexpr auto fmt = BUT_FORMAT("!${0}!")
    EXPECT_EQ( 1u, fmt.expectedArguments() );
    EXPECT_EQ( "!ok!", fmt.format("ok") );
  }
  {
    constexpr auto fmt = BUT_FORMAT("!${0#info}!");
    EXPECT_EQ( 1u, fmt.expectedArguments() );
    EXPECT_EQ( "!ok!", fmt.format("ok") );
  }
}


TEST_F(ButFormat, DenseFormats)
{
  {
    constexpr auto fmt = BUT_FORMAT("$$$0");
    EXPECT_EQ( 1u, fmt.expectedArguments() );
    EXPECT_EQ( "$kszy", fmt.format("kszy") );
  }
  {
    constexpr auto fmt = BUT_FORMAT("$$x");
    EXPECT_EQ( 0u, fmt.expectedArguments() );
    EXPECT_EQ( "$x", fmt.format() );
  }
  {
    constexpr auto fmt = BUT_FORMAT("$0 $0");
    EXPECT_EQ( 1u, fmt.expectedArguments() );
    EXPECT_EQ( "1 1", fmt.format(1) );
  }
  {
    constexpr auto fmt = BUT_FORMAT("$0 $0 $0");
    EXPECT_EQ( 1u, fmt.expectedArguments() );
    EXPECT_EQ( "1 1 1", fmt.format(1) );
  }
  {
    constexpr auto fmt = BUT_FORMAT("$0 $0 $0 $0");
    EXPECT_EQ( 1u, fmt.expectedArguments() );
    EXPECT_EQ( "1 1 1 1", fmt.format(1) );
  }
  {
    constexpr auto fmt = BUT_FORMAT("$0 $0 $0 $0 $0");
    EXPECT_EQ( 1u, fmt.expectedArguments() );
    EXPECT_EQ( "1 1 1 1 1", fmt.format(1) );
  }
  {
    constexpr auto fmt = BUT_FORMAT("$0 $0 $0 $0 $0 $0");
    EXPECT_EQ( 1u, fmt.expectedArguments() );
    EXPECT_EQ( "1 1 1 1 1 1", fmt.format(1) );
  }
  {
    constexpr auto fmt = BUT_FORMAT("$0 $0 $0 $0 $0 $0 $0");
    EXPECT_EQ( 1u, fmt.expectedArguments() );
    EXPECT_EQ( "1 1 1 1 1 1 1", fmt.format(1) );
  }
  {
    constexpr auto fmt = BUT_FORMAT("$0 $0 $0 $0 $0 $0 $0 $0");
    EXPECT_EQ( 1u, fmt.expectedArguments() );
    EXPECT_EQ( "1 1 1 1 1 1 1 1", fmt.format(1) );
  }
}


TEST_F(ButFormat, GettingInputFormat)
{
  constexpr auto fmt = BUT_FORMAT("foo/bar");
  EXPECT_EQ( std::string{"foo/bar"}, fmt.inputFormat() );
}


TEST_F(ButFormat, RuntimeFormat)
{
  char rtFmt[] = "$0 has $1";
  const auto fmt = BUT_FORMAT_RUNTIME(rtFmt);
  EXPECT_EQ( "alice has a cat", fmt.format("alice", "a cat") );
}


/*
// NOTE: these tests are now actually forced to take place at compile-time
TEST_F(ButFormat, UnusedArgumentsDetected)
{
  EXPECT_THROW( ( BUT_FORMAT("$1") ), Invalid ) << "all previous indexes must be used";
  EXPECT_THROW( ( BUT_FORMAT("$2 $0") ), Invalid ) << "all previous indexes must be used";
}
*/

}
