#pragma once
#include <sstream>
#include <iostream>
#include "But/Log/Backend/toString.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

/** @brief simples possible output, without any synchornization, buffers, optimizations, locking.
 *         siutable for trivial use cases.
 */
struct NaiveConsole final
{
  template<typename ...Args>
  void log(Args&& ...args)
  {
    append( std::forward<Args>(args)... );
  }

  void reload() { }
  void flush() { std::cout << std::flush; }

  auto operator->() { return this; }

private:
  template<typename H, typename ...T>
  static void append(H&& head, T&& ...tail)
  {
    using Backend::toString;
    std::cout << toString(head);
    append( std::forward<T>(tail)... );
  }
  static void append() { }

  static std::string endlineType()
  {
    std::stringstream ss;
    ss << std::endl;
    return ss.str();
  }

  const std::string endline_{ endlineType() };
};

}
}
}
