#pragma once
#include <iostream>
#include "TextStream.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class JsonConsole: public TextStream
{
public:
  /** @brief initializes cout.
   *  @param syncWithStdio synchronization with stdio is usaully not needed and disabling it gives ~10% speed extra.
   */
  explicit JsonConsole(const bool syncWithStdio = false):
    TextStream{std::cout}
  {
    std::cout.sync_with_stdio(syncWithStdio);
  }

private:
  void reloadImplUnderLock() override { }
  void toStreamFormat(std::ostream& os, Backend::Entry const& entry) override;
  void toStreamFormat(std::ostream& os, Field::FormattedString const& str, Backend::Entry const& entry) override;
};

}
}
}
