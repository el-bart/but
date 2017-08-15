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
constexpr auto argumentsCount(ParserState<N> const& ps)
{
  Container::Array<bool,N> tab{};
  tab.fill(false);
  for(auto i=0u; i<ps.count_; ++i)
  {
    auto& e = ps.segments_[i];
    if( e.type_ == State::Type::Value || e.type_ == State::Type::TypeName )
      tab[e.referencedArgument_] = true;
  }
  return bitsCount(tab);
}

}
}
}