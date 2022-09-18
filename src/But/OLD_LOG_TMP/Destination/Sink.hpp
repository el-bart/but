#pragma once
#include <But/Log/Backend/FieldInfo.hpp>

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

  void log(Backend::FieldInfo&& fi) { logImpl(fi); }
  void log(Backend::FieldInfo const& fi) { logImpl(fi); }
  void reload() { reloadImpl(); }
  void flush() { flushImpl(); }

private:
  virtual void logImpl(Backend::FieldInfo const& fi) = 0;
  virtual void reloadImpl() = 0;
  virtual void flushImpl() = 0;
};

}
}
}
