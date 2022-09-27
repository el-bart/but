#include <gtest/gtest.h>
#include <But/Log/Field/FileName.hpp>

using But::Log::Field::FileName;

namespace
{

struct ButLogFieldFileName: public testing::Test
{ };


TEST_F(ButLogFieldFileName, ConvertingToFieldInfo)
{
  const auto fn = FileName{__FILE__};
  EXPECT_EQ( fieldName(&fn), "But::FileName" );
  EXPECT_EQ( fieldValue(fn), std::string{__FILE__} );
}

}
