#pragma once
#include <utility>
#include <type_traits>

namespace But
{
namespace Threading
{

/** @brief class ensuring given type is held inside a cache lines alone.
 *
 *  using this template, instead of raw T type prevents false-sharing
 *  from degrading performance. for details see:
 *  http://www.drdobbs.com/parallel/eliminate-false-sharing/217500206
 */
template<typename T, unsigned CLS=64>   // CacheLineSize
struct alignas(CLS) CacheLine
{
  static_assert( std::is_pod<T>::value, "cannot guarantee expected memory layout for non-PODs" );

  using value_type = T;

  T data_;
  // NOTE: padding is done automatically, since structure must be aligned! :-D
};

}
}
