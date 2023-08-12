#include <But/Log/Field/UtcTime.hpp>
#include <But/Log/Backend/EntryRootTestBase.ut.hpp>

using But::Log::Field::UtcTime;
using But::Log::Field::UtcIsoDateTime;

namespace
{

struct ButLogFieldUtcTime: public But::Log::Backend::EntryRootTestBase
{ };


TEST_F(ButLogFieldUtcTime, ConvertingToString)
{
  EXPECT_EQ( "00:20:34", fieldValue( UtcTime{1234} ) );
  EXPECT_EQ( "05:33:20", fieldValue( UtcTime{34580000} ) );
}


TEST_F(ButLogFieldUtcTime, ConvertingToFieldInfo)
{
  const auto ut = UtcTime{1234};
  EXPECT_EQ( "But::UtcTime", fieldName(&ut) );
  EXPECT_EQ( "00:20:34", fieldValue(ut) );
}


TEST_F(ButLogFieldUtcTime, Logging)
{
  const auto ut = UtcTime{1234};
  er_.proxy().nest(ut);
  EXPECT_EQ_JSON(R"({ "But::UtcTime": "00:20:34" })", er_);
}


TEST_F(ButLogFieldUtcTime, AutomaticCurrentDateExtraction)
{
  const UtcIsoDateTime pre{};
  const UtcTime now{};
  const UtcIsoDateTime post{};
  EXPECT_LE( pre.timestamp_, now.timestamp_.timestamp_ );
}

}
