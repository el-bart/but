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
  /** @brief initializes cout.
   *  @param syncWithStdio synchronization with stdio is usaully not needed and disabling it gives ~10% speed extra.
   */
  explicit Console(const bool syncWithStdio = false):
    Stream{std::cout}
  {
    std::cout.sync_with_stdio(syncWithStdio);
  }

private:
  void reloadImplUnderLock() override { }
};

}
}
}
