#include <vector>
#include <sstream>
#include "gtest/gtest.h"
#include "toJsonStream.hpp"

using But::Log::Backend::Entry;
using But::Log::Destination::Common::toJsonStream;

namespace
{

struct ButLogDestinationCommonToJsonStream: public testing::Test
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


  template<typename T>
  auto testType(std::string const& typeName, const T t, std::string const& expectedValue) const
  {
    std::stringstream ss;
    toJsonStream( ss, toEntry(t) );
    const auto expected = "[{\"" + typeName + "\":" + expectedValue + "}]";
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
  toJsonStream( ss_, toEntry("A\nB\rC") );
  EXPECT_EQ( "[{\"string\":\"A\\nB\\rC\"}]", ss_.str() );
}


TEST_F(ButLogDestinationCommonToJsonStream, SomeLogs)
{
  toJsonStream( ss_, toEntry("answer", 42) );
  toJsonStream( ss_, toEntry("foo", "bar") );

  EXPECT_EQ( R"xx([{"string":"answer"},{"int":42}])xx"
             R"xx([{"string":"foo"},{"string":"bar"}])xx"
             , ss_.str() );
}


TEST_F(ButLogDestinationCommonToJsonStream, SomeFormattedLog)
{
  toJsonStream( ss_, toEntry( But::Log::Field::FormattedString{"kszy"}, "answer", 42 ) );
  EXPECT_EQ( R"xx([{"But::Formatted":"kszy"},{"string":"answer"},{"int":42}])xx",
             ss_.str() );
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
