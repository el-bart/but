#include <memory>
#include <sstream>
#include "gtest/gtest.h"
#include "LoggerProxy.hpp"
#include "Destination/Foreign.hpp"

using But::Log::LoggerProxy;
using But::Log::Destination::Foreign;
using But::Log::Backend::Entry;
using But::Log::Field::FormattedString;

namespace
{

struct TestForeignDestination final: public Foreign
{
  explicit TestForeignDestination(std::stringstream& ss): ss_{&ss} { }

  void logImpl(Entry const& e) override
  {
    for(auto& f: e)
      (*ss_) << f.type() << "=" << f.value() << " ";
  }

  void logImpl(FormattedString const& str, Entry const& e) override
  {
    (*ss_) << str.value_ << " @@ ";
    for(auto& f: e)
      (*ss_) << f.type() << "=" << f.value() << " ";
  }

  void reloadImpl() override { }
  void flushImpl() override { }

  std::stringstream* ss_;
};


struct TestNativeDestination final
{
  explicit TestNativeDestination(std::stringstream& ss): ss_{&ss} { }

  void log(int) { *ss_ << "int|"; }
  void log(std::string const&) { *ss_ << "string|"; }
  void log(double) { *ss_ << "double|"; }
  void log(int, std::string const&, double) { *ss_ << "int,string,double|"; }
  void log(FormattedString const& fs, std::string const&) { *ss_ << "[formatted:" << fs.value_ << "],string|"; }
  void log(FormattedString const& fs, std::string const&, int) { *ss_ << "[formatted:" << fs.value_ << "],string,int|"; }

  auto operator->() { return this; }

  void reload() { ++reloads_; }
  void flush() { ++flushes_; }

  std::stringstream* ss_;
  unsigned reloads_{0};
  unsigned flushes_{0};
};


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


TEST_F(ButLogLoggerProxy, LoggingViaSmartPointerToDestination)
{
  LoggerProxy< std::shared_ptr<TestNativeDestination> > log{ std::make_shared<TestNativeDestination>(buffer_) };
  log.log(42, "foo", 3.14);
  EXPECT_EQ( buffer_.str(), "int,string,double|");
}


TEST_F(ButLogLoggerProxy, ForeignTypeValueLogging)
{
  LoggerProxy<std::unique_ptr<TestForeignDestination>> log{ std::make_unique<TestForeignDestination>(buffer_) };
  log.log(42, "foo", 'a');
  EXPECT_EQ( buffer_.str(), "int=42 string=foo int=a ");
}


TEST_F(ButLogLoggerProxy, ForeignFormattedLogging)
{
  LoggerProxy<std::unique_ptr<TestForeignDestination>> log{ std::make_unique<TestForeignDestination>(buffer_) };
  log.log( BUT_FORMAT("${0} = $1"), "answer", 42 );
  EXPECT_EQ( buffer_.str(), "answer = 42 @@ string=answer int=42 ");
}


struct SomeThrowingType { };
std::string toString(SomeThrowingType const&) { throw std::runtime_error{"this one is ignored"}; }

TEST_F(ButLogLoggerProxy, InternalExceptionsAreNotPropagatedToCaller)
{
  LoggerProxy<std::unique_ptr<TestForeignDestination>> log{ std::make_unique<TestForeignDestination>(buffer_) };
  EXPECT_NO_THROW( log.log( SomeThrowingType{} ) );
}


TEST_F(ButLogLoggerProxy, LoggerIsConst)
{
  const LoggerProxy<std::unique_ptr<TestForeignDestination>> log{ std::make_unique<TestForeignDestination>(buffer_) };
  log.log(42);
}


TEST_F(ButLogLoggerProxy, LoggerIsMovable)
{
  LoggerProxy< std::unique_ptr<TestNativeDestination> > log{ std::make_unique<TestNativeDestination>(buffer_) };
  auto other = std::move(log);
}


TEST_F(ButLogLoggerProxy, LogReloadingIsForwarder)
{
  TestNativeDestination dst{buffer_};
  LoggerProxy<TestNativeDestination*> log{&dst};
  EXPECT_EQ( 0u, dst.reloads_ );
  log.reload();
  EXPECT_EQ( 1u, dst.reloads_ );
}


TEST_F(ButLogLoggerProxy, LogFlushingIsForwarder)
{
  TestNativeDestination dst{buffer_};
  LoggerProxy<TestNativeDestination*> log{&dst};
  EXPECT_EQ( 0u, dst.flushes_ );
  log.flush();
  EXPECT_EQ( 1u, dst.flushes_ );
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


TEST_F(ButLogLoggerProxy, ExplicitFormatting)
{
  LoggerProxy<std::unique_ptr<TestNativeDestination>> log{ std::make_unique<TestNativeDestination>(buffer_) };
  log.log( BUT_FORMAT("$0 says $1"), std::string{"computer"}, int{42} );
  EXPECT_EQ( buffer_.str(), "[formatted:computer says 42],string,int|" );
}


TEST_F(ButLogLoggerProxy, AllowDefaultConstructionWhenNoArgumentsNeeded)
{
  LoggerProxy<DestinationStub, But::Log::Localization::None> log;
  log.log("foo", "bar");
}


struct CustomTranslator
{
  template<unsigned N, unsigned M>
  auto translate(But::Format::Parsed<N,M>&& parsed) const
  {
    ++counter_;
    if(throws_)
      throw std::runtime_error{"translation error, as requested"};
    return std::move(parsed);
  }

  bool throws_{false};
  mutable unsigned counter_{0};
};


TEST_F(ButLogLoggerProxy, TranslationsAreNotAffectingNonFormattedLogs)
{
  CustomTranslator ct;
  LoggerProxy<DestinationStub, CustomTranslator const*> log{&ct};
  EXPECT_EQ(0u, ct.counter_);
  log.log("not affected");
  EXPECT_EQ(0u, ct.counter_);
}


TEST_F(ButLogLoggerProxy, BuildingWithDefaultDestinationAndCustomTranslator)
{
  CustomTranslator ct;
  LoggerProxy<DestinationStub, CustomTranslator const*> log{&ct};
  EXPECT_EQ(0u, ct.counter_);
  log.log( BUT_FORMAT("test") );
  EXPECT_EQ(1u, ct.counter_);
}


TEST_F(ButLogLoggerProxy, UsingCustomTranslator)
{
  CustomTranslator ct;
  LoggerProxy<DestinationStub, CustomTranslator const*> log{ DestinationStub{}, &ct };
  EXPECT_EQ(0u, ct.counter_);
  log.log( BUT_FORMAT("test") );
  EXPECT_EQ(1u, ct.counter_);
}


TEST_F(ButLogLoggerProxy, ExceptionsFromTranslationsAreNotPropagated)
{
  CustomTranslator ct{true};
  TestNativeDestination dst{buffer_};
  LoggerProxy<TestNativeDestination*, CustomTranslator const*> log{&dst, &ct};
  EXPECT_NO_THROW( log.log( BUT_FORMAT("test $0"), "xx" ) );
  EXPECT_EQ( buffer_.str(), "[formatted:test xx],string|" );
}

}
