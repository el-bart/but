#pragma once
#include <boost/thread/thread.hpp>
#include <boost/thread/future.hpp>

namespace But
{
namespace Threading
{

struct ThreadPoolBoostPolicy
{
  template<typename T>
  using promise_type = boost::promise<T>;
  using thread_type = boost::thread;
};
}
}
