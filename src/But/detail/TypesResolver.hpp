#pragma once

#include <type_traits>

namespace But
{
namespace detail
{

template<typename T, bool IsRawPtr = true>
struct TypesResolverImpl
{
  using element_type = typename std::remove_pointer<T>::type;
};

template<typename T>
struct TypesResolverImpl<T, false>
{
  using element_type = typename T::element_type;
};


template<typename T>
using TypesResolver = TypesResolverImpl<T, std::is_pointer<T>::value>;

}
}
