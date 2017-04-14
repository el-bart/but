#pragma once
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

inline std::string typeString(ThreadId const&) { return "But::ThreadId"; }
std::string toString(ThreadId const& tid);

}
}
}
