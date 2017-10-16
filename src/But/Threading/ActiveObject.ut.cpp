#include <string>
#include <chrono>
#include <gtest/gtest.h>
#include <But/Threading/ActiveObject.hpp>
#include <But/Threading/Policy/Std.hpp>
#include <But/Threading/Policy/Boost.hpp>
#include <But/Threading/detail/waitForFuture.ut.hpp>

using But::Threading::detail::waitForFuture;


namespace
{

template<typename Policy>
struct ButThreadingActiveObject: public testing::Test
{
  But::Threading::ActiveObject<Policy> ao_;
};

TYPED_TEST_CASE_P(ButThreadingActiveObject);


TYPED_TEST_P(ButThreadingActiveObject, ClosingRightAway)
{
  // leave this empty
}


TYPED_TEST_P(ButThreadingActiveObject, ProcessingIsRunningInSeparateThread)
{
  auto f = this->ao_.run( []{ return std::this_thread::get_id(); } );
  ASSERT_TRUE( waitForFuture(f) );
  EXPECT_TRUE( f.get() != std::this_thread::get_id() );
}


REGISTER_TYPED_TEST_CASE_P(ButThreadingActiveObject,
        ClosingRightAway,
        ProcessingIsRunningInSeparateThread
    );


INSTANTIATE_TYPED_TEST_CASE_P(Std,   ButThreadingActiveObject, ::testing::Types<But::Threading::Policy::Std>);
INSTANTIATE_TYPED_TEST_CASE_P(Boost, ButThreadingActiveObject, ::testing::Types<But::Threading::Policy::Boost>);

}
