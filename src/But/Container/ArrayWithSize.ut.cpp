#include <memory>
#include "gtest/gtest.h"
#include "ArrayWithSize.hpp"

using But::Container::ArrayWithSize;

namespace
{

struct ButContainerArrayWithSize: public testing::Test
{
  using Sequence = ArrayWithSize<std::string, 3>;

  Sequence        s_;
  Sequence const& cs_ = s_;
};


TEST_F(ButContainerArrayWithSize, NewlyCreatedCollectionIsEmpty)
{
  EXPECT_TRUE( cs_.empty() );
  EXPECT_EQ( cs_.size(), 0u );
  EXPECT_EQ( cs_.max_size(), 3u );
}


TEST_F(ButContainerArrayWithSize, AddingElements)
{
  EXPECT_EQ( cs_.max_size(), 3u );

  s_.push_back("one");
  EXPECT_FALSE( cs_.empty() );
  EXPECT_EQ( cs_.size(), 1u );
  EXPECT_EQ( cs_[0], "one" );

  s_.push_back("two");
  EXPECT_EQ( cs_.size(), 2u );
  EXPECT_EQ( cs_[1], "two" );

  s_.push_back("three");
  ASSERT_EQ( cs_.size(), 3u );
  EXPECT_EQ( cs_[2], "three" );
}


TEST_F(ButContainerArrayWithSize, ConstIterating)
{
  s_.push_back("one");
  s_.push_back("two");

  auto it = begin(cs_);
  ASSERT_TRUE( it != end(cs_) );
  EXPECT_EQ( *it, "one" );

  ++it;
  ASSERT_TRUE( it != end(cs_) );
  EXPECT_EQ( *it, "two" );

  ++it;
  ASSERT_TRUE( it == end(cs_) );
}


TEST_F(ButContainerArrayWithSize, Iterating)
{
  s_.push_back("one");
  s_.push_back("two");

  auto it = begin(s_);
  ASSERT_TRUE( it != end(s_) );
  EXPECT_EQ( *it, "one" );

  ++it;
  ASSERT_TRUE( it != end(s_) );
  EXPECT_EQ( *it, "two" );

  ++it;
  ASSERT_TRUE( it == end(s_) );
}


TEST_F(ButContainerArrayWithSize, RandomAccess)
{
  s_.push_back("one");
  s_.push_back("two");
  ASSERT_EQ( s_.size(), 2u );
  EXPECT_EQ( cs_[0], "one" );
  EXPECT_EQ( cs_[1], "two" );
}


TEST_F(ButContainerArrayWithSize, OverwritingExistingElement)
{
  s_.push_back("one");
  s_.push_back("two");

  s_[0] = "0";
  s_[1] = "1";
  ASSERT_EQ( cs_.size(), 2u );
  EXPECT_EQ( cs_[0], "0" );
  EXPECT_EQ( cs_[1], "1" );
}


TEST_F(ButContainerArrayWithSize, RemovingElementFromTheEnd)
{
  s_.push_back("one");
  s_.push_back("two");
  s_.pop_back();
  ASSERT_EQ( s_.size(), 1u );
  EXPECT_EQ( cs_[0], "one" );
}


TEST_F(ButContainerArrayWithSize, WhenElementIsPopedItIsOverwritenWithDefaultValue)
{
  auto num = std::make_shared<int>(42);
  std::weak_ptr<int> weak{num};
  EXPECT_TRUE( weak.lock() );

  ArrayWithSize<std::shared_ptr<int>, 3> tab;

  tab.push_back( std::move(num) );
  EXPECT_TRUE( weak.lock() );

  tab.pop_back();
  EXPECT_FALSE( weak.lock() );
}


TEST_F(ButContainerArrayWithSize, SizeTypeIsMinimalForParametrizedSize)
{
  // yes, yes... this is not portable, not guaranteed, etc...

  using Small = ArrayWithSize<char, 3>;
  EXPECT_EQ( sizeof(Small), sizeof(std::array<char,3>) + 1u );

  using Medium = ArrayWithSize<char, 3000>;
  EXPECT_EQ( sizeof(Medium), sizeof(std::array<char,3000>) + 2u );

  using Large = ArrayWithSize<char, 123000>;
  EXPECT_EQ( sizeof(Large), sizeof(std::array<char,123000>) + 4u );
}

}