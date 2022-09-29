#include <iomanip>
#include <sstream>
#include <But/Log/Field/Timestamp.hpp>

namespace But
{
namespace Log
{
namespace Field
{

double fieldValue(Timestamp const& ts)
{
  const auto duration = ts.timePoint().time_since_epoch();
  const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
  const auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration-seconds);
  return seconds.count() + ( nanoseconds.count() / double{1000*1000*1000} );
}

}
}
}
