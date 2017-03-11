#pragma once
#include <iosfwd>
#include <cctype>
#include "But/Log/Backend/Entry.hpp"
#include "But/Log/Backend/toString.hpp"
#include "trimNonPrintable.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class Stream final
{
public:
  explicit Stream(std::ostream& os): os_{&os} { }

  template<typename ...Args>
  void log(Args const& ...args)
  {
    append(args...);
  }

  auto operator->() { return this; }

private:
  template<typename H, typename ...T>
  void append(H const& head, T const& ...tail)
  {
    using Backend::toString;
    (*os_) << trimNonPrintable( toString(head) );
    append(tail...);
  }
  void append()
  {
    (*os_) << std::endl;
  }

  std::ostream* os_;
};

}
}
}