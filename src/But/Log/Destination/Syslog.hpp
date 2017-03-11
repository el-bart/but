#pragma once
#include <sstream>
#include "But/Log/Backend/toString.hpp"
#include "But/Log/Backend/trimNonPrintable.hpp"
#include "Foregin.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class Syslog final: public Foregin
{
public:
  template<typename ...Args>
  void log(Args const& ...args)
  {
    std::stringstream ss;
    append(ss, args...);
  }

  auto operator->() { return this; }

private:
  template<typename H, typename ...T>
  void append(std::stringstream& ss, H const& head, T const& ...tail)
  {
    using Backend::toString;
    ss << Backend::trimNonPrintable( toString(head) );
    append(ss, tail...);
  }
  void append(std::stringstream& ss);
  void logImpl(Backend::Entry e) override;

  void reloadImpl() override { }
};

}
}
}
