#pragma once
#include "ParserState.hpp"

namespace But
{
namespace Format
{
namespace detail
{

template<unsigned N>
constexpr auto lastArgumentNumber(ParserState<N> const& ps)
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
