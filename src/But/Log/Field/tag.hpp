#pragma once
#include <But/Log/Field/detail/DynamicallyNamedObject.hpp>
#include <type_traits>

namespace But::Log::Field
{

/** @brief helper that can nest given object/value into a dynamically-named type.
 */
template<typename T>
auto tag(std::string_view name, T&& t)
{
  using U = typename std::decay<T>::type;
  return detail::DynamicallyNamedObject<U>{ std::move(name), std::forward<T>(t) };
}

}
