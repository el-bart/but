#include <atomic>
#include <But/Log/Field/ThreadNo.hpp>

namespace But
{
namespace Log
{
namespace Field
{

namespace
{
std::atomic<uint64_t> g_nextFreeThreadNo{0};
thread_local const uint64_t g_currentThreadNumber{ g_nextFreeThreadNo++ };
}

ThreadNo::ThreadNo():
  value_{g_currentThreadNumber}
{ }

}
}
}
