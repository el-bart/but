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

inline auto fieldName(FormattedString const*) { return std::string_view{"But::FormattedString"}; }
inline auto fieldValue(FormattedString const& fs) { return std::string_view{fs.value_}; }

inline auto operator==(FormattedString const& lhs, FormattedString const& rhs) { return lhs.value_ == rhs.value_; }

}
}
}
