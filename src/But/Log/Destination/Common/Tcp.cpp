#include "Tcp.hpp"

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

Tcp::Tcp(std::string host, uint16_t port):
  host_{ std::move(host) },
  port_{port}
{ }


void Tcp::write(std::string const& data)
{
  (void)data;
  // TODO...
}


void Tcp::close()
{
  // TODO...
}


System::Descriptor Tcp::connect() const
{
  throw 42;
}

}
}
}
}
