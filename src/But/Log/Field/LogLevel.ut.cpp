#include <But/Log/Field/LogLevel.hpp>
#include <But/Log/Backend/EntryRootTestBase.utg.hpp>

using But::Log::Field::LogLevel;

namespace
{

struct ButLogFieldLogLevel: public But::Log::Backend::EntryRootTestBase
{ };


TEST_F(ButLogFieldLogLevel, ConvertingToFieldInfo)
{
  {
    const auto p = LogLevel::debug;
    EXPECT_EQ( "But::LogLevel", fieldName(&p) );
    EXPECT_EQ( "debug", fieldValue(p) );
  }
  {
    const auto p = LogLevel::info;
    EXPECT_EQ( "But::LogLevel", fieldName(&p) );
    EXPECT_EQ( "info", fieldValue(p) );
  }
  {
    const auto p = LogLevel::warning;
    EXPECT_EQ( "But::LogLevel", fieldName(&p) );
    EXPECT_EQ( "warning", fieldValue(p) );
  }
  {
    const auto p = LogLevel::error;
    EXPECT_EQ( "But::LogLevel", fieldName(&p) );
    EXPECT_EQ( "error", fieldValue(p) );
  }
}


TEST_F(ButLogFieldLogLevel, Logging)
{
  const auto p = LogLevel::info;
  er_.proxy().nest(p);
  EXPECT_EQ_JSON(R"({ "But::LogLevel": "info" })", er_);
}

}
