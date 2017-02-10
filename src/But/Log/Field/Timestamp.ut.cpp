#include "gtest/gtest.h"
#include "Timestamp.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

using But::Log::Field::Timestamp;

namespace
{

struct ButLogFieldTimestamp: public testing::Test
{ };


TEST_F(ButLogFieldTimestamp, ExplicitlyProvidedTimestamp)
{
  const auto tp = Timestamp::Clock::time_point{} + std::chrono::seconds{42};
  EXPECT_EQ( toString( Timestamp{tp} ), "42.000000000" );
  EXPECT_EQ( toString( Timestamp{tp + std::chrono::nanoseconds{997}} ), "42.000000997" );
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
  const auto fi = But::Log::Backend::FieldInfo{ Timestamp{tp} };
  EXPECT_EQ( fi.type(), "But::Timestamp" );
  EXPECT_EQ( fi.value(), "666.000000000" );
}

}
