#pragma once
#include <But/NotNull.hpp>
#include <But/Log/Destination/Sink.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

/** @brief proxy-sink, that runs flush() after each log.
 */
class AutoFlush final: public Sink
{
public:
  /** @param chained sink to forward logs to in a background thread.
   *  @param maximumQueueSize number of elements that can wait in a queue. 0 means no limit.
   */
  explicit AutoFlush(But::NotNullShared<Sink> chained):
    chained_{ std::move(chained) }
  { }

private:
  void logImpl(Backend::FieldInfo const& fi) override
  {
    chained_->log(fi);
    chained_->flush();
  }
  void reloadImpl() override { chained_->reload(); }
  void flushImpl() override { chained_->flush(); }

  But::NotNullShared<Sink> chained_;
};

}
}
}
