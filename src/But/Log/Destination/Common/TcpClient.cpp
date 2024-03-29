#if __GNUC__ >= 12
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtsan"
#endif
#include <boost/asio.hpp>
#if __GNUC__ >= 12
#pragma GCC diagnostic pop
#endif

#include <But/Log/Destination/Common/TcpClient.hpp>

using boost::asio::ip::tcp;

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

struct TcpClient::Pimpl final
{
  Pimpl(std::string const& host, const uint16_t port)
  {
    const tcp::resolver::query query{ host, std::to_string(port) };
    const auto endpoint = resolver_.resolve(query);
    boost::asio::connect(socket_, endpoint);
  }

  boost::asio::io_service io_;
  tcp::resolver resolver_{io_};
  tcp::socket socket_{io_};
};


TcpClient::TcpClient(std::string host, const uint16_t port):
  host_{ std::move(host) },
  port_{port}
{ }


TcpClient::~TcpClient() = default;


void TcpClient::write(std::string const& data)
{
  connect();
  try
  {
    const auto size = boost::asio::write( pimpl_->socket_, boost::asio::buffer(data) );
    if( size != data.size() )
      BUT_THROW(IncompleteWrite, "wrote " << size << " bytes of " << data.size() << " bytes requested");
  }
  catch(IncompleteWrite const&)
  {
    throw;
  }
  catch(...)
  {
    close();
  }
}


void TcpClient::close()
{
  pimpl_.reset();
}


void TcpClient::connect()
{
  if(not pimpl_)
    pimpl_ = std::make_unique<Pimpl>(host_, port_);
}

}
}
}
}
