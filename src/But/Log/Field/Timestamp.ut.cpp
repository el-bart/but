#include <But/Log/Field/Timestamp.hpp>
#include <But/Log/Backend/EntryRootTestBase.ut.hpp>

using But::Log::Field::Timestamp;

namespace
{

struct ButLogFieldTimestamp: public But::Log::Backend::EntryRootTestBase
{ };


TEST_F(ButLogFieldTimestamp, ExplicitlyProvidedTimestamp)
{
  const auto tp = Timestamp::Clock::time_point{} + std::chrono::seconds{42};
  EXPECT_EQ( 42.0, fieldValue( Timestamp{tp} ) );
  EXPECT_EQ( 42.000000997, fieldValue( Timestamp{tp + std::chrono::nanoseconds{997}} ) );
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
  const auto ts = Timestamp{tp};
  EXPECT_EQ( "But::Timestamp", fieldName(&ts) );
  EXPECT_EQ( 666.0, fieldValue(ts) );
}

TEST_F(ButLogFieldTimestamp, Logging)
{
  const auto tp = Timestamp::Clock::time_point{} + std::chrono::seconds{666} + std::chrono::milliseconds{250};
  const auto ts = Timestamp{tp};
  er_.proxy().nest(ts);
  EXPECT_EQ_JSON(R"({ "But::Timestamp": 666.25 })", er_);
}

}
