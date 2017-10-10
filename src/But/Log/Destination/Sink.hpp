#pragma once
#include "But/Log/Backend/FieldInfo.hpp"
#include "But/Log/Backend/toFieldInfo.hpp"
#include "But/Log/Field/FormattedString.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class Foreign
{
public:
  Foreign() = default;
  virtual ~Foreign() = default;

  Foreign(Foreign const&) = delete;
  Foreign& operator=(Foreign const&) = delete;
  Foreign(Foreign&&) = delete;
  Foreign& operator=(Foreign&&) = delete;

  template<typename ...Args>
  void log(Args&& ...args)
  {
    using ::But::Log::Backend::toFieldInfo;
    const auto fi = Backend::FieldInfo{ Backend::Type{"log"}, { toFieldInfo( std::forward<Args>(args) )... } };
    logImpl(fi);
  }

  void log(Backend::FieldInfo&& fi) { logImpl(fi); }
  void log(Backend::FieldInfo const& fi) { logImpl(fi); }
  void reload() { reloadImpl(); }
  void flush() { flushImpl(); }

  auto operator->() { return this; }

private:
  virtual void logImpl(Backend::FieldInfo const& fi) = 0;
  virtual void reloadImpl() = 0;
  virtual void flushImpl() = 0;
};

}
}
}
