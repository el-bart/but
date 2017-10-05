#pragma once
#include <thread>
#include "But/Log/Backend/FieldInfo.hpp"

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

Backend::FieldInfo toFieldInfo(ThreadId const& tid);

}
}
}
