#pragma once
#include <string>
#include "But/Log/Backend/Type.hpp"
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

inline auto toType(FormattedString const&) { return Backend::Type{"But::Formatted"}; }
inline auto toValue(FormattedString fs) { return Backend::Value{ std::move(fs.value_) }; }

inline auto operator==(FormattedString const& lhs, FormattedString const& rhs) { return lhs.value_ == rhs.value_; }

}
}
}
