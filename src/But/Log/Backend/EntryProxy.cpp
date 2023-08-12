#include "EntryProxy.hpp"
#include <nlohmann/json.hpp>
#include <cassert>

namespace But::Log::Backend
{

namespace
{
nlohmann::json& j(void* impl)
{
  assert(impl);
  return *static_cast<nlohmann::json*>(impl);
}
} // unnamed namespace


EntryProxy EntryProxy::object(std::string_view name)
{
  auto& so = j(impl_)[ std::string{name} ];
  so = nlohmann::json::object();
  assert( so.is_object() );
  return EntryProxy{&so};
}


EntryArray EntryProxy::array(std::string_view name)
{
  auto& sa = j(impl_)[ std::string{name} ];
  sa = nlohmann::json::array();
  assert( sa.is_array() );
  return EntryArray{&sa};
}


void EntryProxy::value(std::string_view name)                     { j(impl_)[ std::string{name} ] = nullptr; }
void EntryProxy::value(std::string_view name, bool v)             { j(impl_)[ std::string{name} ] = v; }
void EntryProxy::value(std::string_view name, std::string_view v) { j(impl_)[ std::string{name} ] = std::string{v}; }
void EntryProxy::value(std::string_view name, double v)           { j(impl_)[ std::string{name} ] = v; }
void EntryProxy::value(std::string_view name, int64_t v)          { j(impl_)[ std::string{name} ] = v; }
void EntryProxy::value(std::string_view name, uint64_t v)         { j(impl_)[ std::string{name} ] = v; }

}
