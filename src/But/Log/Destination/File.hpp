#pragma once
#include <fstream>
#include <filesystem>
#include <But/Log/Destination/Common/LockedStream.hpp>
#include <But/Log/Destination/Sink.hpp>
#include <But/Exception.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

/** @brief typical output - text file, with one log per line.
 */
class File: public Common::LockedStream
{
public:
  BUT_DEFINE_EXCEPTION(OpeningLogFileFailed, Exception, "opening log file failed");

  explicit File(std::filesystem::path path);

private:
  void reloadImplUnderLock() override;

  const std::filesystem::path path_;
  std::ofstream file_;
};

}
}
}
