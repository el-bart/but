#pragma once
#include <iostream>
#include "Stream.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class Console final
{
public:
  Console() = default;

  template<typename ...Args>
  void log(Args const& ...args)
  {
    s_.log(args...);
  }

  void relad() { s_.reload(); }

  auto operator->() { return &s_; }

private:
  Stream s_{std::cout};
};

}
}
}
