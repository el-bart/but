#pragma once
#include <memory>
#include <future>
#include "Fifo.hpp"
#include "JoiningThread.hpp"
#include "detail/Command.hpp"

namespace But
{
namespace Threading
{

class ActiveObject final
{
public:
  ActiveObject():
    quit_{false},
    th_{&ActiveObject::processingLoop, this}
  { }

  ~ActiveObject()
  {
    try
    {
      quit_ = true;
      // unblock thread, if needed
      Queue::lock_type lock{q_};
      q_.push(Queue::value_type{});
    }
    catch(...)
    {
      assert(!"failed to stop thread - givin up...");
      abort();
    }
  }

  template<typename F>
  auto run(F f)
  {
    auto cmd = std::make_unique<detail::Task<F>>( std::move(f) );
    auto fut = cmd->promise_.get_future();
    {
      const Queue::lock_type lock{q_};
      q_.push( std::move(cmd) );
    }
    return fut;
  }

private:
  auto getCommand()
  {
    Queue::lock_type lock{q_};
    auto cmd = std::move( q_.top(lock) );
    q_.pop();
    return cmd;
  }

  void processingLoop() noexcept
  {
    while(not quit_)
    {
      try
      {
        auto cmd = getCommand();
        if(cmd)
          cmd->run();
      }
      catch(...)
      {
        assert(!"unexpected exception from processing thread - this should never happen");
      }
    }
  }

  using Queue = Fifo<std::unique_ptr<detail::Command>>;

  Queue q_;
  std::atomic<bool> quit_;
  JoiningThread<std::thread> th_;
};

}
}
