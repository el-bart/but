#include "EntryRoot.hpp"
#include <nlohmann/json.hpp>
#include <cassert>


namespace But::Log::Backend
{

struct EntryRoot::Impl
{
  nlohmann::json json_{ nlohmann::json::object() };
};


EntryRoot::EntryRoot():
  impl_{ std::make_shared<Impl>() }
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


EntryRoot::EntryRoot(Impl const& impl):
  impl_{ std::make_shared<Impl>(impl) }
{ }

}
