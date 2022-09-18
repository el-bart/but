#include <thread>
#include <boost/asio.hpp>
#include <gtest/gtest.h>
#include <But/Log/Destination/Common/Tcp.hpp>
#include <But/Threading/JoiningThread.hpp>

using But::Log::Destination::Common::Tcp;
using boost::asio::ip::tcp;
using Thread = But::Threading::JoiningThread<std::thread>;

namespace
{

struct ButLogDestinationCommonTcp: public testing::Test
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

  Tcp client_{"127.0.0.1", 1230};
};


TEST_F(ButLogDestinationCommonTcp, NoConnectionByDefault)
{
  EXPECT_NO_THROW( (Tcp{"no-such-host", 6666}) );
}


TEST_F(ButLogDestinationCommonTcp, ConnectingWhenSending)
{
  const auto msg = std::string{"hello world!"};
  Thread th{ [&] { client_.write(msg); } };
  accept();
  EXPECT_EQ( msg, read( msg.size() ) );
}


TEST_F(ButLogDestinationCommonTcp, ThrowingOnIoError)
{
  Tcp client{"127.6.6.6", 6666};
  EXPECT_ANY_THROW( client.write("oops") );
}


TEST_F(ButLogDestinationCommonTcp, ReconnectingOnConnectionLoss)
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


TEST_F(ButLogDestinationCommonTcp, ExplicitClosingAllowsFutureReconnections)
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
