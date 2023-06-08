#pragma once
#include <But/Log/Destination/Sink.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

class Null final: public Sink
{
private:
  void logImpl(std::string&&) override { }
  void reloadImpl() override { }
  void flushImpl() override { }
};

}
}
}
