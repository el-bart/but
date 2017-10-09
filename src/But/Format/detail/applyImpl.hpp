#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "But/assert.hpp"
#include "But/Format/Parsed.hpp"
#include "But/Format/toString.hpp"

namespace But
{
namespace Format
{
namespace detail
{

inline void formatBlock(std::ostringstream& os, detail::Segment const& segment, std::vector<std::string> const& arguments)
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
std::string applyOnIterators(const It begin, const It end, std::vector<std::string> const& arguments)
{
  BUT_ASSERT( size_t( std::distance(begin, end) ) == arguments.size() );
  std::ostringstream os;
  for(auto it=begin; it!=end; ++it)
    formatBlock(os, *it, arguments);
  return os.str();
}

template<typename Parsed, typename ...Args>
std::string applyImpl(Parsed const& p, Args const& ...args)
{
  using ::But::Format::toString;
  const std::vector<std::string> arguments{ toString(args)... };
  return applyOnIterators( p.begin(), p.end(), arguments );
}

}
}
}
