#include <string>
#include <chrono>
#include "gtest/gtest.h"
#include "ThreadPool.hpp"
#include "Policy/Std.hpp"
#include "Policy/Boost.hpp"
#include "Event.hpp"

using But::Threading::ThreadsCount;
using ThreadPoolStd = But::Threading::ThreadPool<But::Threading::Policy::Std>;
using ThreadPoolBoost = But::Threading::ThreadPool<But::Threading::Policy::Boost>;

namespace
{

template<typename ThreadPoolType>
struct ButThreadingThreadPool: public testing::Test
{
  using Type = ThreadPoolType;

  template<typename T>
  bool waitForFuture(std::future<T>& f) const
  {
    return f.wait_for(timeout_) == std::future_status::ready;
  }

  template<typename T>
#ifdef BOOST_THREAD_PROVIDES_FUTURE
  bool waitForFuture(boost::future<T>& f) const
#else
  bool waitForFuture(boost::unique_future<T>& f) const
#endif
  {
    return f.wait_for( boost::chrono::seconds{timeout_.count()} ) == boost::future_status::ready;
  }

  ThreadPoolType tp_{ ThreadsCount{1} };
  std::chrono::seconds timeout_{5};
};

TYPED_TEST_CASE_P(ButThreadingThreadPool);



TYPED_TEST_P(ButThreadingThreadPool, ClosingRightAway)
{
  // leave this empty
}


TYPED_TEST_P(ButThreadingThreadPool, PassSomethingToProcess)
{
  auto f = this->tp_.run( []{ return std::string{"narf"}; } );
  ASSERT_TRUE( this->waitForFuture(f) );
  EXPECT_EQ( f.get(), "narf" );
}


int getAnswer() { return 42; }

TYPED_TEST_P(ButThreadingThreadPool, ProcessingFunction)
{
  auto f = this->tp_.run(getAnswer);
  ASSERT_TRUE( this->waitForFuture(f) );
  EXPECT_EQ( f.get(), 42 );
}


struct MovableOnly
{
  MovableOnly() = default;
  MovableOnly(MovableOnly&&) = default;
  MovableOnly& operator=(MovableOnly&&) = default;
  int operator()() { return 997; }
};

TYPED_TEST_P(ButThreadingThreadPool, ProcessingMovableOnlyFunctor)
{
  auto f = this->tp_.run(MovableOnly{});
  ASSERT_TRUE( this->waitForFuture(f) );
  EXPECT_EQ( f.get(), 997 );
}


TYPED_TEST_P(ButThreadingThreadPool, ForwardingException)
{
  auto f = this->tp_.run( []()->int { throw std::runtime_error{"expected"}; } );
  ASSERT_TRUE( this->waitForFuture(f) );
  EXPECT_ANY_THROW( f.get() );  // note: boost throws a different exception type here...
}


template<int N>
int getSth() { return N; }

TYPED_TEST_P(ButThreadingThreadPool, MultipleCalls)
{
  auto f0 = this->tp_.run(getSth<0>);
  auto f1 = this->tp_.run(getSth<1>);
  auto f2 = this->tp_.run(getSth<2>);
  ASSERT_TRUE( this->waitForFuture(f0) );
  EXPECT_EQ( f0.get(), 0 );
  ASSERT_TRUE( this->waitForFuture(f1) );
  EXPECT_EQ( f1.get(), 1 );
  ASSERT_TRUE( this->waitForFuture(f2) );
  EXPECT_EQ( f2.get(), 2 );
}


void doNothing() { }

TYPED_TEST_P(ButThreadingThreadPool, NoReturnValueSmokeTest)
{
  auto f = this->tp_.run(doNothing);
  ASSERT_TRUE( this->waitForFuture(f) );
}


struct CustomExceptionType { };

TYPED_TEST_P(ButThreadingThreadPool, ForwardingExceptionWithAnExactTypeInStdVersion)
{
  // this test does not work in boost... :/
  if( std::is_same<decltype(this->tp_), ThreadPoolBoost>::value )
    return;
  auto f = this->tp_.run( []()->int { throw CustomExceptionType{}; } );
  ASSERT_TRUE( this->waitForFuture(f) );
  EXPECT_THROW( f.get(), CustomExceptionType );
}


TYPED_TEST_P(ButThreadingThreadPool, ProcessingIsRunningInSeparateThread)
{
  auto f = this->tp_.run( []{ return std::this_thread::get_id(); } );
  ASSERT_TRUE( this->waitForFuture(f) );
  EXPECT_TRUE( f.get() != std::this_thread::get_id() );
}


TYPED_TEST_P(ButThreadingThreadPool, GettingThreadPoolSize)
{
  using PoolType = decltype(this->tp_);
  EXPECT_EQ( 3u, PoolType{ ThreadsCount{3} }.size() );
  EXPECT_EQ( 1u, this->tp_.size() );
}


TYPED_TEST_P(ButThreadingThreadPool, RunningOnMultipleThreads)
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
                                event.wait(this->timeout_);
                                return std::this_thread::get_id();
                              } ) );

  while( started != count )
    std::this_thread::yield();

  event.set();
  for(auto& f: ids)
    ASSERT_TRUE( this->waitForFuture(f) );

  std::set<std::thread::id> uniqueIds;
  for(auto& f: ids)
    uniqueIds.insert( f.get() );
  EXPECT_EQ( count, uniqueIds.size() ) << "not all requests were spawned from a separate thread";

  for(auto& id: uniqueIds)
    EXPECT_TRUE( id != std::this_thread::get_id() );
}


REGISTER_TYPED_TEST_CASE_P(ButThreadingThreadPool,
        ClosingRightAway,
        PassSomethingToProcess,
        ProcessingFunction,
        ProcessingMovableOnlyFunctor,
        ForwardingException,
        MultipleCalls,
        NoReturnValueSmokeTest,
        ForwardingExceptionWithAnExactTypeInStdVersion,
        ProcessingIsRunningInSeparateThread,
        GettingThreadPoolSize,
        RunningOnMultipleThreads
    );


INSTANTIATE_TYPED_TEST_CASE_P(Std,   ButThreadingThreadPool, ::testing::Types<ThreadPoolStd>);
INSTANTIATE_TYPED_TEST_CASE_P(Boost, ButThreadingThreadPool, ::testing::Types<ThreadPoolBoost>);

}
