#include "gtest/gtest.h"
#include "allArgumentsUsed.hpp"
#include "parse.hpp"

using But::Format::Invalid;
using But::Format::detail::parse;
using But::Format::detail::allArgumentsUsed;

namespace
{

struct ButFormatDetailAllArgumentsUsed: public testing::Test
{ };


TEST_F(ButFormatDetailAllArgumentsUsed, ConsecutiveOrder)
{
  {
    constexpr auto ps = parse<10>("foo $0 bar");
    constexpr auto ret = allArgumentsUsed(ps);
    EXPECT_TRUE(ret);
  }

  {
    constexpr auto ps = parse<10>("foo $0 bar $1");
    constexpr auto ret = allArgumentsUsed(ps);
    EXPECT_TRUE(ret);
  }

  {
    constexpr auto ps = parse<10>("foo $0 bar $1 nard $2");
    constexpr auto ret = allArgumentsUsed(ps);
    EXPECT_TRUE(ret);
  }
}


TEST_F(ButFormatDetailAllArgumentsUsed, Unordered)
{
  {
    constexpr auto ps = parse<10>("foo $2 and $0 bar $1");
    constexpr auto ret = allArgumentsUsed(ps);
    EXPECT_TRUE(ret);
  }

  {
    constexpr auto ps = parse<10>("foo $1 bar $0");
    constexpr auto ret = allArgumentsUsed(ps);
    EXPECT_TRUE(ret);
  }

  {
    constexpr auto ps = parse<10>("foo $2 and bar $1");
    constexpr auto ret = allArgumentsUsed(ps);
    EXPECT_FALSE(ret);
  }

  {
    constexpr auto ps = parse<10>("foo $2 and bar $0");
    constexpr auto ret = allArgumentsUsed(ps);
    EXPECT_FALSE(ret);
  }
}


TEST_F(ButFormatDetailAllArgumentsUsed, RepeatedArguments)
{
  {
    constexpr auto ps = parse<10>("foo $1 bar $0 and $1");
    constexpr auto ret = allArgumentsUsed(ps);
    EXPECT_TRUE(ret);
  }

  {
    constexpr auto ps = parse<10>("foo $2 bar $0 and $2");
    constexpr auto ret = allArgumentsUsed(ps);
    EXPECT_FALSE(ret);
  }
}


TEST_F(ButFormatDetailAllArgumentsUsed, BothValuesAndTypesCount)
{
  {
    constexpr auto ps = parse<10>("foo ${T1} bar ${V0}");
    constexpr auto ret = allArgumentsUsed(ps);
    EXPECT_TRUE(ret);
  }

  {
    constexpr auto ps = parse<10>("foo ${T1} bar ${V2}");
    constexpr auto ret = allArgumentsUsed(ps);
    EXPECT_FALSE(ret);
  }
}


TEST_F(ButFormatDetailAllArgumentsUsed, IndexOutOfRange)
{
  constexpr auto ps = parse<10>("foo $42");
  constexpr auto ret = allArgumentsUsed(ps);
  EXPECT_FALSE(ret);
}

}
