#include <But/Log/Destination/File.hpp>
#include <But/System/TempFile.hpp>
#include <gtest/gtest.h>

using But::Log::Destination::File;
using But::Log::Destination::Sink;

namespace
{

struct ButLogDestinationFile: public testing::Test
{
  ~ButLogDestinationFile()
  {
    tmp_.unlink();
  }

  auto countLinesNoFlush() const
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

  auto countLines()
  {
    tf_.flush();
    return countLinesNoFlush();
  }

  But::System::TempFile tmp_{"/tmp/but_text_file_destination_it"};
  File tf_{ tmp_.path() };
};


TEST_F(ButLogDestinationFile, WritingDataToFile)
{
  EXPECT_EQ( 0u, countLines() );

  tf_.log("hello, world!");
  EXPECT_EQ( 1u, countLines() );
}


TEST_F(ButLogDestinationFile, UsingBaseSinkInterface)
{
  auto& base = static_cast<Sink&>(tf_);

  base.log("foo");
  EXPECT_EQ( 1u, countLines() );
}


TEST_F(ButLogDestinationFile, ReloadFileWorks)
{
  tf_.log("bar");
  EXPECT_EQ( 1u, countLines() );

  tmp_.unlink();
  tf_.reload();
  EXPECT_EQ( 0u, countLines() );

  tf_.log("test");
  EXPECT_EQ( 1u, countLines() );
}


TEST_F(ButLogDestinationFile, FlushingWorks)
{
  tf_.log("foo bar");
  EXPECT_EQ( 0u, countLinesNoFlush() );
  tf_.flush();
  EXPECT_EQ( 1u, countLinesNoFlush() );
}

}
