#pragma once
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
  explicit JsonConsole(bool syncWithStdio = false);

private:
  void reloadImplUnderLock() override { }
  void toStreamFormat(std::ostream& os, Backend::FieldInfo const& fi) override;
};

}
}
}
