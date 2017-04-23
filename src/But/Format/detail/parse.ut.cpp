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
}


TEST_F(ButFormatDetailParse, EscapeSequence)
{
  {
    constexpr auto ps = parse<1>("$$");
    ASSERT_EQ( 1u, ps.count_ );
    constexpr auto s = ps.segments_[0];
    EXPECT_TRUE( s.begin_ != s.end_ );
    EXPECT_EQ( "$", std::string(s.begin_, s.end_) );
    EXPECT_EQ( State::Type::String, s.type_ );
  }

  EXPECT_THROW( parse<10>("$"), Invalid ) << "parameter declaraion too short";

  {
    constexpr auto ps = parse<3>("$$at the beginning");
    ASSERT_EQ( 2u, ps.count_ );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "$", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::String, s.type_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( "at the beginning", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::String, s.type_ );
    }
  }

  {
    constexpr auto ps = parse<3>("in the$$middle");
    ASSERT_EQ( 3u, ps.count_ );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "in the", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::String, s.type_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( "$", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::String, s.type_ );
    }
    // 2
    {
      constexpr auto s = ps.segments_[2];
      EXPECT_EQ( "middle", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::String, s.type_ );
    }
  }

  {
    constexpr auto ps = parse<3>("at the end$$");
    ASSERT_EQ( 2u, ps.count_ );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "at the end", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::String, s.type_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( "$", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::String, s.type_ );
    }
  }

  EXPECT_THROW( parse<10>("oops $"), Invalid ) << "parameter declaraion too short";
}


TEST_F(ButFormatDetailParse, SimpleArgument)
{
  {
    constexpr auto ps = parse<3>("$42");
    ASSERT_EQ( 1u, ps.count_ );
    constexpr auto s = ps.segments_[0];
    EXPECT_EQ( "$42", std::string(s.begin_, s.end_) );
    EXPECT_EQ( State::Type::Value, s.type_ );
    EXPECT_EQ( 42u, s.referencedArgument_ );
  }

  {
    constexpr auto ps = parse<3>("$42 beginning");
    ASSERT_EQ( 2u, ps.count_ );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "$42", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::Value, s.type_ );
      EXPECT_EQ( 42u, s.referencedArgument_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( " beginning", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::String, s.type_ );
    }
  }

  {
    constexpr auto ps = parse<3>("in $42 the middle");
    ASSERT_EQ( 3u, ps.count_ );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "in ", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::String, s.type_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( "$42", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::Value, s.type_ );
      EXPECT_EQ( 42u, s.referencedArgument_ );
    }
    // 2
    {
      constexpr auto s = ps.segments_[2];
      EXPECT_EQ( " the middle", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::String, s.type_ );
    }
  }

  {
    constexpr auto ps = parse<3>("the end $42");
    ASSERT_EQ( 2u, ps.count_ );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "the end ", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::String, s.type_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( "$42", std::string(s.begin_, s.end_) );
      EXPECT_EQ( State::Type::Value, s.type_ );
      EXPECT_EQ( 42u, s.referencedArgument_ );
    }
  }

  EXPECT_THROW( parse<10>("$12x"), Invalid ) << "invalid number";
  EXPECT_THROW( parse<10>("$oops"), Invalid ) << "not a number";
  EXPECT_THROW( parse<10>("$ "), Invalid ) << "missing number";
  EXPECT_THROW( parse<10>("$-2"), Invalid ) << "wrong number";
}


TEST_F(ButFormatDetailParse, BraceArgument)
{
  EXPECT_THROW( parse<10>("${12"), Invalid ) << "brace not closed";
  EXPECT_THROW( parse<10>("${x}"), Invalid ) << "invalid number";
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
  constexpr auto ps = parse<42>("");    // smoke test, in fact... ;)
  ASSERT_EQ( 0u, ps.count_ );
}


TEST_F(ButFormatDetailParse, MixedTokens)
{
}

}
