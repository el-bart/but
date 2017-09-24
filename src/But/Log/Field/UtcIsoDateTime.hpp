#pragma once
#include <string>
#include <ctime>
#include "ConversionError.hpp"
#include "But/Log/Backend/Type.hpp"
#include "But/Log/Backend/Value.hpp"

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


inline auto toType(UtcIsoDateTime const&) { return Backend::Type{"But::UtcIsoDT"}; }
inline auto toValue(UtcIsoDateTime const& d) { return Backend::Value{ d.date() + "T" + d.time() + "Z" }; }

}
}
}
