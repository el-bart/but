#include "gtest/gtest.h"
#include "Timestamp.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

using But::Log::Backend::Tag;
using But::Log::Field::Timestamp;

namespace
{

struct ButLogFieldTimestamp: public testing::Test
{ };


TEST_F(ButLogFieldTimestamp, ExplicitlyProvidedTimestamp)
{
  const auto tp = Timestamp::Clock::time_point{} + std::chrono::seconds{42};
  EXPECT_EQ( 42.0, toFieldInfo( Timestamp{tp} ).value().get<double>() );
  EXPECT_EQ( 42.000000997, toFieldInfo( Timestamp{tp + std::chrono::nanoseconds{997}} ).value().get<double>() );
}


TEST_F(ButLogFieldTimestamp, AutoObtainingCurrentTimestamp)
{
  const auto pre = Timestamp::Clock::now();
  const auto uts = Timestamp{};
  const auto post = Timestamp::Clock::now();
  EXPECT_TRUE( pre <= uts.timePoint() );
  EXPECT_TRUE( uts.timePoint() <= post );
}


TEST_F(ButLogFieldTimestamp, ConvertingToFieldInfo)
{
  const auto tp = Timestamp::Clock::time_point{} + std::chrono::seconds{666};
  const auto fi = toFieldInfo( Timestamp{tp} );
  EXPECT_EQ( Tag{"But::Timestamp"}, fi.tag() );
  EXPECT_EQ( 666.0, fi.value().get<double>() );
}

}
