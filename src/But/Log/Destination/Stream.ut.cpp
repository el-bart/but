#include "gtest/gtest.h"
#include "Stream.hpp"
#include "But/Log/Field/LineNumber.hpp"

using But::Log::Destination::Stream;
using But::Log::Field::LineNumber;

namespace
{

struct ButLogDestinationStream: public testing::Test
{
  std::stringstream ss_;
  Stream s_{ss_};
};


TEST_F(ButLogDestinationStream, PrintingSampleData)
{
  s_.log( "line: ", LineNumber{42} );
  EXPECT_EQ( ss_.str(), "line: 42\n" );
}


TEST_F(ButLogDestinationStream, CheckArrowOperator)
{
  s_->log( "line: ", LineNumber{42} );
  EXPECT_EQ( ss_.str(), "line: 42\n" );
}


TEST_F(ButLogDestinationStream, RemovingNonPrintableCharacters)
{
  s_.log( "beep \07 / CRLF \r\n / normal: ", LineNumber{42} );
  EXPECT_EQ( ss_.str(), "beep . / CRLF .. / normal: 42\n" );
}

}
