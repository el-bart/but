#pragma once
#include <mutex>
#include <iosfwd>
#include <sstream>
#include <cctype>
#include "But/Log/Backend/toString.hpp"
#include "But/Log/Backend/NonPrintableTrimmer.hpp"
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
  explicit Stream(std::ostream& os): os_{&os} { }

private:
  void logImpl(Backend::Entry e) override final
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    toStreamFormat( *os_, std::move(e) );
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
      os << trim_( std::move(f).value() );
    os << endline_;
  }

  static std::string endlineType()
  {
    std::stringstream ss;
    ss << std::endl;
    return ss.str();
  }

  std::mutex mutex_;
  std::ostream* os_;
  const std::string endline_{ endlineType() };
  const Backend::NonPrintableTrimmer trim_;
};

}
}
}
