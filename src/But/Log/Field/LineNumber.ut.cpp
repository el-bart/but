#include <gtest/gtest.h>
#include <But/Log/Field/LineNumber.hpp>

using But::Log::Backend::Tag;
using But::Log::Field::LineNumber;

namespace
{

struct ButLogFieldLineNumber: public testing::Test
{ };


TEST_F(ButLogFieldLineNumber, ConvertingToFieldInfo)
{
  const auto fi = toFieldInfo( LineNumber{666} );
  EXPECT_EQ( Tag{"But::LineNumber"}, fi.tag() );
  EXPECT_EQ( 666u, fi.value().get<uint64_t>() );
}

}
