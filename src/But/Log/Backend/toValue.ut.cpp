#include <typeinfo>
#include "gtest/gtest.h"
#include "toValue.hpp"

using But::Log::Backend::toValue;

namespace
{

struct ButLogBackendToValue: public testing::Test
{ };


TEST_F(ButLogBackendToValue, SignedNumericTypes)
{
  EXPECT_EQ( "a", toValue( char{'a'} ).get<std::string>() );
  EXPECT_EQ( 42, toValue( short{42} ).get<int64_t>() );
  EXPECT_EQ( 42, toValue( int{42} ).get<int64_t>() );
  EXPECT_EQ( 42, toValue( long{42} ).get<int64_t>() );
  EXPECT_EQ( 42, toValue( long {42} ).get<int64_t>() );
}


TEST_F(ButLogBackendToValue, UnsignedNumericTypes)
{
  {
    unsigned char tmp = 42;
    EXPECT_EQ( 42u, toValue(tmp).get<uint64_t>() );
  }
  {
    unsigned short tmp = 42;
    EXPECT_EQ( 42u, toValue(tmp).get<uint64_t>() );
  }
  {
    unsigned int tmp = 42;
    EXPECT_EQ( 42u, toValue(tmp).get<uint64_t>() );
  }
  {
    unsigned long tmp = 42;
    EXPECT_EQ( 42u, toValue(tmp).get<uint64_t>() );
  }
  {
    unsigned long  tmp = 42;
    EXPECT_EQ( 42u, toValue(tmp).get<uint64_t>() );
  }
}


TEST_F(ButLogBackendToValue, FloatingPointNumericTypes)
{
  EXPECT_EQ( 10.5, toValue( float{10.5} ).get<double>() );
  EXPECT_EQ( 10.5, toValue( double{10.5} ).get<double>() );
  {
    long double tmp = 10.5;
    EXPECT_EQ( 10.5, toValue(tmp).get<double>() );
  }
}


TEST_F(ButLogBackendToValue, StringTypes)
{
  EXPECT_EQ( "test", toValue("test").get<std::string>() );
  EXPECT_EQ( "test", toValue( std::string{"test"} ).get<std::string>() );
}


TEST_F(ButLogBackendToValue, BooleanType)
{
  EXPECT_EQ( true, toValue(true).get<bool>() );
  EXPECT_EQ( false, toValue(false).get<bool>() );
}

}
