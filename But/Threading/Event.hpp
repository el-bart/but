#pragma once

#include <mutex>
#include <condition_variable>

#include "But/Exception.hpp"
#include "WaitWrapper.hpp"

namespace But
{
namespace Threading
{

/** @brief simple synchronization mechanism - one thread signals others that are waiting for an event to happen.
 */
class Event final
{
public:
  BUT_DEFINE_EXCEPTION(Timeout, Exception, "timeout while waiting for event");

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

  /** @brief wait for an event. can take 0 or 1 arguments. zero means wait forever. one can be either
   * timeout (ex. std::chrono::seconds) or deadline (ex. std::chrono::steady_clock::timepoint).
   */
  template<typename ...Args>
  void wait(Args&& ...args) const
  {
    Lock lock{m_};
    WaitHelper::wait(unblocked_, lock, [&]{ return not blocked_; }, std::forward<Args>(args)...);
  }

private:
  using Lock = std::unique_lock<std::mutex>;
  using WaitHelper = WaitWrapper<Timeout, std::condition_variable, Lock>;

  bool                            blocked_;
  mutable std::mutex              m_;
  mutable std::condition_variable unblocked_;
};

}
}
