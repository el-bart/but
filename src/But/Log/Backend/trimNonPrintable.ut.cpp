#include "gtest/gtest.h"
#include "trimNonPrintable.hpp"

using But::Log::Backend::trimNonPrintable;

namespace
{

struct ButLogBackendTrimNonPrintable: public testing::Test
{ };


TEST_F(ButLogBackendTrimNonPrintable, DataNotRequiringAnyChanges)
{
  EXPECT_EQ( trimNonPrintable(""), "" );
  EXPECT_EQ( trimNonPrintable("test string !@# 42"), "test string !@# 42" );
}


TEST_F(ButLogBackendTrimNonPrintable, RemovingNonPrintableCharacters)
{
  EXPECT_EQ( trimNonPrintable("beep \07 / CRLF \r\n / normal: 42"), "beep . / CRLF .. / normal: 42" );
}


TEST_F(ButLogBackendTrimNonPrintable, CheckWole8bitRange)
{
  auto printable = 0u;
  for(auto i=0u; i<256; ++i)
    if( isprint(i) )
      ++printable;

  std::stringstream ss;
  for(auto i=0u; i<256; ++i)
    ss << (char)i;

  const auto out = trimNonPrintable( ss.str() );
  auto dots = 0u;
  for(auto c: out)
    if( c == '.' )
      ++dots;

  EXPECT_EQ( dots + printable, 256u+1u );   // +1 for '.' character... :)
}

}
