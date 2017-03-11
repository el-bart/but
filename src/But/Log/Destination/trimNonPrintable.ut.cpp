#include "gtest/gtest.h"
#include "trimNonPrintable.hpp"

using But::Log::Destination::trimNonPrintable;

namespace
{

struct ButLogDestinationTrimNonPrintable: public testing::Test
{ };


TEST_F(ButLogDestinationTrimNonPrintable, DataNotRequiringAnyChanges)
{
  EXPECT_EQ( trimNonPrintable(""), "" );
  EXPECT_EQ( trimNonPrintable("test string !@# 42"), "test string !@# 42" );
}


TEST_F(ButLogDestinationTrimNonPrintable, RemovingNonPrintableCharacters)
{
  EXPECT_EQ( trimNonPrintable("beep \07 / CRLF \r\n / normal: 42"), "beep . / CRLF .. / normal: 42" );
}

}
