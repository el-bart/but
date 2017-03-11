#include <memory>
#include <sstream>
#include "gtest/gtest.h"
#include "LoggerProxy.hpp"
#include "Destination/Foregin.hpp"

using But::Log::LoggerProxy;
using But::Log::Destination::Foregin;
using But::Log::Backend::Entry;

namespace
{

struct TestForeginDestination: public Foregin
{
  explicit TestForeginDestination(std::stringstream& ss): ss_{&ss} { }

   void logImpl(Entry e) override
   {
     for(auto& f: e)
       (*ss_) << f.type() << "=" << f.value() << " ";
   }

  std::stringstream* ss_;
};


struct TestNativeDestination final
{
  explicit TestNativeDestination(std::stringstream& ss): ss_{&ss} { }

  void log(int) { *ss_ << "int|"; }
  void log(std::string const&) { *ss_ << "string|"; }
  void log(double) { *ss_ << "double|"; }
  void log(int, std::string const&, double) { *ss_ << "int,string,double|"; }

  auto operator->() { return this; }

  std::stringstream* ss_;
};


struct ButLogLoggerProxy: public testing::Test
{
  std::stringstream buffer_;
};


TEST_F(ButLogLoggerProxy, LoggingSimpleValuesOneAtATime)
{
  LoggerProxy<TestNativeDestination> log{ TestNativeDestination{buffer_} };
  log.log(42);
  log.log("foo");
  log.log(3.14);
  EXPECT_EQ( buffer_.str(), "int|string|double|" );
}


TEST_F(ButLogLoggerProxy, LoggingMultipleSimpleValuesAtOnce)
{
  LoggerProxy<TestNativeDestination> log{ TestNativeDestination{buffer_} };
  log.log(42, "foo", 3.14);
  EXPECT_EQ( buffer_.str(), "int,string,double|");
}


TEST_F(ButLogLoggerProxy, ForeginTypeValueLogging)
{
  LoggerProxy<TestForeginDestination> log{ TestForeginDestination{buffer_} };
  log.log(42, "foo", 'a');
  EXPECT_EQ( buffer_.str(), "int=42 std::string=foo char=a ");
}

// TODO: exceptions are not propagated

}
