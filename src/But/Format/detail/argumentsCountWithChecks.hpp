#pragma once
#include "parse.hpp"
#include "argumentsCount.hpp"

namespace But
{
namespace Format
{
namespace detail
{

template<unsigned N>
constexpr auto argumentsUsageCount(char const* fmt)
{
  const auto ps = parse<N>(fmt);
  return argumentsCount(ps);
}

}
}
}
