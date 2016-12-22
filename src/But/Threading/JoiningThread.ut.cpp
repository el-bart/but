#include <thread>
#include <atomic>
#include <chrono>

#include "gtest/gtest.h"
#include "JoiningThread.hpp"

namespace
{

using Th = But::Threading::JoiningThread<std::thread>;

struct ButThreadingJoiningThread: public testing::Test
{
  void markDone() { done_ = true; }
  void wait()
  {
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds{5};
    while(not done_)
    {
      if( std::chrono::steady_clock::now() > deadline )
        FAIL() << "timeout while wating for thread to mark done";
      std::this_thread::yield();
    }
  }

  std::atomic<bool> done_{false};
};


TEST_F(ButThreadingJoiningThread, StartFromLambda)
{
  Th th{ [&]{ done_ = true; } };
  wait();
}


void func(std::atomic<bool>& done)
{
  done = true;
}

TEST_F(ButThreadingJoiningThread, StartFromFunction)
{
  Th th{func, std::ref(done_)};
  wait();
}


struct Functor
{
  void operator()(std::atomic<bool>& done)
  {
    done = true;
  }
};

TEST_F(ButThreadingJoiningThread, StartFromFunctor)
{
  Th th{Functor{}, std::ref(done_)};
  wait();
}


TEST_F(ButThreadingJoiningThread, StartFromMethod)
{
  Th th{&ButThreadingJoiningThread::markDone, this};
  wait();
}


TEST_F(ButThreadingJoiningThread, Joinability)
{
  Th th{ []{} };
  EXPECT_TRUE( th.joinable() );
  th.join();
  EXPECT_FALSE( th.joinable() );
}


TEST_F(ButThreadingJoiningThread, Swapping)
{
  Th th1{ []{} };
  Th th2{ []{} };
  th1.join();
  EXPECT_FALSE( th1.joinable() );
  EXPECT_TRUE(  th2.joinable() );
  th1.swap(th2);
  EXPECT_TRUE(  th1.joinable() );
  EXPECT_FALSE( th2.joinable() );
}


TEST_F(ButThreadingJoiningThread, SwappingFreeFunction)
{
  Th th1{ []{} };
  Th th2{ []{} };
  th1.join();
  EXPECT_FALSE( th1.joinable() );
  EXPECT_TRUE(  th2.joinable() );
  swap(th1, th2);
  EXPECT_TRUE(  th1.joinable() );
  EXPECT_FALSE( th2.joinable() );
}


TEST_F(ButThreadingJoiningThread, Movability)
{
  Th th1{ []{} };
  Th th2{ std::move(th1) };
  EXPECT_FALSE( th1.joinable() );
  EXPECT_TRUE(  th2.joinable() );

  th1 = std::move(th2);
  EXPECT_TRUE(  th1.joinable() );
  EXPECT_FALSE( th2.joinable() );
}

}
