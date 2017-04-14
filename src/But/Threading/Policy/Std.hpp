#pragma once
#include <future>
#include <thread>

namespace But
{
namespace Threading
{
namespace Policy
{

struct Std
{
  template<typename T>
  using promise_type = std::promise<T>;
  using thread_type = std::thread;

  static auto currentException() { return std::current_exception(); }
};

}
}
}
