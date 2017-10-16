#pragma once
#include <string>
#include <ctime>
#include <But/Log/Field/ConversionError.hpp>
#include <But/Log/Backend/FieldInfo.hpp>

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


inline auto toFieldInfo(const UtcIsoDateTime uits)
{
  return Backend::FieldInfo{ Backend::Tag{"But::UtcIsoDT"}, Backend::Value{ uits.date() + "T" + uits.time() + "Z" } };
}

}
}
}
