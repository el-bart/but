#pragma once
#include <string>
#include <cctype>
#include <But/Exception.hpp>

namespace But
{
namespace Log
{
namespace Destination
{
namespace Common
{

class Tcp final
{
public:
  BUT_DEFINE_EXCEPTION(IncompleteWrite, But::Exception, "incomplete write");

  explicit Tcp(std::string host, uint16_t port);
  ~Tcp();

  void write(std::string const& data);
  void close();

private:
  struct Pimpl;

  void connect();

  const std::string host_;
  const uint16_t port_;

  std::unique_ptr<Pimpl> pimpl_;
};

}
}
}
}
