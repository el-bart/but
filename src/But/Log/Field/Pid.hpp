#pragma once
#include <sys/types.h>
#include <unistd.h>
#include <string>

namespace But
{
namespace Log
{
namespace Field
{

struct Pid final
{
  pid_t value_{ getpid() };
};

inline constexpr auto fieldName(Pid const*) { return std::string_view{"But::Pid"}; }
inline auto fieldValue(Pid const& o) { return o.value_; }


}
}
}
