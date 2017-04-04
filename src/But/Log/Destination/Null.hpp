#pragma once
#include "Foreign.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class Null final: public Foreign
{
public:
  template<typename ...Args>
  void log(Args&& ...) { }

  auto operator->() { return this; }

private:
  void logImpl(Backend::Entry) override { }
  void reloadImpl() override { }
  void flushImpl() override { }
};

}
}
}
