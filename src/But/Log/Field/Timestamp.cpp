#include <iomanip>
#include <sstream>
#include "Timestamp.hpp"

namespace But
{
namespace Log
{
namespace Field
{

namespace
{
Backend::Value toValue(const Timestamp uts)
{
  const auto duration = uts.timePoint().time_since_epoch();
  const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
  const auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration-seconds);
  return Backend::Value{ seconds.count() + ( nanoseconds.count() / double{1000*1000*1000} ) };
}
}


Backend::FieldInfo toFieldInfo(const Timestamp uts)
{
  return Backend::FieldInfo{ Backend::Type{"But::Timestamp"}, toValue(uts) };
}

}
}
}
