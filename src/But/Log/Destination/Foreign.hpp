#pragma once
#include "But/Log/Backend/Entry.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class Foreign
{
public:
  Foreign() = default;
  virtual ~Foreign() = default;

  Foreign(Foreign const&) = delete;
  Foreign& operator=(Foreign const&) = delete;
  Foreign(Foreign&&) = delete;
  Foreign& operator=(Foreign&&) = delete;

  template<typename ...Args>
  void log(Args&& ...args)
  {
    Backend::Entry e;
    e.reserve( sizeof...(args) );
    append(e, std::forward<Args>(args)...);
    logImpl( std::move(e) );
  }

  void log(Backend::Entry e) { logImpl( std::move(e) ); }
  void reload() { reloadImpl(); }
  void flush() { flushImpl(); }

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
  virtual void flushImpl() = 0;
};

}
}
}
