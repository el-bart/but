#pragma once
#include <future>
#include <boost/thread/future.hpp>

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

template<typename T>
bool waitForFuture(boost::BOOST_THREAD_FUTURE<T>& f, boost::chrono::seconds timeout = boost::chrono::seconds{5})
{
  return f.wait_for(timeout) == boost::future_status::ready;
}

}
}
}
