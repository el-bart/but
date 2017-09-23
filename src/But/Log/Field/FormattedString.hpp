#pragma once
#include <string>
#include "But/Log/Backend/Value.hpp"

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

inline std::string toType(FormattedString const&) { return "But::Formatted"; }
inline Backend::Value toValue(FormattedString fs) { return std::move(fs.value_); }

inline auto operator==(FormattedString const& lhs, FormattedString const& rhs) { return lhs.value_ == rhs.value_; }

}
}
}
