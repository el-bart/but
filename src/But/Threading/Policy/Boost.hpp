#pragma once
#include <boost/thread/thread.hpp>
#include <boost/thread/future.hpp>

namespace But
{
namespace Threading
{
namespace Policy
{

struct Boost
{
  template<typename T>
  using promise_type = boost::promise<T>;
  using thread_type = boost::thread;

  static auto currentException() { return boost::current_exception(); }
};

}
}
}
