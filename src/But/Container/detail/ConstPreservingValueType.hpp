#pragma once

namespace But
{
namespace Container
{
namespace detail
{

template<typename C>
struct ConstPreservingValueType final
{
  using type = typename C::value_type;
};


template<typename C>
struct ConstPreservingValueType<const C> final
{
  using type = const typename C::value_type;
};

}
}
}
