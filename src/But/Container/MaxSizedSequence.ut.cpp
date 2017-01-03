#include "gtest/gtest.h"
#include "MaxSizedSequence.hpp"

using But::Container::MaxSizedSequence;

namespace
{

struct ButContainerMaxSizedSequence: public testing::Test
{
  using Sequence = MaxSizedSequence<std::string, 3>;

  Sequence        s_;
  Sequence const& cs_ = s_;
};


TEST_F(ButContainerMaxSizedSequence, NewlyCreatedCollectionIsEmpty)
{
  EXPECT_TRUE( cs_.empty() );
  EXPECT_EQ( cs_.size(), 0u );
  EXPECT_EQ( cs_.max_size(), 3u );
}


TEST_F(ButContainerMaxSizedSequence, AddingElementsThatFit)
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


TEST_F(ButContainerMaxSizedSequence, OverflowingDoesNotAddNewElements)
{
  EXPECT_TRUE( s_.push_back("one") );
  EXPECT_TRUE( s_.push_back("two") );
  EXPECT_TRUE( s_.push_back("three") );
  EXPECT_FALSE( s_.push_back("four") );

  // TODO: check content
}


TEST_F(ButContainerMaxSizedSequence, ConstIterating)
{
  EXPECT_TRUE( s_.push_back("one") );
  EXPECT_TRUE( s_.push_back("two") );

  // TODO: check content
}


TEST_F(ButContainerMaxSizedSequence, Iterating)
{
  EXPECT_TRUE( s_.push_back("one") );
  EXPECT_TRUE( s_.push_back("two") );

  // TODO: check content
}


TEST_F(ButContainerMaxSizedSequence, OverwritingExistingElement)
{
  EXPECT_TRUE( s_.push_back("one") );
  EXPECT_TRUE( s_.push_back("two") );

  // TODO: check content
}


TEST_F(ButContainerMaxSizedSequence, RemovingElementFromTheEnd)
{
  EXPECT_TRUE( s_.push_back("one") );
  EXPECT_TRUE( s_.push_back("two") );
  s_.pop_back();
  EXPECT_EQ( s_.size(), 1u );

  // TODO: check content
}


TEST_F(ButContainerMaxSizedSequence, ArrayWorksWithAdapter)
{
  FAIL();
  /*
  MaxSizedSequence<std::string, 3, std::array<std::string,3>> s;
  EXPECT_TRUE( s.push_back("one") );
  EXPECT_TRUE( s.push_back("two") );
  s.pop_back();
  EXPECT_EQ( s.size(), 1u );
  */

  // TODO: check content
}

}
