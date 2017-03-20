#pragma once
#include <mutex>
#include <iosfwd>
#include <sstream>
#include <cctype>
#include "But/Log/Backend/toString.hpp"
#include "But/Log/Backend/trimNonPrintable.hpp"
#include "Foregin.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class Stream final: public Foregin
{
public:
  explicit Stream(std::ostream& os): os_{&os} { }

  template<typename ...Args>
  void log(Args&& ...args)
  {
    std::stringstream ss;
    append(ss, std::forward<Args>(args)...);
  }

  auto operator->() { return this; }

private:
  template<typename H, typename ...T>
  void append(std::stringstream& ss, H&& head, T&& ...tail)
  {
    using Backend::toString;
    ss << Backend::trimNonPrintable( toString( std::forward<H>(head) ) );
    append(ss, std::forward<T>(tail)... );
  }
  void append(std::stringstream& ss)
  {
    ss << std::endl;
    const std::lock_guard<std::mutex> lock(mutex_);
    (*os_) << ss.rdbuf();
  }

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
