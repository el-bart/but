#pragma once
#include <iosfwd>
#include <string>

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

void toJsonStream(std::ostream& os, std::string const& str);

}
}
}
}
