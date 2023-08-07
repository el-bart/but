#include <But/Log/Destination/Tcp.hpp>
#include <But/Log/Destination/Common/toJson.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

Tcp::Tcp(std::string host, const uint16_t port):
  remote_{ std::move(host), port }
{ }


void Tcp::logImpl(std::string&& str)
{
  str.push_back('\n');
  remote_.write( std::move(str) );
}


void Tcp::reloadImpl()
{
  remote_.close();
}

}
}
}
