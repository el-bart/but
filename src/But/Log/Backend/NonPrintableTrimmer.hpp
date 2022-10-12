#pragma once
#include <array>
#include <string>
#include <cinttypes>

namespace But
{
namespace Log
{
namespace Backend
{

struct NonPrintableTrimmer final
{
  NonPrintableTrimmer();

  std::string operator()(std::string in) const
  {
    for(auto& e: in)
      e = lut_[ (uint8_t)e ];
    return in;
  }

private:
  const std::array<char,256> lut_;
};

}
}
}
