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
#include <But/Log/Destination/Tcp.hpp>
#include <But/Threading/JoiningThread.hpp>

using But::Log::Destination::Tcp;
using boost::asio::ip::tcp;
using Thread = But::Threading::JoiningThread<std::thread>;

namespace
{

struct ButLogDestinationTcp: public testing::Test
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

  void logAndRead()
  {
    Thread th{ [&] { sink_.log("test"); } };
    accept();
    const auto expected = std::string{"test\n"};
    EXPECT_EQ( expected, read( expected.size() ) );
  }

  boost::asio::io_service io_;
  tcp::acceptor acceptor_{ io_, tcp::endpoint( tcp::v4(), 1230 ) };
  tcp::socket socket_{io_};

  Tcp sink_{"127.0.0.1", 1230};
};


TEST_F(ButLogDestinationTcp, SendingLogsWorks)
{
  logAndRead();
}


TEST_F(ButLogDestinationTcp, FlushingIsNoOp)
{
  sink_.flush();
}


TEST_F(ButLogDestinationTcp, ReloadingWorks)
{
  logAndRead();

  sink_.reload();
  close();

  logAndRead();
}

}
