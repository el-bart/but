#include <vector>
#include <sstream>
#include "gtest/gtest.h"
#include "toJsonStream.hpp"
#include "But/Log/Field/FormattedString.hpp"
#include "But/Log/Backend/toFieldInfo.hpp"
#include "But/Log/Destination/Common/rootElementTag.hpp"

using But::Log::Backend::Tag;
using But::Log::Backend::FieldInfo;
using But::Log::Backend::toFieldInfo;
using But::Log::Destination::Common::toJsonStream;
using But::Log::Destination::Common::rootElementTag;

namespace
{

struct ButLogDestinationCommonToJsonStream: public testing::Test
{
  void addFieldInfo(FieldInfo&) const { }

  template<typename ...Args>
  auto makeFieldInfo(const Args... args) const
  {
    return FieldInfo{ rootElementTag(), std::vector<FieldInfo>{ toFieldInfo(args)... } };
  }


  template<typename T>
  auto testType(std::string const& typeName, const T t, std::string const& expectedValue) const
  {
    std::stringstream ss;
    toJsonStream( ss, toFieldInfo(t) );
    const auto expected = "{\"" + typeName + "\":" + expectedValue + "}";
    const auto read = ss.str();
    EXPECT_EQ(expected, read);
    return expected==read;
  }

  template<typename T>
  auto testType(std::string const& typeName, const T t) const
  {
    return testType( typeName, t, std::to_string(t) );
  }

  std::stringstream ss_;
};


TEST_F(ButLogDestinationCommonToJsonStream, EscapingNonprintableCharacters)
{
  toJsonStream( ss_, toFieldInfo("A\nB\rC") );
  EXPECT_EQ( ss_.str(), "{\"string\":\"A\\nB\\rC\"}" );
}


TEST_F(ButLogDestinationCommonToJsonStream, SomeLogs)
{
  toJsonStream( ss_, makeFieldInfo("answer", 42) );
  toJsonStream( ss_, makeFieldInfo("foo", "bar") );

  EXPECT_EQ( ss_.str(),
             R"xx({"int":42,"string":"answer"})xx"
             R"xx({"string0":"foo","string1":"bar"})xx" );
}


TEST_F(ButLogDestinationCommonToJsonStream, SomeFormattedLog)
{
  toJsonStream( ss_, makeFieldInfo( But::Log::Field::FormattedString{"kszy"}, "answer", 42 ) );
  EXPECT_EQ( ss_.str(), R"xx({"But::Formatted":"kszy","int":42,"string":"answer"})xx" );
}


TEST_F(ButLogDestinationCommonToJsonStream, NumericTypes)
{
  EXPECT_TRUE( testType<short>("int", -42) );
  EXPECT_TRUE( testType<unsigned short>("unsigned int", 42) );

  EXPECT_TRUE( testType<int>("int", -42) );
  EXPECT_TRUE( testType<unsigned int>("unsigned int", 42) );

  EXPECT_TRUE( testType<long>("int", -42) );
  EXPECT_TRUE( testType<unsigned long>("unsigned int", 42) );

  EXPECT_TRUE( testType<long long>("int", -42) );
  EXPECT_TRUE( testType<unsigned long long>("unsigned int", 42) );

  EXPECT_TRUE( testType<float>("double", -4.0, "-4") );
  EXPECT_TRUE( testType<double>("double", -4.2, "-4.2") );
  EXPECT_TRUE( testType<long double>("double", -4.2, "-4.2") );
}

}
