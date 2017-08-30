#include "But/assert.hpp"
#include "Exception.hpp"

namespace But
{

std::stringstream Exception::defineBegining(char const* file, unsigned line)
{
  std::stringstream ss;
  BUT_ASSERT(file);
  ss << file << ':' << line << ' ';
  return ss;
}


std::stringstream&& Exception::defineEnding(std::stringstream&& ss, char const* function)
{
  BUT_ASSERT(function);
  ss << " (in " << function << ')';
  return std::move(ss);
}

}
