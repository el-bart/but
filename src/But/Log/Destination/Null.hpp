#pragma once
#include "Sink.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class Null final: public Sink
{
private:
  void logImpl(Backend::FieldInfo const&) override { }
  void reloadImpl() override { }
  void flushImpl() override { }
};

}
}
}
