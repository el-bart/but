#pragma once
#include <But/Log/Destination/Common/LockedStream.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

class Console: public Common::LockedStream
{
public:
  /** @brief initializes cout.
   *  @param syncWithStdio synchronization with stdio is usaully not needed and disabling it gives ~10% speed extra.
   */
  explicit Console(bool syncWithStdio = false);

private:
  void reloadImplUnderLock() override { }
};

}
}
}
