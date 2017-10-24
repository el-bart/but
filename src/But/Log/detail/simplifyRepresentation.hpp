#pragma once
#include <type_traits>
#include <But/Format/toString.hpp>
#include <But/Log/Backend/FieldInfo.hpp>
#include <But/Log/Backend/toFieldInfo.hpp>
#include <But/Log/Field/FormattedString.hpp>

namespace But
{
namespace Log
{
namespace detail
{

using Format::toString;

template<typename T>
struct HasToString
{
  template<typename U, typename = decltype( toString( std::declval<U>() ) )>
  static std::true_type test(int);

  template <typename U>
  static std::false_type test(...);

  static constexpr bool value = decltype(test<T>(0))::value;
};

template<bool UseToString = true>
struct Simplifier
{
  template<typename T>
  static auto&& convert(T&& t)
  {
    return std::forward<T>(t);
  }
};

template<>
struct Simplifier<false>
{
  template<typename T>
  static auto convert(T&& t)
  {
    using Backend::toFieldInfo;
    return toFieldInfo( std::forward<T>(t) );
  }
};


template<typename T>
auto simplifyRepresentation(T&& t)
{
  using Wrapper = Simplifier< HasToString<std::decay_t<T>>::value >;
  return Wrapper::convert( std::forward<T>(t) );
}

template<size_t N, size_t M>
auto&& simplifyRepresentation(Format::ParsedCompiletime<N,M>&& parsed)
{
  return std::move(parsed);
}

}
}
}
