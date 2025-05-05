#include <string>
#include <chrono>
#include <thread>
#include <gtest/gtest.h>
#include <But/Threading/Event.hpp>
#include <But/Threading/ThreadPool.hpp>
#include <But/Threading/detail/waitForFuture.ut.hpp>
#include <But/gtest_compat.utg.hpp>
#include <But/detail/tsan.hpp>

using But::Threading::ThreadsCount;
using But::Threading::detail::waitForFuture;

namespace
{

struct ButThreadingThreadPool: public testing::Test
{
  But::Threading::ThreadPool tp_{ ThreadsCount{1} };
};

BUT_TYPED_TEST_SUITE_P(ButThreadingThreadPool);



TEST_F(ButThreadingThreadPool, ClosingRightAway)
{
  // leave this empty
}


TEST_F(ButThreadingThreadPool, PassSomethingToProcess)
{
  auto f = this->tp_.run( []{ return std::string{"narf"}; } );
  ASSERT_TRUE( waitForFuture(f) );
  EXPECT_EQ( f.get(), "narf" );
}


int getAnswer() { return 42; }

TEST_F(ButThreadingThreadPool, ProcessingFunction)
{
  auto f = this->tp_.run(getAnswer);
  ASSERT_TRUE( waitForFuture(f) );
  EXPECT_EQ( f.get(), 42 );
}


struct MovableOnly
{
  MovableOnly() = default;
  MovableOnly(MovableOnly&&) = default;
  MovableOnly& operator=(MovableOnly&&) = default;
  int operator()() { return 997; }
};

TEST_F(ButThreadingThreadPool, ProcessingMovableOnlyFunctor)
{
  auto f = this->tp_.run(MovableOnly{});
  ASSERT_TRUE( waitForFuture(f) );
  EXPECT_EQ( f.get(), 997 );
}


TEST_F(ButThreadingThreadPool, ForwardingException)
{
  auto f = this->tp_.run( []()->int { throw std::runtime_error{"expected"}; } );
  ASSERT_TRUE( waitForFuture(f) );
  EXPECT_THROW( f.get(), std::runtime_error );
}


template<int N>
int getSth() { return N; }

TEST_F(ButThreadingThreadPool, MultipleCalls)
{
  auto f0 = this->tp_.run(getSth<0>);
  auto f1 = this->tp_.run(getSth<1>);
  auto f2 = this->tp_.run(getSth<2>);
  ASSERT_TRUE( waitForFuture(f0) );
  EXPECT_EQ( f0.get(), 0 );
  ASSERT_TRUE( waitForFuture(f1) );
  EXPECT_EQ( f1.get(), 1 );
  ASSERT_TRUE( waitForFuture(f2) );
  EXPECT_EQ( f2.get(), 2 );
}


void doNothing() { }

TEST_F(ButThreadingThreadPool, NoReturnValueSmokeTest)
{
  auto f = this->tp_.run(doNothing);
  ASSERT_TRUE( waitForFuture(f) );
}


struct CustomExceptionType { };

TEST_F(ButThreadingThreadPool, ForwardingExceptionWithAnExactType)
{
  auto f = this->tp_.run( []()->int { throw CustomExceptionType{}; } );
  ASSERT_TRUE( waitForFuture(f) );
  EXPECT_THROW( f.get(), CustomExceptionType );
}


TEST_F(ButThreadingThreadPool, ProcessingIsRunningInSeparateThread)
{
  auto f = this->tp_.run( []{ return std::this_thread::get_id(); } );
  ASSERT_TRUE( waitForFuture(f) );
  EXPECT_TRUE( f.get() != std::this_thread::get_id() );
}


TEST_F(ButThreadingThreadPool, GettingThreadPoolSize)
{
  using PoolType = decltype(this->tp_);
  EXPECT_EQ( 3u, PoolType{ ThreadsCount{3} }.size() );
  EXPECT_EQ( 1u, this->tp_.size() );
}


#ifdef BUT_THREAD_SANITIZER_WORKAROUND_NEEDED

TEST_F(ButThreadingThreadPool, RunningOnMultipleThreads)
{
  constexpr auto count = 4u;
  using PoolType = decltype(this->tp_);
  PoolType tp{ ThreadsCount{count} };

  std::atomic<unsigned> started{0};
  But::Threading::Event event;
  event.clear();

  std::vector< decltype(typename PoolType::template promise_type<std::thread::id>{}.get_future()) > ids;
  for(auto i=0u; i<count; ++i)
    ids.push_back( tp.run( [&]{
                                ++started;
                                event.wait( std::chrono::seconds{5} );
                                return std::this_thread::get_id();
                              } ) );

  while( started != count )
    std::this_thread::yield();

  event.set();
  for(auto& f: ids)
    ASSERT_TRUE( waitForFuture(f) );

  std::set<std::thread::id> uniqueIds;
  for(auto& f: ids)
    uniqueIds.insert( f.get() );
  EXPECT_EQ( count, uniqueIds.size() ) << "not all requests were spawned from a separate thread";

  for(auto& id: uniqueIds)
    EXPECT_TRUE( id != std::this_thread::get_id() );
}

#endif // BUT_THREAD_SANITIZER_WORKAROUND_NEEDED

}
