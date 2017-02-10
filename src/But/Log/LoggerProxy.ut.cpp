#include <memory>
#include <sstream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "LoggerProxy.hpp"

using But::Log::LoggerProxy;
//using But::Log::Destination;

namespace
{

/*
struct TestDestination: public Destination
{
  // TODO
};
*/


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
  LoggerProxy<TestNativeDestination> log_{ TestNativeDestination{buffer_} };
};


TEST_F(ButLogLoggerProxy, LoggingSimpleValuesOneAtATime)
{
  log_.log(42);
  log_.log("foo");
  log_.log(3.14);
  EXPECT_EQ( buffer_.str(), "int|string|double|" );
}


TEST_F(ButLogLoggerProxy, LoggingMultipleSimpleValuesAtOnce)
{
  log_.log(42, "foo", 3.14);
  EXPECT_EQ( buffer_.str(), "int,string,double|");
}

// TODO: exceptions are not propagated

// TODO: foregin destination

}
