#pragma once
#include <future>

namespace But
{
namespace Threading
{
namespace detail
{

template<typename T>
bool waitForFuture(std::future<T>& f, std::chrono::seconds timeout = std::chrono::seconds{5})
{
  return f.wait_for(timeout) == std::future_status::ready;
}

}
}
}
