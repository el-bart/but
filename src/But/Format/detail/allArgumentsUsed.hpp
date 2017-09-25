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

template<typename Ps, typename Used>
constexpr auto allArgumentsUsedImpl(Ps const& ps, Used&& used)
{
  auto hasArgs = false;
  for(auto& e: ps.segments_)
  {
    if( e.type_ != Segment::Type::Value && e.type_ != Segment::Type::TypeName )
      continue;
    hasArgs = true;
    const auto pos = e.referencedArgument_;
    if( pos >= ps.size() )
      return false;
    used[pos] = true;
  }

  if(not hasArgs)
    return true;

  const auto last = lastArgumentNumber(ps);
  if( last >= ps.size() )
    return false;
  for(auto i=0u; i<=last; ++i)
    if( not used[i] )
      return false;
  return true;
}


template<size_t N>
constexpr auto allArgumentsUsed(ParsedFormatCt<N> const& ps)
{
  Container::Array<bool,N> used{};
  used.fill(false);
  return allArgumentsUsedImpl( ps, std::move(used) );
}


inline auto allArgumentsUsed(ParsedFormatRt const& ps)
{
  std::vector<bool> used( ps.size(), false );
  return allArgumentsUsedImpl( ps, std::move(used) );
}

}
}
}
