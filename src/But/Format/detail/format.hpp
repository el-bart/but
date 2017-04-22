#pragma once
#include <string>
#include "countFormatArgs.hpp"

namespace But
{
namespace detail
{

template<unsigned N, typename ...Args>
std::string format(std::string const& fmt, int /*ignored*/, Args&&... /*args*/)
{
// TODO...
  return fmt;
}


#define BUT_DETAIL_FORMAT(ignored, fmt, ...) ::But::detail::format< ::But::detail::countFormatArgs(fmt) >( fmt, __VA_ARGS__ )

}
}
