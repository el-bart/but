#pragma once
#include "lastArgumentNumber.hpp"

namespace But
{
namespace Format
{
namespace detail
{

template<unsigned N>
constexpr auto argumentsCount(ParserState<N> const& ps)
{
  // TODO: repeated arguments should not be taken into account!
  auto count = 0u;
  for(auto i=0u; i<ps.count_; ++i)
  {
    auto& e = ps.segments_[i];
    if( e.type_ == State::Type::Value || e.type_ == State::Type::TypeName )
      ++count;
  }
  return count;
}

}
}
}
