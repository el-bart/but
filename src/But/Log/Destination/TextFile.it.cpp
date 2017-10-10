#include "gtest/gtest.h"
#include "TextFile.hpp"
#include "But/System/TempFile.hpp"

using But::Log::Destination::TextFile;
using But::Log::Destination::Sink;

namespace
{

struct ButLogDestinationTextFile: public testing::Test
{
  ~ButLogDestinationTextFile()
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
  TextFile tf_{ tmp_.path() };
};


TEST_F(ButLogDestinationTextFile, WritingDataToFile)
{
  EXPECT_EQ( 0u, countLines() );

  tf_.log("hello,", "world!");
  EXPECT_EQ( 1u, countLines() );

  tf_.log("the answer is:", 42);
  EXPECT_EQ( 2u, countLines() );

  tf_.log("non-printable\nchars\nare\nignored");
  EXPECT_EQ( 3u, countLines() );
}


TEST_F(ButLogDestinationTextFile, UsingBaseSinkInterface)
{
  auto& base = static_cast<Sink&>(tf_);

  base.log("test", "data");
  EXPECT_EQ( 1u, countLines() );

  base.log("is anybody there?");
  EXPECT_EQ( 2u, countLines() );
}


TEST_F(ButLogDestinationTextFile, ReloadFileWorks)
{
  tf_.log("test", "data");
  EXPECT_EQ( 1u, countLines() );

  tmp_.unlink();
  tf_.reload();
  EXPECT_EQ( 0u, countLines() );

  tf_.log("is anybody there?");
  EXPECT_EQ( 1u, countLines() );
}


TEST_F(ButLogDestinationTextFile, FlushingWorks)
{
  tf_.log("test", "data");
  EXPECT_EQ( 0u, countLinesNoFlush() );
  tf_.flush();
  EXPECT_EQ( 1u, countLinesNoFlush() );
}

}
