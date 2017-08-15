#pragma once
#include <sstream>
#include "But/Log/Backend/toString.hpp"
#include "But/Log/Backend/trimNonPrintable.hpp"
#include "But/Log/Field/Priority.hpp"
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
  void logImpl(Backend::Entry e) override;

  Field::Priority selectPriority(Field::Priority /*pOld*/, Field::Priority pNew) const { return pNew; }
  template<typename T>
  Field::Priority selectPriority(Field::Priority p, T const&) const { return p; }

  void reloadImpl() override { }
  void flushImpl() override { }
};

}
}
}
