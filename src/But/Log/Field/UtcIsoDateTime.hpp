#pragma once
#include <string>
#include <ctime>
#include "ConversionError.hpp"

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

  std::string date() const;
  std::string time() const;

  time_t timestamp_;
};


inline std::string toType(UtcIsoDateTime const&) { return "But::UtcIsoDT"; }
inline std::string toValue(UtcIsoDateTime const& d) { return d.date() + "T" + d.time() + "Z"; }

}
}
}
