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
struct FlushForwarder final
{
  template<typename T>
  static void pass(T& t)
  {
    try
    {
      std::get<I-1>(t)->flush();
    }
    catch(...)
    { /* this is logger - nothing reasonable can be done here... */ }

    FlushForwarder<I+1,N>::pass(t);
  }
};


template<size_t N>
struct FlushForwarder<N, N> final
{
  template<typename T>
  static void pass(T& t)
  {
    try
    {
      std::get<N-1>(t)->flush();
    }
    catch(...)
    { /* this is logger - nothing reasonable can be done here... */ }
  }
};

}
}
}
}
