#pragma once
#include <chrono>
#include <utility>

namespace But
{
namespace Threading
{

template<typename ConditionVariable, typename Lock>
struct WaitWrapper final
{
  WaitWrapper() = delete;

  template<typename Pred>
  static bool wait(ConditionVariable& cv, Lock& lock, Pred p)
  {
    cv.wait(lock, std::move(p));
    return true;
  }

  template<typename R, typename P, typename Pred>
  static bool wait(ConditionVariable& cv, Lock& lock, Pred&& p, const std::chrono::duration<R,P>& timeout)
  {
    return cv.wait_for( lock, timeout, std::forward<Pred>(p) );
  }

  template<typename C, typename D, typename Pred>
  static auto wait(ConditionVariable& cv, Lock& lock, Pred&& p, const std::chrono::time_point<C,D>& deadline)
  {
    return cv.wait_until( lock, deadline, std::forward<Pred>(p) );
  }
};

}
}
