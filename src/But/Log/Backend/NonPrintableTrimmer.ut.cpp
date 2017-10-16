#include <gtest/gtest.h>
#include <But/Log/Backend/NonPrintableTrimmer.hpp>

using But::Log::Backend::NonPrintableTrimmer;

namespace
{

struct ButLogBackendNonPrintableTrimmer: public testing::Test
{
  const NonPrintableTrimmer trim_;
};


TEST_F(ButLogBackendNonPrintableTrimmer, DataNotRequiringAnyChanges)
{
  EXPECT_EQ( trim_(""), "" );
  EXPECT_EQ( trim_("test string !@# 42"), "test string !@# 42" );
}


TEST_F(ButLogBackendNonPrintableTrimmer, RemovingNonPrintableCharacters)
{
  EXPECT_EQ( trim_("beep \07 / CRLF \r\n / normal: 42"), "beep . / CRLF .. / normal: 42" );
}


TEST_F(ButLogBackendNonPrintableTrimmer, CheckWole8bitRange)
{
  auto printable = 0u;
  for(auto i=0u; i<256; ++i)
    if( isprint(i) )
      ++printable;

  std::stringstream ss;
  for(auto i=0u; i<256; ++i)
    ss << (char)i;

  const auto out = trim_( ss.str() );
  auto dots = 0u;
  for(auto c: out)
    if( c == '.' )
      ++dots;

  EXPECT_EQ( dots + printable, 256u+1u );   // +1 for '.' character... :)
}

}
