#include <memory>
#include <sstream>
#include "gtest/gtest.h"
#include "Proxy.hpp"
#include "Destination/Foreign.hpp"

using But::Log::Proxy;

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


struct ButLogProxy: public testing::Test
{
  Proxy<DestinationStub> lp_;
};


TEST_F(ButLogProxy, LoggingApiSmokeTest)
{
  lp_.log("the", "answer", "is", 42);
  lp_.log( BUT_FORMAT("$0 is ok"), "pinky" );
}


TEST_F(ButLogProxy, LoggerIsMovable)
{
  Proxy<DestinationStub> log;
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

TEST_F(ButLogProxy, AllErrorsFromActualDestinationsAreIgnored)
{
  Proxy<ThrowingDestination> log{ ThrowingDestination{} };
  EXPECT_NO_THROW( log.log("hello", "john") );
  EXPECT_NO_THROW( log.reload() );
  EXPECT_NO_THROW( log.flush() );
}
struct CustomTranslator
{
  template<size_t N, size_t M>
  auto translate(But::Format::Parsed<N,M>&& parsed) const { return parsed; }
};


TEST_F(ButLogProxy, ConstructorsForDifferentObjects)
{
  {
    Proxy<DestinationStub, CustomTranslator> lp;
  }
  {
    Proxy<DestinationStub, CustomTranslator> lp{ DestinationStub{} };
  }
  {
    Proxy<DestinationStub, CustomTranslator> lp{ CustomTranslator{} };
  }
  {
    Proxy<DestinationStub, CustomTranslator> lp{ DestinationStub{}, CustomTranslator{} };
  }
}

}
