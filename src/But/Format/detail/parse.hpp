#pragma once
#include "ParserState.hpp"
#include "common.hpp"
#include "But/Mpl/parse.hpp"

namespace But
{
namespace Format
{
namespace detail
{

constexpr auto parseBraceVariable(State& st, char const* fmt)
{
  (void)st;
  // TODO...
  return throwOnInvalidSyntax(true, "not yet implemented", fmt);
}

constexpr auto parseSimpleVariable(State& st, char const* fmt)
{
  throwOnInvalidSyntax( not isDigit(*fmt), "simple variable declaration is not followed with a number", fmt );
  st.type_ = State::Type::Value;
  st.end_  = fmt+1;
  while( isDigit(*st.end_) )
    ++st.end_;
  throwOnInvalidSyntax( not isEos(*st.end_) && not isSpace(*st.end_), "variable does not end with end of data not space", st.end_ );
  st.referencedArgument_ = Mpl::parseUnsigned<unsigned>(st.begin_+1, st.end_);
  return st.end_;
}

constexpr auto parseStringVariable(State& st, char const* fmt)
{
  st.type_ = State::Type::String;
  st.end_ = fmt;
  return fmt+1;
}

constexpr auto parseVariable(State& st, char const* fmt)
{
  st.begin_ = fmt;
  ++fmt;
  throwOnInvalidSyntax( isEos(*fmt), "end of data while declaring a variable", fmt );

  if( isVariableBegin(*fmt) )   // "$$" case
    return parseStringVariable(st, fmt);

  if( *fmt=='{' )
    return parseBraceVariable(st, fmt+1);
  return parseSimpleVariable(st, fmt);
}


constexpr auto parseString(State& st, char const* fmt)
{
  st.type_ = State::Type::String;
  st.begin_ = fmt;
  st.end_  = fmt;
  while( not isEos(*st.end_) && not isVariableBegin(*st.end_) )
    ++st.end_;
  return st.end_;
}


template<unsigned N>
constexpr auto parseImpl(ParserState<N>&& ps, char const* fmt)
{
  while( not isEos(*fmt) )
  {
    State st;
    if( isVariableBegin(*fmt) )
      fmt = parseVariable(st, fmt);
    else
      fmt = parseString(st, fmt);

    ps.segments_[ps.count_] = std::move(st);
    ++ps.count_;
  }
  return ps;
}


template<unsigned N>
constexpr auto parse(char const* fmt)
{
  return parseImpl( ParserState<N>{}, fmt );
}

}
}
}
