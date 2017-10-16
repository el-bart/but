#pragma once
#include <But/Log/Destination/Sink.hpp>
#include <But/Thrading/Fifo.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

/** @brief proxy-sink, that accepts all logs and forwards them to a separate thread.
 *  @warning this proxy does not forward errors from the background thread - they are ignored, instead.
 */
class BackgroundThread final: public Sink
{
public:
  BackgroundThread(But::NotNullShared<Sink> chained);
  ~BackgroundThread();

private:
  void logImpl(Backend::FieldInfo const& fi);
  void reloadImpl() override;
  void flushImpl() override;

  But::NotNullShared<Sink> chained_;
};

}
}
}
