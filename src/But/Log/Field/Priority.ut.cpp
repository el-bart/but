#include "gtest/gtest.h"
#include "Priority.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

using But::Log::Field::Priority;

namespace
{

struct ButLogFieldPriority: public testing::Test
{ };


TEST_F(ButLogFieldPriority, ConvertingToString)
{
  EXPECT_EQ( toValue(Priority::debug),   "debug" );
  EXPECT_EQ( toValue(Priority::info),    "info" );
  EXPECT_EQ( toValue(Priority::warning), "warning" );
  EXPECT_EQ( toValue(Priority::error),   "error" );
}


TEST_F(ButLogFieldPriority, ConvertingToStringOfConstantLength)
{
  EXPECT_EQ( toValueConstLen(Priority::debug),   "debug  " );
  EXPECT_EQ( toValueConstLen(Priority::info),    "info   " );
  EXPECT_EQ( toValueConstLen(Priority::warning), "warning" );
  EXPECT_EQ( toValueConstLen(Priority::error),   "error  " );
}


TEST_F(ButLogFieldPriority, ConvertingToFieldInfo)
{
  const auto fi = But::Log::Backend::FieldInfo{Priority::info};
  EXPECT_EQ( fi.type(), "But::Priority" );
  EXPECT_EQ( fi.value(), "info" );
}

}
