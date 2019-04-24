#include <sstream>
#include <gtest/gtest.h>
#include <But/Log/Backend/FieldInfo.hpp>
#include <But/Log/Backend/toFieldInfo.hpp>

using But::Log::Backend::Tag;
using But::Log::Backend::Value;
using But::Log::Backend::FieldInfo;

namespace
{

struct ButLogBackendFieldInfo: public testing::Test
{ };


TEST_F(ButLogBackendFieldInfo, ImplicitSimpleType)
{
  const auto fi = FieldInfo{42};
  EXPECT_EQ( Tag{"int"}, fi.tag() );
  EXPECT_EQ( 42, fi.value().get<int64_t>() );
}


TEST_F(ButLogBackendFieldInfo, ExplicitlyProvidedTypeName)
{
  const auto fi = FieldInfo{ Tag{"type"}, Value{"value"} };
  EXPECT_EQ( fi.tag(), Tag{"type"} );
  EXPECT_EQ( fi.value().get<std::string>(), "value" );
}


TEST_F(ButLogBackendFieldInfo, Comparisons)
{
  const auto fi1 = FieldInfo{ Tag{"one"}, Value{"1"} };
  const auto fi2 = FieldInfo{ Tag{"two"}, Value{"2"} };
  EXPECT_TRUE(  fi1 == fi1 );
  EXPECT_FALSE( fi1 == fi2 );
  EXPECT_FALSE( fi1 != fi1 );
  EXPECT_TRUE(  fi1 != fi2 );
}


struct Visitor
{
  void operator()(Tag const& t, Value const& v)
  {
    ss_ << ">" << t << ":" << v;
  }
  void operator()(Tag const& t, std::vector<FieldInfo> const& fis)
  {
    ss_ << ">" << t << ":[";
    for(auto& e: fis)
    {
      e.visit(*this);
      ss_ << ",";
    }
    ss_ << "]";
  }

  std::stringstream ss_;
};

TEST_F(ButLogBackendFieldInfo, Visiting)
{
  const auto fi1 = FieldInfo{ Tag{"one"}, Value{"1"} };
  const auto fi2 = FieldInfo{ Tag{"two"}, Value{"2"} };
  const auto fi3 = FieldInfo{ Tag{"three"}, { fi1, fi2 } };
  const auto fi4 = FieldInfo{ Tag{"four"}, { fi1, fi3 } };
  {
    Visitor v;
    fi1.visit(v);
    EXPECT_EQ( ">one:\"1\"", v.ss_.str() );
  }
  {
    Visitor v;
    fi3.visit(v);
    EXPECT_EQ( ">three:[>one:\"1\",>two:\"2\",]", v.ss_.str() );
  }
  {
    Visitor v;
    fi4.visit(v);
    EXPECT_EQ( ">four:[>one:\"1\",>three:[>one:\"1\",>two:\"2\",],]", v.ss_.str() );
  }
}


TEST_F(ButLogBackendFieldInfo, GettingFieldTypesExplicitly)
{
  const auto fi1 = FieldInfo{ Tag{"one"}, Value{"1"} };
  const auto fi2 = FieldInfo{ Tag{"two"}, Value{"2"} };
  const auto fi3 = FieldInfo{ Tag{"three"}, { fi1, fi2 } };

  EXPECT_EQ( "1", fi1.value().get<std::string>() );
  EXPECT_EQ( "1", fi3.array().at(0).value().get<std::string>() );
}


TEST_F(ButLogBackendFieldInfo, RetaggingRvalue)
{
  const auto fi = FieldInfo{ Tag{"foo"}, Value{"1"} }.retag( Tag{"bar"} );

  EXPECT_EQ( "1", fi.value().get<std::string>() );
  EXPECT_EQ( "bar", fi.tag().str() );
}


struct Foo final
{
  std::string bar_{"bar"};
};

inline FieldInfo toFieldInfo(Foo foo)
{
  using But::Log::Backend::toFieldInfo;
  return toFieldInfo( std::move(foo.bar_) ).retag( Tag{"Foo"} );
}

TEST_F(ButLogBackendFieldInfo, StreammingOutSimpleDataStructure)
{
  const auto fi = toFieldInfo( Foo{} );
  std::stringstream ss;
  ss << fi;
  EXPECT_EQ( "Foo=\"bar\"", ss.str() );
}


struct Nested final
{
  int answer_{42};
  Foo foo_;
};

inline auto toFieldInfo(Nested n)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Tag{"Nested"}, { toFieldInfo(n.answer_).retag(Tag{"Answer"}), toFieldInfo( std::move(n.foo_) ) } };
}

TEST_F(ButLogBackendFieldInfo, StreammingOutNestedStructure)
{
  const auto fi = toFieldInfo( Nested{} );
  std::stringstream ss;
  ss << fi;
  EXPECT_EQ( "Nested={Answer=42,Foo=\"bar\"}", ss.str() );
}


TEST_F(ButLogBackendFieldInfo, ToStringConversion)
{
  const auto fi = toFieldInfo( Nested{} );
  EXPECT_EQ( "Nested={Answer=42,Foo=\"bar\"}", toString(fi) );
}


TEST_F(ButLogBackendFieldInfo, EMptyVectorLogsTag)
{
  using But::Log::Backend::toFieldInfo;
  const auto fi = toFieldInfo( std::vector<int>{} );
  EXPECT_EQ( "sequence={}", toString(fi) );
}


TEST_F(ButLogBackendFieldInfo, ConvertingVectorOfExplicitValues)
{
  using But::Log::Backend::toFieldInfo;
  const auto fi = toFieldInfo( std::vector<int>{1,2,3} );
  EXPECT_EQ( "sequence={int=1,int=2,int=3}", toString(fi) );
}


TEST_F(ButLogBackendFieldInfo, ExceptionsToFieldInfo)
{
  using But::Log::Backend::toFieldInfo;
  {
    const auto fi = toFieldInfo( static_cast<std::exception const&>( std::runtime_error{"testing..."} ) );
    EXPECT_EQ( "Exception={Type=\"std::runtime_error\",Message=\"testing...\"}", toString(fi) );
  }
  {
    const auto fi = toFieldInfo( std::runtime_error{"testing..."} );
    EXPECT_EQ( "Exception={Type=\"std::runtime_error\",Message=\"testing...\"}", toString(fi) );
  }
}


TEST_F(ButLogBackendFieldInfo, PairToFieldInfo)
{
  using But::Log::Backend::toFieldInfo;
  const auto fi = toFieldInfo( std::make_pair(42, false) );
  EXPECT_EQ( "tuple={int=42,bool=false}", toString(fi) );
}


TEST_F(ButLogBackendFieldInfo, EmptyTupleToFieldInfo)
{
  using But::Log::Backend::toFieldInfo;
  const auto fi = toFieldInfo( std::make_tuple() );
  EXPECT_EQ( "tuple={}", toString(fi) );
}


TEST_F(ButLogBackendFieldInfo, TupleToFieldInfo)
{
  using But::Log::Backend::toFieldInfo;
  const auto fi = toFieldInfo( std::make_tuple(42, "foo-bar", true) );
  EXPECT_EQ( "tuple={int=42,string=\"foo-bar\",bool=true}", toString(fi) );
}


TEST_F(ButLogBackendFieldInfo, MixingTemplateToFieldInfos)
{
  using But::Log::Backend::toFieldInfo;
  const auto t = std::make_tuple( std::make_pair(13, false) );
  const auto fi = toFieldInfo( std::vector<std::remove_cv_t<decltype(t)>>{t, t} );
  EXPECT_EQ( "sequence={tuple={tuple={int=13,bool=false}},tuple={tuple={int=13,bool=false}}}", toString(fi) );
}


template<typename T>
void testTimeUnit(T time, std::string const& unit)
{
  using But::Log::Backend::toFieldInfo;
  const auto fi = toFieldInfo(time);
  EXPECT_EQ( "time [" + unit + "]=42", toString(fi) );
}

TEST_F(ButLogBackendFieldInfo, ToFieldInfoNanoseconds)
{
  testTimeUnit( std::chrono::nanoseconds{42}, "ns" );
  testTimeUnit( std::chrono::microseconds{42}, "us" );
  testTimeUnit( std::chrono::milliseconds{42}, "ms" );
  testTimeUnit( std::chrono::seconds{42}, "s" );
  testTimeUnit( std::chrono::minutes{42}, "min" );
  testTimeUnit( std::chrono::hours{42}, "h" );
  /* TODO: waiting for C++20...
  testTimeUnit( std::chrono::days{42}, "d" );
  testTimeUnit( std::chrono::months{42}, "mon" );
  testTimeUnit( std::chrono::years{42}, "y" );
  */
}


struct CopySpy
{
  explicit CopySpy(int& copyCount): copyCount_{&copyCount} { }

  CopySpy(CopySpy const &other):
    copyCount_{other.copyCount_}
  {
    ++*copyCount_;
  }
  CopySpy &operator=(CopySpy const &)
  {
    ++*copyCount_;
    return *this;
  }

  CopySpy(CopySpy &&) = default;
  CopySpy &operator=(CopySpy &&) = default;

  int* copyCount_{nullptr};
};

FieldInfo toFieldInfo(CopySpy const &cs)
{
  return FieldInfo{ Tag{"copies_made"}, Value{*cs.copyCount_} };
}

TEST_F(ButLogBackendFieldInfo, RetaggingTemporaryShouldMove)
{
  using But::Log::Backend::toFieldInfo;

  {
    auto copies = 0;
    CopySpy{copies};
    std::make_tuple( CopySpy{copies} );
    EXPECT_EQ(0, copies);
  }

  {
    auto copies = 0;
    toFieldInfo( CopySpy{copies} );
    EXPECT_EQ(0, copies);
    toFieldInfo( CopySpy{copies} ).retag( Tag{"foo"} );
    EXPECT_EQ(0, copies);
  }

  {
    auto copies = 0;
    toFieldInfo( std::make_pair( CopySpy{copies}, true ) );
    EXPECT_EQ(0, copies);
    toFieldInfo( std::make_pair( CopySpy{copies}, true ) ).retag( Tag{"foo"} );
    EXPECT_EQ(0, copies);
  }

  {
    auto copies = 0;
    toFieldInfo( std::make_tuple( CopySpy{copies} ) );
    EXPECT_EQ(0, copies);
    toFieldInfo( std::make_tuple( CopySpy{copies} ) ).retag( Tag{"foo"} );
    EXPECT_EQ(0, copies);
  }
}

}
