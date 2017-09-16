#include "gtest/gtest.h"
#include "UtcDate.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

using But::Log::Field::UtcDate;
using But::Log::Field::UtcIsoDateTime;

namespace
{

struct ButLogFieldUtcDate: public testing::Test
{ };


TEST_F(ButLogFieldUtcDate, ConvertingToString)
{
  EXPECT_EQ( toValue( UtcDate{1234} ), "1970-01-01" );
  EXPECT_EQ( toValue( UtcDate{34580000} ), "1971-02-05" );
}


TEST_F(ButLogFieldUtcDate, ConvertingToFieldInfo)
{
  const auto fi = But::Log::Backend::FieldInfo{ UtcDate{1234} };
  EXPECT_EQ( fi.type(), "But::UtcDate" );
  EXPECT_EQ( fi.value(), "1970-01-01" );
}


TEST_F(ButLogFieldUtcDate, AutomaticCurrentDateExtraction)
{
  const UtcIsoDateTime pre{};
  const UtcDate now{};
  const UtcIsoDateTime post{};
  EXPECT_LE( pre.timestamp_, now.timestamp_.timestamp_ );
}

}
