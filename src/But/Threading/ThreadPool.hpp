#pragma once
#include <vector>
#include <memory>
#include <atomic>
#include <But/Threading/Fifo.hpp>
#include <But/Threading/ThreadsCount.hpp>
#include <But/Threading/JoiningThread.hpp>
#include <But/Threading/detail/Command.hpp>

namespace But
{
namespace Threading
{

class ThreadPool final
{
public:
  template<typename T>
  using promise_type = std::promise<T>;
  using thread_type = std::thread;

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
      BUT_ASSERT(!"failed to stop thread - givin up...");
      abort();
    }
  }

  template<typename F>
  auto run(F f)
  {
    auto cmd = std::make_unique< detail::Task<F> >( std::move(f) );
    auto fut = cmd->promise_.get_future();
    {
      const Queue::lock_type lock{q_};
      q_.push( std::move(cmd) );
    }
    return fut;
  }

  ThreadsCount::value_type size() const { return threads_.size(); }

private:
  auto getCommand()
  {
    Queue::lock_type lock{q_};
    q_.waitForNonEmpty(lock);
    auto cmd = std::move( q_.top() );
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
        BUT_ASSERT(!"unexpected exception from processing thread - this should never happen");
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
