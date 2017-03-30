#include <typeinfo>
#include "gtest/gtest.h"
#include "toString.hpp"

using But::Log::Backend::toString;

namespace
{

struct ButLogBackendToString: public testing::Test
{ };


TEST_F(ButLogBackendToString, SignedNumericTypes)
{
  EXPECT_EQ( "a", toString( char{'a'} ) );
  EXPECT_EQ( "42", toString( short{42} ) );
  EXPECT_EQ( "42", toString( int{42} ) );
  EXPECT_EQ( "42", toString( long{42} ) );
  EXPECT_EQ( "42", toString( long {42} ) );
}


TEST_F(ButLogBackendToString, UnsignedNumericTypes)
{
  {
    unsigned char tmp = 42;
    EXPECT_EQ( "42", toString(tmp) );
  }
  {
    unsigned short tmp = 42;
    EXPECT_EQ( "42", toString(tmp) );
  }
  {
    unsigned int tmp = 42;
    EXPECT_EQ( "42", toString(tmp) );
  }
  {
    unsigned long tmp = 42;
    EXPECT_EQ( "42", toString(tmp) );
  }
  {
    unsigned long  tmp = 42;
    EXPECT_EQ( "42", toString(tmp) );
  }
}


TEST_F(ButLogBackendToString, FloatingPointNumericTypes)
{
  EXPECT_EQ( "4.2", toString( float{4.2} ).substr(0,3) );
  EXPECT_EQ( "4.2", toString( double{4.2} ).substr(0,3) );
  {
    long double tmp = 4.2;
    EXPECT_EQ( "4.2", toString(tmp).substr(0,3) );
  }
}


TEST_F(ButLogBackendToString, StringTypes)
{
  EXPECT_EQ( "test", toString("test") );
  EXPECT_EQ( "test", toString( std::string{"test"} ) );
}

}
