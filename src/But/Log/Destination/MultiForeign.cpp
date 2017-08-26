#include "MultiForeign.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

void MultiForeign::logImpl(Backend::Entry const& e)
{
  if( dsts_.empty() )
    return;

  for(collection_type::size_type i = 0u; i < dsts_.size() - 1u; ++i)
  {
    try
    {
      dsts_[i]->log(e);
    }
    catch(...)
    { /* this is logger - ignoring any errors when forwarind logs! */ }
  }

  try
  {
    dsts_.back()->log(e);
  }
  catch(...)
  { /* this is logger - ignoring any errors when forwarind logs! */ }
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
