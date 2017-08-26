#include <thread>
#include "gtest/gtest.h"
#include "But/Threading/JoiningThread.hpp"
#include "Stream.hpp"
#include "But/Log/Field/LineNumber.hpp"

using But::Log::Destination::Stream;
using But::Log::Destination::Foreign;
using But::Log::Field::LineNumber;
using But::Log::Field::FormattedString;
using Thread = But::Threading::JoiningThread<std::thread>;

namespace
{

struct StringStream: public Stream
{
  StringStream(): Stream{ss_} { }
  void reloadImplUnderLock() override { }
  std::stringstream ss_;
};

struct ButLogDestinationStream: public testing::Test
{
  StringStream s_;
};


TEST_F(ButLogDestinationStream, PrintingSampleData)
{
  s_.log( "line: ", LineNumber{42} );
  EXPECT_EQ( s_.ss_.str(), "line: 42\n" );
}


TEST_F(ButLogDestinationStream, CheckArrowOperator)
{
  s_->log( "line: ", LineNumber{42} );
  EXPECT_EQ( s_.ss_.str(), "line: 42\n" );
}


TEST_F(ButLogDestinationStream, OperatingViaBaseClass)
{
  auto& base = static_cast<Foreign&>(s_);
  base->log( "line: ", LineNumber{42} );
  EXPECT_EQ( s_.ss_.str(), "line: 42\n" );
}


TEST_F(ButLogDestinationStream, RemovingNonPrintableCharacters)
{
  s_.log( "beep \07 / CRLF \r\n / normal: ", LineNumber{42} );
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

TEST_F(ButLogDestinationStream, MultithreadedExecutionDoesNotInterleaveOutput)
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


TEST_F(ButLogDestinationStream, MultithreadedExecutionDoesNotInterleaveOutputWhenUsedViaBaseClass)
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


TEST_F(ButLogDestinationStream, ReloadingSmokeTest)
{
  s_.reload();
}


TEST_F(ButLogDestinationStream, FlushingSmokeTest)
{
  s_.flush();
}


struct CustomFormatting: public StringStream
{
  void toStreamFormat(std::ostream& os, But::Log::Backend::Entry const& e) override
  { os << e.size() << "x"; }
  void toStreamFormat(std::ostream& os, But::Log::Field::FormattedString const& str, But::Log::Backend::Entry const&) override
  { os << str.value_.size() << "y"; }
};

TEST_F(ButLogDestinationStream, ProvidingDifferentToStreamFormatting)
{
  CustomFormatting cs;
  cs.log("alice", "has", "a cat");
  cs.log("so tech", "much wow");
  cs.log( FormattedString{"$2 / $1"}, 2, 84 );
  EXPECT_EQ( "3x2x7y", cs.ss_.str() );
}

}
