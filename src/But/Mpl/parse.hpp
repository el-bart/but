#pragma once
#include "But/Exception.hpp"

namespace But
{
namespace Mpl
{

BUT_DEFINE_EXCEPTION(ParseError, Exception, "parse error");

namespace detail
{
constexpr unsigned isDigit(const char c)
{
  return '0' <= c && c <= '9';
}
constexpr unsigned parseDigit(const char c)
{
  if( not isDigit(c) )
    throw ParseError{__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, "invalid digit: " + std::string{c}};
  return c - '0';
}
}


template<typename T>
constexpr auto parseUnsigned(char const* fmt)
{
  T out{0};
  for(; *fmt; ++fmt)
    out = out*10u + detail::parseDigit(*fmt);
  return out;
}


template<typename T>
constexpr auto parseSigned(char const* fmt)
{
  T out{0};
  T sig{+1};
  switch(*fmt)
  {
    case '-':
         sig = -1;
         ++fmt;
         break;
    case '+':
         sig = +1;
         ++fmt;
         break;
  }
  for(; *fmt; ++fmt)
    out = out*10u + detail::parseDigit(*fmt);
  return sig*out;
}

}
}
