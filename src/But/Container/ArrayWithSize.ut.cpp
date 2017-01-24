#include <memory>
#include <type_traits>
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


TEST_F(ButContainerArrayWithSize, ElementsCanBeMovedInWithPushBack)
{
  ArrayWithSize<std::unique_ptr<int>, 3> tab;
  tab.push_back( std::make_unique<int>(42) );
  EXPECT_EQ( tab.size(), 1u );
}


TEST_F(ButContainerArrayWithSize, SizeTypeIsMinimalForParametrizedSize)
{
  // yes, yes... this is not portable, not guaranteed, etc...

  using Small = ArrayWithSize<char, 3>;
  EXPECT_EQ( sizeof(Small), sizeof(std::array<char,3>) + 1u );

  using Medium = ArrayWithSize<char, 1022>;
  EXPECT_EQ( sizeof(Medium), sizeof(std::array<char,1022>) + 2u );

  using Large = ArrayWithSize<char, 123000>;
  EXPECT_EQ( sizeof(Large), sizeof(std::array<char,123000>) + 4u );
}


TEST_F(ButContainerArrayWithSize, InitializingWithInitializerList)
{
  Sequence s{"test", "values"};
  ASSERT_EQ( s.size(), 2u );
  EXPECT_EQ( s[0], "test" );
  EXPECT_EQ( s[1], "values" );
}


TEST_F(ButContainerArrayWithSize, MovableAndCopyable)
{
  EXPECT_TRUE( std::is_copy_constructible<Sequence>::value );
  EXPECT_TRUE( std::is_copy_assignable<Sequence>::value );

  EXPECT_TRUE( std::is_move_constructible<Sequence>::value );
  EXPECT_TRUE( std::is_move_assignable<Sequence>::value );
}


struct CopyObserver final
{
  CopyObserver() = default;

  explicit CopyObserver(unsigned& copies):
    copies_{&copies}
  { }

  CopyObserver(CopyObserver const& other):
    copies_{other.copies_}
  {
    ++(*copies_);
  }

  CopyObserver& operator=(CopyObserver const& other)
  {
    copies_ = other.copies_;
    ++(*copies_);
    return *this;
  }

  unsigned* copies_{nullptr};
};


TEST_F(ButContainerArrayWithSize, CopyConstructingContainerCopiesOnlyElementsInUse)
{
  auto copyCount = 0u;
  CopyObserver co1{copyCount};
  CopyObserver co2{copyCount};

  using CpSeq= ArrayWithSize<CopyObserver, 3>;
  CpSeq cs1{co1, co2};
  copyCount = 0;
  CpSeq cs2{cs1};
  EXPECT_EQ( cs2.size(), cs1.size() );
  EXPECT_EQ(copyCount, 2u);
}


TEST_F(ButContainerArrayWithSize, CopyAssignmenTofContainerCopiesOnlyElementsInUse)
{
  auto copyCount = 0u;
  CopyObserver co1{copyCount};
  CopyObserver co2{copyCount};

  using CpSeq= ArrayWithSize<CopyObserver, 3>;
  CpSeq cs1{co1, co2};
  CpSeq cs2;
  copyCount = 0;
  cs2 = cs1;
  EXPECT_EQ(copyCount, 2u);
}


TEST_F(ButContainerArrayWithSize, CopyAsignmentOverwritesPreviousContent)
{
  Sequence s1{"abc"};
  const Sequence s2{"test", "values"};
  s1 = s2;
  ASSERT_EQ( s1.size(), 2u );
  EXPECT_EQ( s1[0], "test" );
  EXPECT_EQ( s1[1], "values" );
}


TEST_F(ButContainerArrayWithSize, CopyAsignmentToSelfDoesNothing)
{
  Sequence s{"test", "values"};
  s = s;
  ASSERT_EQ( s.size(), 2u );
  EXPECT_EQ( s[0], "test" );
  EXPECT_EQ( s[1], "values" );
}


TEST_F(ButContainerArrayWithSize, ClearingContainersContent)
{
  Sequence s{"test", "values"};
  EXPECT_EQ( s.size(), 2u );
  s.clear();
  EXPECT_EQ( s.size(), 0u );
}

}
