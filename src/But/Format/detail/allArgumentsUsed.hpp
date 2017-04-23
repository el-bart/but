#pragma once
#include "But/Container/Array.hpp"
#include "ParserState.hpp"
#include "lastArgumentNumber.hpp"

namespace But
{
namespace Format
{
namespace detail
{

template<unsigned N>
constexpr auto allArgumentsUsed(ParserState<N> const& ps)
{
  auto hasArgs = false;
  Container::Array<bool,N> used{};
  used.fill(false);
  for(auto i=0u; i<ps.count_; ++i)
  {
    if( ps.segments_[i].type_ != State::Type::Value && ps.segments_[i].type_ != State::Type::TypeName )
      continue;
    hasArgs = true;
    const auto pos = ps.segments_[i].referencedArgument_;
    if( pos >= N )
      return false;
    used[pos] = true;
  }

  if(not hasArgs)
    return true;

  const auto last = lastArgumentNumber(ps);
  if( last >= N )
    return false;
  for(auto i=0u; i<=last; ++i)
    if( not used[i] )
      return false;
  return true;
}

}
}
}
