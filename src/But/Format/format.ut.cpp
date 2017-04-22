#include "gtest/gtest.h"
#include "format.hpp"

using But::format;

namespace
{

struct ButFormat: public testing::Test
{ };


TEST_F(ButFormat, SampleString)
{
  EXPECT_EQ( "", format("") );
  EXPECT_EQ( "kszy", format("kszy") );
  EXPECT_EQ( "foo bar", format("foo bar") );
}


TEST_F(ButFormat, MacroSampleStrings)
{
  EXPECT_EQ( "", BUT_FORMAT("") );
  EXPECT_EQ( "kszy", BUT_FORMAT("kszy") );
  EXPECT_EQ( "foo bar", BUT_FORMAT("foo bar") );
}


TEST_F(ButFormat, MacroOneArgument)
{
  EXPECT_EQ( "foo!=bar", BUT_FORMAT("foo!=$1", "bar") );
}


TEST_F(ButFormat, MacroMultipleArguments)
{
  // TODO
}


TEST_F(ButFormat, MacroMultipleArgumentsMultipleStyles)
{
  // TODO
}

}
