#include "gtest/gtest.h"
#include "Priority.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

using But::Log::Backend::Type;
using But::Log::Field::Priority;

namespace
{

struct ButLogFieldPriority: public testing::Test
{ };


TEST_F(ButLogFieldPriority, ConvertingToFieldInfo)
{
  {
    const auto fi = toFieldInfo(Priority::debug);
    EXPECT_EQ( Type{"But::Priority"}, fi.type() );
    EXPECT_EQ( "debug", fi.value().get<std::string>() );
  }
  {
    const auto fi = toFieldInfo(Priority::info);
    EXPECT_EQ( Type{"But::Priority"}, fi.type() );
    EXPECT_EQ( "info", fi.value().get<std::string>() );
  }
  {
    const auto fi = toFieldInfo(Priority::warning);
    EXPECT_EQ( Type{"But::Priority"}, fi.type() );
    EXPECT_EQ( "warning", fi.value().get<std::string>() );
  }
  {
    const auto fi = toFieldInfo(Priority::error);
    EXPECT_EQ( Type{"But::Priority"}, fi.type() );
    EXPECT_EQ( "error", fi.value().get<std::string>() );
  }
}

}
