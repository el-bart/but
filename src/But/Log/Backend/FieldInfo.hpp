#pragma once
#include <string>
#include "toString.hpp"
#include "typeString.hpp"

namespace But
{
namespace Log
{
namespace Backend
{

class FieldInfo final
{
public:
  template<typename T>
  explicit FieldInfo(T const& value):
    type_{ typeString(value) },
    value_{ toString(value) }
  { }

  explicit FieldInfo(std::string&& value):
    type_{ typeString(value) },
    value_{ std::move(value) }
  { }

  FieldInfo(std::string type, std::string value):
    type_{ std::move(type) },
    value_{ std::move(value) }
  { }

  FieldInfo(FieldInfo&&) = default;
  FieldInfo& operator=(FieldInfo&&) = default;

  FieldInfo(FieldInfo const&) = default;
  FieldInfo& operator=(FieldInfo const&) = default;

  bool operator==(FieldInfo const& other) const
  {
    return type_ == other.type_ && value_ == other.value_;
  }

  std::string const& type() const & { return type_; }
  std::string const& value() const & { return value_; }

  std::string&& type() && { return std::move(type_); }
  std::string&& value() && { return std::move(value_); }

private:
  std::string type_;
  std::string value_;
};

}
}
}
