#pragma once
#include <iostream>
#include <But/Log/Destination/Common/VerboseTextStream.hpp>
#include <But/Log/Destination/Sink.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

class VerboseTextConsole: public Common::VerboseTextStream
{
public:
  /** @brief initializes cout.
   *  @param syncWithStdio synchronization with stdio is usaully not needed and disabling it gives ~10% speed extra.
   */
  explicit VerboseTextConsole(const Common::VerboseTextStream::Depth depth, const bool syncWithStdio = false):
    Common::VerboseTextStream{std::cout, depth}
  {
    std::cout.sync_with_stdio(syncWithStdio);
  }

private:
  void reloadImplUnderLock() override { }
};

}
}
}
