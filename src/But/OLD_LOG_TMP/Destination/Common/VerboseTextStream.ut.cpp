#include <gtest/gtest.h>
#include <But/Log/Destination/Common/VerboseTextStream.hpp>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Destination/detail/args2FieldInfo.hpp>

using But::Log::Destination::detail::args2FieldInfo;
using But::Log::Destination::Common::VerboseTextStream;
using But::Log::Field::LineNumber;
using But::Log::Field::FormattedString;
using But::Log::Backend::Tag;
using But::Log::Backend::FieldInfo;

namespace
{

struct StringStream: public VerboseTextStream
{
  explicit StringStream(size_t depth): VerboseTextStream{ ss_, VerboseTextStream::Depth{depth} } { }
  void reloadImplUnderLock() override { }
  std::stringstream ss_;
};

struct ButLogDestinationVerboseTextStream: public testing::Test
{
  StringStream s_{10};
};


struct NestedString
{
  std::string value_;
};

auto toFieldInfo(NestedString const& ns)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Tag{"NestedString"}, { toFieldInfo(ns.value_) } };
}


TEST_F(ButLogDestinationVerboseTextStream, RootStringDoesNotGetTypePrinted)
{
  s_.log( args2FieldInfo( "root string", NestedString{"foo"} ) );
  EXPECT_EQ( s_.ss_.str(), "root string NestedString={ string=\"foo\" }\n" );
}


TEST_F(ButLogDestinationVerboseTextStream, PrintingSampleData)
{
  s_.log( args2FieldInfo( "line:", LineNumber{42} ) );
  EXPECT_EQ( s_.ss_.str(), "line: But::LineNumber=42\n" );
}


TEST_F(ButLogDestinationVerboseTextStream, RemovingNonPrintableCharacters)
{
  s_.log( args2FieldInfo( "beep \07 / CRLF \r\n / normal:", LineNumber{42} ) );
  EXPECT_EQ( s_.ss_.str(), "beep . / CRLF .. / normal: But::LineNumber=42\n" );
}


TEST_F(ButLogDestinationVerboseTextStream, ExplicitFormatAtTheBeginning)
{
  s_.log( args2FieldInfo( FormattedString{"my format"}, 2, 84, 69 ) );
  EXPECT_EQ( "my format\n", s_.ss_.str() );
}


TEST_F(ButLogDestinationVerboseTextStream, ExplicitFormatInTheMiddle)
{
  s_.log( args2FieldInfo( 42, FormattedString{"my format"}, 2, 84, 69 ) );
  EXPECT_EQ( "int=42 my format\n", s_.ss_.str() );
}


TEST_F(ButLogDestinationVerboseTextStream, SpaceIsInsertBetweenEachArgument)
{
  s_.log( args2FieldInfo(1, "a", 'b', 42) );
  EXPECT_EQ( s_.ss_.str(), "int=1 a b int=42\n" );
}


TEST_F(ButLogDestinationVerboseTextStream, ZeroArgumentsLogIsJustAnEmptyLine)
{
  s_.log( args2FieldInfo() );
  EXPECT_EQ( s_.ss_.str(), "\n" );
}


struct Point
{
  int x_;
  int y_;
  int z_;
};

auto toFieldInfo(Point const& p)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Tag{"Point"}, { toFieldInfo(p.x_), toFieldInfo(p.y_), toFieldInfo(p.z_) } };
}

TEST_F(ButLogDestinationVerboseTextStream, StreamingNestedStructure)
{
  s_.log( args2FieldInfo( "location in space:", Point{13,42,69} ) );
  EXPECT_EQ( s_.ss_.str(), "location in space: Point={ int=13 int=42 int=69 }\n" );
}


struct Line
{
  Point from_;
  Point to_;
};

auto toFieldInfo(Line const& l)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Tag{"Line"}, { toFieldInfo(l.from_), toFieldInfo(l.to_) } };
}


TEST_F(ButLogDestinationVerboseTextStream, StreamingDeeplyNestedStructure)
{
  s_.log( args2FieldInfo( "2D:", Line{ Point{13,42,69}, Point{9,9,7} } ) );
  EXPECT_EQ( s_.ss_.str(), "2D: Line={ Point={ int=13 int=42 int=69 } Point={ int=9 int=9 int=7 } }\n" );
}


TEST_F(ButLogDestinationVerboseTextStream, TruncatinAtDepth0)
{
  StringStream s{0};
  s.log( args2FieldInfo( "2D:", Line{ Point{13,42,69}, Point{9,9,7} } ) );
  EXPECT_EQ( s.ss_.str(), "2D: Line={ ... }\n" );
}


TEST_F(ButLogDestinationVerboseTextStream, TruncatinAtDepth1)
{
  StringStream s{1};
  s.log( args2FieldInfo( "2D:", Line{ Point{13,42,69}, Point{9,9,7} } ) );
  EXPECT_EQ( s.ss_.str(), "2D: Line={ Point={ ... } Point={ ... } }\n" );
}


TEST_F(ButLogDestinationVerboseTextStream, TruncatinAtDepth2When2IsMax)
{
  StringStream s{2};
  s.log( args2FieldInfo( "2D:", Line{ Point{13,42,69}, Point{9,9,7} } ) );
  EXPECT_EQ( s.ss_.str(), "2D: Line={ Point={ int=13 int=42 int=69 } Point={ int=9 int=9 int=7 } }\n" );
}

}
