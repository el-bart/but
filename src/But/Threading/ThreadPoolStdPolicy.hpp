#pragma once
#include <future>
#include <thread>

namespace But
{
namespace Threading
{

struct ThreadPoolStdPolicy
{
  template<typename T>
  using promise_type = std::promise<T>;
  using thread_type = std::thread;
};
}
}
