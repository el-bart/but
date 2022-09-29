#include <But/Log/Field/PreciseDateTime.hpp>
#include <But/Log/Backend/detail/EntryRoot.ut.hpp>

using But::Log::Field::ConversionError;
using But::Log::Field::PreciseDateTime;

namespace
{

struct ButLogFieldPreciseDateTime: public But::Log::Backend::detail::EntryRootTestBase
{ };


TEST_F(ButLogFieldPreciseDateTime, ConvertingToString)
{
  EXPECT_EQ( "1970-01-01T00:20:34.000000000Z", fieldValue( PreciseDateTime{1234} ) );
  EXPECT_EQ( "1971-02-05T05:33:20.000000000Z", fieldValue( PreciseDateTime{34580000} ) );
}


TEST_F(ButLogFieldPreciseDateTime, Logging)
{
  const PreciseDateTime pdt{34580000};
  er_.proxy().nest(pdt);
  EXPECT_EQ_JSON(R"({ "But::PreciseDT": "1971-02-05T05:33:20.000000000Z" })", er_);
}


TEST_F(ButLogFieldPreciseDateTime, ConvertingToFieldInfo)
{
  const auto tv = timespec{1234,42};
  const auto pdt = PreciseDateTime{tv};
  EXPECT_EQ( "But::PreciseDT", fieldName(&pdt) );
  EXPECT_EQ( "1970-01-01T00:20:34.000000042Z", fieldValue(pdt) );
}


TEST_F(ButLogFieldPreciseDateTime, HelperMethods)
{
  const PreciseDateTime pdt{34580000};

  EXPECT_EQ( "1971-02-05", pdt.date() );
  EXPECT_EQ( "05:33:20.000000000", pdt.time() );
}


TEST_F(ButLogFieldPreciseDateTime, AutomaticExtractionOfCurrentTimestamp)
{
  const auto pre = time(nullptr);
  const PreciseDateTime pdt{};
  const auto post = time(nullptr);
  EXPECT_LE( pre, pdt.tv_.tv_sec );
  EXPECT_LE( pdt.tv_.tv_nsec, post );
}


TEST_F(ButLogFieldPreciseDateTime, OutOfRangeErrorOnInvalidMillisecondsCount)
{
  {
    const auto tv = timespec{1234,1000*1000*1000};
    EXPECT_THROW( PreciseDateTime{tv}.time(), ConversionError );
  }
  {
    const auto tv = timespec{1234,-1};
    EXPECT_THROW( PreciseDateTime{tv}.time(), ConversionError );
  }
}


TEST_F(ButLogFieldPreciseDateTime, MillisecondsAreZeroPaded)
{
  const auto time2str = [](long usec) { return PreciseDateTime{ timespec{0, usec} }.time(); };
  EXPECT_EQ( "00:00:00.000000000", time2str(0) );
  EXPECT_EQ( "00:00:00.000000001", time2str(1) );
  EXPECT_EQ( "00:00:00.000000021", time2str(21) );
  EXPECT_EQ( "00:00:00.000000321", time2str(321) );
  EXPECT_EQ( "00:00:00.000004321", time2str(4321) );
  EXPECT_EQ( "00:00:00.000054321", time2str(54321) );
  EXPECT_EQ( "00:00:00.000654321", time2str(654321) );
  EXPECT_EQ( "00:00:00.007654321", time2str(7654321) );
  EXPECT_EQ( "00:00:00.087654321", time2str(87654321) );
  EXPECT_EQ( "00:00:00.987654321", time2str(987654321) );
  EXPECT_EQ( "00:00:00.999999999", time2str(999999999) );
}

}
