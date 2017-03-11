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
  std::string date() const;
  std::string time() const;

  time_t timestamp_;
};


inline std::string typeString(UtcIsoDateTime const&) { return "But::UtcIsoDT"; }
inline std::string toString(UtcIsoDateTime const& d) { return d.date() + "T" + d.time() + "Z"; }

}
}
}
