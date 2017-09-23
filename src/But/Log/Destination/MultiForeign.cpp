#include "MultiForeign.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

void MultiForeign::logImplNe(Backend::Entry const& e) noexcept
{
  for(auto& d: dsts_)
  {
    try
    {
      d->log(e);
    }
    catch(...)
    { /* this is logger - ignoring any errors when forwarind logs! */ }
  }
}


void MultiForeign::logImplNe(Field::FormattedString const& str, Backend::Entry const& e) noexcept
{
  for(auto& d: dsts_)
  {
    try
    {
      d->log(str, e);
    }
    catch(...)
    { /* this is logger - ignoring any errors when forwarind logs! */ }
  }
}


void MultiForeign::reloadImplNe() noexcept
{
  for(auto& d: dsts_)
  {
    try
    {
      d->reload();
    }
    catch(...)
    { /* this is logger - ignoring any errors when reloading logs destinations! */ }
  }
}


void MultiForeign::flushImplNe() noexcept
{
  for(auto& d: dsts_)
  {
    try
    {
      d->flush();
    }
    catch(...)
    { /* this is logger - ignoring any errors when flushing logs! */ }
  }
}

}
}
}
