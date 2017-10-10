#pragma once
#include <string>
#include "But/Log/Backend/FieldInfo.hpp"

namespace But
{
namespace Log
{
namespace Field
{

struct FormattedString final
{
  std::string value_;
};

inline auto toFieldInfo(FormattedString fs)
{
  return Backend::FieldInfo{ Backend::Tag{"But::Formatted"}, Backend::Value{ std::move(fs.value_) } };
}

inline auto operator==(FormattedString const& lhs, FormattedString const& rhs) { return lhs.value_ == rhs.value_; }

}
}
}
