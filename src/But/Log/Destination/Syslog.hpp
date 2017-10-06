#pragma once
#include <sstream>
#include "But/Log/Backend/NonPrintableTrimmer.hpp"
#include "Foreign.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class Syslog final: public Foreign
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
