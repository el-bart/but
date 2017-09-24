#pragma once
#include <thread>
#include "But/Log/Backend/Type.hpp"
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

inline auto toType(ThreadId const&) { return Backend::Type{"But::ThreadId"}; }
Backend::Value toValue(ThreadId const& tid);

}
}
}
