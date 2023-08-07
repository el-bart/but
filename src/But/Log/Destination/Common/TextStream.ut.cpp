#include <thread>
#include <But/Threading/JoiningThread.hpp>
#include <But/Log/Destination/Common/TextStream.hpp>
#include <gtest/gtest.h>

using But::Log::Destination::Common::TextStream;
using But::Log::Destination::Sink;
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
  s_.log("foo");
  EXPECT_EQ( s_.ss_.str(), "foo\n" );
}


TEST_F(ButLogDestinationTextStream, OperatingViaBaseClass)
{
  auto& base = static_cast<Sink&>(s_);
  base.log("foo");
  EXPECT_EQ( s_.ss_.str(), "foo\n" );
}


TEST_F(ButLogDestinationTextStream, PreservingNonPrintableCharacters)
{
  s_.log("beep \07 / CRLF \r\n / normal");
  EXPECT_EQ( s_.ss_.str(), "beep \07 / CRLF \r\n / normal\n" );
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
    auto multiLog = [logger] { for(auto i=0; i<100; ++i) logger->log( "new input: " + std::to_string(i) + " - in progress"); };
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
    std::shared_ptr<Sink> logger = loggerSS;
    auto multiLog = [logger] { for(auto i=0; i<100; ++i) logger->log( "new input: " + std::to_string(i) + " - in progress"); };
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


TEST_F(ButLogDestinationTextStream, EmptyLogIsNewLine)
{
  s_.log("");
  EXPECT_EQ( s_.ss_.str(), "\n" );
}

}
