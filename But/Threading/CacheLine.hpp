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
 *
 *  @warning as of the end of 2014 many compilers do not properly align heap-alocated
 *           objects, even though it is required by the standard (C++11: 3.11/9).
 *
 *  @note 64-bytes default alignment is just a typical value, that will work for MOST
 *        users. check if this is a cache line of your CPU, before usage!
 */
template<typename T, std::size_t Align=64>
struct alignas(Align) CacheLine
{
  static_assert( std::is_standard_layout<T>::value, "cannot guarantee expected memory layout for non-standard-layout objects" );

  using value_type = T;

  T data_;
  // NOTE: padding is done automatically, since structure must be aligned! :-D
};

}
}
