#include <string>
#include <chrono>

#include "gtest/gtest.h"
#include "ActiveObject.hpp"

using ActObj = But::Threading::ActiveObject;

namespace
{

struct ButThreadingActiveObject: public testing::Test
{
  ActObj               ao_;
  std::chrono::seconds timeout_{5};
};


TEST_F(ButThreadingActiveObject, ClosingRightAway)
{
  // leave this empty
}


TEST_F(ButThreadingActiveObject, PassSomethingToProcess)
{
  auto f = ao_.run( []{ return std::string{"narf"}; } );
  ASSERT_TRUE( f.wait_for(timeout_)==std::future_status::ready );
  EXPECT_EQ( f.get(), "narf" );
}


int getAnswer() { return 42; }

TEST_F(ButThreadingActiveObject, ProcessingFunction)
{
  auto f = ao_.run(getAnswer);
  ASSERT_TRUE( f.wait_for(timeout_)==std::future_status::ready );
  EXPECT_EQ( f.get(), 42 );
}


struct MovableOnly
{
  MovableOnly() = default;
  MovableOnly(MovableOnly&&) = default;
  MovableOnly& operator=(MovableOnly&&) = default;
  int operator()() { return 997; }
};

TEST_F(ButThreadingActiveObject, ProcessingMovableOnlyFunctor)
{
  auto f = ao_.run(MovableOnly{});
  ASSERT_TRUE( f.wait_for(timeout_)==std::future_status::ready );
  EXPECT_EQ( f.get(), 997 );
}


struct CustomExceptionType { };

TEST_F(ButThreadingActiveObject, ForwardingExceptions)
{
  auto f = ao_.run( []()->int { throw CustomExceptionType{}; } );
  ASSERT_TRUE( f.wait_for(timeout_)==std::future_status::ready );
  EXPECT_THROW( f.get(), CustomExceptionType );
}


template<int N>
int getSth() { return N; }

TEST_F(ButThreadingActiveObject, MultipleCalls)
{
  auto f0 = ao_.run(getSth<0>);
  auto f1 = ao_.run(getSth<1>);
  auto f2 = ao_.run(getSth<2>);
  ASSERT_TRUE( f0.wait_for(timeout_)==std::future_status::ready );
  EXPECT_EQ( f0.get(), 0 );
  ASSERT_TRUE( f1.wait_for(timeout_)==std::future_status::ready );
  EXPECT_EQ( f1.get(), 1 );
  ASSERT_TRUE( f2.wait_for(timeout_)==std::future_status::ready );
  EXPECT_EQ( f2.get(), 2 );
}


void doNothing() { }

TEST_F(ButThreadingActiveObject, NoReturnValueDoesNothing)
{
  auto f = ao_.run(doNothing);
  ASSERT_TRUE( f.wait_for(timeout_)==std::future_status::ready );
}

}
