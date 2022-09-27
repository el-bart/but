#pragma once
#include <filesystem>
#include <string>

namespace But
{
namespace Log
{
namespace Field
{

struct FileName final
{
  std::filesystem::path value_;
};


inline auto fieldName(FileName const*) { return std::string_view{"But::FileName"}; }
inline auto fieldValue(FileName const& o) { return std::string_view{ o.value_.c_str() }; }

}
}
}
