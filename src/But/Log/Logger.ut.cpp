#include <memory>
#include <sstream>
#include <gtest/gtest.h>
#include <But/Log/Logger.hpp>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Destination/FakeSink.ut.hpp>

using But::Log::Logger;
using But::Log::Field::LineNumber;
using But::Log::Destination::Sink;
using But::Log::Destination::FakeSink;

namespace
{

struct ButLogLogger: public testing::Test
{
  Logger<> lp_{ But::makeSharedNN<FakeSink>() };
};


TEST_F(ButLogLogger, LoggingApiSmokeTest)
{
  lp_.log("no substance");
  lp_.log("the message", LineNumber{42});
  lp_.log( BUT_FORMAT("line $0 is ok"), LineNumber{42} );
}


TEST_F(ButLogLogger, LoggerIsMovable)
{
  auto other = std::move(lp_);
  (void)other;
}


struct ThrowingDestination final: Sink
{
  void logImpl(std::string &&) override { throw std::runtime_error{"ignored"}; }
  void reloadImpl() override { throw std::runtime_error{"ignored"}; }
  void flushImpl() override { throw std::runtime_error{"ignored"}; }
};

TEST_F(ButLogLogger, AllErrorsFromActualDestinationsAreIgnored)
{
  Logger<> log{ But::makeSharedNN<ThrowingDestination>() };
  EXPECT_NO_THROW( log.log("hello, world") );
  EXPECT_NO_THROW( log.reload() );
  EXPECT_NO_THROW( log.flush() );
}


struct CustomTranslator
{
  template<size_t N, size_t M>
  auto translate(But::Format::ParsedCompiletime<N,M>&& parsed) const { return parsed; }
};

TEST_F(ButLogLogger, ConstructorsForDifferentObjects)
{
  const auto dst = But::makeSharedNN<FakeSink>();
  {
    Logger<CustomTranslator> lp{dst};
  }
  {
    Logger<CustomTranslator> lp{ dst, CustomTranslator{} };
  }
}


TEST_F(ButLogLogger, LoggerWithDefaultFieldsSmokeTest)
{
  auto proxy = lp_.withFields( LineNumber{42} );
  proxy.log("test");
}


TEST_F(ButLogLogger, LoggerWithDefaultFieldsDoesNotThrow)
{
  Logger<> log{ But::makeSharedNN<ThrowingDestination>() };
  auto proxy = log.withFields( LineNumber{42} );
  EXPECT_NO_THROW( proxy.log("ok") );
}

}
