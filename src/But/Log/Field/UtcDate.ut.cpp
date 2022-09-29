#include <But/Log/Field/UtcDate.hpp>
#include <But/Log/Backend/detail/EntryRoot.ut.hpp>

using But::Log::Field::UtcDate;
using But::Log::Field::UtcIsoDateTime;

namespace
{

struct ButLogFieldUtcDate: public But::Log::Backend::detail::EntryRootTestBase
{ };


TEST_F(ButLogFieldUtcDate, ConvertingToString)
{
  EXPECT_EQ( "1970-01-01", fieldValue( UtcDate{1234} ) );
  EXPECT_EQ( "1971-02-05", fieldValue( UtcDate{34580000} ) );
}


TEST_F(ButLogFieldUtcDate, ConvertingToFieldInfo)
{
  const auto ud = UtcDate{1234};
  EXPECT_EQ( "But::UtcDate", fieldName(&ud) );
  EXPECT_EQ( "1970-01-01", fieldValue(ud) );
}


TEST_F(ButLogFieldUtcDate, Logging)
{
  const auto ud = UtcDate{1234};
  er_.proxy().nest(ud);
  EXPECT_EQ_JSON(R"({ "But::UtcDate": "1970-01-01" })", er_);
}


TEST_F(ButLogFieldUtcDate, AutomaticCurrentDateExtraction)
{
  const UtcIsoDateTime pre{};
  const UtcDate now{};
  const UtcIsoDateTime post{};
  EXPECT_LE( pre.timestamp_, now.timestamp_.timestamp_ );
}

}
