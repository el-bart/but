#include "gtest/gtest.h"
#include "LineNumber.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

using But::Log::Field::LineNumber;

namespace
{

struct ButLogFieldLineNumber: public testing::Test
{ };


TEST_F(ButLogFieldLineNumber, ConvertingToString)
{
  EXPECT_EQ( toString( LineNumber{0} ), "0" );
  EXPECT_EQ( toString( LineNumber{42} ), "42" );
}


TEST_F(ButLogFieldLineNumber, ConvertingToFieldInfo)
{
  const auto fi = But::Log::Backend::FieldInfo{ LineNumber{666} };
  EXPECT_EQ( fi.type(), "But::LineNumber" );
  EXPECT_EQ( fi.value(), "666" );
}

}
