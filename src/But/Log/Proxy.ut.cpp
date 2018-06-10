#include <memory>
#include <sstream>
#include <gtest/gtest.h>
#include <But/Log/Proxy.hpp>

using But::Log::Proxy;
using But::Log::Backend::FieldInfo;
using But::Log::Destination::Sink;

namespace
{

struct DestinationStub final: Sink
{
  void logImpl(FieldInfo const&) override { }
  void reloadImpl() override { }
  void flushImpl() override { }
};


struct ButLogProxy: public testing::Test
{
  Proxy<> lp_{ But::makeSharedNN<DestinationStub>() };
};


TEST_F(ButLogProxy, LoggingApiSmokeTest)
{
  lp_.log("the", "answer", "is", 42);
  lp_.log( BUT_FORMAT("$0 is ok"), "pinky" );
}


TEST_F(ButLogProxy, LoggerIsMovable)
{
  auto other = std::move(lp_);
  (void)other;
}


struct ThrowingDestination final: Sink
{
  void logImpl(FieldInfo const&) override { throw std::runtime_error{"ignored"}; }
  void reloadImpl() override { throw std::runtime_error{"ignored"}; }
  void flushImpl() override { throw std::runtime_error{"ignored"}; }
};

TEST_F(ButLogProxy, AllErrorsFromActualDestinationsAreIgnored)
{
  Proxy<> log{ But::makeSharedNN<ThrowingDestination>() };
  EXPECT_NO_THROW( log.log("hello", "john") );
  EXPECT_NO_THROW( log.reload() );
  EXPECT_NO_THROW( log.flush() );
}


struct CustomTranslator
{
  template<size_t N, size_t M>
  auto translate(But::Format::ParsedCompiletime<N,M>&& parsed) const { return parsed; }
};

TEST_F(ButLogProxy, ConstructorsForDifferentObjects)
{
  const auto dst = But::makeSharedNN<DestinationStub>();
  {
    Proxy<CustomTranslator> lp{dst};
  }
  {
    Proxy<CustomTranslator> lp{ dst, CustomTranslator{} };
  }
}


TEST_F(ButLogProxy, ProxyWithDefaultFieldsSmokeTest)
{
  auto proxy = lp_.withFields("foo", "bar");
  proxy.log(42);
}

}
