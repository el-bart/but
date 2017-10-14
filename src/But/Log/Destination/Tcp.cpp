#include "Tcp.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

Tcp::Tcp(std::string host, const uint16_t port):
  remote_{ std::move(host), port }
{ }


void Tcp::logImpl(Backend::FieldInfo const& fi)
{
  (void)fi;
  // TODO...
}


void Tcp::reloadImpl()
{
  // TODO...
}

}
}
}
