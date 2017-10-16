#include <boost/asio.hpp>
#include <But/Log/Destination/Common/Tcp.hpp>

using boost::asio::ip::tcp;

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

struct Tcp::Pimpl final
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


Tcp::Tcp(std::string host, const uint16_t port):
  host_{ std::move(host) },
  port_{port}
{ }


Tcp::~Tcp() = default;


void Tcp::write(std::string const& data)
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


void Tcp::close()
{
  pimpl_.reset();
}


void Tcp::connect()
{
  if(not pimpl_)
    pimpl_ = std::make_unique<Pimpl>(host_, port_);
}

}
}
}
}
