#pragma once

namespace But
{
namespace Log
{
namespace Destination
{
namespace detail
{

template<size_t I, size_t N>
struct ReloadForwarder final
{
  template<typename T>
  static void pass(T& t)
  {
    try
    {
      std::get<I-1>(t)->reload();
    }
    catch(...)
    { /* this is logger - nothing reasonable can be done here... */ }

    ReloadForwarder<I+1,N>::pass(t);
  }
};


template<size_t N>
struct ReloadForwarder<N, N> final
{
  template<typename T>
  static void pass(T& t)
  {
    try
    {
      std::get<N-1>(t)->reload();
    }
    catch(...)
    { /* this is logger - nothing reasonable can be done here... */ }
  }
};

}
}
}
}
