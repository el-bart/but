#pragma once
#include <fstream>
#include <boost/filesystem/path.hpp>
#include <But/Log/Destination/TextStream.hpp>
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
class TextFile: public TextStream
{
public:
  BUT_DEFINE_EXCEPTION(OpeningLogFileFailed, Exception, "opening log file failed");

  explicit TextFile(boost::filesystem::path path);

private:
  void reloadImplUnderLock() override;

  const boost::filesystem::path path_;
  std::ofstream file_;
};

}
}
}
