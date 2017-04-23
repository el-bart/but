#pragma once
#include "ParserState.hpp"
#include "common.hpp"

namespace But
{
namespace Format
{
namespace detail
{

constexpr auto parseVariable(State& , char const* fmt)
{
  return throwOnInvalidSyntax( true, "not yet implemented", fmt );
}


constexpr auto parseStringVar(State& st, char const* fmt)
{
  st.type_ = State::Type::String;
  st.begin_ = fmt;
  st.end_  = fmt+2;
  return st.end_;
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
    {
      throwOnInvalidSyntax( isEos(fmt[1]), "end of data while declaring a variable", fmt+1 );
      if( isVariableBegin(fmt[1]) ) // "$$" case
        fmt = parseStringVar(st, fmt);
      else
        fmt = parseVariable(st, fmt);
    }
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
