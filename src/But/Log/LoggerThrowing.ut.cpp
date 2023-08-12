#include <memory>
#include <vector>
#include <But/Log/LoggerThrowing.hpp>
#include <But/Log/Field/tag.hpp>
#include <But/Log/Backend/unifyJson.ut.hpp>
#include <But/Log/Destination/FakeSink.ut.hpp>
#include <nlohmann/json.hpp>
#include <gtest/gtest.h>

using But::Log::LoggerThrowing;
using But::Log::Destination::Sink;
using But::Log::Destination::FakeSink;
using But::Log::Field::tag;
using But::Log::Field::FormattedString;
using nlohmann::json;

namespace
{

struct ButLogLoggerThrowing: public testing::Test
{
  But::NotNullShared<FakeSink> sink_{ But::makeSharedNN<FakeSink>() };
  LoggerThrowing<> pt_{sink_};
};


struct Integer
{
  int value_;
};
constexpr auto fieldName(Integer const*) { return "Integer"; }
auto fieldValue(Integer const& n) { return n.value_; }

struct Float
{
  double value_;
};
constexpr auto fieldName(Float const*) { return "Float"; }
auto fieldValue(Float const& n) { return n.value_; }

struct Aggregate
{
  int a_{0};
  int b_{0};
};
constexpr auto fieldName(Aggregate const*) { return "Aggregate"; }
void objectValue(But::Log::Backend::EntryProxy& proxy, Aggregate const& a)
{
  proxy.value("a", a.a_);
  proxy.value("b", a.b_);
}

struct Array
{
  int values_[3];
};
constexpr auto fieldName(Array const*) { return "Array"; }
void arrayValue(But::Log::Backend::EntryArray& array, Array const& a)
{
  array.add( std::begin(a.values_), std::end(a.values_) );
}

struct String
{
  std::string value_;
};
constexpr auto fieldName(String const*) { return "String"; }
auto fieldValue(String const& n) { return n.value_; }

struct StringView
{
  std::string_view value_;
};
constexpr auto fieldName(StringView const*) { return "StringView"; }
auto fieldValue(StringView const& n) { return n.value_; }


TEST_F(ButLogLoggerThrowing, NoArgumentsToLog)
{
  pt_.log("test");
  EXPECT_EQ_JSON( sink_->parse(0), ( json{ {"message", "test"} } ) );
}


TEST_F(ButLogLoggerThrowing, LoggingSimpleValuesOneAtATime)
{
  {
    pt_.log("m", Integer{42});
    EXPECT_EQ_JSON( sink_->parse(0), ( json{ {"message", "m"}, {"Integer", 42} } ) );
  }
  {
    pt_.log("m");
    EXPECT_EQ_JSON( sink_->parse(1), ( json{ {"message", "m"} } ) );
  }
  {
    pt_.log("m", Float{3.14});
    EXPECT_EQ_JSON( sink_->parse(2), ( json{ {"message", "m"}, {"Float", 3.14} } ) );
  }
  {
    pt_.log("m", Aggregate{13, 997});
    auto exp = json{ {"message", "m"} };
    exp["Aggregate"]["a"] = 13;
    exp["Aggregate"]["b"] = 997;
    EXPECT_EQ_JSON( sink_->parse(3), exp );
  }
  {
    pt_.log("m", Array{1,2,3});
    EXPECT_EQ_JSON( sink_->parse(4), ( json{ {"message", "m"}, {"Array", std::vector<int>{1,2,3}} } ) );
  }
}


TEST_F(ButLogLoggerThrowing, LoggingMultipleSimpleValuesAtOnce)
{
  pt_.log("foo", Float{3.14}, Integer{42});
  EXPECT_EQ_JSON( sink_->parse(0), ( json{ {"message", "foo"}, {"Integer", 42}, {"Float", 3.14} } ) );
}


TEST_F(ButLogLoggerThrowing, SinkFormattedLogging)
{
  pt_.log( BUT_FORMAT("${0} != $1"), Float{13}, Integer{42} );
  auto expected = json{
            {"message", "${0} != $1"},
            {"Integer", 42},
            {"Float", 13.0}
    };
  {
    auto array = json::array();
    array.push_back(13.0);
    array.push_back(42);
    expected["But::Format"]["args"] = std::move(array);
    expected["But::Format"]["format"] = "${0} != $1";
    expected["But::Format"]["But::FormattedString"] = "13.000000 != 42";
  }
  EXPECT_EQ_JSON( sink_->parse(0), expected );
}


TEST_F(ButLogLoggerThrowing, FormattedLoggingOfAggregateIsReadable)
{
  pt_.log( BUT_FORMAT("${0} != $1"), Aggregate{997, 51}, Integer{42} );
  auto expected = json{
            {"message", "${0} != $1"},
            {"Integer", 42},
    };
  expected["Aggregate"]["a"] = 997;
  expected["Aggregate"]["b"] = 51;
  {
    auto array = json::array();
    array.push_back( json{ {"a", 997}, {"b", 51} } );
    array.push_back(42);
    expected["But::Format"]["args"] = std::move(array);
    expected["But::Format"]["format"] = "${0} != $1";
    expected["But::Format"]["But::FormattedString"] = R"({"Aggregate":{"a":997,"b":51}} != 42)";
  }
  EXPECT_EQ_JSON( sink_->parse(0), expected );
}


struct SomeThrowingType { };
constexpr auto fieldName(SomeThrowingType const*) { return "SomeThrowingType"; }
bool fieldValue(SomeThrowingType const&) { throw std::runtime_error{"this one is ignored"}; }

TEST_F(ButLogLoggerThrowing, InternalExceptionsArePropagatedToCaller)
{
  EXPECT_THROW( pt_.log("let's throw", SomeThrowingType{}), std::runtime_error );
}


TEST_F(ButLogLoggerThrowing, LoggerIsConst)
{
  auto const& pt = pt_;
  pt.log("doh");
}


TEST_F(ButLogLoggerThrowing, LogReloadingIsForwarder)
{
  EXPECT_EQ( 0u, sink_->reloads_ );
  pt_.reload();
  EXPECT_EQ( 1u, sink_->reloads_ );
}


TEST_F(ButLogLoggerThrowing, LogFlushingIsForwarder)
{
  EXPECT_EQ( 0u, sink_->flushes_ );
  pt_.flush();
  EXPECT_EQ( 1u, sink_->flushes_ );
}


struct ThrowingDestination final: public Sink
{
  void logImpl(std::string &&) override { throw std::runtime_error{"ignored"}; }
  void reloadImpl() override { throw std::runtime_error{"ignored"}; }
  void flushImpl()  override { throw std::runtime_error{"ignored"}; }
};

TEST_F(ButLogLoggerThrowing, AllErrorsFromActualDestinationsAreForwarded)
{
  LoggerThrowing<> log{ But::makeSharedNN<ThrowingDestination>() };
  EXPECT_THROW( log.log("hello"), std::runtime_error );
  EXPECT_THROW( log.reload(), std::runtime_error  );
  EXPECT_THROW( log.flush(), std::runtime_error  );
}


struct Misc
{
  int value_{0};
};
constexpr auto fieldName(Misc const*) { return "Misc"; }
int fieldValue(Misc const& m) { return m.value_; }
inline auto toString(Misc const m) { return "answer_" + std::to_string(m.value_); }

TEST_F(ButLogLoggerThrowing, FormattingNonStandardTypesWithToStringFreeFunction)
{
  pt_.log( BUT_FORMAT("computer says $0"), Misc{42} );
  auto expected = json{
            {"message", "computer says $0"},
            {"Misc", 42}
    };
  {
    auto array = json::array();
    array.push_back(42);
    expected["But::Format"]["args"] = std::move(array);
    expected["But::Format"]["format"] = "computer says $0";
    expected["But::Format"]["But::FormattedString"] = "computer says answer_42";
  }
  EXPECT_EQ_JSON( sink_->parse(0), expected );
}


struct CustomTranslator
{
  template<size_t N, size_t M>
  auto translate(But::Format::ParsedCompiletime<N,M> const& parsed) const
  {
    ++counter_;
    if(throws_)
      throw std::runtime_error{"translation error, as requested"};
    return parsed;
  }

  auto& operator->() const { return *this; }

  bool throws_{false};
  mutable unsigned counter_{0};
};


TEST_F(ButLogLoggerThrowing, TranslationsAreNotAffectingNonFormattedLogs)
{
  CustomTranslator ct;
  LoggerThrowing<CustomTranslator const*> pt{sink_, &ct};
  EXPECT_EQ(0u, ct.counter_);
  pt.log("not affected");
  EXPECT_EQ(0u, ct.counter_);
}


TEST_F(ButLogLoggerThrowing, UsingCustomTranslator)
{
  CustomTranslator ct;
  LoggerThrowing<CustomTranslator const*> pt{sink_, &ct};
  EXPECT_EQ(0u, ct.counter_);
  pt.log( BUT_FORMAT("test") );
  EXPECT_EQ(1u, ct.counter_);
}


TEST_F(ButLogLoggerThrowing, ExceptionsFromTranslationsArePropagated)
{
  CustomTranslator ct{true};
  LoggerThrowing<CustomTranslator const*> pt{sink_, &ct};
  EXPECT_THROW( pt.log( BUT_FORMAT("test $0"), Integer{42} ), std::runtime_error );
}


TEST_F(ButLogLoggerThrowing, StringsAreCorrectlyFormatted)
{
  CustomTranslator ct{true};
  LoggerThrowing<CustomTranslator const*> pt{sink_, &ct};
  EXPECT_THROW( pt.log( BUT_FORMAT("test $0"), Integer{42} ), std::runtime_error );

  pt_.log( BUT_FORMAT("${0} != $1"), String{"foo"}, StringView{"bar"} );
  auto expected = json{
            {"message", "${0} != $1"},
            {"String", "foo"},
            {"StringView", "bar"}
    };
  {
    auto array = json::array();
    array.push_back("foo");
    array.push_back("bar");
    expected["But::Format"]["args"] = std::move(array);
    expected["But::Format"]["format"] = "${0} != $1";
    expected["But::Format"]["But::FormattedString"] = R"(foo != bar)";
  }
  EXPECT_EQ_JSON( sink_->parse(0), expected );
}


TEST_F(ButLogLoggerThrowing, CopyableAndMovable)
{
  using Logger = LoggerThrowing<>;

  EXPECT_TRUE( std::is_copy_constructible<Logger>::value );
  EXPECT_TRUE( std::is_copy_assignable<Logger>::value );

  EXPECT_TRUE( std::is_move_constructible<Logger>::value );
  EXPECT_TRUE( std::is_move_assignable<Logger>::value );
}


TEST_F(ButLogLoggerThrowing, ProxyWithDefaultFields)
{
  const auto cpt = pt_;
  const auto proxy = cpt.withFields(Integer{42}, Misc{13});
  proxy.log("narf");
  proxy.log("yay!!!");
  EXPECT_EQ_JSON( sink_->parse(0), ( json{ {"message", "narf"},   {"Integer", 42}, {"Misc", 13} } ) );
  EXPECT_EQ_JSON( sink_->parse(1), ( json{ {"message", "yay!!!"}, {"Integer", 42}, {"Misc", 13} } ) );
}


TEST_F(ButLogLoggerThrowing, ProxyWithDefaultFieldsGetsDerived)
{
  const auto tmp = pt_.withFields(Integer{42});
  const auto proxy = tmp.withFields(Misc{13});
  proxy.log("narf");
  EXPECT_EQ_JSON( sink_->parse(0), ( json{ {"message", "narf"}, {"Integer", 42}, {"Misc", 13} } ) );
}


TEST_F(ButLogLoggerThrowing, ProxyWithDefaultFieldsGetsDerivedButDoesNotAffectBase)
{
  const auto proxy = pt_.withFields(Integer{42});
  const auto other = proxy.withFields(Misc{13});
  proxy.log("narf");
  EXPECT_EQ_JSON( sink_->parse(0), ( json{ {"message", "narf"}, {"Integer", 42} } ) );
}


TEST_F(ButLogLoggerThrowing, LoggingElementByReferenceWorks)
{
  Integer n{42};
  pt_.log("ref is fine", n);
}


TEST_F(ButLogLoggerThrowing, LoggingDynamicallyTaggedValue)
{
  pt_.log("action!", tag("answer", 42));
  EXPECT_EQ_JSON( sink_->parse(0), ( json{ {"message", "action!"}, {"answer", 42} } ) );
}


TEST_F(ButLogLoggerThrowing, LoggingRepeatedDynamicallyTaggedValue)
{
  pt_.log("action!", tag("answer", 42), tag("foo", "bar"));
  EXPECT_EQ_JSON( sink_->parse(0), ( json{ {"message", "action!"}, {"answer", 42}, {"foo", "bar"} } ) );
}


TEST_F(ButLogLoggerThrowing, LoggingDynamicallyTaggedObject)
{
  pt_.log( "action!", tag("answer", Integer{42}) );
  auto exp = json{ {"message", "action!"} };
  exp["answer"]["Integer"] = 42;
  EXPECT_EQ_JSON( sink_->parse(0), exp );
}


TEST_F(ButLogLoggerThrowing, LoggingDynamicallyTaggedArray)
{
  pt_.log( "action!", tag("answer", Array{0,4,2}) );
  auto exp = json{ {"message", "action!"} };
  exp["answer"]["Array"] = std::vector<int>{0,4,2};
  EXPECT_EQ_JSON( sink_->parse(0), exp );
}


//
// NOTE: below tests should fail the compilation. commented out for obvious reasons...
//

#if 0
TEST_F(ButLogLoggerThrowing, RepeatedArgumentsCauseCompilationToFail)
{
  pt_.log("m", Integer{42}, Integer{13});
}
#endif

#if 0
TEST_F(ButLogLoggerThrowing, RepeatedArgumentsCauseCompilationToFail)
{
  (void)pt_.withFields(Integer{42}, Integer{13});
}
#endif

}
