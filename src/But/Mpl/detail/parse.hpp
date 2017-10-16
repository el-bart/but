#pragma once
#include <But/Mpl/ParseError.hpp>

namespace But
{
namespace Mpl
{
namespace detail
{

constexpr auto isEndValue(char const* it, char const*) { return *it == 0; }
constexpr auto isEndPtr(char const* it, char const* end) { return it == end; }

constexpr unsigned isDigit(const char c) { return '0' <= c && c <= '9'; }

constexpr unsigned parseDigit(const char c)
{
  if( not isDigit(c) )
    throw ParseError{__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, "invalid digit: " + std::string{c}};
  return c - '0';
}


template<typename T, typename F>
constexpr auto parseUnsigned(char const* begin, char const* end, F&& isEnd)
{
  T out{0};
  for(auto it=begin; not isEnd(it, end); ++it)
    out = out*10u + parseDigit(*it);
  return out;
}

template<typename T, typename F>
constexpr auto parseSigned(char const* begin, char const* end, F&& isEnd)
{
  T sig{+1};
  switch(*begin)
  {
    case '-':
         sig = -1;
         ++begin;
         break;
    case '+':
         sig = +1;
         ++begin;
         break;
  }
  const auto out = parseUnsigned<T>( begin, end, std::forward<F>(isEnd) );
  return sig * out;
}

}
}
}
