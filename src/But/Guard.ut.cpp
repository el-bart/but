#include <functional>
#include <type_traits>
#include <gtest/gtest.h>
#include <But/Guard.hpp>

using But::Guard;
using But::makeGuard;

namespace
{

struct ButGuard: public testing::Test
{ };


TEST_F(ButGuard, NonCopyable)
{
  using SampleGuard = Guard<void(*)()>;
  EXPECT_FALSE( std::is_copy_assignable<SampleGuard>::value );
  EXPECT_FALSE( std::is_copy_constructible<SampleGuard>::value );
}

unsigned g_triggersCount{0};
void testTrigger() { ++g_triggersCount; }

TEST_F(ButGuard, CreatingFromFunction)
{
  {
    Guard<void(*)()> g{testTrigger};
    EXPECT_EQ( 0u, g_triggersCount );
  }
  EXPECT_EQ( 1u, g_triggersCount );
}


TEST_F(ButGuard, CreatingFromFunctor)
{
  auto count = 0;
  {
    Guard<std::function<void()>> g{ [&](){ ++count; } };
    EXPECT_EQ( 0, count );
  }
  EXPECT_EQ( 1, count );
}


TEST_F(ButGuard, Movable)
{
  auto count = 0;
  {
    Guard<std::function<void()>> g1{ [&](){ ++count; } };
    Guard<std::function<void()>> g2{ std::move(g1) };
    EXPECT_EQ( 0, count );
  }
  EXPECT_EQ( 1, count );
}


TEST_F(ButGuard, CancelingPreventsActionExecution)
{
  auto count = 0;
  {
    Guard<std::function<void()>> g{ [&](){ ++count; } };
    g.cancel();
    EXPECT_EQ( 0, count );
  }
  EXPECT_EQ( 0, count );
}


TEST_F(ButGuard, CancelingCanHappenMultipleTimes)
{
  auto count = 0;
  {
    Guard<std::function<void()>> g{ [&](){ ++count; } };
    g.cancel();
    g.cancel();
    g.cancel();
    EXPECT_EQ( 0, count );
  }
  EXPECT_EQ( 0, count );
}


TEST_F(ButGuard, CreatingWithHelperFunction)
{
  auto count = 0;
  {
    auto g = makeGuard( [&](){ ++count; } );
    EXPECT_EQ( 0, count );
  }
  EXPECT_EQ( 1, count );
}


TEST_F(ButGuard, PassingLvalueResultsInCopy)
{
  auto count = 0;
  auto inc = [&] { ++count; };
  {
    auto g = makeGuard( inc );
    EXPECT_EQ( 0, count );
  }
  EXPECT_EQ( 1, count );
}


TEST_F(ButGuard, PassingConstLvalueResultsInCopy)
{
  auto count = 0;
  const auto inc = [&] { ++count; };
  {
    auto g = makeGuard( inc );
    EXPECT_EQ( 0, count );
  }
  EXPECT_EQ( 1, count );
}


TEST_F(ButGuard, MovableFunctorIsMoved)
{
  auto count = 0;
  struct Movable
  {
    void operator()() { }

    int* count_;
    std::unique_ptr<int> prt_{ std::make_unique<int>(42) };
  };

  {
    auto g = makeGuard( Movable{&count} );
    auto g2 = std::move(g);
  }
}

}
