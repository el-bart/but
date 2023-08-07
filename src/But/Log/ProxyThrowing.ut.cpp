#include <memory>
#include <vector>
#include <But/Log/ProxyThrowing.hpp>
#include <But/Log/Destination/Sink.hpp>
#include <nlohmann/json.hpp>
#include <gtest/gtest.h>

using But::Log::ProxyThrowing;
using But::Log::Destination::Sink;
using But::Log::Field::FormattedString;
using json = nlohmann::ordered_json;

namespace
{

struct TestSink final: public Sink
{
  void logImpl(std::string&& str) override
  {
    logs_.push_back( std::move(str) );
  }

  void reloadImpl() override { }
  void flushImpl() override { }

  json parse(size_t index) const
  {
    return json::parse( logs_.at(index) );
  }

  std::vector<std::string> logs_;
};


struct ButLogProxyThrowing: public testing::Test
{
  But::NotNullShared<TestSink> sink_{ But::makeSharedNN<TestSink>() };
  ProxyThrowing<> pt_{sink_};
};


TEST_F(ButLogProxyThrowing, NoArgumentsToLog)
{
  pt_.log("test");
  EXPECT_EQ( sink_->parse(0), ( json{ {"message", "test"} } ) );
}


#if 0
TEST_F(ButLogProxyThrowing, LoggingSimpleValuesOneAtATime)
{
  pt_.log("m", 42);
  pt_.log("m");
  pt_.log("m", 3.14);
  EXPECT_EQ( sink_->parse(0), ( json{ {"message", "m"}, {"named", 42} } ) );
  EXPECT_EQ( sink_->parse(1), ( json{ {"message", "m"} } ) );
  // TODO
}


TEST_F(ButLogProxyThrowing, LoggingMultipleSimpleValuesAtOnce)
{
  ProxyThrowing<> log{ But::makeSharedNN<TestSink>(buffer_) };
  log.log(42, "foo", 3.14);
  EXPECT_EQ( buffer_.str(), "int='42' | string='foo' | double='3.14' | " );
}


TEST_F(ButLogProxyThrowing, SinkTypeValueLogging)
{
  ProxyThrowing<> log{ But::makeSharedNN<TestSink>(buffer_) };
  log.log(42, "foo", 'a');
  EXPECT_EQ( buffer_.str(), "int='42' | string='foo' | string='a' | ");
}


TEST_F(ButLogProxyThrowing, SinkFormattedLogging)
{
  ProxyThrowing<> log{ But::makeSharedNN<TestSink>(buffer_) };
  log.log( BUT_FORMAT("${0} = $1"), "answer", 42 );
  EXPECT_EQ( buffer_.str(), "But::Formatted='answer = 42' | string='answer' | int='42' | ");
}


struct SomeThrowingType { };
FieldInfo toFieldInfo(SomeThrowingType const&) { throw std::runtime_error{"this one is ignored"}; }

TEST_F(ButLogProxyThrowing, InternalExceptionsArePropagatedToCaller)
{
  ProxyThrowing<> log{ But::makeSharedNN<TestSink>(buffer_) };
  EXPECT_THROW( log.log( SomeThrowingType{} ), std::runtime_error );
}


TEST_F(ButLogProxyThrowing, LoggerIsConst)
{
  const ProxyThrowing<> log{ But::makeSharedNN<TestSink>(buffer_) };
  log.log(42);
}


TEST_F(ButLogProxyThrowing, LogReloadingIsForwarder)
{
  auto dst = But::makeSharedNN<TestSink>(buffer_);
  ProxyThrowing<> log{dst};
  EXPECT_EQ( 0u, dst->reloads_ );
  log.reload();
  EXPECT_EQ( 1u, dst->reloads_ );
}


TEST_F(ButLogProxyThrowing, LogFlushingIsForwarder)
{
  auto dst = But::makeSharedNN<TestSink>(buffer_);
  ProxyThrowing<> log{dst};
  EXPECT_EQ( 0u, dst->flushes_ );
  log.flush();
  EXPECT_EQ( 1u, dst->flushes_ );
}


struct ThrowingDestination final: public Sink
{
  void logImpl(FieldInfo const&) { throw std::runtime_error{"ignored"}; }
  void reloadImpl() { throw std::runtime_error{"ignored"}; }
  void flushImpl()  { throw std::runtime_error{"ignored"}; }
};

TEST_F(ButLogProxyThrowing, AllErrorsFromActualDestinationsAreForwarded)
{
  ProxyThrowing<> log{ But::makeSharedNN<ThrowingDestination>() };
  EXPECT_THROW( log.log("hello", "john"), std::runtime_error );
  EXPECT_THROW( log.reload(), std::runtime_error  );
  EXPECT_THROW( log.flush(), std::runtime_error  );
}


struct Answer
{
  int value_{42};
};

inline auto toFieldInfo(const Answer a)
{
  using But::Log::Backend::toFieldInfo;
  return toFieldInfo(a.value_).retag(Tag{"Answer"});
}

TEST_F(ButLogProxyThrowing, FormattingNonStandardTypes)
{
  ProxyThrowing<> log{ But::makeSharedNN<TestSink>(buffer_) };
  log.log( BUT_FORMAT("$0 says $1"), std::string{"computer"}, Answer{} );
  EXPECT_EQ( buffer_.str(), "But::Formatted='computer says Answer=42' | string='computer' | Answer='42' | " );
}


struct Misc
{
  int value_{42};
};

inline auto toFieldInfo(const Misc m)
{
  using But::Log::Backend::toFieldInfo;
  return toFieldInfo(m.value_).retag(Tag{"Misc"});
}

inline auto toString(const Misc m)
{
  return std::to_string(m.value_);
}

TEST_F(ButLogProxyThrowing, FormattingNonStandardTypesWithToStringFreeFunction)
{
  ProxyThrowing<> log{ But::makeSharedNN<TestSink>(buffer_) };
  log.log( BUT_FORMAT("$0 says $1"), std::string{"computer"}, Misc{} );
  EXPECT_EQ( buffer_.str(), "But::Formatted='computer says 42' | string='computer' | Misc='42' | " );
}


TEST_F(ButLogProxyThrowing, FormattingInTheMiddleOfArgumentsList)
{
  ProxyThrowing<> log{ But::makeSharedNN<TestSink>(buffer_) };
  log.log( "gues", "what!", BUT_FORMAT("$0 says $1"), std::string{"computer"}, int{42} );
  EXPECT_EQ( buffer_.str(), "string='gues' | string='what!' | But::Formatted='computer says 42' | string='computer' | int='42' | " );
}


struct CustomTranslator
{
  template<size_t N, size_t M>
  auto translate(But::Format::ParsedCompiletime<N,M>&& parsed) const
  {
    ++counter_;
    if(throws_)
      throw std::runtime_error{"translation error, as requested"};
    return std::move(parsed);
  }

  auto& operator->() const { return *this; }

  bool throws_{false};
  mutable unsigned counter_{0};
};


TEST_F(ButLogProxyThrowing, TranslationsAreNotAffectingNonFormattedLogs)
{
  CustomTranslator ct;
  ProxyThrowing<CustomTranslator const*> log{ But::makeSharedNN<TestSink>(buffer_), &ct };
  EXPECT_EQ(0u, ct.counter_);
  log.log("not affected");
  EXPECT_EQ(0u, ct.counter_);
}


TEST_F(ButLogProxyThrowing, UsingCustomTranslator)
{
  CustomTranslator ct;
  ProxyThrowing<CustomTranslator const*> log{ But::makeSharedNN<TestSink>(buffer_), &ct };
  EXPECT_EQ(0u, ct.counter_);
  log.log( BUT_FORMAT("test") );
  EXPECT_EQ(1u, ct.counter_);
}


TEST_F(ButLogProxyThrowing, ExceptionsFromTranslationsArePropagated)
{
  CustomTranslator ct{true};
  ProxyThrowing<CustomTranslator const*> log{ But::makeSharedNN<TestSink>(buffer_), &ct };
  EXPECT_THROW( log.log( BUT_FORMAT("test $0"), "xx" ), std::runtime_error );
}


TEST_F(ButLogProxyThrowing, CopyableAndMovable)
{
  using Logger = ProxyThrowing<>;

  EXPECT_TRUE( std::is_copy_constructible<Logger>::value );
  EXPECT_TRUE( std::is_copy_assignable<Logger>::value );

  EXPECT_TRUE( std::is_move_constructible<Logger>::value );
  EXPECT_TRUE( std::is_move_assignable<Logger>::value );
}


TEST_F(ButLogProxyThrowing, ProxyWithDefaultFields)
{
  const auto log = ProxyThrowing<>{ But::makeSharedNN<TestSink>(buffer_) };
  const auto proxy = log.withFields("foo", 42);
  proxy.log("narf");
  EXPECT_EQ( buffer_.str(), "string='foo' | int='42' | string='narf' | " );
  proxy.log("yay!!!");
  EXPECT_EQ( buffer_.str(), "string='foo' | int='42' | string='narf' | "
                            "string='foo' | int='42' | string='yay!!!' | " );
}


TEST_F(ButLogProxyThrowing, ProxyWithDefaultFieldsGetsDerived)
{
  const auto log = ProxyThrowing<>{ But::makeSharedNN<TestSink>(buffer_) };
  const auto tmp = log.withFields("foo");
  const auto proxy = tmp.withFields(42);
  proxy.log("narf");
  EXPECT_EQ( buffer_.str(), "string='foo' | int='42' | string='narf' | " );
}


TEST_F(ButLogProxyThrowing, ProxyWithDefaultFieldsGetsDerivedButDoesNotAffectBase)
{
  const auto log = ProxyThrowing<>{ But::makeSharedNN<TestSink>(buffer_) };
  const auto proxy= log.withFields("foo");
  const auto other= proxy.withFields(42);
  proxy.log("narf");
  EXPECT_EQ( buffer_.str(), "string='foo' | string='narf' | " );
}

#endif

}
