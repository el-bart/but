#include <But/Log/Field/ThreadId.hpp>
#include <sstream>

namespace But
{
namespace Log
{
namespace Field
{

std::string fieldValue(ThreadId const& tid)
{
  std::ostringstream ss;
  ss << tid.value_;
  return ss.str();
}

}
}
}
