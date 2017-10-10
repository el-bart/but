#include "gtest/gtest.h"
#include "toJson.hpp"
#include "But/Log/Backend/toFieldInfo.hpp"

using json = nlohmann::json;
using But::Log::Backend::Tag;
using But::Log::Backend::Value;
using But::Log::Backend::FieldInfo;
using But::Log::Backend::toFieldInfo;
using But::Log::Destination::Common::toJson;

namespace
{

struct ButLogDestinationCommonToJson: public testing::Test
{
  template<typename ...Args>
  auto makeFieldInfo(const Args... args) const
  {
    return FieldInfo{ Tag{"test"}, { toFieldInfo(args)... } };
  }
};


TEST_F(ButLogDestinationCommonToJson, FromFieldInfo)
{
  {
    const auto f = toJson( FieldInfo{"text"} );
    EXPECT_TRUE( f.is_array() );
    EXPECT_TRUE( f.at(0).at("string").is_string() );
    EXPECT_EQ( "text", f.at(0).at("string") );
  }
  {
    const auto f = toJson( FieldInfo{42} );
    EXPECT_TRUE( f.is_array() );
    EXPECT_TRUE( f.at(0).at("int").is_number() );
    EXPECT_EQ( 42, f.at(0).at("int").get<int64_t>() );
  }
  {
    const auto f = toJson( FieldInfo{42u} );
    EXPECT_TRUE( f.is_array() );
    EXPECT_TRUE( f.at(0).at("unsigned int").is_number() );
    EXPECT_EQ( 42u, f.at(0).at("unsigned int").get<uint64_t>() );
  }
  {
    const auto f = toJson( FieldInfo{4.2} );
    EXPECT_TRUE( f.is_array() );
    EXPECT_TRUE( f.at(0).at("double").is_number() );
    EXPECT_EQ( 4.2, f.at(0).at("double").get<double>() );
  }
  {
    const auto f = toJson( FieldInfo{true} );
    EXPECT_TRUE( f.is_array() );
    EXPECT_TRUE( f.at(0).at("bool").is_boolean() );
    EXPECT_EQ( true, f.at(0).at("bool").get<bool>() );
  }
}


TEST_F(ButLogDestinationCommonToJson, CheckingNonUniqueTypes)
{
  const auto f = toJson( FieldInfo{ Tag{"whatever"}, { FieldInfo{"foo"}, FieldInfo{"bar"} } } );
  EXPECT_TRUE( f.is_array() );
  EXPECT_EQ( "foo", f.at(0).at("string").get<std::string>() );
  EXPECT_EQ( "bar", f.at(1).at("string").get<std::string>() );
}


TEST_F(ButLogDestinationCommonToJson, ConvertingStructure)
{
  const auto out = toJson( makeFieldInfo(42, "answer") );
  EXPECT_EQ( 42, out.at(0).at("int").get<int64_t>() );
  EXPECT_EQ( "answer", out.at(1).at("string") );
}


struct Nested
{
  int a_;
  std::string b_;
};

auto toFieldInfo(Nested const& n)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Tag{"Nested"}, { toFieldInfo(n.a_), toFieldInfo(n.b_) } };
}


TEST_F(ButLogDestinationCommonToJson, ConvertingNsetedStructure)
{
  const auto out = toJson( makeFieldInfo( Nested{42, "answer"} ) );
  const auto& internal = out.at(0).at("Nested");
  EXPECT_EQ( 42, internal.at("int").get<int64_t>() );
  EXPECT_EQ( "answer", internal.at("string") );
}

}
