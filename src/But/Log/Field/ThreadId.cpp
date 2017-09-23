#include <sstream>
#include "ThreadId.hpp"

namespace But
{
namespace Log
{
namespace Field
{

Backend::Value toValue(ThreadId const& tid)
{
  std::ostringstream ss;
  ss << tid.value_;
  return Backend::Value{ ss.str() };
}

}
}
}
