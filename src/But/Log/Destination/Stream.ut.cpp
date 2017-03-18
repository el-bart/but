#include <thread>
#include "gtest/gtest.h"
#include "But/Threading/JoiningThread.hpp"
#include "Stream.hpp"
#include "But/Log/Field/LineNumber.hpp"

using But::Log::Destination::Stream;
using But::Log::Destination::Foregin;
using But::Log::Field::LineNumber;
using Thread = But::Threading::JoiningThread<std::thread>;

namespace
{

struct ButLogDestinationStream: public testing::Test
{
  std::stringstream ss_;
  Stream s_{ss_};
};


TEST_F(ButLogDestinationStream, PrintingSampleData)
{
  s_.log( "line: ", LineNumber{42} );
  EXPECT_EQ( ss_.str(), "line: 42\n" );
}


TEST_F(ButLogDestinationStream, CheckArrowOperator)
{
  s_->log( "line: ", LineNumber{42} );
  EXPECT_EQ( ss_.str(), "line: 42\n" );
}


TEST_F(ButLogDestinationStream, OperatingViaBaseClass)
{
  auto& base = static_cast<Foregin&>(s_);
  base->log( "line: ", LineNumber{42} );
  EXPECT_EQ( ss_.str(), "line: 42\n" );
}


TEST_F(ButLogDestinationStream, RemovingNonPrintableCharacters)
{
  s_.log( "beep \07 / CRLF \r\n / normal: ", LineNumber{42} );
  EXPECT_EQ( ss_.str(), "beep . / CRLF .. / normal: 42\n" );
}


namespace
{
auto countLines(std::stringstream& ss)
{
  auto lines = 0u;
  std::string ignored;
  while( std::getline(ss, ignored) )
    ++lines;
  return lines;
}
}

TEST_F(ButLogDestinationStream, MultithreadedExecutionDoesNotInterleaveOutput)
{
  {
    auto logger = std::make_shared<Stream>(ss_);
    auto multiLog = [logger]() { for(auto i=0; i<100; ++i) logger->log("new input: ", i, " - in progress"); };
    Thread th1(multiLog);
    Thread th2(multiLog);
    Thread th3(multiLog);
  }
  EXPECT_EQ( 3u*100u, countLines(ss_) );
}


TEST_F(ButLogDestinationStream, MultithreadedExecutionDoesNotInterleaveOutputWhenUsedViaBaseClass)
{
  {
    std::shared_ptr<Foregin> logger = std::make_shared<Stream>(ss_);
    auto multiLog = [logger]() { for(auto i=0; i<100; ++i) logger->log("new input: ", i, " - in progress"); };
    Thread th1(multiLog);
    Thread th2(multiLog);
    Thread th3(multiLog);
  }
  EXPECT_EQ( 3u*100u, countLines(ss_) );
}


TEST_F(ButLogDestinationStream, ReloadingSmokeTest)
{
  Stream s{ss_};
  s.reload();
}


TEST_F(ButLogDestinationStream, FlushingSmokeTest)
{
  Stream s{ss_};
  s.flush();
}

}
