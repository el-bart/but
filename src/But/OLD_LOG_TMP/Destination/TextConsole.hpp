#pragma once
#include <iostream>
#include <But/Log/Destination/Common/TextStream.hpp>
#include <But/Log/Destination/Sink.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

class TextConsole: public Common::TextStream
{
public:
  /** @brief initializes cout.
   *  @param syncWithStdio synchronization with stdio is usaully not needed and disabling it gives ~10% speed extra.
   */
  explicit TextConsole(const bool syncWithStdio = false):
    Common::TextStream{std::cout}
  {
    std::cout.sync_with_stdio(syncWithStdio);
  }

private:
  void reloadImplUnderLock() override { }
};

}
}
}
