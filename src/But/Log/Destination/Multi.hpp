#pragma once
#include <tuple>
#include "detail/LogForwarder.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

/** @brief template that can hold multiple destinations and forwards all calls to all of them.
 *
 *  @note this destination does NOT offer Foregin interface, since it is mean to be root element.
 *        in order to have multiple foregin interfaces, use MultiForegin base class.
 *
 *  @note it is possible to have multiple destinations and foregin destinations, by creating
 *        set of destinations and one MultiForegin, that aggregates all foregin destinations, required.
 */
template<typename ...Dsts>
class Multi final
{
  static_assert( sizeof...(Dsts) > 0u, "need at least one output" );
public:
  explicit Multi(Dsts... dsts):
    dsts_{ std::forward<Dsts>(dsts)... }
  { }

  template<typename ...Args>
  void log(Args&& ...args)
  {
    detail::LogForwarder<1, sizeof...(Dsts)>::pass( dsts_, std::forward<Args>(args)... );
  }

  void reload()
  {
  }

  auto operator->() { return this; }

private:
  // TODO: last element of tuple should get elements moved in, to save a memory allocation.
  std::tuple<Dsts...> dsts_;
};

}
}
}