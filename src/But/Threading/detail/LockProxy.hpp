#pragma once

namespace But
{
namespace Threading
{
namespace detail
{

template<typename T>
class LockProxy final
{
public:
  explicit LockProxy(T& t):
    t_{&t}
  {
    t_->lock();
  }
  ~LockProxy()
  {
    releaseIfOwned();
  }

  LockProxy(LockProxy const&) = delete;
  LockProxy& operator=(LockProxy const&) = delete;

  LockProxy(LockProxy&& other):
    t_{other.t_}
  {
    other.t_ = nullptr;
  }
  LockProxy& operator=(LockProxy&& other)
  {
    if( &other == this )
      return *this;
    releaseIfOwned();
    t_ = other.t_;
    other.t_ = nullptr;
    return *this;
  }

  T* operator->() { return t_; }

private:
  inline void releaseIfOwned()
  {
    if(not t_)
      return;
    t_->unlock();
    t_ = nullptr;
  }

  T* t_;
};

}
}
}
