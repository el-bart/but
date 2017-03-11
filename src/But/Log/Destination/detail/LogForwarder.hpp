#pragma once
#include <tuple>

namespace But
{
namespace Log
{
namespace Destination
{
namespace detail
{

template<size_t I, size_t N>
struct LogForwarder final
{
  template<typename T, typename... Args>
  static void pass(T& t, Args&& ...args)
  {
    std::get<I-1>(t)->log(args...);
    LogForwarder<I+1,N>::pass( t, std::forward<Args>(args)... );
  }
};

template<size_t N>
struct LogForwarder<N, N> final
{
  template<typename T, typename... Args>
  static void pass(T& t, Args&& ...args)
  {
    std::get<N-1>(t)->log( std::forward<Args>(args)... );
  }
};

}
}
}
}
