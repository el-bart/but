#pragma once
#include <vector>
#include <memory>
#include <atomic>
#include "Fifo.hpp"
#include "ThreadsCount.hpp"
#include "JoiningThread.hpp"
#include "detail/Command.hpp"

namespace But
{
namespace Threading
{

template<typename Policy>
class ThreadPool final
{
public:
  template<typename T>
  using promise_type = typename Policy::template promise_type<T>;
  using thread_type = typename Policy::thread_type;

  explicit ThreadPool(const ThreadsCount tc):
    quit_{false}
  {
    threads_.reserve( tc.count() );
    for(ThreadsCount::value_type i=0; i < tc.count(); ++i)
      threads_.emplace_back( &ThreadPool::processingLoop, this );
  }

  ~ThreadPool()
  {
    try
    {
      quit_ = true;
      // unblock all threads, if needed
      Queue::lock_type lock{q_};
      for(ThreadsCount::value_type i=0; i < size(); ++i)
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
    auto cmd = std::make_unique< detail::Task<F,Policy> >( std::move(f) );
    auto fut = cmd->promise_.get_future();
    {
      const Queue::lock_type lock{q_};
      q_.push( std::move(cmd) );
    }
    return fut;
  }

  auto size() const { return threads_.size(); }

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
  std::vector<JoiningThread<thread_type>> threads_;
};

}
}
