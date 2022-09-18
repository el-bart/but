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
  assert(impl_);
  return EntryProxy{ impl_.get() };
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
