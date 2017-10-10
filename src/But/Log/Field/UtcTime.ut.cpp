#include "gtest/gtest.h"
#include "UtcTime.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

using But::Log::Backend::Tag;
using But::Log::Field::UtcTime;
using But::Log::Field::UtcIsoDateTime;

namespace
{

struct ButLogFieldUtcTime: public testing::Test
{ };


TEST_F(ButLogFieldUtcTime, ConvertingToString)
{
  EXPECT_EQ( "00:20:34", toFieldInfo( UtcTime{1234} ).value().get<std::string>() );
  EXPECT_EQ( "05:33:20", toFieldInfo( UtcTime{34580000} ).value().get<std::string>() );
}


TEST_F(ButLogFieldUtcTime, ConvertingToFieldInfo)
{
  const auto fi = toFieldInfo( UtcTime{1234} );
  EXPECT_EQ( Tag{"But::UtcTime"}, fi.tag() );
  EXPECT_EQ( "00:20:34", fi.value().get<std::string>() );
}


TEST_F(ButLogFieldUtcTime, AutomaticCurrentDateExtraction)
{
  const UtcIsoDateTime pre{};
  const UtcTime now{};
  const UtcIsoDateTime post{};
  EXPECT_LE( pre.timestamp_, now.timestamp_.timestamp_ );
}

}
