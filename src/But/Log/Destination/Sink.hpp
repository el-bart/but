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

class Sink
{
public:
  Sink() = default;
  virtual ~Sink() = default;

  Sink(Sink const&) = delete;
  Sink& operator=(Sink const&) = delete;
  Sink(Sink&&) = delete;
  Sink& operator=(Sink&&) = delete;

  template<typename ...Args>
  void log(Args&& ...args)
  {
    using ::But::Log::Backend::toFieldInfo;
    const auto fi = Backend::FieldInfo{ Backend::Tag{"log"}, { toFieldInfo( std::forward<Args>(args) )... } };
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
