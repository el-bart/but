#pragma once
#include <fstream>
#include <boost/filesystem/path.hpp>
#include "Stream.hpp"
#include "Foreign.hpp"
#include "But/Exception.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

/** @brief typical output - text file, with one log per line.
 */
class TextFile final: public Foreign
{
public:
  BUT_DEFINE_EXCEPTION(OpeningLogFileFailed, Exception, "opening log file failed");

  explicit TextFile(boost::filesystem::path path);

  template<typename ...Args>
  void log(Args&& ...args)
  {
    s_.log( std::forward<Args>(args)... );
  }

  auto operator->() { return this; }

private:
  void logImpl(Backend::Entry e) override { static_cast<Foreign&>(s_).log( std::move(e) ); }
  void reloadImpl() override;
  void flushImpl() override { s_.flush(); }

  boost::filesystem::path path_;
  std::ofstream file_;
  Stream s_;
};

}
}
}
