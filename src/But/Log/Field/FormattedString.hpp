#pragma once
#include <string>

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
inline std::string toValue(FormattedString fs) { return std::move(fs.value_); }

inline auto operator==(FormattedString const& lhs, FormattedString const& rhs) { return lhs.value_ == rhs.value_; }

}
}
}
