#include <memory>
#include <type_traits>
#include <gtest/gtest.h>
#include <But/Container/ArrayWithSize.hpp>
#include <But/assert.hpp>

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

  ASSERT_EQ( s2.size(), 2u );
  EXPECT_EQ( s2[0], "test" );
  EXPECT_EQ( s2[1], "values" );
}


TEST_F(ButContainerArrayWithSize, CopyAsignmentToSelfDoesNothing)
{
  Sequence s{"test", "values"};
  auto *tmp = &s;   // workaorund for (otherwise reasonable) warning from clang, that we're assining to self
  s = *tmp;
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


struct MoveObserver final
{
  MoveObserver() = default;

  explicit MoveObserver(unsigned& moves):
    moves_{&moves}
  { }

  MoveObserver(MoveObserver&& other):
    moves_{other.moves_}
  {
    ++(*moves_);
    other.moves_ = nullptr;
  }

  MoveObserver& operator=(MoveObserver&& other)
  {
    moves_ = other.moves_;
    ++(*moves_);
    other.moves_ = nullptr;
    return *this;
  }

  unsigned* moves_{nullptr};
};


TEST_F(ButContainerArrayWithSize, MoveConstructingContainerMovesOnlyElementsInUse)
{
  auto moveCount = 0u;

  using CpSeq= ArrayWithSize<MoveObserver, 3>;
  CpSeq cs1;
  cs1.push_back( MoveObserver{moveCount} );
  cs1.push_back( MoveObserver{moveCount} );
  moveCount = 0;
  CpSeq cs2{ std::move(cs1) };
  EXPECT_EQ( cs1.size(), 0u );
  EXPECT_EQ( cs2.size(), 2u );
  EXPECT_EQ(moveCount, 2u);
}


TEST_F(ButContainerArrayWithSize, MoveAssignmenTofContainerMovesOnlyElementsInUse)
{
  auto copyCount = 0u;
  using CpSeq= ArrayWithSize<MoveObserver, 3>;
  CpSeq cs1;
  cs1.push_back( MoveObserver{copyCount} );
  cs1.push_back( MoveObserver{copyCount} );
  CpSeq cs2;
  copyCount = 0;
  cs2 = std::move(cs1);
  EXPECT_EQ( cs1.size(), 0u );
  EXPECT_EQ( cs2.size(), 2u );
  EXPECT_EQ(copyCount, 2u);
}


TEST_F(ButContainerArrayWithSize, MoveAsignmentOverwritesPreviousContent)
{
  using CpSeq= ArrayWithSize<std::unique_ptr<int>, 3>;
  CpSeq s1;
  s1.push_back( std::make_unique<int>(997) );
  CpSeq s2;
  s2.push_back( std::make_unique<int>(69) );
  s2.push_back( std::make_unique<int>(42) );
  s1 = std::move(s2);
  EXPECT_EQ( s2.size(), 0u );
  ASSERT_EQ( s1.size(), 2u );
  EXPECT_EQ( *s1[0], 69 );
  EXPECT_EQ( *s1[1], 42 );
}


TEST_F(ButContainerArrayWithSize, MoveAsignmentToSelfDoesNothing)
{
  using CpSeq= ArrayWithSize<std::unique_ptr<int>, 3>;
  CpSeq s;
  s.push_back( std::make_unique<int>(997) );
  s.push_back( std::make_unique<int>(42) );
  auto&& tmp = std::move(s);    // little hack to disable compiler warning
  tmp = std::move(s);
  ASSERT_EQ( s.size(), 2u );
  EXPECT_EQ( *s[0], 997 );
  EXPECT_EQ( *s[1],  42 );
}


struct CtorTest
{
  CtorTest(): called_{0} { }
  CtorTest(int): called_{1} { }
  CtorTest(double): called_{2} { }
  CtorTest(double, int): called_{3} { }

  CtorTest(CtorTest const&) = delete;
  CtorTest& operator=(CtorTest const&) = delete;
  CtorTest(CtorTest&&) = default;
  CtorTest& operator=(CtorTest&&) = default;

  int called_;
};


TEST_F(ButContainerArrayWithSize, EmplacingBack)
{
  using Seq= ArrayWithSize<CtorTest, 4>;
  Seq s;
  s.emplace_back();
  s.emplace_back(42);
  s.emplace_back(3.14);
  s.emplace_back(3.14, 42);
  ASSERT_EQ( s.size(), 4u );
  EXPECT_EQ( s[0].called_, 0 );
  EXPECT_EQ( s[1].called_, 1 );
  EXPECT_EQ( s[2].called_, 2 );
  EXPECT_EQ( s[3].called_, 3 );
}


TEST_F(ButContainerArrayWithSize, ComparingContainers)
{
  using Seq= ArrayWithSize<int, 4>;
  const Seq s1{1, 2, 3};
  const Seq s2{1, 2, 3, 4};
  const Seq s3{1, 4, 3};
  const Seq s5{1, 2, 9};

  EXPECT_TRUE(  s1 == s1 );
  EXPECT_FALSE( s1 == s2 );
  EXPECT_FALSE( s1 == s3 );

  EXPECT_FALSE( s1 != s1 );
  EXPECT_TRUE(  s1 != s2 );
  EXPECT_TRUE(  s1 != s3 );

  EXPECT_FALSE( s1 < s1 );
  EXPECT_TRUE(  s1 < s2 );
  EXPECT_TRUE(  s1 < s5 );

  EXPECT_FALSE( s2 <= s1 );
  EXPECT_TRUE(  s1 <= s1 );
  EXPECT_TRUE(  s1 <= s2 );
  EXPECT_TRUE(  s1 <= s5 );

  EXPECT_FALSE( s1 > s1 );
  EXPECT_TRUE(  s2 > s1 );
  EXPECT_TRUE(  s5 > s1 );

  EXPECT_FALSE( s1 >= s2 );
  EXPECT_TRUE(  s1 >= s1 );
  EXPECT_TRUE(  s2 >= s1 );
  EXPECT_TRUE(  s5 >= s1 );
}


constexpr auto constexprSmokeTest()
{
  ArrayWithSize<int,4> tmp1;
  ArrayWithSize<int,4> tmp2{tmp1};
  tmp1 = tmp2;
  tmp1 = std::move(tmp2);

  {
    ArrayWithSize<int,4> tab{1,2};
    BUT_ASSERT( not tab.empty() );
    BUT_ASSERT( tab.size() == 2u );
    BUT_ASSERT( tab.max_size() == 4u );
    tab.emplace_back(42);
    tab.push_back(13);
    tab.pop_back();
    BUT_ASSERT( tab[0] == 1 );
    auto sum = 0;
    for(auto e: tab)
      sum += e;
    BUT_ASSERT( sum == 1+2+42 );
    tab.clear();
  }

  const ArrayWithSize<int,4> tabObj{4,2,0};
  const auto& tab = tabObj;
  BUT_ASSERT( tab[1] == 2 );
  auto sum = 0;
  for(auto e: tab)
    sum += e;
  BUT_ASSERT( sum == 4+2+0 );
  return tab;
}

TEST_F(ButContainerArrayWithSize, ConstexprSmokeTest)
{
  constexpr const auto seq = constexprSmokeTest();
  EXPECT_EQ( 3, seq.size() );
}

}
