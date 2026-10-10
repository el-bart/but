#pragma once
#include <But/Log/Logger.hpp>
#include <But/Log/Destination/Console.hpp>
#include <But/Log/Field/LogLevel.hpp>
#include <But/Log/Field/PreciseDateTime.hpp>
#include <But/Log/Field/ThreadId.hpp>
#include <But/Log/Field/Pid.hpp>

namespace But::Log
{

struct DefaultLogger
{
  using DestPtr = But::Log::Logger<>::Destination;
  using LogLevel = But::Log::Field::LogLevel;

  explicit DefaultLogger(DestPtr d): impl_{ std::move(d) } { }

  template<typename ...Args>
  void debug(std::string_view message, Args&& ...args) const
  {
    log( LogLevel::debug, message, std::forward<Args>(args)... );
  }

  template<typename ...Args>
  void info(std::string_view message, Args&& ...args) const
  {
    log( LogLevel::info, message, std::forward<Args>(args)... );
  }

  template<typename ...Args>
  void warning(std::string_view message, Args&& ...args) const
  {
    log( LogLevel::warning, message, std::forward<Args>(args)... );
  }

  template<typename ...Args>
  void error(std::string_view message, Args&& ...args) const
  {
    log( LogLevel::error, message, std::forward<Args>(args)... );
  }

  template<typename ...Args>
  auto withFields(Args&& ...args) const
  {
    return DefaultLogger{ impl_.withFields( std::forward<Args>(args)... ) };
  }

  void reload() { impl_.reload(); }
  void flush()  { impl_.flush();  }

private:
  explicit DefaultLogger(But::Log::Logger<>&& l): impl_{ std::move(l) } { }

  template<typename ...Args>
  void log(LogLevel ll, std::string_view message, Args&& ...args) const
  {
    using But::Log::Field::Pid;
    using But::Log::Field::PreciseDateTime;
    using But::Log::Field::ThreadId;
    // here some common fields are adde: UTC ISO timestamp, PID, thread ID
    impl_.log( message, ll, PreciseDateTime{}, Pid{}, ThreadId{}, std::forward<Args>(args)... );
  }

  But::Log::Logger<> impl_;
};


template<typename Dst, typename ...Args>
auto makeDefaultLogger(Args&& ...args)
{
  return DefaultLogger{ But::makeSharedNN<Dst>( std::forward<Args>(args)... ) };
}

inline auto makeDefaultConsoleLogger(bool syncWithStdio = true)
{
  return makeDefaultLogger<But::Log::Destination::Console>(syncWithStdio);
}

}
