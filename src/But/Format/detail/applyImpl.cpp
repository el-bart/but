#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "But/assert.hpp"
#include "applyImpl.hpp"

namespace But
{
namespace Format
{
namespace detail
{

namespace
{

void formatBlock(std::ostringstream& os, detail::Segment const& segment, std::vector<std::string> const& arguments) const
{
  switch(segment.type_)
  {
    case detail::Segment::Type::String:
      os.write( segment.begin_, segment.end_ - segment.begin_ );
      return;
    case detail::Segment::Type::Value:
      os << arguments[segment.referencedArgument_];
      return;
  }
  BUT_ASSERT(!"missing type handle");
}

template<typename It>
std::string formatImpl(const It begin, const It end, std::vector<std::string> const& args) const
{
  BUT_ASSERT( std::distance(begin, end) == args.size() );
  std::ostringstream os;
  for(auto it=begin; it!=end; ++it)
    formatBlock(os, *it, arguments);
  return os.str();
}

}


std::string applyOnIterators(ParsedCompiletime::const_iterator begin,
                             ParsedCompiletime::const_iterator end,
                             std::vector<std::string> const& args)
{
  return formatImpl(begin, end, args);
}


std::string applyOnIterators(ParsedRuntime::const_iterator begin,
                             ParsedRuntime::const_iterator end,
                             std::vector<std::string> const& args)
{
  return formatImpl(begin, end, args);
}


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
