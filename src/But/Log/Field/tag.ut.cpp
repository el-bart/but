#include <But/Log/Field/tag.hpp>
#include <But/Log/Backend/EntryRootTestBase.ut.hpp>

using But::Log::Backend::EntryProxy;
using But::Log::Backend::EntryArray;
using But::Log::Field::tag;
using But::Log::Field::detail::DynamicallyNamedObject;

namespace
{

struct ButLogFieldTag: public But::Log::Backend::EntryRootTestBase
{
  But::Log::Backend::EntryProxy p_{ er_.proxy() };
};


struct Value { int value_; };

struct Object
{
  std::string s_;
  std::unique_ptr<double> pd_;
};

struct Array { int tab_[4]; };


TEST_F(ButLogFieldTag, TaggingBasicValueCreatesValidObject)
{
  DynamicallyNamedObject<int> const d = tag("correct", 42);
  EXPECT_EQ(d.name_, "correct");
  EXPECT_EQ(d.nested_, 42);
}


TEST_F(ButLogFieldTag, TaggingValueCreatesValidObject)
{
  DynamicallyNamedObject<Value> const d = tag("correct", Value{42});
  EXPECT_EQ(d.name_, "correct");
  EXPECT_EQ(d.nested_.value_, 42);
}


TEST_F(ButLogFieldTag, TaggingObjectCreatesValidObject)
{
  DynamicallyNamedObject<Object> const d = tag("correct", Object{"x", nullptr});
  EXPECT_EQ(d.name_, "correct");
  EXPECT_EQ(d.nested_.s_, "x");
  EXPECT_EQ(d.nested_.pd_.get(), nullptr);
}


TEST_F(ButLogFieldTag, TaggingArrayCreatesValidObject)
{
  DynamicallyNamedObject<Array> const d = tag("correct", Array{{1,2,3,4}});
  EXPECT_EQ(d.name_, "correct");
  EXPECT_EQ(d.nested_.tab_[0], 1);
  EXPECT_EQ(d.nested_.tab_[1], 2);
  EXPECT_EQ(d.nested_.tab_[2], 3);
  EXPECT_EQ(d.nested_.tab_[3], 4);
}


struct NamedValue
{
  int value_;
};

auto fieldName(NamedValue const*) { return "NamedValue"; }
int fieldValue(NamedValue const& v) { return v.value_; }


struct NamedObject
{
  std::string s_;
  int n_;
};

auto fieldName(NamedObject const*) { return "NamedObject"; }
void objectValue(EntryProxy& e, NamedObject const& o)
{
  e.value("str", o.s_);
  e.value("n", o.n_);
}


struct NamedArray
{
  int tab_[4];
};

auto fieldName(NamedArray const*) { return "NamedArray"; }
void arrayValue(EntryArray& e, NamedArray const& a)
{
  using std::begin;
  using std::end;
  e.add( begin(a.tab_), end(a.tab_) );
}


TEST_F(ButLogFieldTag, TaggingBasicUnnamedValueCreatesValidObject)
{
  auto const d = tag("correct", 42);
  objectValue(p_, d);
  EXPECT_EQ_JSON( er_.json(), R"( {"correct": 42} )" );
}


TEST_F(ButLogFieldTag, TaggingNamedValueCreatesValidObject)
{
  auto const d = tag("correct", NamedValue{42});
  objectValue(p_, d);
  EXPECT_EQ_JSON( er_.json(), R"( { "correct": { "NamedValue": 42 } } )" );
}


TEST_F(ButLogFieldTag, TaggingNamedObjectCreatesValidObject)
{
  auto const d = tag("correct", NamedObject{"x", 13});
  objectValue(p_, d);
  EXPECT_EQ_JSON( er_.json(), R"( { "correct": { "NamedObject": { "str": "x", "n": 13 } } } )" );
}


TEST_F(ButLogFieldTag, TaggingNamedArrayCreatesValidObject)
{
  auto const d = tag("correct", NamedArray{{1,2,3,4}});
  objectValue(p_, d);
  EXPECT_EQ_JSON( er_.json(), R"( { "correct": { "NamedArray": [1,2,3,4] } } )" );
}

}
