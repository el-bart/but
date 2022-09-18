#include "EntryProxy.hpp"
#include "detail/EntryImpl.hpp"

namespace But::Log::Backend
{

void EntryProxy::value(std::string_view name)                     { impl().json_[ std::string{name} ] = nullptr; }
void EntryProxy::value(std::string_view name, bool v)             { impl().json_[ std::string{name} ] = v; }
void EntryProxy::value(std::string_view name, std::string_view v) { impl().json_[ std::string{name} ] = std::string{v}; }
void EntryProxy::value(std::string_view name, double v)           { impl().json_[ std::string{name} ] = v; }
void EntryProxy::value(std::string_view name, int64_t v)          { impl().json_[ std::string{name} ] = v; }
void EntryProxy::value(std::string_view name, uint64_t v)         { impl().json_[ std::string{name} ] = v; }

}
