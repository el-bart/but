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

  FieldInfo(std::string type, std::string value):
    type_{ std::move(type) },
    value_{ std::move(value) }
  { }

  FieldInfo(FieldInfo&&) = default;
  FieldInfo& operator=(FieldInfo&&) = default;

  FieldInfo(FieldInfo const&) = default;
  FieldInfo& operator=(FieldInfo const&) = default;

  std::string const& type() const { return type_; }
  std::string const& value() const { return value_; }

private:
  std::string type_;
  std::string value_;
};

}
}
}
