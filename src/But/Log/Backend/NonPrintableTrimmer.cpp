#include "NonPrintableTrimmer.hpp"

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

NonPrintableTrimmer::NonPrintableTrimmer():
  lut_{ constructLut() }
{ }

}
}
}
