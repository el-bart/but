#include "gtest/gtest.h"
#include "parse.hpp"

using But::Format::detail::parse;
using But::Format::detail::State;

namespace
{

struct ButFormatDetailParse: public testing::Test
{ };


TEST_F(ButFormatDetailParse, UnformattedStrings)
{
  {
    constexpr auto ps = parse<1>("");
    ASSERT_EQ( 1u, ps.count_ );
    constexpr auto s0 = ps.segments_[0];
    EXPECT_TRUE( s0.begin_ == s0.end_ );
    EXPECT_EQ( 0, *s0.begin_ );
    EXPECT_EQ( State::Type::String, s0.type_ );
  }
}


TEST_F(ButFormatDetailParse, SimpleArguments)
{
}


TEST_F(ButFormatDetailParse, ValueArguments)
{
}


TEST_F(ButFormatDetailParse, TypeArguments)
{
}


TEST_F(ButFormatDetailParse, CommentedArguments)
{
}


TEST_F(ButFormatDetailParse, ArgumentsArrayCanBeLargerThanNeeded)
{
}


TEST_F(ButFormatDetailParse, MixedTokens)
{
}

}
