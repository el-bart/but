#include <But/Log/Field/UtcIsoDateTime.hpp>
#include <But/Log/Backend/EntryRootTestBase.utg.hpp>

using But::Log::Field::UtcIsoDateTime;

namespace
{

struct ButLogFieldUtcIsoDateTime: public But::Log::Backend::EntryRootTestBase
{ };


TEST_F(ButLogFieldUtcIsoDateTime, ConvertingToString)
{
  EXPECT_EQ( "1970-01-01T00:20:34Z", fieldValue( UtcIsoDateTime{1234} ) );
  EXPECT_EQ( "1971-02-05T05:33:20Z", fieldValue( UtcIsoDateTime{34580000} ) );
}


TEST_F(ButLogFieldUtcIsoDateTime, Logging)
{
  const auto dt = UtcIsoDateTime{34580000};
  er_.proxy().nest(dt);
  EXPECT_EQ_JSON(R"({ "But::UtcIsoDT": "1971-02-05T05:33:20Z" })", er_);
}


TEST_F(ButLogFieldUtcIsoDateTime, ConvertingToFieldInfo)
{
  const auto dt = UtcIsoDateTime{1234};
  EXPECT_EQ( "But::UtcIsoDT", fieldName(&dt) );
  EXPECT_EQ( "1970-01-01T00:20:34Z", fieldValue(dt) );
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
