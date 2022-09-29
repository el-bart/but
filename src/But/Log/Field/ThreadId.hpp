#pragma once
#include <thread>
#include <string>

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

inline auto fieldName(ThreadId const*) { return std::string_view{"But::ThreadId"}; }
std::string fieldValue(ThreadId const& tid);

}
}
}
