#include "EntryRoot.hpp"
#include "detail/EntryImpl.hpp"
#include <cassert>


namespace But::Log::Backend
{

EntryRoot::EntryRoot():
  impl_{ std::make_shared<detail::EntryImpl>() }
{ }


EntryRoot::~EntryRoot() = default;


EntryRoot EntryRoot::independencCopy() const
{
}


EntryProxy EntryRoot::proxy()
{
  // TODO
  throw 42;
}


std::string EntryRoot::json() const
{
  // TODO
  throw 42;
}


EntryRoot::EntryRoot(detail::EntryImpl const& impl):
  impl_{ std::make_shared<detail::EntryImpl>(impl) }
{ }

}
