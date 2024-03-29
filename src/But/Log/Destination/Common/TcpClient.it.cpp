#include <thread>

#if __GNUC__ >= 12
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtsan"
#endif
#include <boost/asio.hpp>
#if __GNUC__ >= 12
#pragma GCC diagnostic pop
#endif

#include <gtest/gtest.h>
#include <But/Log/Destination/Common/TcpClient.hpp>
#include <But/Threading/JoiningThread.hpp>

using But::Log::Destination::Common::TcpClient;
using boost::asio::ip::tcp;
using Thread = But::Threading::JoiningThread<std::thread>;

namespace
{

struct ButLogDestinationTcpClient: public testing::Test
{
  void accept()
  {
    acceptor_.accept(socket_);
  }

  auto read(size_t len)
  {
    std::string data;
    data.resize(len);
    boost::asio::read( socket_, boost::asio::buffer(&data[0], len) );
    return data;
  }

  void close()
  {
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    socket_.close();
  }

  boost::asio::io_service io_;
  tcp::acceptor acceptor_{ io_, tcp::endpoint( tcp::v4(), 1230 ) };
  tcp::socket socket_{io_};

  TcpClient client_{"127.0.0.1", 1230};
};


TEST_F(ButLogDestinationTcpClient, NoConnectionByDefault)
{
  EXPECT_NO_THROW( (TcpClient{"no-such-host", 6666}) );
}


TEST_F(ButLogDestinationTcpClient, ConnectingWhenSending)
{
  const auto msg = std::string{"hello world!"};
  Thread th{ [&] { client_.write(msg); } };
  accept();
  EXPECT_EQ( msg, read( msg.size() ) );
}


TEST_F(ButLogDestinationTcpClient, ThrowingOnIoError)
{
  TcpClient client{"127.6.6.6", 6666};
  EXPECT_ANY_THROW( client.write("oops") );
}


TEST_F(ButLogDestinationTcpClient, ReconnectingOnConnectionLoss)
{
  {
    const auto msg = std::string{"hello world!"};
    Thread th{ [&] { client_.write(msg); } };
    accept();
    EXPECT_EQ( msg, read( msg.size() ) );
  }

  close();

  {
    const auto msg = std::string{"hello again!"};
    Thread th{ [&] {
                     // unfortunately boost::asio does not detect closed soclet properly... :/
                     for(auto i=0; i<10; ++i)
                       client_.write(msg);
                   } };
    accept();
    EXPECT_EQ( msg, read( msg.size() ) );
  }
}


TEST_F(ButLogDestinationTcpClient, ExplicitClosingAllowsFutureReconnections)
{
  for(auto i=0; i<3; ++i)
  {
    const auto msg = std::string{"hello world!"};
    Thread th{ [&] { client_.write(msg); } };
    accept();
    EXPECT_EQ( msg, read( msg.size() ) );

    client_.close();
    close();
  }
}

}
