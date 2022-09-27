#include <gtest/gtest.h>
#include <But/Log/Backend/EntryRoot.hpp>
#include <nlohmann/json.hpp>

using But::Log::Backend::EntryRoot;
using But::Log::Backend::EntryProxy;
using But::Log::Backend::EntryArray;

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


TEST_F(ButLogBackendEntryRoot, CreatingSubobjects)
{
  auto p = er_.proxy();
  p.value("str", "data");
  auto p2 = p.object("subobject");
  p2.value("b", true);
  p2.value("n", 42);

  EXPECT_EQ_JSON(
      R"({
           "str": "data",
           "subobject": {
             "b": true,
             "n": 42
           }
         })", er_);
}


TEST_F(ButLogBackendEntryRoot, CreatingSubarrays)
{
  auto p = er_.proxy();
  {
    auto a = p.array("bools");
    a.value(true);
    a.value(false);
  }
  {
    auto a = p.array("ints");
    a.value(int8_t{1});
    a.value(uint8_t{2});
    a.value(int16_t{3});
    a.value(uint16_t{4});
    a.value(int32_t{5});
    a.value(uint32_t{6});
    a.value(int64_t{7});
    a.value(uint64_t{8});
  }
  {
    auto a = p.array("floats");
    a.value(float{1.5});
    a.value(double{2.72});
  }
  {
    auto a = p.array("strings");
    a.value( "cstr" );
    a.value( static_cast<char const*>("cstr ptr") );
    {
      char s[] = "non-const";
      a.value(s);
    }
    {
      char s[] = "non-const ptr";
      a.value( static_cast<char*>(s) );
    }
    a.value( std::string{"string"} );
    a.value( std::string_view{"string_view"} );
  }
  {
    auto a = p.array("misc");
    a.object();
    a.array();
  }

  EXPECT_EQ_JSON(
      R"({
           "bools": [ true, false ],
           "ints": [ 1,2,3,4,5,6,7,8 ],
           "floats": [ 1.5, 2.72 ],
           "strings": [ "cstr", "cstr ptr", "non-const", "non-const ptr", "string", "string_view" ],
           "misc": [ {}, [] ]
         })", er_);
}


struct Answer { int value_{}; };
auto fieldName(Answer const*) { return std::string_view{"Answer"}; }
auto fieldValue(Answer const& o) { return o.value_; }

struct Aggregate
{
  int i_{};
  std::string s_;
};
auto fieldName(Aggregate const*) { return std::string_view{"Aggregate"}; }
void objectValue(EntryProxy& p, Aggregate const& o)
{
  p.value("i", o.i_);
  p.value("s", o.s_);
}

struct SimpleCollection1
{
  std::vector<int> c_;
};
auto fieldName(SimpleCollection1 const*) { return std::string_view{"SimpleCollection1"}; }
void arrayValue(EntryArray& p, SimpleCollection1 const& o)
{
  for(auto e: o.c_)
    p.value(e);
}

struct SimpleCollection2
{
  std::vector<int> c_;
};
auto fieldName(SimpleCollection2 const*) { return std::string_view{"SimpleCollection2"}; }
void arrayValue(EntryArray& p, SimpleCollection2 const& o) { p.add(o.c_); }

TEST_F(ButLogBackendEntryRoot, CreatingObjectsViaNesting)
{
  auto p = er_.proxy();
  p.nest( Answer{42} );
  p.nest( Aggregate{ 13, "foo" } );
  p.nest( SimpleCollection1{ {13, 42, 666} } );
  p.nest( SimpleCollection2{ {997, 10} } );
  EXPECT_EQ_JSON(
      R"({
           "Answer": 42,
           "Aggregate": { "i": 13, "s": "foo" },
           "SimpleCollection1": [ 13, 42, 666 ],
           "SimpleCollection2": [ 997, 10 ]
         })", er_);
}

// TODO: ntesting arrays

}
