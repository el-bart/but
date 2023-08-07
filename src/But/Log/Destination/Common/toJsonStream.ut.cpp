#include <sstream>
#include <But/Log/Destination/Common/toJsonStream.hpp>
#include <nlohmann/json.hpp>
#include <gtest/gtest.h>

using But::Log::Destination::Common::toJsonStream;

namespace
{

struct ButLogDestinationCommonToJsonStream: public testing::Test
{
  std::stringstream ss_;
};


TEST_F(ButLogDestinationCommonToJsonStream, EscapingNonprintableCharacters)
{
  toJsonStream( ss_, R"({"string":"A\nB\rC"})" );
  EXPECT_EQ( ss_.str(), "{\"string\":\"A\\nB\\rC\"}" );
}


TEST_F(ButLogDestinationCommonToJsonStream, SomeLogs)
{
  toJsonStream( ss_, R"({"answer": 42})" );
  toJsonStream( ss_, R"({"foo": "bar"})" );

  EXPECT_EQ( ss_.str(),
             R"xx({"answer":42})xx"
             R"xx({"foo":"bar"})xx" );
}

}
