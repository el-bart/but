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
  EXPECT_EQ( "a", toValue( char{'a'} ) );
  EXPECT_EQ( "42", toValue( short{42} ) );
  EXPECT_EQ( "42", toValue( int{42} ) );
  EXPECT_EQ( "42", toValue( long{42} ) );
  EXPECT_EQ( "42", toValue( long {42} ) );
}


TEST_F(ButLogBackendToValue, UnsignedNumericTypes)
{
  {
    unsigned char tmp = 42;
    EXPECT_EQ( "42", toValue(tmp) );
  }
  {
    unsigned short tmp = 42;
    EXPECT_EQ( "42", toValue(tmp) );
  }
  {
    unsigned int tmp = 42;
    EXPECT_EQ( "42", toValue(tmp) );
  }
  {
    unsigned long tmp = 42;
    EXPECT_EQ( "42", toValue(tmp) );
  }
  {
    unsigned long  tmp = 42;
    EXPECT_EQ( "42", toValue(tmp) );
  }
}


TEST_F(ButLogBackendToValue, FloatingPointNumericTypes)
{
  EXPECT_EQ( "4.2", toValue( float{4.2} ).substr(0,3) );
  EXPECT_EQ( "4.2", toValue( double{4.2} ).substr(0,3) );
  {
    long double tmp = 4.2;
    EXPECT_EQ( "4.2", toValue(tmp).substr(0,3) );
  }
}


TEST_F(ButLogBackendToValue, StringTypes)
{
  EXPECT_EQ( "test", toValue("test") );
  EXPECT_EQ( "test", toValue( std::string{"test"} ) );
}


TEST_F(ButLogBackendToValue, BooleanType)
{
  EXPECT_EQ( "true", toValue(true) );
  EXPECT_EQ( "false", toValue(false) );
}

}
