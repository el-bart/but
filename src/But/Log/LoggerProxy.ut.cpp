#include <memory>
#include <sstream>
#include "gtest/gtest.h"
#include "LoggerProxy.hpp"
#include "Destination/Foreign.hpp"

using But::Log::LoggerProxy;
using But::Log::Destination::Foreign;
using But::Log::Backend::Entry;
using But::Log::Backend::Value;
using But::Log::Field::FormattedString;

namespace
{

struct DestinationStub final
{
  auto operator->() { return this; }

  template<typename... Args>
  void log(Args&&...) { }
  void reload() { }
  void flush() { }
};


struct ButLogLoggerProxy: public testing::Test
{
  LoggerProxy<DestinationStub> lp_;
};


TEST_F(ButLogLoggerProxy, LoggingApiSmokeTest)
{
  lp_.log("the", "answer", "is", 42);
  lp_.log( BUT_FORMAT("$0 is ok"), "pinky" );
}


TEST_F(ButLogLoggerProxy, LoggerIsMovable)
{
  LoggerProxy<DestinationStub> log;
  auto other = std::move(log);
  (void)other;
}


struct ThrowingDestination final
{
  void log(...) { throw std::runtime_error{"ignored"}; }
  void reload() { throw std::runtime_error{"ignored"}; }
  void flush()  { throw std::runtime_error{"ignored"}; }
  auto operator->() { return this; }
};

TEST_F(ButLogLoggerProxy, AllErrorsFromActualDestinationsAreIgnored)
{
  LoggerProxy<ThrowingDestination> log{ ThrowingDestination{} };
  EXPECT_NO_THROW( log.log("hello", "john") );
  EXPECT_NO_THROW( log.reload() );
  EXPECT_NO_THROW( log.flush() );
}
struct CustomTranslator
{
  template<unsigned N, unsigned M>
  auto translate(But::Format::Parsed<N,M>&& parsed) const { return parsed; }
};


TEST_F(ButLogLoggerProxy, ConstructorsForDifferentObjects)
{
  {
    LoggerProxy<DestinationStub, CustomTranslator> lp;
  }
  {
    LoggerProxy<DestinationStub, CustomTranslator> lp{ DestinationStub{} };
  }
  {
    LoggerProxy<DestinationStub, CustomTranslator> lp{ CustomTranslator{} };
  }
  {
    LoggerProxy<DestinationStub, CustomTranslator> lp{ DestinationStub{}, CustomTranslator{} };
  }
}

}
