#include <memory>
#include <sstream>
#include "gtest/gtest.h"
#include "LoggerProxy.hpp"
#include "Destination/Foreign.hpp"

using But::Log::LoggerProxy;
using But::Log::Destination::Foreign;
using But::Log::Backend::Entry;

namespace
{

struct TestForeignDestination final: public Foreign
{
  explicit TestForeignDestination(std::stringstream& ss): ss_{&ss} { }

  void logImpl(Entry e) override
  {
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

  auto operator->() { return this; }

  void reload() { ++reloads_; }
  void flush() { ++flushes_; }

  std::stringstream* ss_;
  unsigned reloads_{0};
  unsigned flushes_{0};
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
  EXPECT_EQ( buffer_.str(), "int=42 std::string=foo char=a ");
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

}
