#pragma once
#include "But/Container/Array.hpp"
#include "lastArgumentNumber.hpp"

namespace But
{
namespace Format
{
namespace detail
{

template<typename C>
constexpr auto bitsCount(C const& tab)
{
  auto count = 0u;
  for(auto b: tab)
    count += b;
  return count;
}

template<typename Ps, typename T>
constexpr auto usedArgumentsMapImpl(Ps const& ps, T&& tab)
{
  for(auto& e: ps.segments_)
    if( e.type_ == Segment::Type::Value || e.type_ == Segment::Type::TypeName )
      tab[e.referencedArgument_] = true;
  return tab;
}

template<size_t N>
constexpr auto usedArgumentsMap(ParsedFormatCt<N> const& ps)
{
  Container::Array<bool,N> tab{};
  tab.fill(false);
  return usedArgumentsMapImpl(ps, tab);
}

inline auto usedArgumentsMap(ParsedFormatRt const& ps)
{
  std::vector<bool> tab( ps.size(), false );
  return usedArgumentsMapImpl(ps, tab);
}

template<typename C>
constexpr auto argumentsCount(ParsedFormat<C> const& ps)
{
  const auto tab = usedArgumentsMap(ps);
  return bitsCount(tab);
}

}
}
}
