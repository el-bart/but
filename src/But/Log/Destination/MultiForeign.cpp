#include "MultiForeign.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

void MultiForeign::logImpl(Backend::Entry const& e)
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


void MultiForeign::logImpl(Field::FormattedString const& str, Backend::Entry const& e)
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


void MultiForeign::reloadImpl()
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


void MultiForeign::flushImpl()
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
