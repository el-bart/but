#pragma once
#include <sstream>
#include "But/Log/Backend/toString.hpp"
#include "But/Log/Backend/trimNonPrintable.hpp"
#include "But/Log/Field/Priority.hpp"
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
  void log(Args&& ...args)
  {
    std::stringstream ss;
    append( ss, Field::Priority::info, std::forward<Args>(args)... );
  }

  auto operator->() { return this; }

private:
  template<typename H, typename ...T>
  void append(std::stringstream& ss, const Field::Priority p, H&& head, T&& ...tail)
  {
    const auto pNew = selectPriority(p, head);
    using Backend::toString;
    ss << Backend::trimNonPrintable( toString( std::forward<H>(head) ) );
    append( ss, pNew, std::forward<T>(tail)... );
  }
  void append(std::stringstream& ss, Field::Priority p);
  void logImpl(Backend::Entry e) override;

  Field::Priority selectPriority(Field::Priority /*pOld*/, Field::Priority pNew) const { return pNew; }
  template<typename T>
  Field::Priority selectPriority(Field::Priority p, T const&) const { return p; }

  void reloadImpl() override { }
  void flushImpl() override { }
};

}
}
}
