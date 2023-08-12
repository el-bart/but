#include "EntryArray.hpp"
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


EntryProxy EntryArray::object()
{
  j(impl_).push_back( nlohmann::json::object() );
  assert( j(impl_).back().is_object() );
  return EntryProxy{ &j(impl_).back() };
}


EntryArray EntryArray::array()
{
  j(impl_).push_back( nlohmann::json::array() );
  assert( j(impl_).back().is_array() );
  return EntryArray{ &j(impl_).back() };
}


void EntryArray::value()                   { j(impl_).push_back(nullptr); }
void EntryArray::value(bool v)             { j(impl_).push_back(v); }
void EntryArray::value(std::string_view v) { j(impl_).push_back(std::string{v}); }
void EntryArray::value(double v)           { j(impl_).push_back(v); }
void EntryArray::value(int64_t v)          { j(impl_).push_back(v); }
void EntryArray::value(uint64_t v)         { j(impl_).push_back(v); }

}
