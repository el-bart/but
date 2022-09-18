#include <But/Log/Destination/JsonTcp.hpp>
#include <But/Log/Destination/Common/toJson.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

JsonTcp::JsonTcp(std::string host, const uint16_t port):
  remote_{ std::move(host), port }
{ }


void JsonTcp::logImpl(Backend::FieldInfo const& fi)
{
  const auto data = Common::toJson(fi).dump();
  remote_.write( data + "\n" );
}


void JsonTcp::reloadImpl()
{
  remote_.close();
}

}
}
}
