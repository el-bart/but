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


struct Repeated
{
  int a_;
  int b_;
};

auto toFieldInfo(Repeated const& r)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Tag{"Repeated"}, { toFieldInfo(r.a_), toFieldInfo(r.b_) } };
}

TEST_F(ButLogDestinationCommonToJson, RepeatedFieldsGetAutoNumbered)
{
  const auto out = toJson( makeFieldInfo( Repeated{42, 24} ) );
  const auto& internal = out.at(0).at("Repeated");
  EXPECT_EQ( 42, internal.at("int").get<int64_t>() );
  EXPECT_EQ( 24, internal.at("int1").get<int64_t>() );
}


struct RepeatedInterleaved
{
  int a_;
  std::string x_;
  int b_;
  std::string y_;
};

auto toFieldInfo(RepeatedInterleaved const& ri)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Tag{"RepeatedInterleaved"}, { toFieldInfo(ri.a_), toFieldInfo(ri.x_), toFieldInfo(ri.b_), toFieldInfo(ri.y_) } };
}

TEST_F(ButLogDestinationCommonToJson, RepeatedInterleavedFieldsGetAutoNumbered)
{
  const auto out = toJson( makeFieldInfo( RepeatedInterleaved{42, "xxx", 24, "yyy"} ) );
  const auto& internal = out.at(0).at("RepeatedInterleaved");
  EXPECT_EQ( 42, internal.at("int").get<int64_t>() );
  EXPECT_EQ( "xxx", internal.at("string").get<std::string>() );
  EXPECT_EQ( 24, internal.at("int1").get<int64_t>() );
  EXPECT_EQ( "yyy", internal.at("string1").get<std::string>() );
}


struct NestedRepeated
{
  Repeated r0_;
  Repeated r1_;
};

auto toFieldInfo(NestedRepeated const& nr)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Tag{"NestedRepeated"}, { toFieldInfo(nr.r0_), toFieldInfo(nr.r1_) } };
}

TEST_F(ButLogDestinationCommonToJson, NestedRepeatedFieldsGetAutoNumbered)
{
  const auto out = toJson( makeFieldInfo( NestedRepeated{ Repeated{4,2}, Repeated{6,9} } ) );
  const auto& internal = out.at(0).at("NestedRepeated");
  // r0
  {
    const auto& ref = internal.at("Repeated");
    EXPECT_EQ( 4, ref.at("int").get<int64_t>() );
    EXPECT_EQ( 2, ref.at("int1").get<int64_t>() );
  }
  // r1
  {
    const auto& ref = internal.at("Repeated1");
    EXPECT_EQ( 6, ref.at("int").get<int64_t>() );
    EXPECT_EQ( 9, ref.at("int1").get<int64_t>() );
  }
}


struct NestedRepeated4
{
  Repeated r0_;
  Repeated r1_;
  Repeated r2_;
  Repeated r3_;
};

auto toFieldInfo(NestedRepeated4 const& nr4)
{
  using But::Log::Backend::toFieldInfo;
  return FieldInfo{ Tag{"NestedRepeated4"}, { toFieldInfo(nr4.r0_), toFieldInfo(nr4.r1_), toFieldInfo(nr4.r2_), toFieldInfo(nr4.r3_) } };
}

TEST_F(ButLogDestinationCommonToJson, NestedRepeatedMultipleTimesFieldsGetAutoNumbered)
{
  const auto out = toJson( makeFieldInfo( NestedRepeated4{ Repeated{4,2}, Repeated{6,9}, Repeated{1,3}, Repeated{5,1} } ) );
  const auto& internal = out.at(0).at("NestedRepeated4");
  // r0
  {
    const auto& ref = internal.at("Repeated");
    EXPECT_EQ( 4, ref.at("int").get<int64_t>() );
    EXPECT_EQ( 2, ref.at("int1").get<int64_t>() );
  }
  // r1
  {
    const auto& ref = internal.at("Repeated1");
    EXPECT_EQ( 6, ref.at("int").get<int64_t>() );
    EXPECT_EQ( 9, ref.at("int1").get<int64_t>() );
  }
  // r2
  {
    const auto& ref = internal.at("Repeated2");
    EXPECT_EQ( 1, ref.at("int").get<int64_t>() );
    EXPECT_EQ( 3, ref.at("int1").get<int64_t>() );
  }
  // r3
  {
    const auto& ref = internal.at("Repeated3");
    EXPECT_EQ( 5, ref.at("int").get<int64_t>() );
    EXPECT_EQ( 1, ref.at("int1").get<int64_t>() );
  }
}

}
