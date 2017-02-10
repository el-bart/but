#pragma once

namespace But
{
namespace Log
{

template<typename Destination>
class LoggerProxy final
{
public:
  explicit LoggerProxy(Destination dst): dst_{ std::move(dst) } { }

  template<typename ...Args>
  void log(Args&& ...args)
  {
    dst_->log( std::forward<Args>(args)... );
  }

private:
  Destination dst_;
};

}
}
