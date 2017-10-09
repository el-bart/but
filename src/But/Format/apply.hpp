#pragma once
#include <string>
#include "But/assert.hpp"
#include "detail/parse.hpp"
#include "detail/argumentsCount.hpp"
#include "detail/allArgumentsUsed.hpp"
#include "Invalid.hpp"
#include "detail/StreamVisitor.hpp"

namespace But
{
namespace Format
{

template<typename ...Args>
std::string apply(ParsedCompiletime const& p, Args const& ...args)
{
  p.validateArgumentsCount( sizeof...(args) );
  using std::to_string;
  const std::vector<std::string> arguments{ to_string(args)... };
  return detail::applyImpl( p.begin(), p.end(), args... );
}

template<typename ...Args>
std::string apply(ParsedRuntime const& p, Args const& ...args)
{
  p.validateArgumentsCount( sizeof...(args) );
  using std::to_string;
  const std::vector<std::string> arguments{ to_string(args)... };
  return detail::applyImpl( p.begin(), p.end(), arguments );
}

}
}
