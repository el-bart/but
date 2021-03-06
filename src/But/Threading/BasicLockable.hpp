#pragma once
#include <mutex>
#include <type_traits>
#include <But/assert.hpp>

namespace But
{
namespace Threading
{

/** @brief class for easy making your type lockable.
 *
 *  <code>
 *  class MyCollection: BasicLocable<MyCollection>
 *  {
 *    // ...
 *    bool empty();
 *    Element& top();
 *    void pop();
 *    // ...
 *  };
 *
 *  MyCollection mc;
 *
 *  // ...
 *
 *  {
 *    // NOTE: this whole block is locked, so sequence of operations is secure!
 *    const std::lock_guard<MyCollection> lock{mc};
 *    if( not mc.empty() )
 *    {
 *      process( mc.top() );
 *      mc.pop();
 *    }
 *  }
 *  </code>
 *
 *  @note each method of your type, should first do 'BUT_ASSERT( locked() )', before proceeding.
 *        'locked()' is only available in debug mode, but can save a lot of time when looking for sync issues.
 */
template<typename Derived>
class BasicLockable
{
public:
  void lock() const
  {
    m_.lock();
    BUT_ASSERT( not locked(true) );
  }

  void unlock() const
  {
    BUT_ASSERT( locked(false) );
    m_.unlock();
  }

protected:
  BasicLockable()
  {
    // test is here, as 'Derived' must be defined, before it can be used... :P
    static_assert( std::is_base_of<BasicLockable, Derived>::value, "this class is meant to be derived from, to make it work" );
  }

  ~BasicLockable() = default;

#ifndef NDEBUG
  /** @brief to be used in derived classes, for checks like 'BUT_ASSERT( locked() )' in methods.
   *  @note this is for debugging purposes in derived class only!
   *  @warning this method is NOT thread-safe on its own!
   *  @warning this method is available only in debug mode!
   */
  bool locked() const { return locked_; }
#endif

private:
#ifndef NDEBUG
  bool locked(const bool newState) const
  {
    const auto prev = locked_;
    locked_ = newState;
    return prev;
  }

  mutable bool locked_{false};
#endif

  mutable std::mutex m_;
};

}
}
