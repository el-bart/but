#pragma once
#include <memory>
#include <future>
#include "Fifo.hpp"
#include "JoiningThread.hpp"

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
    auto cmd = std::make_unique<Task<F>>( std::move(f) );
    auto fut = cmd->promise_.get_future();
    {
      const Queue::lock_type lock{q_};
      q_.push( std::move(cmd) );
    }
    return fut;
  }

private:
  struct Command
  {
    virtual ~Command() { }
    virtual void run() = 0;
  };

  template<typename F>
  struct Task final: public Command
  {
    explicit Task(F f): f_{ std::move(f) } { }

    virtual void run() noexcept
    {
      try
      {
        const auto tag = static_cast<value_type const*>(nullptr);
        runAndSetValue(tag);
      }
      catch(...)
      {
        promise_.set_exception( std::current_exception() );
      }
    }

    void runAndSetValue(void const*)
    {
      f_();
      promise_.set_value();
    }

    template<typename Ignore>
    void runAndSetValue(Ignore const*)
    {
      promise_.set_value( f_() );
    }

    F f_;
    std::promise<decltype(f_())> promise_;

    using value_type = typename std::decay<decltype(f_())>::type;
  };

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

  using Queue = Fifo<std::unique_ptr<Command>>;

  Queue                      q_;
  std::atomic<bool>          quit_;
  JoiningThread<std::thread> th_;
};

}
}
