#include <string>
#include <vector>
#include <json.hpp>
#include <gtest/gtest.h>
#include <But/Log/Destination/JsonFile.hpp>
#include <But/System/TempFile.hpp>
#include <But/Log/Destination/detail/args2FieldInfo.hpp>

using But::Log::Destination::detail::args2FieldInfo;

using But::Log::Destination::JsonFile;
using json = nlohmann::json;

namespace
{

struct ButLogDestinationJsonFile: public testing::Test
{
  ~ButLogDestinationJsonFile()
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

  auto readLogFile()
  {
    tf_.flush();
    std::ifstream is{ tmp_.path().string() };
    if( not is.is_open() )
      throw std::runtime_error{"cannot open file: " + tmp_.path().string() };
    std::stringstream ss;
    ss << is.rdbuf();
    return ss.str();
  }

  But::System::TempFile tmp_{"/tmp/but_text_file_destination_it"};
  JsonFile tf_{ tmp_.path() };
};


TEST_F(ButLogDestinationJsonFile, LinesAreBeingWritten)
{
  EXPECT_EQ( 0u, countLines() );

  tf_.log( args2FieldInfo( "hello, ", "world!" ) );
  EXPECT_EQ( 1u, countLines() );

  tf_.log( args2FieldInfo( "the answer is: ", 42 ));
  EXPECT_EQ( 2u, countLines() );
}

TEST_F(ButLogDestinationJsonFile, SomeLogs)
{
  tf_.log( args2FieldInfo( "answer", 42 ) );
  tf_.log( args2FieldInfo( "foo", "bar" ) );
  EXPECT_EQ( 2u, countLines() );

  EXPECT_EQ( readLogFile(),
             R"xx({"int":42,"string":"answer"})xx"
             "\n"
             R"xx({"string0":"foo","string1":"bar"})xx"
             "\n" );
}


TEST_F(ButLogDestinationJsonFile, SomeFormattedLog)
{
  tf_.log( args2FieldInfo( But::Log::Field::FormattedString{"kszy"}, "answer", 42) );
  EXPECT_EQ( 1u, countLines() );

  EXPECT_EQ( readLogFile(),
             R"xx({"But::Formatted":"kszy","int":42,"string":"answer"})xx"
             "\n" );
}

}
