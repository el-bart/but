#include <ctime>
#include <cstdio>
#include <cstring>
#include "But/assert.hpp"
#include "UtcIsoDateTime.hpp"

using std::to_string;

namespace But
{
namespace Log
{
namespace Field
{

namespace
{
auto convert(time_t timestamp)
{
  struct tm out;
  if( not gmtime_r(&timestamp, &out) )
    BUT_THROW( ConversionError, strerror(errno) );
  return out;
}
}


std::string UtcIsoDateTime::date() const
{
  const auto tmp = convert(timestamp_);
  char out[1+2 + 1+2 + 1];
  assert( tmp.tm_mon+1 <= 12 );
  assert( tmp.tm_mday  <= 31 );
  sprintf(out, "-%02d-%02d", 1+tmp.tm_mon, tmp.tm_mday);
  return to_string(1900+tmp.tm_year) + out;
}


std::string UtcIsoDateTime::time() const
{
  const auto tmp = convert(timestamp_);
  char out[2+1 + 2+1 + 2 + 1];
  assert( tmp.tm_hour < 24 );
  assert( tmp.tm_min  < 60 );
  assert( tmp.tm_sec  < 60 );
  sprintf(out, "%02d:%02d:%02d", tmp.tm_hour, tmp.tm_min, tmp.tm_sec);
  return out;
}

}
}
}
