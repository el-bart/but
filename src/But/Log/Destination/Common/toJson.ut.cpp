#include <gtest/gtest.h>
#include <But/Log/Destination/Common/toJson.hpp>

using But::Log::Destination::Common::toJson;

namespace
{

struct ButLogDestinationCommonToJson: public testing::Test
{
};


TEST_F(ButLogDestinationCommonToJson, JsonHasExpectedContent)
{
  const auto out = toJson( R"xx(
{
  "int": 42,
  "string": "answer"
}
)xx" );
  std::stringstream ss;
  ss << out;
  EXPECT_EQ( R"xx({"int":42,"string":"answer"})xx", ss.str() );
}

}
