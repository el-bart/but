#pragma once
#include <string>
#include <sys/types.h>
#include <unistd.h>

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

inline std::string toType(Pid const&) { return "But::Pid"; }
inline std::string toValue(Pid const& pid) { return std::to_string(pid.value_); }

}
}
}
