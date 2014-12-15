#pragma once

#include <chrono>
#include <utility>
#include "But/Exception.hpp"


namespace But
{
namespace Threading
{

template<typename TimeoutException, typename ConditionVariable, typename Lock>
struct WaitWrapper final
{
  template<typename Pred>
  static void wait(ConditionVariable& cv, Lock& lock, Pred p)
  {
    cv.wait(lock, std::move(p));
  }

  template<typename R, typename P, typename Pred>
  static void wait(ConditionVariable& cv, Lock& lock, Pred p, const std::chrono::duration<R,P>& timeout)
  {
    if( not cv.wait_for(lock, timeout, std::move(p)) )
      BUT_THROW(TimeoutException, std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count() << "[ms] passed");
  }

  template<typename C, typename D, typename Pred>
  static void wait(ConditionVariable& cv, Lock& lock, Pred p, const std::chrono::time_point<C,D>& deadline)
  {
    const auto timeout = deadline - C::now();
    if( not cv.wait_until(lock, deadline, std::move(p)) )
      BUT_THROW(TimeoutException,  std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count() << "[ms] passed");
  }
};

}
}
