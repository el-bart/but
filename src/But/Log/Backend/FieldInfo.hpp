#pragma once
#include "toValue.hpp"
#include "toType.hpp"

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
    type_{ toType(value) },
    value_{ toValue(value) }
  { }

  FieldInfo(Value const& value) = delete;
  FieldInfo(Value&& value) = delete;

  FieldInfo(Type type, Value value):
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

  Type const& type() const & { return type_; }
  Value const& value() const & { return value_; }

  Type type() && { return std::move(type_); }
  Value value() && { return std::move(value_); }

private:
  Type type_;
  Value value_;
};

}
}
}
