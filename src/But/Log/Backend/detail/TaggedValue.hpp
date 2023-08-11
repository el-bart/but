#pragma once
#include <string>
#include <memory>
#include <But/Log/Backend/EntryProxy.hpp>

namespace But::Log::Backend
{

struct TaggedInteger
{
  std::string_view name_;
  int64_t value_;
};

inline constexpr auto fieldName(TaggedInteger const*) { return std::string_view{"But::FileName"}; }
inline auto fieldValue(FileName const& o) { return std::string_view{ o.value_.c_str() }; }

}
}
