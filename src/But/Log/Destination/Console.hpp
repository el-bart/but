#pragma once
#include <iostream>
#include "Stream.hpp"
#include "Foregin.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class Console final: public Foregin
{
public:
  Console() = default;

  template<typename ...Args>
  void log(Args&& ...args)
  {
    s_.log( std::forward<Args>(args)... );
  }

  auto operator->() { return this; }

private:
  void logImpl(Backend::Entry e) override { static_cast<Foregin&>(s_).log( std::move(e) ); }
  void reloadImpl() override { s_.reload(); }
  void flushImpl() override { s_.flush(); }

  Stream s_{std::cout};
};

}
}
}
