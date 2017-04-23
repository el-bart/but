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

constexpr auto parseBraceTypeName(State& st, char const* fmt)
{
  throwOnInvalidSyntax( not isDigit(*fmt), "brace type variable declaration is not complete", fmt );
  ++fmt;
  st.type_ = State::Type::TypeName;
  st.end_ = fmt;
  while( isDigit(*st.end_) )
    ++st.end_;
  throwOnInvalidSyntax( *st.end_!='}', "variable does not end with closing brace", st.end_ );
  st.referencedArgument_ = Mpl::parseUnsigned<unsigned>(st.begin_ + 3, st.end_);
  ++st.end_;
  return st.end_;
}

constexpr auto parseBraceVariable(State& st, char const* fmt)
{
  st.type_ = State::Type::Value;
  st.end_ = fmt;
  while( isDigit(*st.end_) )
    ++st.end_;
  throwOnInvalidSyntax( *st.end_!='}', "variable does not end with closing brace", st.end_ );
  const auto beginOffset = st.begin_[2] == 'V' ? 3 : 2;
  st.referencedArgument_ = Mpl::parseUnsigned<unsigned>(st.begin_ + beginOffset, st.end_);
  ++st.end_;
  return st.end_;
}

constexpr auto parseBrace(State& st, char const* fmt)
{
  throwOnInvalidSyntax( isEos(*fmt), "brace variable declaration is not complete", fmt );
  if( *fmt == 'V' )
    ++fmt;
  if( isDigit(*fmt) )
    return parseBraceVariable(st, fmt+1);
  if( *fmt == 'T' )
    return parseBraceTypeName(st, fmt+1);
  return throwOnInvalidSyntax(true, "invalid brace variable init sequence", fmt);
}

constexpr auto parseSimpleVariable(State& st, char const* fmt)
{
  throwOnInvalidSyntax( not isDigit(*fmt), "simple variable declaration is not followed by a number", fmt );
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
    return parseBrace(st, fmt+1);
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
