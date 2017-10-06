#include <memory>
#include <sstream>
#include "gtest/gtest.h"
#include "ProxyThrowing.hpp"
#include "Destination/Foreign.hpp"

using But::Log::ProxyThrowing;
using But::Log::Destination::Foreign;
using But::Log::Backend::Type;
using But::Log::Backend::Value;
using But::Log::Backend::FieldInfo;
using But::Log::Field::FormattedString;

namespace
{

struct VisitStream final
{
  template<typename T>
  void operator()(T const& t)
  {
    assert(ss_);
    (*ss_) << t;
  }

  std::stringstream* ss_{nullptr};
};

struct TestForeignDestination final: public Foreign
{
  explicit TestForeignDestination(std::stringstream& ss): ss_{&ss} { }

  void logImpl(FieldInfo const& fi) override
  {
    for( auto& f: fi.array() )
    {
      (*ss_) << f.type() << "=";
      VisitStream vs{ss_};
      f.value().visit(vs);
      (*ss_) << " ";
    }
  }

  void reloadImpl() override { }
  void flushImpl() override { }

  std::stringstream* ss_{nullptr};
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


struct ButLogProxyThrowing: public testing::Test
{
  std::stringstream buffer_;
};


TEST_F(ButLogProxyThrowing, LoggingSimpleValuesOneAtATime)
{
  ProxyThrowing<TestNativeDestination> log{ TestNativeDestination{buffer_} };
  log.log(42);
  log.log("foo");
  log.log(3.14);
  EXPECT_EQ( buffer_.str(), "int|string|double|" );
}


TEST_F(ButLogProxyThrowing, LoggingMultipleSimpleValuesAtOnce)
{
  ProxyThrowing<TestNativeDestination> log{ TestNativeDestination{buffer_} };
  log.log(42, "foo", 3.14);
  EXPECT_EQ( buffer_.str(), "int,string,double|");
}


TEST_F(ButLogProxyThrowing, LoggingViaSmartPointerToDestination)
{
  ProxyThrowing< std::shared_ptr<TestNativeDestination> > log{ std::make_shared<TestNativeDestination>(buffer_) };
  log.log(42, "foo", 3.14);
  EXPECT_EQ( buffer_.str(), "int,string,double|");
}


TEST_F(ButLogProxyThrowing, ForeignTypeValueLogging)
{
  ProxyThrowing<std::unique_ptr<TestForeignDestination>> log{ std::make_unique<TestForeignDestination>(buffer_) };
  log.log(42, "foo", 'a');
  EXPECT_EQ( buffer_.str(), "int=42 string=foo string=a ");
}


TEST_F(ButLogProxyThrowing, ForeignFormattedLogging)
{
  ProxyThrowing<std::unique_ptr<TestForeignDestination>> log{ std::make_unique<TestForeignDestination>(buffer_) };
  log.log( BUT_FORMAT("${0} = $1"), "answer", 42 );
  EXPECT_EQ( buffer_.str(), "answer = 42 @@ string=answer int=42 ");
}


struct SomeThrowingType { };
FieldInfo toFieldInfo(SomeThrowingType const&) { throw std::runtime_error{"this one is ignored"}; }

TEST_F(ButLogProxyThrowing, InternalExceptionsArePropagatedToCaller)
{
  ProxyThrowing<std::unique_ptr<TestForeignDestination>> log{ std::make_unique<TestForeignDestination>(buffer_) };
  EXPECT_THROW( log.log( SomeThrowingType{} ), std::runtime_error );
}


TEST_F(ButLogProxyThrowing, LoggerIsConst)
{
  const ProxyThrowing<std::unique_ptr<TestForeignDestination>> log{ std::make_unique<TestForeignDestination>(buffer_) };
  log.log(42);
}


TEST_F(ButLogProxyThrowing, LoggerIsMovable)
{
  ProxyThrowing< std::unique_ptr<TestNativeDestination> > log{ std::make_unique<TestNativeDestination>(buffer_) };
  auto other = std::move(log);
}


TEST_F(ButLogProxyThrowing, LogReloadingIsForwarder)
{
  TestNativeDestination dst{buffer_};
  ProxyThrowing<TestNativeDestination*> log{&dst};
  EXPECT_EQ( 0u, dst.reloads_ );
  log.reload();
  EXPECT_EQ( 1u, dst.reloads_ );
}


TEST_F(ButLogProxyThrowing, LogFlushingIsForwarder)
{
  TestNativeDestination dst{buffer_};
  ProxyThrowing<TestNativeDestination*> log{&dst};
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

TEST_F(ButLogProxyThrowing, AllErrorsFromActualDestinationsAreForwarded)
{
  ProxyThrowing<ThrowingDestination> log{ ThrowingDestination{} };
  EXPECT_THROW( log.log("hello", "john"), std::runtime_error );
  EXPECT_THROW( log.reload(), std::runtime_error  );
  EXPECT_THROW( log.flush(), std::runtime_error  );
}


TEST_F(ButLogProxyThrowing, ExplicitFormatting)
{
  ProxyThrowing<std::unique_ptr<TestNativeDestination>> log{ std::make_unique<TestNativeDestination>(buffer_) };
  log.log( BUT_FORMAT("$0 says $1"), std::string{"computer"}, int{42} );
  EXPECT_EQ( buffer_.str(), "[formatted:computer says 42],string,int|" );
}


TEST_F(ButLogProxyThrowing, AllowDefaultConstructionWhenNoArgumentsNeeded)
{
  ProxyThrowing<DestinationStub, But::Log::Localization::None> log;
  log.log("foo", "bar");
}


struct CustomTranslator
{
  template<size_t N, size_t M>
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


TEST_F(ButLogProxyThrowing, TranslationsAreNotAffectingNonFormattedLogs)
{
  CustomTranslator ct;
  ProxyThrowing<DestinationStub, CustomTranslator const*> log{&ct};
  EXPECT_EQ(0u, ct.counter_);
  log.log("not affected");
  EXPECT_EQ(0u, ct.counter_);
}


TEST_F(ButLogProxyThrowing, BuildingWithDefaultDestinationAndCustomTranslator)
{
  CustomTranslator ct;
  ProxyThrowing<DestinationStub, CustomTranslator const*> log{&ct};
  EXPECT_EQ(0u, ct.counter_);
  log.log( BUT_FORMAT("test") );
  EXPECT_EQ(1u, ct.counter_);
}


TEST_F(ButLogProxyThrowing, UsingCustomTranslator)
{
  CustomTranslator ct;
  ProxyThrowing<DestinationStub, CustomTranslator const*> log{ DestinationStub{}, &ct };
  EXPECT_EQ(0u, ct.counter_);
  log.log( BUT_FORMAT("test") );
  EXPECT_EQ(1u, ct.counter_);
}


TEST_F(ButLogProxyThrowing, ExceptionsFromTranslationsAreNotPropagated)
{
  CustomTranslator ct{true};
  TestNativeDestination dst{buffer_};
  ProxyThrowing<TestNativeDestination*, CustomTranslator const*> log{&dst, &ct};
  EXPECT_NO_THROW( log.log( BUT_FORMAT("test $0"), "xx" ) );
  EXPECT_EQ( buffer_.str(), "[formatted:test xx],string|" );
}

}
