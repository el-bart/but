#pragma once

#include <mutex>
#include <condition_variable>

#include "But/Exception.hpp"

namespace But
{
namespace Threading
{

class Event
{
public:
  BUT_DEFINE_EXCEPTION(Timeout, "timeout while waiting for event")
  Event():
    blocked_{true}
  { }

  void set()
  {
    {
      Lock lock{m_};
      blocked_ = false;
    }
    unblocked_.notify_all();
  }

  void clear()
  {
    Lock lock{m_};
    blocked_ = true;
  }

  template<typename ...Args>
  void wait(Args&&... args) const
  {
  }

private:
  using Lock = std::unique_lock<std::mutex>;

  void wait(Lock& lock) const
  {
    unblocked_.wait(lock, [&]{ return not empty(); });
  }

  template<typename R, typename P>
  void wait(Lock& lock, const std::chrono::duration<R,P>& timeout) const
  {
    if( not nonEmpty_.wait_for(lock, timeout, [&]{ return not empty(); }) )
      BUT_THROW(Timeout, std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count() << "[ms] passed");
  }

  template<typename C, typename D>
  void wait(lock_type& lock, const std::chrono::time_point<C,D>& deadline) const
  {
    const auto timeout = deadline - C::now();
    if( not nonEmpty_.wait_until(lock, deadline, [&]{ return not empty(); }) )
      BUT_THROW(Timeout,  std::chrono::duration_cast<std::chrono::milliseconds>(timeout).count() << "[ms] passed");
  }

  bool                    blocked_;
  std::mutex              m_;
  std::condition_variable unblocked_;
};

}
}
