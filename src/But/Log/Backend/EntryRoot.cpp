#include "EntryRoot.hpp"
#include "detail/EntryImpl.hpp"
#include <cassert>


namespace But::Log::Backend
{

EntryRoot::EntryRoot():
  impl_{ std::make_shared<detail::EntryImpl>() }
{ }


EntryRoot::~EntryRoot() = default;


EntryProxy EntryRoot::proxy()
{
  // TODO
  throw 42;
}


std::string EntryRoot::json() const
{
  assert(impl_);
  return impl_->json_.dump();
}


EntryRoot::EntryRoot(detail::EntryImpl const& impl):
  impl_{ std::make_shared<detail::EntryImpl>(impl) }
{ }

}
