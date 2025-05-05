#include <vector>
#include <But/Log/Destination/Trim.hpp>
#include <gtest/gtest.h>

using But::Log::Destination::Trim;
using But::Log::Destination::Sink;

namespace
{

struct Fake: public Sink
{
  void logImpl(std::string&& str) override { logs_.push_back( std::move(str) ); }
  void reloadImpl()               override { }
  void flushImpl()                override { }

  std::vector<std::string> logs_;
};

struct ButLogDestinationTrim: public testing::Test
{
  But::NotNullShared<Fake> fake_{ But::makeSharedNN<Fake>() };
  But::NotNullShared<Sink> s_{ But::makeSharedNN<Trim>(fake_) };
};


TEST_F(ButLogDestinationTrim, PrintingSampleData)
{
  s_->log("foo");
  EXPECT_EQ( fake_->logs_.at(0), "foo" );
}


TEST_F(ButLogDestinationTrim, PreservingNonPrintableCharacters)
{
  s_->log("beep \07 / CRLF \r\n / normal");
  EXPECT_EQ( fake_->logs_.at(0), "beep . / CRLF .. / normal" );
}


TEST_F(ButLogDestinationTrim, ReloadingSmokeTest)
{
  s_->reload();
}


TEST_F(ButLogDestinationTrim, FlushingSmokeTest)
{
  s_->flush();
}


TEST_F(ButLogDestinationTrim, EmptyLogIsEmpty)
{
  s_->log("");
  EXPECT_EQ( fake_->logs_.at(0), "" );
}

}
