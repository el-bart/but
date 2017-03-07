#include "gtest/gtest.h"
#include "Format.hpp"

using But::Log::Format;

namespace
{

struct ButLogFormat: public testing::Test
{ };


TEST_F(ButLogFormat, DefaultCtorGeneratesEmptyString)
{
  const Format fmt{};
  EXPECT_EQ( fmt.format(), "" );
}


TEST_F(ButLogFormat, EmptyFormatIsFine)
{
  const Format fmt{""};
  EXPECT_EQ( fmt.format(), "" );
}


TEST_F(ButLogFormat, StringOnlyFormatIsAccepted)
{
  EXPECT_EQ( Format{"alice has a cat"}.format(), "alice has a cat" );
  EXPECT_EQ( Format{"!@#$"}.format(), "!@#$" );
}


TEST_F(ButLogFormat, Parameters)
{
  EXPECT_EQ( Format{"{1} has a {2}"}.format(), "{1} has a {2}" );
  EXPECT_EQ( Format{"!@#$"}.format(), "!@#$" );
}


TEST_F(ButLogFormat, InvalidFormatSpecifiers)
{
  EXPECT_THROW( Format{"missing closing brace {1 here"}, Format::Invalid );
  // TODO
}


TEST_F(ButLogFormat, EscapeSequences)
{
  // TODO
}

}
