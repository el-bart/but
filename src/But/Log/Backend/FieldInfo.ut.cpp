#include <sstream>
#include "gtest/gtest.h"
#include "FieldInfo.hpp"

using But::Log::Backend::Type;
using But::Log::Backend::Value;
using But::Log::Backend::FieldInfo;

namespace
{

struct ButLogBackendFieldInfo: public testing::Test
{ };


TEST_F(ButLogBackendFieldInfo, ImplicitSimpleType)
{
  const auto fi = FieldInfo{42};
  EXPECT_EQ( Type{"int"}, fi.type() );
  EXPECT_EQ( 42, fi.value().get<int64_t>() );
}


TEST_F(ButLogBackendFieldInfo, ExplicitlyProvidedTypeName)
{
  const auto fi = FieldInfo{ Type{"type"}, Value{"value"} };
  EXPECT_EQ( fi.type(), Type{"type"} );
  EXPECT_EQ( fi.value().get<std::string>(), "value" );
}


TEST_F(ButLogBackendFieldInfo, Comparisons)
{
  const auto fi1 = FieldInfo{ Type{"one"}, Value{"1"} };
  const auto fi2 = FieldInfo{ Type{"two"}, Value{"2"} };
  EXPECT_TRUE(  fi1 == fi1 );
  EXPECT_FALSE( fi1 == fi2 );
  EXPECT_FALSE( fi1 != fi1 );
  EXPECT_TRUE(  fi1 != fi2 );
}


struct Visitor
{
  void operator()(Type const& t, Value const& v)
  {
    ss_ << ">" << t << ":" << v;
  }
  void operator()(Type const& t, std::vector<FieldInfo> const& fis)
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
  const auto fi1 = FieldInfo{ Type{"one"}, Value{"1"} };
  const auto fi2 = FieldInfo{ Type{"two"}, Value{"2"} };
  const auto fi3 = FieldInfo{ Type{"three"}, { fi1, fi2 } };
  const auto fi4 = FieldInfo{ Type{"four"}, { fi1, fi3 } };
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

}
