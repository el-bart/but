#pragma once
#include <string>
#include <But/Log/Field/ConversionError.hpp>

namespace But
{
namespace Log
{
namespace Field
{
namespace detail
{

std::string date2str(const time_t timestamp);
inline std::string date2str(const struct timespec ts) { return date2str(ts.tv_sec); }

std::string time2str(const time_t timestamp);
inline std::string time2str(const struct timespec ts) { return time2str(ts.tv_sec); }

std::string nanosec2str(const long nsec);
inline std::string nanosec2str(const struct timespec ts) { return nanosec2str(ts.tv_nsec); }

}
}
}
}
