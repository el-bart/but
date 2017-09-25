#pragma once
#include "ParsedFormat.hpp"

namespace But
{
namespace Format
{
namespace detail
{

template<unsigned N, typename C>
constexpr auto lastArgumentNumber(ParsedFormat<N,C> const& ps)
{
  auto last = 0u;
  for(auto& e: ps.segments_)
    if( e.type_ == Segment::Type::Value || e.type_ == Segment::Type::TypeName )
      if( e.referencedArgument_ > last )
        last = e.referencedArgument_;
  return last;
}

}
}
}
