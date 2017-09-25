#pragma once
#include "But/Container/Array.hpp"
#include "lastArgumentNumber.hpp"

namespace But
{
namespace Format
{
namespace detail
{

template<unsigned N>
constexpr auto bitsCount(Container::Array<bool,N> const& tab)
{
  auto count = 0u;
  for(auto b: tab)
    count += b;
  return count;
}

template<unsigned N>
constexpr auto usedArgumentsMap(ParsedFormat<N> const& ps)
{
  Container::Array<bool,N> tab{};
  tab.fill(false);
  for(auto& e: ps.segments_)
    if( e.type_ == Segment::Type::Value || e.type_ == Segment::Type::TypeName )
      tab[e.referencedArgument_] = true;
  return tab;
}

template<unsigned N>
constexpr auto argumentsCount(ParsedFormat<N> const& ps)
{
  const auto tab = usedArgumentsMap(ps);
  return bitsCount(tab);
}

}
}
}
