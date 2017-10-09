#pragma once
#include <string>
#include <vector>
#include "But/Format/Parsed.hpp"

namespace But
{
namespace Format
{
namespace detail
{

std::string applyOnIterators(ParsedCompiletime::const_iterator begin,
                             ParsedCompiletime::const_iterator end,
                             std::vector<std::string> const& args);

std::string applyOnIterators(ParsedRuntime::const_iterator begin,
                             ParsedRuntime::const_iterator end,
                             std::vector<std::string> const& args);


template<typename Parsed, typename ...Args>
std::string applyImpl(Parsed const& p, Args const& ...args)
{
  p.validateArgumentsCount( sizeof...(args) );
  using std::to_string;
  const std::vector<std::string> arguments{ to_string(args)... };
  return applyOnIterators( p.begin(), p.end(), arguments );
}

}
}
}
