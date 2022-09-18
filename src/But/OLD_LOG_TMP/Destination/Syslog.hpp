#pragma once
#include <sstream>
#include <But/Log/Backend/NonPrintableTrimmer.hpp>
#include <But/Log/Destination/Sink.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

class Syslog final: public Sink
{
private:
  void logImpl(Backend::FieldInfo const& fi) override;
  void reloadImpl() override { }
  void flushImpl() override { }

  const Backend::NonPrintableTrimmer trim_;
};

}
}
}
