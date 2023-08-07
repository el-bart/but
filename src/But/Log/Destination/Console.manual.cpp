#include <But/Log/Destination/Console.hpp>

int main()
{
  But::Log::Destination::Console c;
  c.log("hello, world");
  c.flush();
  c.reload();
}
