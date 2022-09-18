#include <thread>
#include <boost/asio.hpp>
#include <gtest/gtest.h>
#include <But/Log/Destination/JsonTcp.hpp>
#include <But/Threading/JoiningThread.hpp>
#include <But/Log/Destination/detail/args2FieldInfo.hpp>

using But::Log::Destination::detail::args2FieldInfo;

using But::Log::Destination::JsonTcp;
using boost::asio::ip::tcp;
using Thread = But::Threading::JoiningThread<std::thread>;

namespace
{

struct ButLogDestinationJsonTcp: public testing::Test
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
    Thread th{ [&] { sink_.log( args2FieldInfo("test" ) ); } };
    accept();
    const auto expected = std::string{R"xx({"string":"test"})xx"} + "\n";
    EXPECT_EQ( expected, read( expected.size() ) );
  }

  boost::asio::io_service io_;
  tcp::acceptor acceptor_{ io_, tcp::endpoint( tcp::v4(), 1230 ) };
  tcp::socket socket_{io_};

  JsonTcp sink_{"127.0.0.1", 1230};
};


TEST_F(ButLogDestinationJsonTcp, SendingLogsWorks)
{
  logAndRead();
}


TEST_F(ButLogDestinationJsonTcp, FlushingIsNoOp)
{
  sink_.flush();
}


TEST_F(ButLogDestinationJsonTcp, ReloadingWorks)
{
  logAndRead();

  sink_.reload();
  close();

  logAndRead();
}

}
