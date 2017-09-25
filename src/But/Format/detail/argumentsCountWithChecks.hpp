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

template<typename Ps>
constexpr auto argumentsCountWithChecksImpl(Ps const& ps)
{
  if( not allArgumentsUsed(ps) )
    throw Invalid{__FILE__, __LINE__, BOOST_CURRENT_FUNCTION, "not all arguments have been used"};
  return argumentsCount(ps);
}

template<unsigned N>
constexpr auto argumentsCountWithChecks(char const* fmt)
{
  const auto ps = parse<N>(fmt);
  return argumentsCountWithChecksImpl(ps);
}

inline auto argumentsCountWithChecks(char const* fmt)
{
  const auto ps = parse(fmt);
  return argumentsCountWithChecksImpl(ps);
}

}
}
}
