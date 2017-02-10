#pragma once
#include "Backend/Entry.hpp"

namespace But
{
namespace Log
{

class ForeginDestination
{
public:
  virtual ~ForeginDestination() = default;

  template<typename ...Args>
  void log(Args&& ...args)
  {
    Backend::Entry e;
    e.reserve( sizeof...(args) );
    append(e, args...);
    logImpl( std::move(e) );
  }

private:
  template<typename H, typename ...T>
  static void append(Backend::Entry& e, H&& head, T&& ...tail)
  {
    e.push_back( Backend::FieldInfo{ std::forward<H>(head) } );
    append(e, std::forward<T>(tail)... );
  }
  static void append(Backend::Entry&) { }

  virtual void logImpl(Backend::Entry e) = 0;
};

}
}
