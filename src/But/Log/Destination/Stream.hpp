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

class Stream: public Foreign
{
protected:
  explicit Stream(std::ostream& os):
    os_{&os},
    endline_{ determineEndline() }
  { }

private:
  void logImpl(Backend::Entry e) override final
  {
    std::stringstream ss;
    toStreamFormat( ss, std::move(e) );
    const std::lock_guard<std::mutex> lock(mutex_);
    (*os_) << ss.rdbuf();
  }

  void reloadImpl() override final
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    reloadImplUnderLock();
  }

  void flushImpl() override final
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    (*os_) << std::flush;
  }

  virtual void reloadImplUnderLock() = 0;

  virtual void toStreamFormat(std::ostream& os, Backend::Entry&& e)
  {
    for(auto& f: e)
      os << Backend::trimNonPrintable( std::move(f).value() );
    os << endline_;
  }

  static std::string determineEndline()
  {
    std::stringstream ss;
    ss << std::endl;
    return ss.str();
  }

  std::mutex mutex_;
  std::ostream* os_;
  const std::string endline_;
};

}
}
}
