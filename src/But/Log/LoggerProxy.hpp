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
  void log(Args&& ...args) const
  {
    try
    {
      dst_->log( std::forward<Args>(args)... );
    }
    catch(...)
    { /* this is <del>sparta</del> logger! */ }
  }

private:
  mutable Destination dst_;
};

}
}
