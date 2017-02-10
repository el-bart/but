#include "gtest/gtest.h"
#include "Priority.hpp"

using But::Log::Field::Priority;

namespace
{

struct ButLogFieldPriority: public testing::Test
{ };


TEST_F(ButLogFieldPriority, ConvertingToString)
{
  EXPECT_EQ( toString(Priority::debug),   "debug" );
  EXPECT_EQ( toString(Priority::info),    "info" );
  EXPECT_EQ( toString(Priority::warning), "warning" );
  EXPECT_EQ( toString(Priority::error),   "error" );
}


TEST_F(ButLogFieldPriority, ConvertingToStringOfConstantLength)
{
  EXPECT_EQ( toStringConstLen(Priority::debug),   "debug  " );
  EXPECT_EQ( toStringConstLen(Priority::info),    "info   " );
  EXPECT_EQ( toStringConstLen(Priority::warning), "warning" );
  EXPECT_EQ( toStringConstLen(Priority::error),   "error  " );
}


TEST_F(ButLogFieldPriority, ConvertingToEntry)
{
  const auto fi = toFieldInfo(Priority::info);
  EXPECT_EQ( fi.type_, "But::Log::Field::Priority" );
  EXPECT_EQ( fi.value_, "info" );
}

}
