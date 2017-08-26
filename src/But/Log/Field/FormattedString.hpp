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

inline std::string typeString(FormattedString const&) { return "But::FormatStr"; }
inline std::string toString(FormattedString fs) { return std::move(fs.value_); }

}
}
}
