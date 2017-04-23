#pragma once
#include "But/Format/Invalid.hpp"

namespace But
{
namespace Format
{
namespace detail
{

/** @brief this funciton is a workaround for GCC bug... */
constexpr char const* throwOnInvalidSyntax(const bool error, char const* msg, char const* fmt)
{
  if(error)
  {
    //BUT_THROW(Invalid, "expected either number or opening brace ('{') after variable declaration; got: '" << *fmt << "' instead");
    throw Invalid{__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, std::string{msg} + "; got '" + std::string{*fmt} + "' instead"};
    return static_cast<char const*>(nullptr);
  }
  return fmt;
}

constexpr auto isVariableBegin(const char c) { return c == '$'; }
constexpr auto isEos(const char c) { return c == 0; }
constexpr auto isDigit(const char c) { return '0' <= c && c <= '9'; }
constexpr auto isNewLine(const char c) { return c=='\n' || c=='\r'; }
constexpr auto isSpace(const char c) { return c==' ' || c=='\t' || isNewLine(c); }

}
}
}
