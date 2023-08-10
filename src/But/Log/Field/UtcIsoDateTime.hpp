#pragma once
#include <string>
#include <ctime>
#include <But/Log/Field/detail/date2str.hpp>

namespace But
{
namespace Log
{
namespace Field
{

struct UtcIsoDateTime final
{
  UtcIsoDateTime(): timestamp_{ ::time(nullptr) } { }
  explicit UtcIsoDateTime(time_t ts): timestamp_{ts} { }

  std::string date() const { return detail::date2str(timestamp_); }
  std::string time() const { return detail::time2str(timestamp_); }

  time_t timestamp_;
};


inline constexpr auto fieldName(UtcIsoDateTime const*) { return std::string_view{"But::UtcIsoDT"}; }
inline auto fieldValue(const UtcIsoDateTime uits) { return uits.date() + "T" + uits.time() + "Z"; }

}
}
}
