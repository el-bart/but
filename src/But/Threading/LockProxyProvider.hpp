#pragma once
#include <type_traits>
#include "detail/LockProxy.hpp"

namespace But
{
namespace Threading
{

/** @brief TODO
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
 *  @note each method of your type, should first do 'assert( locked() )', before proceeding.
 *        'locked()' is only available in debug mode, but can save a lot of time when looking for sync issues.
 */
template<typename Derived>
class LockProxyProvider
{
public:
  auto withLock()
  {
    return detail::LockProxy<Derived>{ static_cast<Derived&>(*this) };
  }
  auto withLock() const
  {
    return detail::LockProxy<const Derived>{ static_cast<Derived const&>(*this) };
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
