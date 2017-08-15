#include <fstream>
#include "gtest/gtest.h"
#include "Stream.hpp"
#include "But/System/TempFile.hpp"

using But::Log::Destination::Stream;

namespace
{

struct OutputFileStream: public Stream
{
  explicit OutputFileStream(std::string path):
    Stream{os_},
    os_{ std::move(path) }
  { }

  void reloadImplUnderLock() override { }

  std::ofstream os_;
};

struct ButLogDestinationStream: public testing::Test
{
  ~ButLogDestinationStream()
  {
    tmp_.unlink();
  }

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
  OutputFileStream s_{ tmp_.path().string() };
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
