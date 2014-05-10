#pragma once

#include <deque>
#include <mutex>
#include <utility>
#include <condition_variable>
#include <cassert>

#include "But/Exception.hpp"

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
class Fifo
{
  using Queue = std::deque<T>;

public:
  BUT_DEFINE_EXCEPTION(Timeout, Exception, "timeout while wating for data");

  using value_type = T;
  using size_type  = typename Queue::size_type;
  using lock_type  = std::unique_lock<Fifo<T>>;

  Fifo() = default;

  void lock()
  {
    m_.lock();
    assert(not locked_);
    locked_ = true;
  }

  void unlock()
  {
    assert(locked_);
    locked_ = false;
    m_.unlock();
  }

  template<typename U>
  void push(U&& u)
  {
    assert(locked_);
    q_.push_back(std::forward<U>(u));
    nonEmpty_.notify_all();
  }

  bool empty() const
  {
    assert(locked_);
    return q_.empty();
  }

  size_type size() const
  {
    assert(locked_);
    return q_.size();
  }

  T& top()
  {
    assert(locked_);
    assert( not empty() );
    return q_.front();
  }

  template<typename ...Args>
  T& top(lock_type& lock, Args&&... args)
  {
    assert(locked_);
    assert(lock.owns_lock());
    wait(lock, std::forward<Args>(args)...);
    return q_.front();
  }

  T const& top() const
  {
    assert(locked_);
    assert( not empty() );
    return q_.front();
  }

  template<typename ...Args>
  T const& top(lock_type& lock, Args&&... args) const
  {
    assert(locked_);
    assert(lock.owns_lock());
    wait(lock, std::forward<Args>(args)...);
    return q_.front();
  }

  void pop()
  {
    assert(locked_);
    assert( not empty() );
    q_.pop_front();
  }

  template<typename ...Args>
  void pop(lock_type& lock, Args&&... args)
  {
    assert(locked_);
    assert(lock.owns_lock());
    wait(lock, std::forward<Args>(args)...);
    q_.pop_front();
  }

private:
  void wait(lock_type& lock) const
  {
    nonEmpty_.wait(lock, [&]{ return not empty(); });
  }

  template<typename R, typename P>
  void wait(lock_type& lock, const std::chrono::duration<R,P>& timeout) const
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

  Queue                               q_;
  std::mutex                          m_;
  bool                                locked_{false};
  mutable std::condition_variable_any nonEmpty_;
};

}
}
