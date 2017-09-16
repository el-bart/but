#include "gtest/gtest.h"
#include "FieldInfo.hpp"

using But::Log::Backend::FieldInfo;

struct GlobalCustomTypeForFieldTesting { };
std::string toType(GlobalCustomTypeForFieldTesting const&) { return "GlobalCustomTypeForFieldTesting - custom"; }
std::string toValue(GlobalCustomTypeForFieldTesting const&) { return "<empty>"; }

namespace
{

struct ButLogBackendFieldInfo: public testing::Test
{ };


TEST_F(ButLogBackendFieldInfo, ExplicitlyProvidedTypeName)
{
  const auto fi = FieldInfo{"type", "value"};
  EXPECT_EQ( fi.type(), "type" );
  EXPECT_EQ( fi.value(), "value" );
}


struct MyCustomType { };
std::string toType(MyCustomType const&) { return "foo-bar!"; }
std::string toValue(MyCustomType const&) { return "custom type worked! :D"; }

TEST_F(ButLogBackendFieldInfo, CustomizationPoints)
{
  EXPECT_EQ( FieldInfo{ std::string{"stuff"} }.value(), "stuff" );
  EXPECT_EQ( FieldInfo{ std::string{"stuff"} }.type(), "string" );

  EXPECT_EQ( FieldInfo{ MyCustomType{} }.type(), "foo-bar!" );
  EXPECT_EQ( FieldInfo{ MyCustomType{} }.value(), "custom type worked! :D" );

  EXPECT_EQ( FieldInfo{ GlobalCustomTypeForFieldTesting{} }.type(), "GlobalCustomTypeForFieldTesting - custom" );
  EXPECT_EQ( FieldInfo{ GlobalCustomTypeForFieldTesting{} }.value(), "<empty>" );
}


struct AnotherCustomType { };
std::string toValue(AnotherCustomType const&) { return "whatever"; }

TEST_F(ButLogBackendFieldInfo, AutoGettingTypeNames)
{
  EXPECT_EQ( FieldInfo{ AnotherCustomType{} }.type(), "(anonymous namespace)::AnotherCustomType" );
  EXPECT_EQ( FieldInfo{ AnotherCustomType{} }.value(), "whatever" );
}


TEST_F(ButLogBackendFieldInfo, CompareValues)
{
  const auto fi1 = FieldInfo{"one", "1"};
  const auto fi2 = FieldInfo{"two", "2"};
  EXPECT_TRUE( fi1 == fi1 );
  EXPECT_FALSE( fi1 == fi2 );
}

}
