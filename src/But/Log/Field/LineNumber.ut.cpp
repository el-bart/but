#include "gtest/gtest.h"
#include "LineNumber.hpp"

using But::Log::Backend::Type;
using But::Log::Field::LineNumber;

namespace
{

struct ButLogFieldLineNumber: public testing::Test
{ };


TEST_F(ButLogFieldLineNumber, ConvertingToFieldInfo)
{
  const auto fi = toFieldInfo( LineNumber{666} );
  EXPECT_EQ( Type{"But::LineNumber"}, fi.type() );
  EXPECT_EQ( 666u, fi.value().get<uint64_t>() );
}

}
