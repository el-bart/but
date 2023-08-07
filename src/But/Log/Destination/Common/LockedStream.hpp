#pragma once
#include <mutex>
#include <iosfwd>
#include <sstream>
#include <cctype>
#include <But/Log/Destination/Sink.hpp>

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

class LockedStream: public Sink
{
protected:
  explicit LockedStream(std::ostream& os): os_{os} { }

  auto const& endline() const { return endline_; }

private:
  void logImpl(std::string&& str) override final
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    os_ << std::move(str) << endline_;
  }

  void reloadImpl() override final
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    reloadImplUnderLock();
  }

  void flushImpl() override final
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    os_ << std::flush;
  }

  virtual void reloadImplUnderLock() = 0;

  static std::string endlineType()
  {
    std::stringstream ss;
    ss << std::endl;
    return ss.str();
  }

  std::mutex mutex_;
  // TODO: consider std::osyncstream
  // https://en.cppreference.com/w/cpp/io/basic_osyncstream
  std::ostream& os_;
  const std::string endline_{ endlineType() };
};

}
}
}
}
