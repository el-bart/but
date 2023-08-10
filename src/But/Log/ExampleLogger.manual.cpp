/** @brief Logger is more of a template for building custom loggers. here's an example how.
 */

#include <But/Log/Logger.hpp>
#include <But/Log/Destination/Console.hpp>
#include <But/Log/Field/UtcIsoDateTime.hpp>
#include <But/Log/Field/ThreadId.hpp>
#include <But/Log/Field/Pid.hpp>
#include <But/Log/Field/LineNumber.hpp>
#include <But/Log/Field/FileName.hpp>

using namespace But::Log::Field;

// note that Logger does not enforce any specific severity class to be used
enum class MySev { Info, Warning, Error };
inline constexpr auto fieldName(MySev const*) { return "MySev"; }
auto fieldValue(MySev ms)
{
  switch(ms)
  {
    case MySev::Info:    return "INFO";
    case MySev::Warning: return "WARNING";
    case MySev::Error:   return "ERROR";
  }
}


struct MyLogger
{
  using DestPtr = But::Log::Logger<>::Destination;

  explicit MyLogger(DestPtr d): impl_{ std::move(d) } { }

  template<typename ...Args>
  void info(std::string_view message, Args&& ...args) const
  {
    log( MySev::Info, message, std::forward<Args>(args)... );
  }

  template<typename ...Args>
  void warning(std::string_view message, Args&& ...args) const
  {
    log( MySev::Warning, message, std::forward<Args>(args)... );
  }

  template<typename ...Args>
  void error(std::string_view message, Args&& ...args) const
  {
    log( MySev::Error, message, std::forward<Args>(args)... );
  }

  template<typename ...Args>
  auto withFields(Args&& ...args) const
  {
    return MyLogger{ impl_.withFields( std::forward<Args>(args)... ) };
  }

  void reload() { impl_.reload(); }
  void flush()  { impl_.flush();  }

private:
  explicit MyLogger(But::Log::Logger<>&& l): impl_{ std::move(l) } { }

  template<typename ...Args>
  void log(MySev ms, std::string_view message, Args&& ...args) const
  {
    // here some common fields are adde: UTC ISO timestamp, PID, thread ID
    impl_.log( message, ms, UtcIsoDateTime{}, Pid{}, ThreadId{}, std::forward<Args>(args)... );
  }

  But::Log::Logger<> impl_;
};


int main()
{
  MyLogger log{ But::makeSharedNN<But::Log::Destination::Console>() };
  log.info("hello, world");
  {
    auto proxy = log.withFields( FileName{__FILE__} );
    proxy.warning("that does not look right - PTAL here", LineNumber{__LINE__} );
    proxy.error("a nah - sorry - false alarm");
  }
  log.flush();
  log.reload();
}
