#include <gtest/gtest.h>
#include <But/Format/detail/argumentsCount.hpp>
#include <But/Format/detail/parse.hpp>

using But::Format::detail::parseCt;
using But::Format::detail::argumentsCount;

namespace
{

struct ButFormatDetailArgumentsCount: public testing::Test
{ };


TEST_F(ButFormatDetailArgumentsCount, EmptySet)
{
  constexpr auto ps = parseCt<10>("");
  constexpr auto cnt = argumentsCount(ps);
  EXPECT_EQ(0u, cnt);
}


TEST_F(ButFormatDetailArgumentsCount, NoArgumentsJustStrings)
{
  constexpr auto ps = parseCt<10>("some test $$ data");
  constexpr auto cnt = argumentsCount(ps);
  EXPECT_EQ(0u, cnt);
}


TEST_F(ButFormatDetailArgumentsCount, OneArgument)
{
  {
    constexpr auto ps = parseCt<10>("some test $1 data");
    constexpr auto cnt = argumentsCount(ps);
    EXPECT_EQ(1u, cnt);
  }

  {
    constexpr auto ps = parseCt<10>("some test $0 data");
    constexpr auto cnt = argumentsCount(ps);
    EXPECT_EQ(1u, cnt);
  }

  {
    constexpr auto ps = parseCt<10>("some test $2 data");
    constexpr auto cnt = argumentsCount(ps);
    EXPECT_EQ(1u, cnt);
  }
}


TEST_F(ButFormatDetailArgumentsCount, MultipleArguments)
{
  constexpr auto ps = parseCt<10>("some $4 test $2 data");
  constexpr auto cnt = argumentsCount(ps);
  EXPECT_EQ(2u, cnt);
}


TEST_F(ButFormatDetailArgumentsCount, Repeated)
{
  constexpr auto ps = parseCt<10>("some $2 test $2 data");
  constexpr auto cnt = argumentsCount(ps);
  EXPECT_EQ(1u, cnt);
}


TEST_F(ButFormatDetailArgumentsCount, NonConsecutiveArguments)
{
  constexpr auto ps = parseCt<10>("some $5 test $1 data");
  constexpr auto cnt = argumentsCount(ps);
  EXPECT_EQ(2u, cnt);
}

}
