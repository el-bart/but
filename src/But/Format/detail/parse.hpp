#pragma once
#include "ParserState.hpp"

namespace But
{
namespace Format
{
namespace detail
{

template<unsigned N>
constexpr auto parse(char const* /*fmt*/)
{
  ParserState<N> ps;
  // TODO....
  return ps;
}

}
}
}
