#include <iomanip>
#include <sstream>
#include "UnixTimestamp.hpp"

namespace But
{
namespace Log
{
namespace Field
{

std::string toString(const UnixTimestamp uts)
{
  const auto duration = uts.timePoint().time_since_epoch();
  const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
  const auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration-seconds);
  std::stringstream ss;
  ss << seconds.count() << "."
     << std::setw(9) << std::setfill('0') << nanoseconds.count();
  return ss.str();
}

}
}
}
