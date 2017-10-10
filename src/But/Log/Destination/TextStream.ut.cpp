#include <thread>
#include "gtest/gtest.h"
#include "But/Threading/JoiningThread.hpp"
#include "TextStream.hpp"
#include "But/Log/Field/LineNumber.hpp"

using But::Log::Destination::TextStream;
using But::Log::Destination::Foreign;
using But::Log::Field::LineNumber;
using But::Log::Field::FormattedString;
using But::Log::Backend::Type;
using But::Log::Backend::FieldInfo;
using Thread = But::Threading::JoiningThread<std::thread>;

namespace
{

struct StringStream: public TextStream
{
  StringStream(): TextStream{ss_} { }
  void reloadImplUnderLock() override { }
  std::stringstream ss_;
};

struct ButLogDestinationTextStream: public testing::Test
{
  StringStream s_;
};


TEST_F(ButLogDestinationTextStream, PrintingSampleData)
{
  s_.log( "line:", LineNumber{42} );
  EXPECT_EQ( s_.ss_.str(), "line: 42\n" );
}


TEST_F(ButLogDestinationTextStream, CheckArrowOperator)
{
  s_->log( "line:", LineNumber{42} );
  EXPECT_EQ( s_.ss_.str(), "line: 42\n" );
}


TEST_F(ButLogDestinationTextStream, OperatingViaBaseClass)
{
  auto& base = static_cast<Foreign&>(s_);
  base->log( "line:", LineNumber{42} );
  EXPECT_EQ( s_.ss_.str(), "line: 42\n" );
}


TEST_F(ButLogDestinationTextStream, RemovingNonPrintableCharacters)
{
  s_.log( "beep \07 / CRLF \r\n / normal:", LineNumber{42} );
  EXPECT_EQ( s_.ss_.str(), "beep . / CRLF .. / normal: 42\n" );
}


auto countLines(std::stringstream& ss)
{
  auto lines = 0u;
  std::string ignored;
  while( std::getline(ss, ignored) )
    ++lines;
  return lines;
}

TEST_F(ButLogDestinationTextStream, MultithreadedExecutionDoesNotInterleaveOutput)
{
  auto logger = std::make_shared<StringStream>();
  {
    auto multiLog = [logger]() { for(auto i=0; i<100; ++i) logger->log("new input: ", i, " - in progress"); };
    Thread th1(multiLog);
    Thread th2(multiLog);
    Thread th3(multiLog);
  }
  EXPECT_EQ( 3u*100u, countLines(logger->ss_) );
}


TEST_F(ButLogDestinationTextStream, MultithreadedExecutionDoesNotInterleaveOutputWhenUsedViaBaseClass)
{
  auto loggerSS = std::make_shared<StringStream>();
  {
    std::shared_ptr<Foreign> logger = loggerSS;
    auto multiLog = [logger]() { for(auto i=0; i<100; ++i) logger->log("new input: ", i, " - in progress"); };
    Thread th1(multiLog);
    Thread th2(multiLog);
    Thread th3(multiLog);
  }
  EXPECT_EQ( 3u*100u, countLines(loggerSS->ss_) );
}


TEST_F(ButLogDestinationTextStream, ReloadingSmokeTest)
{
  s_.reload();
}


TEST_F(ButLogDestinationTextStream, FlushingSmokeTest)
{
  s_.flush();
}


struct CustomFormatting: public StringStream
{
  void toStreamFormat(std::ostream& os, But::Log::Backend::FieldInfo const& fi) override
  {
    os << fi.array().size() << "x";
  }
};

TEST_F(ButLogDestinationTextStream, ProvidingDifferentToStreamFormatting)
{
  CustomFormatting cs;
  cs.log("alice", "has", "a cat");
  cs.log("so tech", "much wow");
  cs.log( FormattedString{"$2 / $1 / $0"}, 2, 84, 69 );
  EXPECT_EQ( "3x2x4x", cs.ss_.str() );
}


TEST_F(ButLogDestinationTextStream, SpaceIsInsertBetweenEachArgument)
{
  s_.log(1, "a", 'b', 42);
  EXPECT_EQ( s_.ss_.str(), "1 a b 42\n" );
}


TEST_F(ButLogDestinationTextStream, ZeroArgumentsLogIsJustAnEmptyLine)
{
  s_.log();
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
  return FieldInfo{ Type{"Point"}, { toFieldInfo(p.x_), toFieldInfo(p.y_), toFieldInfo(p.z_) } };
}

TEST_F(ButLogDestinationTextStream, StreamingNestedStructure)
{
  s_.log( "location in space:", Point{13,42,69} );
  EXPECT_EQ( s_.ss_.str(), "location in space: Point={13,42,69}\n" );
}


struct Line
{
  Point from_;
  Point to_;
};

auto toFieldInfo(Line const& l)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Type{"Line"}, { toFieldInfo(l.from_), toFieldInfo(l.to_) } };
}


TEST_F(ButLogDestinationTextStream, StreamingDeeplyNestedStructure)
{
  s_.log( "2D:", Line{ Point{13,42,69}, Point{9,9,7} } );
  EXPECT_EQ( s_.ss_.str(), "2D: Line={Point={13,42,69},Point={9,9,7}}\n" );
}

}
