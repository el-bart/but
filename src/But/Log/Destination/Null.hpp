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
  void logImpl(Backend::Entry const&) override { }
  void logImpl(Field::FormattedString const&, Backend::Entry const&) override { }
  void reloadImpl() override { }
  void flushImpl() override { }
};

}
}
}
