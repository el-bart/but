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
std::string toType(MyCustomType const&) { return "foo-bar!"; }
std::string toValue(MyCustomType const&) { return "custom type worked! :D"; }

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
std::string toValue(AnotherCustomType const&) { return "whatever"; }
std::string toType(AnotherCustomType const&) { return "blabla-type"; }

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

}
