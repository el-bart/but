#pragma once
#include <iosfwd>
#include <mutex>
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
  void log(Args const& ...args)
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    append(args...);
  }

  auto operator->() { return this; }

private:
  template<typename H, typename ...T>
  void append(H const& head, T const& ...tail)
  {
    using Backend::toString;
    (*os_) << Backend::trimNonPrintable( toString(head) );
    append(tail...);
  }
  void append()
  {
    (*os_) << std::endl;
  }

  void logImpl(Backend::Entry e) override
  {
    const std::lock_guard<std::mutex> lock(mutex_);
    for(auto& f: e)
      (*os_) << Backend::trimNonPrintable( f.value() );
    (*os_) << std::endl;
  }

  void reloadImpl() override { }

  std::mutex mutex_;
  std::ostream* os_;
};

}
}
}
