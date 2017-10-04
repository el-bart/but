#include <sstream>
#include "gtest/gtest.h"
#include "FieldInfo.hpp"

using But::Log::Backend::toValue;
using But::Log::Backend::Type;
using But::Log::Backend::Value;
using But::Log::Backend::FieldInfo;

struct GlobalCustomTypeForFieldTesting { };
auto toType(GlobalCustomTypeForFieldTesting const&) { return Type{"GlobalCustomTypeForFieldTesting - custom"}; }
auto toValue(GlobalCustomTypeForFieldTesting const&) { return Value{"<empty>"}; }

namespace
{

struct ButLogBackendFieldInfo: public testing::Test
{ };


TEST_F(ButLogBackendFieldInfo, ExplicitlyProvidedTypeName)
{
  const auto fi = FieldInfo{ Type{"type"}, toValue("value") };
  EXPECT_EQ( fi.type(), Type{"type"} );
  EXPECT_EQ( fi.value().get<std::string>(), "value" );
}


struct MyCustomType { };
auto toType(MyCustomType const&) { return Type{"foo-bar!"}; }
auto toValue(MyCustomType const&) { return Value{"custom type worked! :D"}; }

TEST_F(ButLogBackendFieldInfo, CustomizationPoints)
{
  EXPECT_EQ( FieldInfo{ std::string{"stuff"} }.value().get<std::string>(), "stuff" );
  EXPECT_EQ( FieldInfo{ std::string{"stuff"} }.type(), Type{"string"} );

  EXPECT_EQ( FieldInfo{ MyCustomType{} }.type(), Type{"foo-bar!"} );
  EXPECT_EQ( FieldInfo{ MyCustomType{} }.value().get<std::string>(), "custom type worked! :D" );

  EXPECT_EQ( FieldInfo{ GlobalCustomTypeForFieldTesting{} }.type(), Type{"GlobalCustomTypeForFieldTesting - custom"} );
  EXPECT_EQ( FieldInfo{ GlobalCustomTypeForFieldTesting{} }.value().get<std::string>(), "<empty>" );
}


struct AnotherCustomType { };
auto toType(AnotherCustomType const&) { return Type{"blabla-type"}; }
auto toValue(AnotherCustomType const&) { return Value{"whatever"}; }

TEST_F(ButLogBackendFieldInfo, AutoGettingTypeNames)
{
  EXPECT_EQ( FieldInfo{ AnotherCustomType{} }.type(), Type{"blabla-type"} );
  EXPECT_EQ( FieldInfo{ AnotherCustomType{} }.value().get<std::string>(), "whatever" );
}


TEST_F(ButLogBackendFieldInfo, CompareValues)
{
  using But::Log::Backend::toValue;
  const auto fi1 = FieldInfo{ Type{"one"}, toValue("1") };
  const auto fi2 = FieldInfo{ Type{"two"}, toValue("2") };
  EXPECT_TRUE( fi1 == fi1 );
  EXPECT_FALSE( fi1 == fi2 );
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
  using But::Log::Backend::toValue;
  const auto fi1 = FieldInfo{ Type{"one"}, toValue("1") };
  const auto fi2 = FieldInfo{ Type{"two"}, toValue("2") };
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
