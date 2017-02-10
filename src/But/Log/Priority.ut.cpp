#include "gtest/gtest.h"
#include "Priority.hpp"

using But::Log::Priority;

namespace
{

struct ButLogPriority: public testing::Test
{ };


TEST_F(ButLogPriority, ConvertingToString)
{
  EXPECT_EQ( toString(Priority::debug),   "debug" );
  EXPECT_EQ( toString(Priority::info),    "info" );
  EXPECT_EQ( toString(Priority::warning), "warning" );
  EXPECT_EQ( toString(Priority::error),   "error" );
}


TEST_F(ButLogPriority, ConvertingToStringOfConstantLength)
{
  EXPECT_EQ( toStringConstLen(Priority::debug),   "debug  " );
  EXPECT_EQ( toStringConstLen(Priority::info),    "info   " );
  EXPECT_EQ( toStringConstLen(Priority::warning), "warning" );
  EXPECT_EQ( toStringConstLen(Priority::error),   "error  " );
}

}
