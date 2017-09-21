#include "gtest/gtest.h"
#include "toJson.hpp"

using json = nlohmann::json;
using But::Log::Backend::Entry;
using But::Log::Backend::FieldInfo;
using But::Log::Destination::Common::toJson;
using But::Log::Destination::Common::toJsonField;

namespace
{

struct ButLogDestinationCommonToJson: public testing::Test
{
  void addEntry(Entry&) const { }

  template<typename Head, typename ...Tail>
  void addEntry(Entry& e, Head const& head, Tail const& ...tail) const
  {
    e.emplace_back(head);
    addEntry(e, tail...);
  }

  template<typename ...Args>
  auto toEntry(const Args... args) const
  {
    Entry e;
    addEntry(e, args...);
    return e;
  }
};


TEST_F(ButLogDestinationCommonToJson, FromFieldInfo)
{
  {
    const auto f = toJsonField( FieldInfo{"text"} );
    EXPECT_TRUE( f.is_object() );
    EXPECT_TRUE( f["string"].is_string() );
    EXPECT_EQ( "text", f["string"] );
  }
  {
    const auto f = toJsonField( FieldInfo{42} );
    EXPECT_TRUE( f.is_object() );
    EXPECT_TRUE( f["int"].is_number() );
    EXPECT_EQ( 42, f["int"].get<int64_t>() );
  }
  {
    const auto f = toJsonField( FieldInfo{42u} );
    EXPECT_TRUE( f.is_object() );
    EXPECT_TRUE( f["unsigned int"].is_number() );
    EXPECT_EQ( 42u, f["unsigned int"].get<uint64_t>() );
  }
  {
    const auto f = toJsonField( FieldInfo{4.2} );
    EXPECT_TRUE( f.is_object() );
    EXPECT_TRUE( f["double"].is_number() );
    EXPECT_EQ( 4.2, f["double"].get<double>() );
  }
  {
    const auto f = toJsonField( FieldInfo{true} );
    EXPECT_TRUE( f.is_object() );
    EXPECT_TRUE( f["bool"].is_boolean() );
    EXPECT_EQ( true, f["bool"].get<bool>() );
  }
}


TEST_F(ButLogDestinationCommonToJson, ExplicitOutputTypeApi)
{
  auto out = nlohmann::json::array();
  toJson( out, toEntry(42, "answer") );
  EXPECT_EQ( 42, out.at(0)["int"].get<int>() );
  EXPECT_EQ( "answer", out.at(1)["string"] );
}


TEST_F(ButLogDestinationCommonToJson, AutoOutputTypeApi)
{
  const auto out = toJson( toEntry(42, "answer") );
  ASSERT_TRUE( out.is_array() );
  EXPECT_EQ( 42, out.at(0)["int"].get<int>() );
  EXPECT_EQ( "answer", out.at(1)["string"] );
}

}
