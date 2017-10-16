#pragma once
#include <But/Format/Parsed.hpp>
#include <But/Format/detail/applyImpl.hpp>

namespace But
{
namespace Format
{

template<size_t ArgumentsCount, size_t MaxSegments, typename ...Args>
std::string apply(ParsedCompiletime<ArgumentsCount, MaxSegments> const& p, Args const& ...args)
{
  using Ct = ParsedCompiletime<ArgumentsCount, MaxSegments>;
  static_assert( sizeof...(args) == Ct::expectedArguments(), "invalid number of arguments for a format" );
  return detail::applyImpl(p, args...);
}


template<typename ...Args>
std::string apply(ParsedRuntime const& p, Args const& ...args)
{
  if( sizeof...(args) != p.expectedArguments() )
    BUT_THROW(ParsedRuntime::ArityError, "expected " << p.expectedArguments() << " arguments - got " << sizeof...(args) << " instead");
  return detail::applyImpl(p, args...);
}

}
}
