#pragma once
#include "But/Log/Backend/Entry.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class Foregin
{
public:
  virtual ~Foregin() = default;

  template<typename ...Args>
  void log(Args&& ...args)
  {
    Backend::Entry e;
    e.reserve( sizeof...(args) );
    append(e, std::forward<Args>(args)...);
    logImpl( std::move(e) );
  }

  void reload()
  {
    reloadImpl();
  }

  auto operator->() { return this; }

private:
  template<typename H, typename ...T>
  static void append(Backend::Entry& e, H&& head, T&& ...tail)
  {
    e.push_back( Backend::FieldInfo{ std::forward<H>(head) } );
    append(e, std::forward<T>(tail)... );
  }
  static void append(Backend::Entry&) { }

  virtual void logImpl(Backend::Entry e) = 0;
  virtual void reloadImpl() = 0;
};

}
}
}
