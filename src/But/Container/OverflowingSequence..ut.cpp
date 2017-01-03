#include "gtest/gtest.h"
#include "OverflowingSequence.hpp"
#include "ArrayWithSize.hpp"

using But::Container::OverflowingSequence;

namespace
{

struct ButContainerOverflowingSequence: public testing::Test
{
  using Sequence = OverflowingSequence<std::string, 3>;

  Sequence        s_;
  Sequence const& cs_ = s_;
};


TEST_F(ButContainerOverflowingSequence, NewlyCreatedCollectionIsEmpty)
{
  EXPECT_TRUE( cs_.empty() );
  EXPECT_EQ( cs_.size(), 0u );
  EXPECT_EQ( cs_.max_size(), 3u );
}


TEST_F(ButContainerOverflowingSequence, AddingElementsThatFit)
{
  EXPECT_EQ( cs_.max_size(), 3u );

  EXPECT_TRUE( s_.push_back("one") );

  EXPECT_FALSE( cs_.empty() );
  EXPECT_EQ( cs_.size(), 1u );

  EXPECT_TRUE( s_.push_back("two") );
  EXPECT_EQ( cs_.size(), 2u );

  EXPECT_TRUE( s_.push_back("three") );
  ASSERT_EQ( cs_.size(), 3u );

  // TODO: check content
}


TEST_F(ButContainerOverflowingSequence, OverflowingDoesNotAddNewElements)
{
  EXPECT_TRUE( s_.push_back("one") );
  EXPECT_TRUE( s_.push_back("two") );
  EXPECT_TRUE( s_.push_back("three") );
  EXPECT_FALSE( s_.push_back("four") );

  // TODO: check content
}


TEST_F(ButContainerOverflowingSequence, ConstIterating)
{
  EXPECT_TRUE( s_.push_back("one") );
  EXPECT_TRUE( s_.push_back("two") );

  // TODO: check content
}


TEST_F(ButContainerOverflowingSequence, Iterating)
{
  EXPECT_TRUE( s_.push_back("one") );
  EXPECT_TRUE( s_.push_back("two") );

  // TODO: check content
}


TEST_F(ButContainerOverflowingSequence, OverwritingExistingElement)
{
  EXPECT_TRUE( s_.push_back("one") );
  EXPECT_TRUE( s_.push_back("two") );

  // TODO: check content
}


TEST_F(ButContainerOverflowingSequence, RemovingElementFromTheEnd)
{
  EXPECT_TRUE( s_.push_back("one") );
  EXPECT_TRUE( s_.push_back("two") );
  s_.pop_back();
  EXPECT_EQ( s_.size(), 1u );

  // TODO: check content
}


TEST_F(ButContainerOverflowingSequence, ArrayWorksWithAdapter)
{
  OverflowingSequence<std::string, 3, But::Container::ArrayWithSize<std::string,3>> s;
  EXPECT_TRUE( s.push_back("one") );
  EXPECT_TRUE( s.push_back("two") );
  s.pop_back();
  EXPECT_EQ( s.size(), 1u );

  // TODO: check content
}

}
