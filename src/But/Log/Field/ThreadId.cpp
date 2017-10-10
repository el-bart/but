#include <sstream>
#include "ThreadId.hpp"

namespace But
{
namespace Log
{
namespace Field
{

namespace
{
Backend::Value toValue(ThreadId const& tid)
{
  std::ostringstream ss;
  ss << tid.value_;
  return Backend::Value{ ss.str() };
}
}

Backend::FieldInfo toFieldInfo(ThreadId const& tid)
{
  return Backend::FieldInfo{ Backend::Tag{"But::ThreadId"}, toValue(tid) };
}

}
}
}
