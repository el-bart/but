#include "gtest/gtest.h"
#include "parse.hpp"

using But::Format::Invalid;
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
    ASSERT_EQ( 0u, ps.count_ );
  }
  {
    constexpr auto ps = parse<1>("test string");
    ASSERT_EQ( 1u, ps.count_ );
    constexpr auto s = ps.segments_[0];
    EXPECT_TRUE( s.begin_ != s.end_ );
    EXPECT_EQ( "test string", std::string(s.begin_, s.end_) );
    EXPECT_EQ( State::Type::String, s.type_ );
  }
  {
    constexpr auto ps = parse<1>("test\n\r\tstring");
    ASSERT_EQ( 1u, ps.count_ );
    constexpr auto s = ps.segments_[0];
    EXPECT_TRUE( s.begin_ != s.end_ );
    EXPECT_EQ( "test\n\r\tstring", std::string(s.begin_, s.end_) );
    EXPECT_EQ( State::Type::String, s.type_ );
  }
  {
    constexpr auto ps = parse<1>("$$");
    ASSERT_EQ( 1u, ps.count_ );
    constexpr auto s = ps.segments_[0];
    EXPECT_EQ( "$$", std::string(s.begin_, s.end_) );
    EXPECT_EQ( State::Type::String, s.type_ );
  }
  {
    constexpr auto ps = parse<3>("some $$ non-var");
    ASSERT_EQ( 3u, ps.count_ );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "some ", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::String, s.type_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( "$$", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::String, s.type_ );
    }
    // 2
    {
      constexpr auto s = ps.segments_[2];
      EXPECT_EQ( " non-var", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::String, s.type_ );
    }
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


TEST_F(ButFormatDetailParse, InvalidFormats)
{
  EXPECT_THROW( parse<10>("$"), Invalid ) << "parameter declaraion too short";
  EXPECT_THROW( parse<10>("oops $"), Invalid ) << "parameter declaraion too short";
  EXPECT_THROW( parse<10>("$oops"), Invalid ) << "not a number";
}

}
