#pragma once
#include <string>
#include <thread>

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
std::string toValue(ThreadId const& tid);

}
}
}
