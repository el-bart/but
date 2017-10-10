#include "MultiForeign.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

void MultiForeign::logImplNe(Backend::FieldInfo const& fi) noexcept
{
  for(auto& d: dsts_)
  {
    try
    {
      d->log(fi);
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
