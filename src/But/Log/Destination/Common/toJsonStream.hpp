#pragma once
#include <iosfwd>
#include "But/Log/Field/FormattedString.hpp"
#include "But/Log/Backend/Entry.hpp"

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

void toJsonStream(std::ostream& os, Backend::Entry const& entry);
void toJsonStream(std::ostream& os, Field::FormattedString const& str, Backend::Entry const& entry);

}
}
}
}
