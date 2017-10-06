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
private:
  void logImpl(Backend::FieldInfo const&) override { }
  void reloadImpl() override { }
  void flushImpl() override { }
};

}
}
}
