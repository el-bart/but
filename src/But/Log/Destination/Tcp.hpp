#pragma once
#include <mutex>
#include <But/Log/Destination/Sink.hpp>
#include <But/Log/Destination/Common/TcpClient.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

class Tcp final: public Sink
{
public:
  explicit Tcp(std::string host, uint16_t port);

private:
  void logImpl(std::string&& str) override;
  void reloadImpl() override;
  void flushImpl() override { }

  std::mutex mutex_;
  Common::TcpClient remote_;
};

}
}
}
