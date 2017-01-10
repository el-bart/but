#include <vector>
#include <string>
#include <sstream>
#include <type_traits>
#include "gtest/gtest.h"
#include "OffsetIterator.hpp"

using But::Container::OffsetIterator;

namespace
{

struct ButContainerOffsetIterator: public testing::Test
{
  using Collection = std::vector<std::string>;
  using Iter      = OffsetIterator<Collection>;
  using ConstIter = OffsetIterator<const Collection>;

  Collection c_{"alice", "has", "a", "cat"};
  Collection const& cc_{c_};
  std::stringstream ss_;
};


TEST_F(ButContainerOffsetIterator, Iteration)
{
  const Iter begin{c_, 0};
  const Iter end{c_, c_.size()};

  for(Iter it=begin; it!=end; ++it)
    ss_ << *it;
  EXPECT_EQ( ss_.str(), "alicehasacat");
}


TEST_F(ButContainerOffsetIterator, ModifyingIteration)
{
  const Iter it{c_, 1};
  *it = "value";
  EXPECT_EQ( *it, "value" );
  EXPECT_EQ( cc_[1], "value" );
}


TEST_F(ButContainerOffsetIterator, ConstIteration)
{
  const ConstIter begin{c_, 0};
  const ConstIter end{c_, c_.size()};

  for(ConstIter it=begin; it!=end; ++it)
    ss_ << *it;
  EXPECT_EQ( ss_.str(), "alicehasacat");
}


TEST_F(ButContainerOffsetIterator, MovingBackwards)
{
  ConstIter it{c_, 1};
  EXPECT_EQ( *it, "has" );
  --it;
  EXPECT_EQ( *it, "alice" );
}


TEST_F(ButContainerOffsetIterator, IteratorAndConstIteratorInteroperability)
{
  const Iter       it{c_, 1};
  const ConstIter cit{c_, 1};

  ConstIter tmp{it};
  EXPECT_TRUE( tmp == it );
  EXPECT_TRUE( it == cit );
  EXPECT_TRUE( cit == it );
}


TEST_F(ButContainerOffsetIterator, RequiredTypedefsAreThere)
{
  EXPECT_TRUE( ( std::is_same<Iter::value_type, std::string>::value ) );
  EXPECT_TRUE( ( std::is_same<ConstIter::value_type, const std::string>::value ) );

  EXPECT_TRUE( ( std::is_same<Iter::iterator_category, std::bidirectional_iterator_tag>::value ) );
}


TEST_F(ButContainerOffsetIterator, ComparisonOperators)
{
  const Iter       it{c_, 2};
  const ConstIter cit{c_, 1};

  EXPECT_TRUE( it  != cit );
  EXPECT_TRUE( cit != it );
}

}
