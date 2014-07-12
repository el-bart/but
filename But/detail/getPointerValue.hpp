#pragma once

#include <type_traits>

namespace But
{
namespace detail
{

template<bool IsRaw = true>
struct PointerValueGetterImpl
{
  template<typename P>
  auto static get(P& p) { return p; }
};

template<>
struct PointerValueGetterImpl<false>
{
  template<typename P>
  auto static get(P& p) { return p.get(); }
};

template<typename P>
auto static getPointerValue(P& p)
{
  using          T = typename std::remove_const<P>::type;
  constexpr auto V = std::is_pointer<T>::value;
  return PointerValueGetterImpl<V>::get(p);
}

}
}
