#include "gtest/gtest.h"
#include "lastArgumentNumber.hpp"
#include "parse.hpp"

using But::Format::detail::parse;
using But::Format::detail::lastArgumentNumber;

namespace
{

struct ButFormatDetailLastArgumentNumber: public testing::Test
{ };


TEST_F(ButFormatDetailLastArgumentNumber, EmptySet)
{
  constexpr auto ps = parse<10>("");
  constexpr auto last= lastArgumentNumber(ps);
  EXPECT_EQ(0u, last);
}


TEST_F(ButFormatDetailLastArgumentNumber, NoArgumentsJustStrings)
{
  constexpr auto ps = parse<10>("some test $$ data");
  constexpr auto last= lastArgumentNumber(ps);
  EXPECT_EQ(0u, last);
}


TEST_F(ButFormatDetailLastArgumentNumber, OneArgument)
{
  {
    constexpr auto ps = parse<10>("some test $1 data");
    constexpr auto last= lastArgumentNumber(ps);
    EXPECT_EQ(1u, last);
  }

  {
    constexpr auto ps = parse<10>("some test $0 data");
    constexpr auto last= lastArgumentNumber(ps);
    EXPECT_EQ(0u, last);
  }

  {
    constexpr auto ps = parse<10>("some test $2 data");
    constexpr auto last= lastArgumentNumber(ps);
    EXPECT_EQ(2u, last);
  }
}


TEST_F(ButFormatDetailLastArgumentNumber, MultipleArguments)
{
  constexpr auto ps = parse<10>("some $42 test $19 data");
  constexpr auto last= lastArgumentNumber(ps);
  EXPECT_EQ(42u, last);
}

}