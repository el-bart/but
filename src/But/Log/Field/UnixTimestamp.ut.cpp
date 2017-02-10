#include "gtest/gtest.h"
#include "UnixTimestamp.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

using But::Log::Field::UnixTimestamp;

namespace
{

struct ButLogFieldUnixTimestamp: public testing::Test
{ };


TEST_F(ButLogFieldUnixTimestamp, ExplicitlyProvidedTimestamp)
{
  const auto tp = UnixTimestamp::Clock::time_point{} + std::chrono::seconds{42};
  EXPECT_EQ( toString( UnixTimestamp{tp} ), "42.000000000" );
  EXPECT_EQ( toString( UnixTimestamp{tp + std::chrono::nanoseconds{997}} ), "42.000000997" );
}


TEST_F(ButLogFieldUnixTimestamp, AutoObtainingCurrentTimestamp)
{
  const auto pre = UnixTimestamp::Clock::now();
  const auto uts = UnixTimestamp{};
  const auto post = UnixTimestamp::Clock::now();
  EXPECT_TRUE( pre <= uts.timePoint() );
  EXPECT_TRUE( uts.timePoint() <= post );
}


TEST_F(ButLogFieldUnixTimestamp, ConvertingToFieldInfo)
{
  const auto tp = UnixTimestamp::Clock::time_point{} + std::chrono::seconds{666};
  const auto fi = But::Log::Backend::FieldInfo{ UnixTimestamp{tp} };
  EXPECT_EQ( fi.type(), "But::Log::Field::UnixTimestamp" );
  EXPECT_EQ( fi.value(), "666.000000000" );
}

}
