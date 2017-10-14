#pragma once
#include <mutex>
#include "Sink.hpp"
#include "Common/Tcp.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

class Tcp final: public Sink
{
protected:
  explicit Tcp(std::string host, uint16_t port);

private:
  void logImpl(Backend::FieldInfo const& fi) override;
  void reloadImpl() override;
  void flushImpl() override { }

  std::mutex mutex_;
  Common::Tcp remote_;
};

}
}
}
