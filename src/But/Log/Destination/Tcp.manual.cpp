#include <But/Log/Destination/Tcp.hpp>

int main()
{
  But::Log::Destination::Tcp sink{"127.0.0.1", 4242};
  sink.log(R"( { "answer": 42 } )");
  sink.flush();
  sink.reload();
}
