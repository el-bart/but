#include <random>
#include <string>
#include <algorithm>
#include <type_traits>

#include "gtest/gtest.h"
#include "UnorderedArray.hpp"

using But::UnorderedArray;

namespace
{

struct Data
{
  Data(std::string s, const int n): str_(std::move(s)), number_(n) { }

  Data(Data const&) = default;
  Data& operator=(Data const&) = default;

  Data(Data&&) = default;
  Data& operator=(Data&&) = default;

  std::string str_;
  int         number_;
};

struct ButUnorderedArray: public testing::Test
{
  using DataArray = UnorderedArray<Data>;
  DataArray data_;
};


TEST_F(ButUnorderedArray, AddingNewElementByConstRef)
{
  EXPECT_TRUE( data_.empty() );
  EXPECT_EQ( data_.size(), 0u );
  Data d{"answer", 42};
  data_.add(d);
  EXPECT_FALSE( data_.empty() );
  EXPECT_EQ( data_.size(), 1u );
}


TEST_F(ButUnorderedArray, AddingNewElementByRvalue)
{
  EXPECT_TRUE( data_.empty() );
  EXPECT_EQ( data_.size(), 0u );
  data_.add( Data{"answer", 42} );
  EXPECT_FALSE( data_.empty() );
  EXPECT_EQ( data_.size(), 1u );
}


TEST_F(ButUnorderedArray, EmplacingNewElement)
{
  EXPECT_TRUE( data_.empty() );
  EXPECT_EQ( data_.size(), 0u );
  data_.emplace("police", 997);
  EXPECT_FALSE( data_.empty() );
  EXPECT_EQ( data_.size(), 1u );
}


TEST_F(ButUnorderedArray, ModifyingElementWithIterator)
{
  data_.emplace("abc", 123);
  std::begin(data_)->str_ = "foo";
  EXPECT_EQ( std::begin(data_)->str_, "foo" );
}


TEST_F(ButUnorderedArray, ReferencingElementWithConstIterator)
{
  data_.emplace("devil", 666);
  auto const& cdata = data_;
  EXPECT_EQ( std::begin(cdata)->str_, "devil" );
}


TEST_F(ButUnorderedArray, ModifyingElementWithIndex)
{
  data_.emplace("abc", 123);
  data_[0].number_ = 69;
  EXPECT_EQ(69, data_[0].number_);
}


TEST_F(ButUnorderedArray, ReferencingElementWithIndex)
{
  data_.emplace("abc", 123);
  auto const& cdata = data_;
  EXPECT_EQ(123, cdata[0].number_);
}


TEST_F(ButUnorderedArray, RemovingLastElement)
{
  data_.emplace("a", 1);
  data_.emplace("b", 2);
  ASSERT_EQ( data_.size(), 2u );
  const auto itFirst = std::begin( static_cast<DataArray const&>(data_) );
  const auto itLast  = itFirst + 1;
  const auto first = *itFirst;
  const auto last  = *itLast;
  data_.erase(itLast);
  ASSERT_EQ( data_.size(), 1u );
  EXPECT_EQ(first.number_, data_[0].number_);
}


TEST_F(ButUnorderedArray, RemovingFirstElement)
{
  data_.emplace("a", 1);
  data_.emplace("b", 2);
  ASSERT_EQ( data_.size(), 2u );
  const auto itFirst = data_.cbegin();
  const auto itLast  = itFirst + 1;
  const auto first = *itFirst;
  const auto last  = *itLast;
  data_.erase(itFirst);
  ASSERT_EQ( data_.size(), 1u );
  EXPECT_EQ(last.number_, data_[0].number_);
}


TEST_F(ButUnorderedArray, RemovingMiddleElement)
{
  data_.emplace("a", 1);
  data_.emplace("b", 2);
  data_.emplace("c", 3);
  ASSERT_EQ( data_.size(), 3u );
  const auto itFirst  = data_.cbegin();
  const auto itMiddle = itFirst + 1;
  const auto itLast   = itMiddle + 1;
  const std::vector<int> expected{ itFirst->number_, itLast->number_ };
  data_.erase(itMiddle);
  ASSERT_EQ( data_.size(), 2u );
  for(const auto e: data_)
  {
    const auto cmp = [&e](const auto n) { return e.number_ == n; };
    EXPECT_TRUE( (std::find_if( std::begin(expected), std::end(expected), cmp ) != std::end(expected)) );
  }
}


TEST_F(ButUnorderedArray, RemovingTheOnlyElementInCollection)
{
  data_.emplace("a", 1);
  data_.erase( std::begin(data_) );
  EXPECT_EQ( data_.size(), 0u );
}


TEST_F(ButUnorderedArray, IteratorIncrement)
{
  data_.emplace("a", 1);
  auto it = std::begin(data_);
  ++it;
  EXPECT_TRUE( it == std::end(data_) );
}


TEST_F(ButUnorderedArray, RandomAccessIterator)
{
  data_.emplace("a", 1);
  const auto it = std::begin(data_);
  EXPECT_TRUE( it + 1 == std::end(data_) );
}


TEST_F(ButUnorderedArray, IteratorDecrement)
{
  data_.emplace("a", 1);
  auto it = std::begin(data_);
  ++it;
  --it;
  EXPECT_TRUE( it == std::begin(data_) );
}


TEST_F(ButUnorderedArray, ConstIterating)
{
  data_.emplace("a", 1);
  const auto& c = data_;
  const auto  b = c.begin();
  const auto  e = c.end();
  EXPECT_TRUE(b!=e);
  EXPECT_TRUE( (std::is_same<decltype(b), const DataArray::const_iterator>()) );
  EXPECT_TRUE( (std::is_same<decltype(e), const DataArray::const_iterator>()) );
}


TEST_F(ButUnorderedArray, ConstIteratingWithC)
{
  data_.emplace("a", 1);
  const auto b = data_.cbegin();
  const auto e = data_.cend();
  EXPECT_TRUE(b!=e);
  EXPECT_TRUE( (std::is_same<decltype(b), const DataArray::const_iterator>()) );
  EXPECT_TRUE( (std::is_same<decltype(e), const DataArray::const_iterator>()) );
}


TEST_F(ButUnorderedArray, DecreasingAllocationWhenRemoving)
{
  for(auto i=0; i<100; ++i)
    data_.emplace("a", i);
  const auto capPre = data_.capacity();

  for(auto i=0; i<60; ++i)
    data_.erase( std::begin(data_) );
  const auto capPost = data_.capacity();

  EXPECT_TRUE( capPre > capPost );
}


TEST_F(ButUnorderedArray, DoNotDecreaseCapacityForSmallContainers)
{
  for(auto i=0; i<5; ++i)
    data_.emplace("a", i);
  const auto capPre = data_.capacity();

  for(auto i=0; i<2; ++i)
    data_.erase( std::begin(data_) );
  const auto capPost = data_.capacity();

  EXPECT_TRUE( capPre == capPost );
}


TEST_F(ButUnorderedArray, NeededTypedefs)
{
  EXPECT_TRUE( (std::is_same<DataArray::value_type, Data>()) );
  EXPECT_TRUE( std::is_unsigned<DataArray::size_type>() );
  DataArray::iterator it;
  DataArray::const_iterator itc;
}


TEST_F(ButUnorderedArray, RemovingAllElementsAtOnce)
{
  for(auto i=0; i<100; ++i)
    data_.emplace("a", i);
  data_.clear();
  EXPECT_TRUE( data_.empty() );
  EXPECT_LT( data_.capacity(), 100u );
}


TEST_F(ButUnorderedArray, ReservingSpaceForElements)
{
  data_.reserve(1000);
  EXPECT_GE( data_.capacity(), 1000u );
}


TEST_F(ButUnorderedArray, ReservingSpaceForElementsOnNonEmptyContainer)
{
  data_.emplace("a", 911);
  data_.emplace("a", 911);
  data_.reserve(1000);
  EXPECT_GE( data_.capacity(), 1000u );
  // TODO: capacity+reserve
}


TEST_F(ButUnorderedArray, SortingContainerWorksFine)
{
  // prepare random input data
  {
    constexpr auto                     seed = 42;
    std::mt19937                       gen(seed);
    std::uniform_int_distribution<int> dist(0, 1000);
    for(auto i=0; i<200; ++i)
      data_.emplace("not used", dist(gen));
  }

  const auto cmp = [](auto const& lhs, auto const& rhs) { return lhs.number_ < rhs.number_; };
  EXPECT_FALSE( (std::is_sorted( std::begin(data_), std::end(data_), cmp )) );
  std::sort( std::begin(data_), std::end(data_), cmp );
  EXPECT_TRUE( (std::is_sorted( std::begin(data_), std::end(data_), cmp )) );
}


TEST_F(ButUnorderedArray, ConstructingWithInitializerList)
{
  const DataArray data{ {"abc", 1}, {"def", 2} };
  ASSERT_EQ( 2u, data.size() );
  std::set<int> s;
  for(auto const& e: data)
    s.insert(e.number_);
  EXPECT_TRUE( (s == std::set<int>{1,2}) );
}


TEST_F(ButUnorderedArray, Copyable)
{
  EXPECT_TRUE( std::is_copy_constructible<DataArray>() );
  EXPECT_TRUE( std::is_copy_assignable<DataArray>() );
}


TEST_F(ButUnorderedArray, Movable)
{
  EXPECT_TRUE( std::is_move_constructible<DataArray>() );
  EXPECT_TRUE( std::is_move_assignable<DataArray>() );
}


TEST_F(ButUnorderedArray, AllocatorAware)
{
  // TODO: ac
}

}
