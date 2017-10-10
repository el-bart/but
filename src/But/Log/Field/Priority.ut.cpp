#include "gtest/gtest.h"
#include "Priority.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

using But::Log::Backend::Tag;
using But::Log::Field::Priority;

namespace
{

struct ButLogFieldPriority: public testing::Test
{ };


TEST_F(ButLogFieldPriority, ConvertingToFieldInfo)
{
  {
    const auto fi = toFieldInfo(Priority::debug);
    EXPECT_EQ( Tag{"But::Priority"}, fi.tag() );
    EXPECT_EQ( "debug", fi.value().get<std::string>() );
  }
  {
    const auto fi = toFieldInfo(Priority::info);
    EXPECT_EQ( Tag{"But::Priority"}, fi.tag() );
    EXPECT_EQ( "info", fi.value().get<std::string>() );
  }
  {
    const auto fi = toFieldInfo(Priority::warning);
    EXPECT_EQ( Tag{"But::Priority"}, fi.tag() );
    EXPECT_EQ( "warning", fi.value().get<std::string>() );
  }
  {
    const auto fi = toFieldInfo(Priority::error);
    EXPECT_EQ( Tag{"But::Priority"}, fi.tag() );
    EXPECT_EQ( "error", fi.value().get<std::string>() );
  }
}

}
