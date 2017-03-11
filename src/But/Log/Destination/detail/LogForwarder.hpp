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
    try
    {
      std::get<I-1>(t)->log(args...);
    }
    catch(...)
    { /* this is logger - nothing reasonable can be done here... */ }

    LogForwarder<I+1,N>::pass( t, std::forward<Args>(args)... );
  }
};


template<size_t N>
struct LogForwarder<N, N> final
{
  template<typename T, typename... Args>
  static void pass(T& t, Args&& ...args)
  {
    try
    {
      std::get<N-1>(t)->log( std::forward<Args>(args)... );
    }
    catch(...)
    { /* this is logger - nothing reasonable can be done here... */ }
  }
};

}
}
}
}
