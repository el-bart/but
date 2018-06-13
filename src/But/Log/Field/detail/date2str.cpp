#include <ctime>
#include <cstdio>
#include <cstring>
#include <But/assert.hpp>
#include <But/Log/Field/detail/date2str.hpp>

using std::to_string;

namespace But
{
namespace Log
{
namespace Field
{
namespace detail
{

namespace
{
auto convert(const time_t timestamp)
{
  struct tm out;
  if( not gmtime_r(&timestamp, &out) )
    BUT_THROW( ConversionError, strerror(errno) );
  return out;
}
}


std::string date2str(const time_t timestamp)
{
  const auto tmp = convert(timestamp);
  char out[64];     // kept big, to silence GCC's 8 warning...
  BUT_ASSERT( tmp.tm_mon+1 <= 12 );
  BUT_ASSERT( tmp.tm_mday  <= 31 );
  sprintf(out, "-%02d-%02d", 1+tmp.tm_mon, tmp.tm_mday);
  return to_string(1900+tmp.tm_year) + out;
}


std::string time2str(const time_t timestamp)
{
  const auto tmp = convert(timestamp);
  char out[64];     // kept big, to silence GCC's 8 warning...
  BUT_ASSERT( tmp.tm_hour < 24 );
  BUT_ASSERT( tmp.tm_min  < 60 );
  BUT_ASSERT( tmp.tm_sec  < 60 );
  sprintf(out, "%02d:%02d:%02d", tmp.tm_hour, tmp.tm_min, tmp.tm_sec);
  return out;
}

}
}
}
}
