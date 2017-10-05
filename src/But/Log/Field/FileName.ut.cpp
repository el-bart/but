#include "gtest/gtest.h"
#include "FileName.hpp"

using But::Log::Backend::Type;
using But::Log::Field::FileName;

namespace
{

struct ButLogFieldFileName: public testing::Test
{ };


TEST_F(ButLogFieldFileName, ConvertingToFieldInfo)
{
  const auto fi = toFieldInfo( FileName{__FILE__} );
  EXPECT_EQ( Type{"But::FileName"}, fi.type() );
  EXPECT_EQ( __FILE__, fi.value().get<std::string>() );
}

}
