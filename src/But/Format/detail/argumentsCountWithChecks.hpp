#pragma once
#include "But/Format/Invalid.hpp"
#include "parse.hpp"
#include "argumentsCount.hpp"
#include "allArgumentsUsed.hpp"

namespace But
{
namespace Format
{
namespace detail
{

template<unsigned N>
constexpr auto argumentsCountWithChecks(char const* fmt)
{
  const auto ps = parse<N>(fmt);
  if( not allArgumentsUsed(ps) )
    throw Invalid{__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, "not all arguments have been used"};
  return argumentsCount(ps);
}

}
}
}