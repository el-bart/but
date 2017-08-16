#pragma once
#include <iostream>
#include "Stream.hpp"
#include "Foreign.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class Console: public Stream
{
public:
  Console():
    Stream{std::cout}
  {
    std::cout.sync_with_stdio(false);
  }

private:
  void reloadImplUnderLock() override { }
};

}
}
}
