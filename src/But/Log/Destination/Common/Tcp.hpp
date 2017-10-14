#pragma once
#include <string>
#include <cctype>
#include "But/System/Descriptor.hpp"

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
  explicit Tcp(std::string host, uint16_t port);

  void write(std::string const& data);
  void close();

private:
  System::Descriptor connect() const;

  const std::string host_;
  const uint16_t port_;
  System::Descriptor socket_;
};

}
}
}
}
