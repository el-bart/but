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
  void log(Args const& ...args)
  {
    s_.log(args...);
  }

  void relad() { s_.reload(); }

  auto operator->() { return &s_; }

private:
  void logImpl(Backend::Entry e) override { static_cast<Foregin&>(s_).log( std::move(e) ); }
  void reloadImpl() override { reload(); }

  Stream s_{std::cout};
};

}
}
}
