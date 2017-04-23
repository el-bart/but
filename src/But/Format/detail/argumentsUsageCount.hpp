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

constexpr auto consumeUntilTheEndOfString(char const* fmt)
{
  while( not isVariableBegin(*fmt) && not isEos(*fmt) )
    ++fmt;
  return fmt;
}

constexpr auto consumeUntilEndOfSimepleVariable(char const* fmt)
{
  while( isDigit(*fmt) )
    ++fmt;
  if( isEos(*fmt) )
    return fmt;
  return throwOnInvalidSyntax( not isSpace(*fmt), "invalid number", fmt );
}

constexpr auto consumeUntilEndOfBraceVariable(char const* fmt)
{
  while( *fmt!='}' && not isEos(*fmt) )
    ++fmt;
  return throwOnInvalidSyntax( *fmt!='}', "variable declration not ended with a '}'", fmt );
}

constexpr auto consumeUntilEndOfVariable(char const* fmt)
{
  if( isDigit(*fmt) )
    return consumeUntilEndOfSimepleVariable(fmt+1);
  if( *fmt=='{')
    return consumeUntilEndOfBraceVariable(fmt+1);
  return throwOnInvalidSyntax(true, "expected either number or opening brace ('{') after variable declaration", fmt);
}


/** @brief helper function that determines number of arguments, based on a simplified syntax grammar.
 */
constexpr auto argumentsUsageCount(char const* fmt)
{
  auto args = 0u;
  while(*fmt)
  {
    if( isVariableBegin(*fmt) )
    {
      ++fmt;
      if( isEos(*fmt) )
        throwOnInvalidSyntax(true, "variable declaration terminated in the middle of parsing", fmt);
      if( isVariableBegin(*fmt) )
        ++fmt;
      else
      {
        fmt = consumeUntilEndOfVariable(fmt);
        ++args;
      }
    }
    else
      fmt = consumeUntilTheEndOfString(fmt+1);
  }
  return args;
}

}
}
}
