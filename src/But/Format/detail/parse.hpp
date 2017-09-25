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

constexpr auto skipUntilEndOfComment(char const* fmt)
{
  while( not isEos(*fmt) && *fmt!='}' )
    ++fmt;
  return throwOnInvalidSyntax( *fmt!='}', "variable does not end with closing brace", fmt );
}

constexpr auto parseBraceTypeName(Segment& st, char const* fmt)
{
  throwOnInvalidSyntax( not isDigit(*fmt), "brace type variable declaration is not complete", fmt );
  ++fmt;
  st.type_ = Segment::Type::TypeName;
  st.end_ = fmt;
  while( isDigit(*st.end_) )
    ++st.end_;
  const auto numberEnd = st.end_;
  if( *st.end_ == '#' )
    st.end_ = skipUntilEndOfComment(st.end_);
  throwOnInvalidSyntax( *st.end_!='}', "variable does not end with closing brace", st.end_ );
  ++st.end_;
  st.referencedArgument_ = Mpl::parseUnsigned<unsigned>(st.begin_ + 3, numberEnd);
  return st.end_;
}

constexpr auto parseBraceVariable(Segment& st, char const* fmt)
{
  st.type_ = Segment::Type::Value;
  st.end_ = fmt;
  while( isDigit(*st.end_) )
    ++st.end_;
  const auto numberEnd = st.end_;
  if( *st.end_ == '#' )
    st.end_ = skipUntilEndOfComment(st.end_);
  throwOnInvalidSyntax( *st.end_!='}', "variable does not end with closing brace", st.end_ );
  ++st.end_;
  const auto beginOffset = st.begin_[2] == 'V' ? 3 : 2;
  st.referencedArgument_ = Mpl::parseUnsigned<unsigned>(st.begin_ + beginOffset, numberEnd);
  return st.end_;
}

constexpr auto parseBrace(Segment& st, char const* fmt)
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

constexpr auto parseSimpleVariable(Segment& st, char const* fmt)
{
  throwOnInvalidSyntax( not isDigit(*fmt), "simple variable declaration is not followed by a number", fmt );
  st.type_ = Segment::Type::Value;
  st.end_  = fmt+1;
  while( isDigit(*st.end_) )
    ++st.end_;
  throwOnInvalidSyntax( not isEos(*st.end_) && not isSpace(*st.end_), "variable does not end with end of data or space", st.end_ );
  st.referencedArgument_ = Mpl::parseUnsigned<unsigned>(st.begin_+1, st.end_);
  return st.end_;
}

constexpr auto parseStringVariable(Segment& st, char const* fmt)
{
  st.type_ = Segment::Type::String;
  st.end_ = fmt;
  return fmt+1;
}

constexpr auto parseVariable(Segment& st, char const* fmt)
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


constexpr auto parseString(Segment& st, char const* fmt)
{
  st.type_ = Segment::Type::String;
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
    throwOnInvalidSyntax( ps.segments_.max_size() == ps.segments_.size(), "format too long for a declared states count - expected end", fmt );
    Segment st;
    if( isVariableBegin(*fmt) )
      fmt = parseVariable(st, fmt);
    else
      fmt = parseString(st, fmt);

    ps.segments_.push_back( std::move(st) );
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
