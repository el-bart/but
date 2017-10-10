#include "gtest/gtest.h"
#include "UtcIsoDateTime.hpp"

using But::Log::Backend::Tag;
using But::Log::Field::UtcIsoDateTime;

namespace
{

struct ButLogFieldUtcIsoDateTime: public testing::Test
{ };


TEST_F(ButLogFieldUtcIsoDateTime, ConvertingToString)
{
  EXPECT_EQ( "1970-01-01T00:20:34Z", toFieldInfo( UtcIsoDateTime{1234} ).value().get<std::string>() );
  EXPECT_EQ( "1971-02-05T05:33:20Z", toFieldInfo( UtcIsoDateTime{34580000} ).value().get<std::string>() );
}


TEST_F(ButLogFieldUtcIsoDateTime, ConvertingToFieldInfo)
{
  const auto fi = toFieldInfo( UtcIsoDateTime{1234} );
  EXPECT_EQ( Tag{"But::UtcIsoDT"}, fi.type() );
  EXPECT_EQ( "1970-01-01T00:20:34Z", fi.value().get<std::string>() );
}


TEST_F(ButLogFieldUtcIsoDateTime, HelperMethods)
{
  const UtcIsoDateTime uidt{34580000};

  EXPECT_EQ( "1971-02-05", uidt.date() );
  EXPECT_EQ( "05:33:20", uidt.time() );
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
