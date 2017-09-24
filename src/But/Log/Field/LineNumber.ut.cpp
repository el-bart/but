#include "gtest/gtest.h"
#include "LineNumber.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

using But::Log::Backend::Type;
using But::Log::Field::LineNumber;

namespace
{

struct ButLogFieldLineNumber: public testing::Test
{ };


TEST_F(ButLogFieldLineNumber, ConvertingToString)
{
  EXPECT_EQ( toValue( LineNumber{0} ).get<uint64_t>(), 0u );
  EXPECT_EQ( toValue( LineNumber{42} ).get<uint64_t>(), 42u );
}


TEST_F(ButLogFieldLineNumber, ConvertingToFieldInfo)
{
  const auto fi = But::Log::Backend::FieldInfo{ LineNumber{666} };
  EXPECT_EQ( fi.type(), Type{"But::LineNumber"} );
  EXPECT_EQ( fi.value().get<uint64_t>(), 666u );
}

}
