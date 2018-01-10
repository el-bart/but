#pragma once
#include <type_traits>
#include <But/Threading/LockProxy.hpp>

namespace But
{
namespace Threading
{

/** @brief class to be used as a base, that provides convenient way of doing 1-liner ops under a lock.
 *
 *  @note user's class must either implement lock()/unlock() methods or derive from BasicLockabke template too.
 *
 *  <code>
 *  class MyObject: LockProxyProvider<MyObject>
 *  {
 *    // ...
 *    void lock();
 *    void unlock();
 *    // ...
 *    void changeState();
 *    // ...
 *  };
 *
 *  MyObject mo;
 *
 *  // ...
 *
 *  mo.withLock()->changeState(); // this one line happens under a lock!
 *  </code>
 */
template<typename Derived>
class LockProxyProvider
{
public:
  auto withLock()
  {
    return LockProxy<Derived>{ static_cast<Derived&>(*this) };
  }
  auto withLock() const
  {
    return LockProxy<const Derived>{ static_cast<Derived const&>(*this) };
  }

protected:
  LockProxyProvider()
  {
    // test is here, as 'Derived' must be defined, before it can be used... :P
    static_assert( std::is_base_of<LockProxyProvider, Derived>::value, "this class is meant to be derived from, to make it work" );
  }

  ~LockProxyProvider() = default;
};

}
}
