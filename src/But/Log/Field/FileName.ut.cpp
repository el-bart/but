#include "gtest/gtest.h"
#include "FileName.hpp"
#include "But/Log/Backend/FieldInfo.hpp"

using But::Log::Field::FileName;

namespace
{

struct ButLogFieldFileName: public testing::Test
{ };


TEST_F(ButLogFieldFileName, ConvertingToString)
{
  EXPECT_EQ( toValue( FileName{"a/c"} ).get<std::string>(), "a/c" );
  EXPECT_EQ( toValue( FileName{"/foo/bar"} ).get<std::string>(), "/foo/bar" );
}


TEST_F(ButLogFieldFileName, ConvertingToFieldInfo)
{
  const auto fi = But::Log::Backend::FieldInfo{ FileName{__FILE__} };
  EXPECT_EQ( fi.type(), "But::FileName" );
  EXPECT_EQ( fi.value().get<std::string>(), __FILE__ );
}

}
