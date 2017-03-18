#include <fstream>
#include "gtest/gtest.h"
#include "Stream.hpp"
#include "But/System/TempFile.hpp"

using But::Log::Destination::Stream;

namespace
{

struct ButLogDestinationStream: public testing::Test
{
  auto countLines() const
  {
    std::ifstream is{ tmp_.path().string() };
    if( not is.is_open() )
      throw std::runtime_error{"cannot open file: " + tmp_.path().string() };
    auto lines = 0u;
    std::string ignored;
    while( std::getline(is, ignored) )
      ++lines;
    return lines;
  }

  But::System::TempFile tmp_{"/tmp/but_text_file_destination_it"};
  std::ofstream os_{ tmp_.path().string() };
  Stream s_{os_};
};


TEST_F(ButLogDestinationStream, FlushingWorks)
{
  EXPECT_EQ( 0u, countLines() );
  s_.log("test");
  EXPECT_EQ( 0u, countLines() );

  s_.flush();
  EXPECT_EQ( 1u, countLines() );
}

}
