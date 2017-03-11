#pragma once
#include <fstream>
#include <boost/filesystem/path.hpp>
#include "Stream.hpp"
#include "Foregin.hpp"
#include "But/Exception.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

/** @brief typical output - text file, with one log per line.
 */
class TextFile final: public Foregin
{
public:
  BUT_DEFINE_EXCEPTION(OpeningLogFileFailed, Exception, "opening log file failed");

  explicit TextFile(boost::filesystem::path path);

  template<typename ...Args>
  void log(Args const& ...args)
  {
    s_.log(args...);
  }

  auto operator->() { return &s_; }

private:
  void logImpl(Backend::Entry e) override { static_cast<Foregin&>(s_).log( std::move(e) ); }
  void reloadImpl() override;

  boost::filesystem::path path_;
  std::ofstream file_;
  Stream s_;
};

}
}
}
