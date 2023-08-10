#pragma once
#include <string>
#include <ctime>
#include <cerrno>
#include <cstring>
#include <string>
#include <But/Log/Field/detail/date2str.hpp>

namespace But
{
namespace Log
{
namespace Field
{

struct PreciseDateTime final
{
  PreciseDateTime()
  {
    if( clock_gettime(CLOCK_REALTIME, &tv_) != 0 )
      BUT_THROW( ConversionError, "clock_gettime(CLOCK_REALTIME) failed: " << strerror(errno) );
  }
  explicit PreciseDateTime(time_t ts): tv_{ts, 0} { }
  explicit PreciseDateTime(struct timespec tv): tv_{tv} { }

  std::string date() const { return detail::date2str(tv_); }
  std::string time() const { return detail::time2str(tv_) + '.' + detail::nanosec2str(tv_); }

  struct timespec tv_;
};


inline constexpr auto fieldName(PreciseDateTime const*) { return std::string_view{"But::PreciseDT"}; }
inline auto fieldValue(PreciseDateTime const& o) { return o.date() + "T" + o.time() + "Z"; }

}
}
}
