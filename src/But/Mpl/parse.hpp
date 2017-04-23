#pragma once
#include "But/Exception.hpp"
#include "ParseError.hpp"
#include "detail/parse.hpp"

namespace But
{
namespace Mpl
{

template<typename T>
constexpr auto parseUnsigned(char const* str)
{
  return detail::parseUnsigned<T>( str, nullptr, detail::isEndValue );
}

template<typename T>
constexpr auto parseUnsigned(char const* begin, char const* end)
{
  return detail::parseUnsigned<T>( begin, end, detail::isEndPtr );
}


template<typename T>
constexpr auto parseSigned(char const* str)
{
  return detail::parseSigned<T>( str, nullptr, detail::isEndValue );
}

template<typename T>
constexpr auto parseSigned(char const* begin, char const* end)
{
  return detail::parseSigned<T>( begin, end, detail::isEndPtr );
}

}
}
