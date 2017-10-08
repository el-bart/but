#pragma once
#include "But/Container/Array.hpp"
#include "ParsedFormat.hpp"
#include "lastArgumentNumber.hpp"

namespace But
{
namespace Format
{
namespace detail
{

template<size_t N>
constexpr auto allArgumentsUsed(ParsedFormat<N> const& ps)
{
  Container::Array<bool,N> used{};
  used.fill(false);

  auto hasArgs = false;
  for(auto& e: ps.segments_)
  {
    if( e.type_ != Segment::Type::Value && e.type_ != Segment::Type::TypeName )
      continue;
    hasArgs = true;
    const auto pos = e.referencedArgument_;
    if( pos >= ps.segments_.size() )
      return false;
    used[pos] = true;
  }

  if(not hasArgs)
    return true;

  const auto last = lastArgumentNumber(ps);
  if( last >= ps.segments_.size() )
    return false;
  for(auto i=0u; i<=last; ++i)
    if( not used[i] )
      return false;
  return true;
}

}
}
}
