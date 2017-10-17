#pragma once
#include <thread>
#include <But/NotNull.hpp>
#include <But/Optional.hpp>
#include <But/Threading/Fifo.hpp>
#include <But/Threading/JoiningThread.hpp>
#include <But/Log/Destination/Sink.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

/** @brief proxy-sink, that accepts all logs and forwards them to a separate thread.
 *  @warning this proxy does not forward logging errors from the background thread - they are ignored, instead.
 *           flush() and reload() are run in callers thread, thus forward errors.
 *  @note flush() blocks until all logs in the queue are processed and then calls chained's flush(), and returns.
 */
class BackgroundThread final: public Sink
{
public:
  /** @param chained sink to forward logs to in a background thread.
   *  @param maximumQueueSize number of elements that can wait in a queue. 0 means no limit.
   */
  BackgroundThread(But::NotNullShared<Sink> chained, size_t maximumQueueSize);
  ~BackgroundThread();

private:
  void logImpl(Backend::FieldInfo const& fi);
  void reloadImpl() override { chained_->reload(); }
  void flushImpl() override;

  void threadLoop();

  using Queue = But::Threading::Fifo< But::Optional<Backend::FieldInfo> >;

  const size_t maximumQueueSize_;
  But::NotNullShared<Sink> chained_;
  Queue queue_;
  But::Threading::JoiningThread<std::thread> thread_;
};

}
}
}
