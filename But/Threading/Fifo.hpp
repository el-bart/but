#pragma once

#include <deque>
#include <mutex>
#include <utility>
#include <condition_variable>
#include <cassert>

namespace But
{
namespace Threading
{

template<typename T>
class Fifo
{
  using Queue = std::deque<T>;

public:
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

  T& top(lock_type& lock)
  {
    assert(locked_);
    assert(lock.owns_lock());
    nonEmpty_.wait(lock, [&]{ return not empty(); });
    return q_.front();
  }

  T const& top() const
  {
    assert(locked_);
    assert( not empty() );
    return q_.front();
  }

  T const& top(lock_type& lock) const
  {
    assert(locked_);
    assert(lock.owns_lock());
    nonEmpty_.wait(lock, [&]{ return not empty(); });
    return q_.front();
  }

  void pop()
  {
    assert(locked_);
    assert( not empty() );
    q_.pop_front();
  }

  void pop(lock_type& lock)
  {
    assert(locked_);
    assert(lock.owns_lock());
    nonEmpty_.wait(lock, [&]{ return not empty(); });
    q_.pop_front();
  }

private:
  bool                                locked_{false};
  Queue                               q_;
  std::mutex                          m_;
  mutable std::condition_variable_any nonEmpty_;
};

}
}
