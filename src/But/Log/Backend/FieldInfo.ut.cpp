#include <sstream>
#include "gtest/gtest.h"
#include "FieldInfo.hpp"

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
  EXPECT_EQ( Tag{"int"}, fi.type() );
  EXPECT_EQ( 42, fi.value().get<int64_t>() );
}


TEST_F(ButLogBackendFieldInfo, ExplicitlyProvidedTypeName)
{
  const auto fi = FieldInfo{ Tag{"type"}, Value{"value"} };
  EXPECT_EQ( fi.type(), Tag{"type"} );
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
    EXPECT_EQ( ">one:1", v.ss_.str() );
  }
  {
    Visitor v;
    fi3.visit(v);
    EXPECT_EQ( ">three:[>one:1,>two:2,]", v.ss_.str() );
  }
  {
    Visitor v;
    fi4.visit(v);
    EXPECT_EQ( ">four:[>one:1,>three:[>one:1,>two:2,],]", v.ss_.str() );
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

}
