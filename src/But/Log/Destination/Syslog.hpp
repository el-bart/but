#pragma once
#include <sstream>
#include <But/Log/Destination/Sink.hpp>
#include <But/Log/Backend/NonPrintableTrimmer.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

class Syslog final: public Sink
{
private:
  void logImpl(std::string&& str) override;
  void reloadImpl() override { }
  void flushImpl() override { }

  const Backend::NonPrintableTrimmer trim_;
};

}
}
}
