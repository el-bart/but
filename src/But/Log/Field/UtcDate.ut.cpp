#include "gtest/gtest.h"
#include "UtcDate.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

using But::Log::Backend::Type;
using But::Log::Field::UtcDate;
using But::Log::Field::UtcIsoDateTime;

namespace
{

struct ButLogFieldUtcDate: public testing::Test
{ };


TEST_F(ButLogFieldUtcDate, ConvertingToString)
{
  EXPECT_EQ( "1970-01-01", toFieldInfo( UtcDate{1234} ).value().get<std::string>() );
  EXPECT_EQ( "1971-02-05", toFieldInfo( UtcDate{34580000} ).value().get<std::string>() );
}


TEST_F(ButLogFieldUtcDate, ConvertingToFieldInfo)
{
  const auto fi = toFieldInfo( UtcDate{1234} );
  EXPECT_EQ( fi.type(), Type{"But::UtcDate"} );
  EXPECT_EQ( fi.value().get<std::string>(), "1970-01-01" );
}


TEST_F(ButLogFieldUtcDate, AutomaticCurrentDateExtraction)
{
  const UtcIsoDateTime pre{};
  const UtcDate now{};
  const UtcIsoDateTime post{};
  EXPECT_LE( pre.timestamp_, now.timestamp_.timestamp_ );
}

}
