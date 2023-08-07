#include <But/Log/Destination/Console.hpp>
#include <iostream>

namespace But
{
namespace Log
{
namespace Destination
{

Console::Console(const bool syncWithStdio):
  Common::LockedStream{std::cout}
{
  std::cout.sync_with_stdio(syncWithStdio);
}

}
}
}
