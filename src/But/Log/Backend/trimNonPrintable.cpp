#include <array>
#include <cinttypes>
#include "trimNonPrintable.hpp"

namespace But
{
namespace Log
{
namespace Backend
{

namespace
{
auto constructLut()
{
  std::array<char, 256> out;
  for(auto i=0u; i<256; ++i)
  {
    const auto c = (char)i;
    out[i] = isprint(c) ? c : '.';
  }
  return out;
}
}

std::string trimNonPrintable(std::string in)
{
  static const auto lut = constructLut();
  for(auto& e: in)
    e = lut[ (uint8_t)e ];
  return in;
}

}
}
}
