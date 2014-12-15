#pragma once
#include <memory>
#include <future>
#include <thread>

#include "Fifo.hpp"
#include "JoiningThread.hpp"

namespace But
{
namespace Threading
{

class ActiveObject final
{
public:
  ActiveObject();
  ~ActiveObject();

  template<typename F>
  auto run(F f)
  {
    using Ret = decltype(f());
    auto cmd = std::make_unique<Task<Ret>>( std::move(f) );
    auto fut = cmd->task_.get_future();
    Queue::lock_type lock{q_};
    q_.push( std::move(cmd) );
    return fut;
  }

private:
  struct Command
  {
    virtual ~Command() { }
    virtual void run() = 0;
  };

  template<typename R>
  struct Task: public Command
  {
    template<typename F>
    Task(F f): task_{ std::move(f) } { }
    virtual void run() noexcept { task_(); }
    std::packaged_task<R()> task_;
  };

  void processingLoop() noexcept;

  using Queue = Fifo<std::unique_ptr<Command>>;

  Queue                      q_;
  std::atomic<bool>          quit_;
  JoiningThread<std::thread> th_;
};

}
}
