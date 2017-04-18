#include <sstream>
#include "ThreadId.hpp"

namespace But
{
namespace Log
{
namespace Field
{

std::string toString(ThreadId const& tid)
{
  std::ostringstream ss;
  ss << tid.value_;
  return ss.str();
}

}
}
}
