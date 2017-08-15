#pragma once
#include <mutex>
#include <iosfwd>
#include <sstream>
#include <cctype>
#include "But/Log/Backend/toString.hpp"
#include "But/Log/Backend/trimNonPrintable.hpp"
#include "Foreign.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class Stream final: public Foreign
{
public:
  explicit Stream(std::ostream& os): os_{&os} { }

private:
  void logImpl(Backend::Entry e) override
  {
    std::stringstream ss;
    for(auto& f: e)
      ss << Backend::trimNonPrintable( std::move(f).value() );
    ss << std::endl;
    const std::lock_guard<std::mutex> lock(mutex_);
    (*os_) << ss.rdbuf();
  }

  void reloadImpl() override { }
  void flushImpl() override { (*os_) << std::flush; }

  std::mutex mutex_;
  std::ostream* os_;
};

}
}
}
