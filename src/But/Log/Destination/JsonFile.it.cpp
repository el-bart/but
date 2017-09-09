#include <string>
#include <vector>
#include <json.hpp>
#include "gtest/gtest.h"
#include "JsonFile.hpp"
#include "But/System/TempFile.hpp"

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

  auto readLastLogLine()
  {
    tf_.flush();
    std::ifstream is{ tmp_.path().string() };
    if( not is.is_open() )
      throw std::runtime_error{"cannot open file: " + tmp_.path().string() };
    std::vector<std::string> lines;
    while(is)
    {
      std::string line;
      std::getline(is, line);
      lines.push_back( std::move(line) );
    }
    return lines.at( lines.size() - 2u );
  }

  template<typename T>
  auto testType(std::string const& typeName, const T t, std::string const& expectedValue)
  {
    tf_.log(t);
    const auto expected = "[{\"" + typeName + "\":" + expectedValue + "}]";
    const auto read = readLastLogLine();
    EXPECT_EQ(expected, read);
    return expected==read;
  }
  template<typename T>
  auto testType(std::string const& typeName, const T t)
  {
    return testType( typeName, t, std::to_string(t) );
  }

  But::System::TempFile tmp_{"/tmp/but_text_file_destination_it"};
  JsonFile tf_{ tmp_.path() };
};


TEST_F(ButLogDestinationJsonFile, LinesAreBeingWritten)
{
  EXPECT_EQ( 0u, countLines() );

  tf_.log("hello, ", "world!");
  EXPECT_EQ( 1u, countLines() );

  tf_.log("the answer is: ", 42);
  EXPECT_EQ( 2u, countLines() );
}


TEST_F(ButLogDestinationJsonFile, UsingNonprintableCharacters)
{
  tf_.log("A\nB\rC");
  EXPECT_EQ( 1u, countLines() );

  EXPECT_EQ( "[{\"string\":\"A\\nB\\rC\"}]\n", readLogFile() );
}


TEST_F(ButLogDestinationJsonFile, SomeLogs)
{
  tf_.log("answer", 42);
  tf_.log("foo", "bar");
  EXPECT_EQ( 2u, countLines() );

  EXPECT_EQ( R"xx([{"string":"answer"},{"int":42}])xx"
             "\n"
             R"xx([{"string":"foo"},{"string":"bar"}])xx"
             "\n"
             , readLogFile() );
}


TEST_F(ButLogDestinationJsonFile, SomeFormattedLog)
{
  tf_.log( But::Log::Field::FormattedString{"kszy"}, "answer", 42);
  EXPECT_EQ( 1u, countLines() );

  EXPECT_EQ( R"xx([{"But::Formatted":"kszy"},{"string":"answer"},{"int":42}])xx"
             "\n"
             , readLogFile() );
}


TEST_F(ButLogDestinationJsonFile, NumericTypes)
{
  EXPECT_TRUE( testType<short>("int", -42) );
  EXPECT_TRUE( testType<unsigned short>("unsigned int", 42) );

  EXPECT_TRUE( testType<int>("int", -42) );
  EXPECT_TRUE( testType<unsigned int>("unsigned int", 42) );

  EXPECT_TRUE( testType<long>("int", -42) );
  EXPECT_TRUE( testType<unsigned long>("unsigned int", 42) );

  EXPECT_TRUE( testType<long long>("int", -42) );
  EXPECT_TRUE( testType<unsigned long long>("unsigned int", 42) );

  EXPECT_TRUE( testType<float>("double", -4.0, "-4") );
  EXPECT_TRUE( testType<double>("double", -4.2, "-4.2") );
  EXPECT_TRUE( testType<long double>("double", -4.2, "-4.2") );
}

}
