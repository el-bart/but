#pragma once
#include <vector>
#include <But/Format/detail/ParsedFormat.hpp>
#include <But/Format/detail/common.hpp>
#include <But/Mpl/parse.hpp>

namespace But
{
namespace Format
{
namespace detail
{

inline constexpr auto skipUntilEndOfComment(char const* fmt)
{
  while( not isEos(*fmt) && *fmt!='}' )
    ++fmt;
  return throwOnInvalidSyntax( *fmt!='}', "variable does not end with closing brace", fmt );
}

inline constexpr auto parseBraceVariable(Segment& st, char const* fmt)
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
  const auto beginOffset = 2;
  st.referencedArgument_ = Mpl::parseUnsigned<unsigned>(st.begin_ + beginOffset, numberEnd);
  return st.end_;
}

inline constexpr auto parseBrace(Segment& st, char const* fmt)
{
  throwOnInvalidSyntax( isEos(*fmt), "brace variable declaration is not complete", fmt );
  if( *fmt == 'V' )
    ++fmt;
  if( isDigit(*fmt) )
    return parseBraceVariable(st, fmt+1);
  return throwOnInvalidSyntax(true, "invalid brace variable init sequence", fmt);
}

inline constexpr auto parseSimpleVariable(Segment& st, char const* fmt)
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

inline constexpr auto parseStringVariable(Segment& st, char const* fmt)
{
  st.type_ = Segment::Type::String;
  st.end_ = fmt;
  return fmt+1;
}

inline constexpr auto parseVariable(Segment& st, char const* fmt)
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


inline constexpr auto parseString(Segment& st, char const* fmt)
{
  st.type_ = Segment::Type::String;
  st.begin_ = fmt;
  st.end_  = fmt;
  while( not isEos(*st.end_) && not isVariableBegin(*st.end_) )
    ++st.end_;
  return st.end_;
}


template<typename Pf>
constexpr auto parse(Pf&& pf, char const* fmt)
{
  while( not isEos(*fmt) )
  {
    throwOnInvalidSyntax( pf.segments_.max_size() == pf.segments_.size(), "format too long for a declared states count - expected end", fmt );
    Segment st;
    if( isVariableBegin(*fmt) )
      fmt = parseVariable(st, fmt);
    else
      fmt = parseString(st, fmt);

    pf.segments_.push_back( std::move(st) );
  }
  return pf;
}


template<size_t N>
constexpr auto parseCt(char const* fmt) { return parse( ParsedFormatCt<N>{}, fmt ); }

inline auto parseRt(char const* fmt) { return parse( ParsedFormatRt{}, fmt ); }
inline auto parseRt(std::string const& fmt) { return parseRt( fmt.c_str() ); }

}
}
}
