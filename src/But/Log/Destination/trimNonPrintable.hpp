#pragma once
#include <string>

namespace But
{
namespace Log
{
namespace Destination
{

inline std::string trimNonPrintable(std::string in)
{
  for(auto& e: in)
    if( not isprint(e) )
      e = '.';
  return in;
}

}
}
}
