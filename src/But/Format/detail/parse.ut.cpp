#include <type_traits>
#include "gtest/gtest.h"
#include "parse.hpp"

using But::Format::Invalid;
using But::Format::detail::parse;
using But::Format::detail::parseCt;
using But::Format::detail::parseRt;
using But::Format::detail::Segment;

namespace
{

struct ButFormatDetailParse: public testing::Test
{ };


TEST_F(ButFormatDetailParse, UnformattedStrings)
{
  {
    constexpr auto ps = parseCt<1>("");
    ASSERT_EQ( 0u, ps.segments_.size() );
  }

  {
    constexpr auto ps = parseCt<1>("test string");
    ASSERT_EQ( 1u, ps.segments_.size() );
    constexpr auto s = ps.segments_[0];
    EXPECT_TRUE( s.begin_ != s.end_ );
    EXPECT_EQ( "test string", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::String, s.type_ );
  }

  {
    constexpr auto ps = parseCt<1>("test\n\r\tstring");
    ASSERT_EQ( 1u, ps.segments_.size() );
    constexpr auto s = ps.segments_[0];
    EXPECT_TRUE( s.begin_ != s.end_ );
    EXPECT_EQ( "test\n\r\tstring", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::String, s.type_ );
  }
}


TEST_F(ButFormatDetailParse, EscapeSequence)
{
  {
    constexpr auto ps = parseCt<1>("$$");
    ASSERT_EQ( 1u, ps.segments_.size() );
    constexpr auto s = ps.segments_[0];
    EXPECT_TRUE( s.begin_ != s.end_ );
    EXPECT_EQ( "$", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::String, s.type_ );
  }

  EXPECT_THROW( parseCt<10>("$"), Invalid ) << "parameter declaraion too short";

  {
    constexpr auto ps = parseCt<2>("$$at the beginning");
    ASSERT_EQ( 2u, ps.segments_.size() );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "$", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( "at the beginning", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
  }

  {
    constexpr auto ps = parseCt<3>("in the$$middle");
    ASSERT_EQ( 3u, ps.segments_.size() );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "in the", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( "$", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
    // 2
    {
      constexpr auto s = ps.segments_[2];
      EXPECT_EQ( "middle", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
  }

  {
    constexpr auto ps = parseCt<2>("at the end$$");
    ASSERT_EQ( 2u, ps.segments_.size() );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "at the end", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( "$", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
  }

  EXPECT_THROW( parseCt<2>("oops $"), Invalid ) << "parameter declaraion too short";
}


TEST_F(ButFormatDetailParse, SimpleArgument)
{
  {
    constexpr auto ps = parseCt<1>("$42");
    ASSERT_EQ( 1u, ps.segments_.size() );
    constexpr auto s = ps.segments_[0];
    EXPECT_EQ( "$42", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 42u, s.referencedArgument_ );
  }

  {
    constexpr auto ps = parseCt<2>("$42 beginning");
    ASSERT_EQ( 2u, ps.segments_.size() );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "$42", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::Value, s.type_ );
      EXPECT_EQ( 42u, s.referencedArgument_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( " beginning", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
  }

  {
    constexpr auto ps = parseCt<3>("in $42 the middle");
    ASSERT_EQ( 3u, ps.segments_.size() );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "in ", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( "$42", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::Value, s.type_ );
      EXPECT_EQ( 42u, s.referencedArgument_ );
    }
    // 2
    {
      constexpr auto s = ps.segments_[2];
      EXPECT_EQ( " the middle", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
  }

  {
    constexpr auto ps = parseCt<2>("the end $42");
    ASSERT_EQ( 2u, ps.segments_.size() );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "the end ", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( "$42", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::Value, s.type_ );
      EXPECT_EQ( 42u, s.referencedArgument_ );
    }
  }

  EXPECT_THROW( parseCt<2>("$12x"), Invalid ) << "invalid number";
  EXPECT_THROW( parseCt<2>("$oops"), Invalid ) << "not a number";
  EXPECT_THROW( parseCt<2>("$ "), Invalid ) << "missing number";
  EXPECT_THROW( parseCt<2>("$-2"), Invalid ) << "wrong number";
  EXPECT_THROW( parseCt<2>("$2}"), Invalid ) << "missing beginning brace";
}


TEST_F(ButFormatDetailParse, BraceArgument)
{
  {
    constexpr auto ps = parseCt<1>("${42}");
    ASSERT_EQ( 1u, ps.segments_.size() );
    constexpr auto s = ps.segments_[0];
    EXPECT_EQ( "${42}", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 42u, s.referencedArgument_ );
  }

  {
    constexpr auto ps = parseCt<2>("${42} beginning");
    ASSERT_EQ( 2u, ps.segments_.size() );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "${42}", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::Value, s.type_ );
      EXPECT_EQ( 42u, s.referencedArgument_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( " beginning", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
  }

  {
    constexpr auto ps = parseCt<3>("in ${42} the middle");
    ASSERT_EQ( 3u, ps.segments_.size() );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "in ", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( "${42}", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::Value, s.type_ );
      EXPECT_EQ( 42u, s.referencedArgument_ );
    }
    // 2
    {
      constexpr auto s = ps.segments_[2];
      EXPECT_EQ( " the middle", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
  }

  {
    constexpr auto ps = parseCt<2>("the end ${42}");
    ASSERT_EQ( 2u, ps.segments_.size() );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "the end ", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( "${42}", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::Value, s.type_ );
      EXPECT_EQ( 42u, s.referencedArgument_ );
    }
  }

  EXPECT_THROW( parseCt<10>("${12x}"), Invalid ) << "invalid number";
  EXPECT_THROW( parseCt<10>("${oops}"), Invalid ) << "not a number";
  EXPECT_THROW( parseCt<10>("${} "), Invalid ) << "missing number";
  EXPECT_THROW( parseCt<10>("${-2}"), Invalid ) << "wrong number";
  EXPECT_THROW( parseCt<10>("${12"), Invalid ) << "brace not closed";
  EXPECT_THROW( parseCt<10>("${"), Invalid ) << "no number and no closing brace";
}


TEST_F(ButFormatDetailParse, ValueArguments)
{
  {
    constexpr auto ps = parseCt<1>("${42}");
    ASSERT_EQ( 1u, ps.segments_.size() );
    constexpr auto s = ps.segments_[0];
    EXPECT_EQ( "${42}", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 42u, s.referencedArgument_ );
  }

  {
    constexpr auto ps = parseCt<2>("${42} beginning");
    ASSERT_EQ( 2u, ps.segments_.size() );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "${42}", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::Value, s.type_ );
      EXPECT_EQ( 42u, s.referencedArgument_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( " beginning", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
  }

  {
    constexpr auto ps = parseCt<3>("in ${42} the middle");
    ASSERT_EQ( 3u, ps.segments_.size() );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "in ", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( "${42}", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::Value, s.type_ );
      EXPECT_EQ( 42u, s.referencedArgument_ );
    }
    // 2
    {
      constexpr auto s = ps.segments_[2];
      EXPECT_EQ( " the middle", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
  }

  {
    constexpr auto ps = parseCt<2>("the end ${42}");
    ASSERT_EQ( 2u, ps.segments_.size() );
    // 0
    {
      constexpr auto s = ps.segments_[0];
      EXPECT_EQ( "the end ", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::String, s.type_ );
    }
    // 1
    {
      constexpr auto s = ps.segments_[1];
      EXPECT_EQ( "${42}", std::string(s.begin_, s.end_) );
      EXPECT_EQ( Segment::Type::Value, s.type_ );
      EXPECT_EQ( 42u, s.referencedArgument_ );
    }
  }

  EXPECT_THROW( parseCt<10>("${V12x}"), Invalid ) << "invalid number";
  EXPECT_THROW( parseCt<10>("${Voops}"), Invalid ) << "not a number";
  EXPECT_THROW( parseCt<10>("${V} "), Invalid ) << "missing number";
  EXPECT_THROW( parseCt<10>("${V-2}"), Invalid ) << "wrong number";
  EXPECT_THROW( parseCt<10>("${V12"), Invalid ) << "brace not closed";
}


TEST_F(ButFormatDetailParse, EmptyCommentArgument)
{
  {
    constexpr auto ps = parseCt<1>("${42#}");
    ASSERT_EQ( 1u, ps.segments_.size() );
    constexpr auto s = ps.segments_[0];
    EXPECT_EQ( "${42#}", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 42u, s.referencedArgument_ );
  }

  EXPECT_THROW( parseCt<2>("$12#"), Invalid ) << "comment on non-brace variable reference";
  EXPECT_THROW( parseCt<2>("${12#"), Invalid ) << "missing closing brace";
}


TEST_F(ButFormatDetailParse, CommentedArguments)
{
  {
    constexpr auto ps = parseCt<1>("${42#some comment}");
    ASSERT_EQ( 1u, ps.segments_.size() );
    constexpr auto s = ps.segments_[0];
    EXPECT_EQ( "${42#some comment}", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 42u, s.referencedArgument_ );
  }

  EXPECT_THROW( parseCt<10>("$12#tag"), Invalid ) << "comment on non-brace variable reference";
  EXPECT_THROW( parseCt<10>("${12#tag"), Invalid ) << "missing closing brace";
}


TEST_F(ButFormatDetailParse, ArgumentsArrayCanBeLargerThanNeeded)
{
  constexpr auto ps = parseCt<42>("foo/bar"); // smoke test, in fact... ;)
  ASSERT_EQ( 1u, ps.segments_.size() );
}


TEST_F(ButFormatDetailParse, BracesWithoutLeadingDolarSignHaveNoSpecialMinning)
{
  constexpr auto ps = parseCt<1>("}foo{}bar{");
  ASSERT_EQ( 1u, ps.segments_.size() );
  constexpr auto s = ps.segments_[0];
  EXPECT_EQ( "}foo{}bar{", std::string(s.begin_, s.end_) );
  EXPECT_EQ( Segment::Type::String, s.type_ );
}


TEST_F(ButFormatDetailParse, MixedTokens)
{
  constexpr auto args = 10u;
  constexpr auto ps = parseCt<args>("in${42#test}xxx${69#ff}${13}$21 $$${997#narf}${666}");
  ASSERT_EQ( args, ps.segments_.size() );
  // 0
  {
    constexpr auto s = ps.segments_[0];
    EXPECT_EQ( "in", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::String, s.type_ );
  }
  // 1
  {
    constexpr auto s = ps.segments_[1];
    EXPECT_EQ( "${42#test}", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 42u, s.referencedArgument_ );
  }
  // 2
  {
    constexpr auto s = ps.segments_[2];
    EXPECT_EQ( "xxx", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::String, s.type_ );
  }
  // 3
  {
    constexpr auto s = ps.segments_[3];
    EXPECT_EQ( "${69#ff}", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 69u, s.referencedArgument_ );
  }
  // 4
  {
    constexpr auto s = ps.segments_[4];
    EXPECT_EQ( "${13}", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 13u, s.referencedArgument_ );
  }
  // 5
  {
    constexpr auto s = ps.segments_[5];
    EXPECT_EQ( "$21", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 21u, s.referencedArgument_ );
  }
  // 6
  {
    constexpr auto s = ps.segments_[6];
    EXPECT_EQ( " ", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::String, s.type_ );
  }
  // 7
  {
    constexpr auto s = ps.segments_[7];
    EXPECT_EQ( "$", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::String, s.type_ );
  }
  // 8
  {
    constexpr auto s = ps.segments_[8];
    EXPECT_EQ( "${997#narf}", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 997u, s.referencedArgument_ );
  }
  // 9
  {
    constexpr auto s = ps.segments_[9];
    EXPECT_EQ( "${666}", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 666u, s.referencedArgument_ );
  }
}


TEST_F(ButFormatDetailParse, MiscInvalidFormats)
{
  EXPECT_THROW( parseCt<2>("${X12}"), Invalid ) << "invalid modifier";
  EXPECT_THROW( parseCt<2>("$V12"), Invalid ) << "explicit variable without brace is not possible";
  EXPECT_THROW( parseCt<2>("$T12"), Invalid ) << "explicit type name without brace is not possible";
}


TEST_F(ButFormatDetailParse, TooComplexFormatForGivenStatesCount)
{
  EXPECT_NO_THROW( parseCt<3>("$1 $0") ) << "invalid format is raisign an error";
  EXPECT_THROW( parseCt<4>("$1 $2 $0"), Invalid );
  EXPECT_THROW( parseCt<2>("${1}${2}${0}"), Invalid );
}

TEST_F(ButFormatDetailParse, RuntimeFormatParsing)
{
  const auto ps = parseRt("one ${42#test} two ${43} three ${44#tag} four ${45} five $46 six ${47}");
  EXPECT_TRUE( ( std::is_same<decltype(ps.segments_), std::vector<Segment>>::value ) );
  ASSERT_EQ( 12u, ps.segments_.size() );
  // 0
  {
    const auto s = ps.segments_[0];
    EXPECT_EQ( "one ", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::String, s.type_ );
  }
  // 1
  {
    const auto s = ps.segments_[1];
    EXPECT_EQ( "${42#test}", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 42u, s.referencedArgument_ );
  }
  // 2
  {
    const auto s = ps.segments_[2];
    EXPECT_EQ( " two ", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::String, s.type_ );
  }
  // 3
  {
    const auto s = ps.segments_[3];
    EXPECT_EQ( "${43}", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 43u, s.referencedArgument_ );
  }
  // 4
  {
    const auto s = ps.segments_[4];
    EXPECT_EQ( " three ", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::String, s.type_ );
  }
  // 5
  {
    const auto s = ps.segments_[5];
    EXPECT_EQ( "${44#tag}", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 44u, s.referencedArgument_ );
  }
  // 6
  {
    const auto s = ps.segments_[6];
    EXPECT_EQ( " four ", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::String, s.type_ );
  }
  // 7
  {
    const auto s = ps.segments_[7];
    EXPECT_EQ( "${45}", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 45u, s.referencedArgument_ );
  }
  // 8
  {
    const auto s = ps.segments_[8];
    EXPECT_EQ( " five ", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::String, s.type_ );
  }
  // 9
  {
    const auto s = ps.segments_[9];
    EXPECT_EQ( "$46", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 46u, s.referencedArgument_ );
  }
  // 10
  {
    const auto s = ps.segments_[10];
    EXPECT_EQ( " six ", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::String, s.type_ );
  }
  // 11
  {
    const auto s = ps.segments_[11];
    EXPECT_EQ( "${47}", std::string(s.begin_, s.end_) );
    EXPECT_EQ( Segment::Type::Value, s.type_ );
    EXPECT_EQ( 47u, s.referencedArgument_ );
  }
}

}
