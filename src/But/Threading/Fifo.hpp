#pragma once
#include <deque>
#include <mutex>
#include <utility>
#include <condition_variable>
#include <But/assert.hpp>
#include <But/Exception.hpp>
#include <But/Threading/WaitWrapper.hpp>

namespace But
{
namespace Threading
{

/** @brief thread-safe queue for pushing elements between threads.
 *
 * @par
 * queue needs to be externally locked beofre it can be used from multiple threads.
 * <code>
 *   using Queue = Fifo<std::string>;
 *   Queue q;
 *   Queue::lock_type lock{q};
 *   if( not q.empty() )
 *     q.pop();
 * </code>
 *
 * @par
 * each "get" operations (top(), pop()) has 4 ways to be called. no-args call (ex. pop()) expects that there is
 * something to operate on in the queue. there is no runtime check for this, so betting something from empty
 * queue is an undefined behavior. there is lock-taking version (ex. pop(lock)), that blocks forever, until element
 * is available. each such call can also take one more argument: timeout (time duration) or a deadline (time point).
 * if one is passed (ex. pop(lock, std::chrono::seconds{5})) and timeout/dedaline is meet, Timeout exception is
 * thrown.
 */
template<typename T>
class Fifo final
{
  using Queue = std::deque<T>;

public:
  using value_type = T;
  using size_type  = typename Queue::size_type;
  using lock_type  = std::unique_lock<Fifo<T>>;

private:
  using WaitHelper = WaitWrapper<std::condition_variable_any, lock_type>;

public:
  Fifo() = default;

  void lock()
  {
    m_.lock();
    BUT_ASSERT(not locked_);
    locked_ = true;
    pushed_ = false;
    poped_ = false;
  }

  void unlock()
  {
    BUT_ASSERT(locked_);
    locked_ = false;
    // yes, notification should be done under a lock - aaccoring to POSIX this is "the way":
    // http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_signal.html
    // on linux it is aboud 15% faster than first unlocking an then notifying and it's 2x that fast on windows.
    if(pushed_)
      elementsPushed_.notify_all();
    if(poped_)
      elementsPoped_.notify_all();
    m_.unlock();
  }

  template<typename U>
  void push(U&& u)
  {
    BUT_ASSERT(locked_);
    q_.emplace_back( std::forward<U>(u) );
    pushed_ = true;
  }

  bool empty() const
  {
    BUT_ASSERT(locked_);
    return q_.empty();
  }

  size_type size() const
  {
    BUT_ASSERT(locked_);
    return q_.size();
  }

  T& top()
  {
    BUT_ASSERT(locked_);
    BUT_ASSERT( not empty() );
    return q_.front();
  }

  T const& top() const
  {
    BUT_ASSERT(locked_);
    BUT_ASSERT( not empty() );
    return q_.front();
  }

  void pop()
  {
    BUT_ASSERT(locked_);
    BUT_ASSERT( not empty() );
    q_.pop_front();
    poped_ = true;
  }

  template<typename ...Args>
  bool waitForNonEmpty(lock_type& lock, Args&& ...args) const
  {
    BUT_ASSERT(locked_);
    BUT_ASSERT(lock.owns_lock());
    return WaitHelper::wait(elementsPushed_, lock, [&]{ return not empty(); }, std::forward<Args>(args)...);
  }

  template<typename ...Args>
  bool waitForSizeBelow(const size_type limit, lock_type& lock, Args&& ...args) const
  {
    BUT_ASSERT(locked_);
    BUT_ASSERT(lock.owns_lock());
    if(limit == 0u)
      return true;
    return WaitHelper::wait(elementsPoped_, lock, [&]{ return size() < limit; }, std::forward<Args>(args)...);
  }

private:

  Queue                               q_;
  std::mutex                          m_;
  bool                                locked_{false};
  mutable std::condition_variable_any elementsPushed_;
  bool                                pushed_ = false;
  mutable std::condition_variable_any elementsPoped_;
  bool                                poped_= false;
};

}
}
