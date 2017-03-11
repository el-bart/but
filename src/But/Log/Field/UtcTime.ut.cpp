#include "gtest/gtest.h"
#include "UtcTime.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

using But::Log::Field::UtcTime;
using But::Log::Field::UtcIsoDateTime;

namespace
{

struct ButLogFieldUtcTime: public testing::Test
{ };


TEST_F(ButLogFieldUtcTime, ConvertingToString)
{
  EXPECT_EQ( toString( UtcTime{1234} ), "00:20:34" );
  EXPECT_EQ( toString( UtcTime{34580000} ), "05:33:20" );
}


TEST_F(ButLogFieldUtcTime, ConvertingToFieldInfo)
{
  const auto fi = But::Log::Backend::FieldInfo{ UtcTime{1234} };
  EXPECT_EQ( fi.type(), "But::UtcTime" );
  EXPECT_EQ( fi.value(), "00:20:34" );
}


TEST_F(ButLogFieldUtcTime, AutomaticCurrentDateExtraction)
{
  const UtcIsoDateTime pre{};
  const UtcTime now{};
  const UtcIsoDateTime post{};
  EXPECT_LE( pre.timestamp_, now.timestamp_.timestamp_ );
}

}
