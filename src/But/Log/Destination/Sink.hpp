#pragma once
#include <string>

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

  void log(std::string str) { logImpl( std::move(str) ); }
  void reload() { reloadImpl(); }
  void flush() { flushImpl(); }

private:
  virtual void logImpl(std::string&& str) = 0;
  virtual void reloadImpl() = 0;
  virtual void flushImpl() = 0;
};

}
}
}
