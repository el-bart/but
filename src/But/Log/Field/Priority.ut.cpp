#include <But/Log/Field/Priority.hpp>
#include <But/Log/Backend/detail/EntryRoot.ut.hpp>

using But::Log::Field::Priority;

namespace
{

struct ButLogFieldPriority: public But::Log::Backend::detail::EntryRootTestBase
{ };


TEST_F(ButLogFieldPriority, ConvertingToFieldInfo)
{
  {
    const auto p = Priority::debug;
    EXPECT_EQ( "But::Priority", fieldName(&p) );
    EXPECT_EQ( "debug", fieldValue(p) );
  }
  {
    const auto p = Priority::info;
    EXPECT_EQ( "But::Priority", fieldName(&p) );
    EXPECT_EQ( "info", fieldValue(p) );
  }
  {
    const auto p = Priority::warning;
    EXPECT_EQ( "But::Priority", fieldName(&p) );
    EXPECT_EQ( "warning", fieldValue(p) );
  }
  {
    const auto p = Priority::error;
    EXPECT_EQ( "But::Priority", fieldName(&p) );
    EXPECT_EQ( "error", fieldValue(p) );
  }
}


TEST_F(ButLogFieldPriority, Logging)
{
  const auto p = Priority::info;
  er_.proxy().nest(p);
  EXPECT_EQ_JSON(R"({ "But::Priority": "info" })", er_);
}

}
