#pragma once
#include <string_view>

namespace But::Log::detail
{

template<size_t N>
constexpr bool allNamesUniqueImpl(std::string_view (&names)[N])
{
  // doing some O(N^2) gymnastics here, as std::sort() is constexpr only since C++20.
  // however number of elements is small here, code is executed at compile time, so
  // it's not much of a practical issue.
  for(size_t i=1; i<N; ++i)
    for(size_t j=0; j<i; ++j)
      if( names[i] == names[j] )
        return false;
  return true;
}

constexpr bool allNamesUnique()
{
  return true;
}

template<typename ...Args>
constexpr bool allNamesUnique(Args ...args)
{
  std::string_view names[] = { args... };
  return allNamesUniqueImpl(names);
}

}
