#pragma once
#include <iostream>
#include "Stream.hpp"
#include "Foreign.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class Console final: public Foreign
{
private:
  void logImpl(Backend::Entry e) override { static_cast<Foreign&>(s_).log( std::move(e) ); }
  void reloadImpl() override { s_.reload(); }
  void flushImpl() override { s_.flush(); }

  Stream s_{std::cout};
};

}
}
}
