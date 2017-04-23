#include "gtest/gtest.h"
#include "parse.hpp"

using But::Mpl::parseSigned;
using But::Mpl::parseUnsigned;
using But::Mpl::ParseError;

namespace
{

struct ButMplParse: public testing::Test
{ };


TEST_F(ButMplParse, Unsigned)
{
  EXPECT_EQ( 0u, parseUnsigned<unsigned>("0") );
  EXPECT_EQ( 1u, parseUnsigned<unsigned>("1") );
  EXPECT_EQ( 42u, parseUnsigned<unsigned>("42") );
  EXPECT_EQ( 42u, parseUnsigned<unsigned>("0042") );
}


TEST_F(ButMplParse, InvalidUnsignedNumbers)
{
  EXPECT_THROW( parseUnsigned<unsigned>("5l"), ParseError );
  EXPECT_THROW( parseUnsigned<unsigned>("-1"), ParseError );
  EXPECT_THROW( parseUnsigned<unsigned>("1.11"), ParseError );
}


TEST_F(ButMplParse, ConstexprUnsigned)
{
  constexpr auto n = parseUnsigned<unsigned>("42");
  EXPECT_EQ( 42u, n );
}


TEST_F(ButMplParse, Signed)
{
  EXPECT_EQ( 0, parseSigned<int>("0") );
  EXPECT_EQ( 1, parseSigned<int>("1") );
  EXPECT_EQ( 42, parseSigned<int>("42") );
  EXPECT_EQ( 42, parseSigned<int>("0042") );

  EXPECT_EQ( 0, parseSigned<int>("-0") );
  EXPECT_EQ( -1, parseSigned<int>("-1") );
  EXPECT_EQ( -42, parseSigned<int>("-42") );
  EXPECT_EQ( -42, parseSigned<int>("-0042") );

  EXPECT_EQ( 0, parseSigned<int>("+0") );
  EXPECT_EQ( 1, parseSigned<int>("+1") );
  EXPECT_EQ( 42, parseSigned<int>("+42") );
  EXPECT_EQ( 42, parseSigned<int>("+0042") );
}


TEST_F(ButMplParse, InvalidSignedNumbers)
{
  EXPECT_THROW( parseSigned<int>("5l"), ParseError );
  EXPECT_THROW( parseSigned<int>("x1"), ParseError );
  EXPECT_THROW( parseSigned<int>("1.11"), ParseError );
}


TEST_F(ButMplParse, ConstexprSigned)
{
  {
    constexpr auto n = parseSigned<int>("42");
    EXPECT_EQ( 42, n );
  }
  {
    constexpr auto n = parseSigned<int>("-42");
    EXPECT_EQ( -42, n );
  }
  {
    constexpr auto n = parseSigned<int>("+42");
    EXPECT_EQ( 42, n );
  }
}

}
