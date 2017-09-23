#pragma once
#include <tuple>
#include "detail/LogForwarder.hpp"
#include "detail/ReloadForwarder.hpp"
#include "detail/FlushForwarder.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

/** @brief template that can hold multiple destinations and forwards all calls to all of them.
 *
 *  @note this destination does NOT offer Foreign interface, since it is mean to be root element.
 *        in order to have multiple foreign interfaces, use MultiForeign base class.
 *
 *  @note it is possible to have multiple destinations and foreign destinations, by creating
 *        set of destinations and one MultiForeign, that aggregates all foreign destinations, required.
 *
 *  @note this destination does NOT forward exceptions from any destinations it forwards log to.
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
  void log(Args&& ...args) noexcept
  {
    detail::LogForwarder<1, sizeof...(Dsts)>::pass( dsts_, std::forward<Args>(args)... );
  }

  void reload() noexcept
  {
    detail::ReloadForwarder<1, sizeof...(Dsts)>::pass( dsts_ );
  }

  void flush() noexcept
  {
    detail::FlushForwarder<1, sizeof...(Dsts)>::pass( dsts_ );
  }

  auto operator->() { return this; }

private:
  std::tuple<Dsts...> dsts_;
};

}
}
}
