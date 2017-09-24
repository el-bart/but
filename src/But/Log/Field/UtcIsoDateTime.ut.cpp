#include "gtest/gtest.h"
#include "UtcIsoDateTime.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

using But::Log::Backend::Type;
using But::Log::Field::UtcIsoDateTime;

namespace
{

struct ButLogFieldUtcIsoDateTime: public testing::Test
{ };


TEST_F(ButLogFieldUtcIsoDateTime, ConvertingToString)
{
  EXPECT_EQ( toValue( UtcIsoDateTime{1234} ).get<std::string>(), "1970-01-01T00:20:34Z" );
  EXPECT_EQ( toValue( UtcIsoDateTime{34580000} ).get<std::string>(), "1971-02-05T05:33:20Z" );
}


TEST_F(ButLogFieldUtcIsoDateTime, ConvertingToFieldInfo)
{
  const auto fi = But::Log::Backend::FieldInfo{ UtcIsoDateTime{1234} };
  EXPECT_EQ( fi.type(), Type{"But::UtcIsoDT"} );
  EXPECT_EQ( fi.value().get<std::string>(), "1970-01-01T00:20:34Z" );
}


TEST_F(ButLogFieldUtcIsoDateTime, HelperMethods)
{
  const UtcIsoDateTime uidt{34580000};

  EXPECT_EQ( uidt.date(), "1971-02-05" );
  EXPECT_EQ( uidt.time(), "05:33:20" );
}


TEST_F(ButLogFieldUtcIsoDateTime, AutomaticExtractionOfCurrentTimestamp)
{
  const auto pre = time(nullptr);
  const UtcIsoDateTime uidt{};
  const auto post = time(nullptr);
  EXPECT_LE( pre, uidt.timestamp_ );
  EXPECT_LE( uidt.timestamp_, post );
}

}
