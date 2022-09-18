#include <gtest/gtest.h>
#include <But/Log/Backend/EntryRoot.hpp>
#include <nlohmann/json.hpp>

using But::Log::Backend::EntryRoot;

namespace
{

struct ButLogBackendEntryRoot: public testing::Test
{
  EntryRoot er_;
};

auto unify(std::string const& in)
{
  const auto tmp = nlohmann::json::parse(in);
  return tmp.dump(2);
}

#define EXPECT_EQ_JSON(a, b) \
        EXPECT_EQ( unify(a), unify( b.json() ) )


TEST_F(ButLogBackendEntryRoot, EmptyObjectByDefault)
{
  EXPECT_EQ_JSON("{}", er_);
}


TEST_F(ButLogBackendEntryRoot, ImplObjectIsSharedByDefault)
{
  er_.proxy().value("n", 42);
  auto er2 = er_;
  er2.proxy().value("test", "all good");
  EXPECT_EQ_JSON(R"({"test":"all good", "n": 42 })", er_);
  EXPECT_EQ_JSON(R"({"test":"all good", "n": 42 })", er2);
}


TEST_F(ButLogBackendEntryRoot, IndependentCopyCanBeMadeExplicitly)
{
  er_.proxy().value("initial", true);
  auto er2 = er_.independentCopy();
  er2.proxy().value("test", "all good");
  EXPECT_EQ_JSON(R"({"initial": true })", er_);
  EXPECT_EQ_JSON(R"({"test":"all good", "initial": true })", er2);
}


TEST_F(ButLogBackendEntryRoot, AddingBasicValuesViaProxy)
{
  auto p = er_.proxy();
  p.value("b", true);
  p.value("n", 42);
  p.value("fp", 3.14);
  p.value("nope");
  p.value("text", std::string_view{"foo bar"});

  EXPECT_EQ_JSON(
      R"({
           "b": true,
           "n": 42,
           "fp": 3.14,
           "nope": null,
           "text": "foo bar"
         })", er_);
}


TEST_F(ButLogBackendEntryRoot, AddingAllIntegerValues)
{
  auto p = er_.proxy();
  p.value("int",      0);
  p.value("unsigned", 1u);
  p.value("int8_t",   int8_t  {2});
  p.value("uint8_t",  uint8_t {3});
  p.value("int16_t",  int16_t {4});
  p.value("uint16_t", uint16_t{5});
  p.value("int32_t",  int32_t {6});
  p.value("uint32_t", uint32_t{7});
  p.value("int64_t",  int64_t {8});
  p.value("uint64_t", uint64_t{9});

  EXPECT_EQ_JSON(
      R"({
           "int":      0,
           "unsigned": 1,
           "int8_t":   2,
           "uint8_t":  3,
           "int16_t":  4,
           "uint16_t": 5,
           "int32_t":  6,
           "uint32_t": 7,
           "int64_t":  8,
           "uint64_t": 9
         })", er_);
}


TEST_F(ButLogBackendEntryRoot, AddingAllStringTypes)
{
  auto p = er_.proxy();
  {
    char str[] = "doh";
    p.value("raw", str);
  }
  {
    char str[] = "doh-2";
    p.value("raw ptr", static_cast<char*>(str));
  }
  p.value("const raw", "foo");
  p.value("const raw ptr", static_cast<char const*>("foo-2"));
  p.value("string", std::string{"bar"});
  p.value("string_view", std::string_view{"narf"});

  EXPECT_EQ_JSON(
      R"({
           "raw": "doh",
           "raw ptr": "doh-2",
           "const raw": "foo",
           "const raw ptr": "foo-2",
           "string": "bar",
           "string_view": "narf"
         })", er_);
}

}
