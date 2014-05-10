#pragma once

#include <chrono>
#include <utility>
#include "But/Exception.hpp"


namespace But
{
namespace Threading
{

template<typename TimeoutException, typename CondidionVariable, typename Lock>
struct WaitWrapper
{
  template<typename Pred>
  static void wait(ConditionVariable& cv, Lock& lock, Pred p) const
  {
    cv.wait(lock, std::move(p));
  }

  template<typename R, typename P, typename Pred>
  static void wait(Lock& lock, const std::chrono::duration<R,P>& timeout, Pred p) const
  {
    if( not nonEmpty_.wait_for(lock, timeout, std::move(p)) )
      BUT_THROW(TimeoutException, std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count() << "[ms] passed");
  }

  template<typename C, typename D, typename Pred>
  static void wait(lock_type& lock, const std::chrono::time_point<C,D>& deadline, Pred p) const
  {
    const auto timeout = deadline - C::now();
    if( not nonEmpty_.wait_until(lock, deadline, std::move(p)) )
      BUT_THROW(TimeoutException,  std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count() << "[ms] passed");
  }
};

}
}
