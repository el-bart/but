#include <But/Log/Destination/BackgroundThread.hpp>
#include <But/assert.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

BackgroundThread::BackgroundThread(But::NotNullShared<Sink> chained):
  chained_{ std::move(chained) },
  thread_{ [this]{ this->threadLoop(); } }
{ }


BackgroundThread::~BackgroundThread()
{
  Queue::lock_type lock{queue_};
  queue_.push( Queue::value_type{} );
}

void BackgroundThread::logImpl(Backend::FieldInfo const& fi)
{
  auto copy = fi;
  Queue::lock_type lock{queue_};
  queue_.push( std::move(copy) );
}


void BackgroundThread::flushImpl()
{
  {
    Queue::lock_type lock{queue_};
    queue_.waitForSizeBelow(1, lock);
    BUT_ASSERT( queue_.empty() );
  }
  chained_->flush();
}


void BackgroundThread::threadLoop()
{
  while(true)
  {
    try
    {
      Queue::lock_type lock{queue_};
      queue_.waitForNonEmpty(lock);
      auto opt = std::move( queue_.top() );
      queue_.pop();
      lock.unlock();

      if(not opt)
      {
        Queue::lock_type lock{queue_};
        BUT_ASSERT( queue_.empty() && "some elements are left in the queue - data race on user's d-tor?" );
        break;
      }

      chained_->log( std::move(*opt) );
    }
    catch(...)
    { }
  }
}

}
}
}
