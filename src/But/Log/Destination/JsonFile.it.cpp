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

  EXPECT_EQ( "[{\"std::string\":\"A\\nB\\rC\"}]\n", readLogFile() );
}


TEST_F(ButLogDestinationJsonFile, SomeLogs)
{
  tf_.log("answer", 42);
  tf_.log("foo", "bar");
  EXPECT_EQ( 2u, countLines() );

  EXPECT_EQ( R"xx([{"std::string":"answer"},{"int":42}])xx"
             "\n"
             R"xx([{"std::string":"foo"},{"std::string":"bar"}])xx"
             "\n"
             , readLogFile() );
}


TEST_F(ButLogDestinationJsonFile, NumericTypes)
{
  EXPECT_TRUE( testType<short>("short", -42) );
  EXPECT_TRUE( testType<unsigned short>("unsigned short", 42) );

  EXPECT_TRUE( testType<int>("int", -42) );
  EXPECT_TRUE( testType<unsigned int>("unsigned int", 42) );

  EXPECT_TRUE( testType<long>("long", -42) );
  EXPECT_TRUE( testType<unsigned long>("unsigned long", 42) );

  EXPECT_TRUE( testType<long long>("long long", -42) );
  EXPECT_TRUE( testType<unsigned long long>("unsigned long long", 42) );

  EXPECT_TRUE( testType<float>("float", -4.0, "-4") );
  EXPECT_TRUE( testType<double>("double", -4.2, "-4.2") );
  EXPECT_TRUE( testType<long double>("long double", -4.2, "-4.2") );
}

}
