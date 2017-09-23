#pragma once
#include <string>
#include <thread>
#include "But/Log/Backend/Value.hpp"

namespace But
{
namespace Log
{
namespace Field
{

struct ThreadId final
{
  std::thread::id value_{ std::this_thread::get_id() };
};

inline std::string toType(ThreadId const&) { return "But::ThreadId"; }
Backend::Value toValue(ThreadId const& tid);

}
}
}
