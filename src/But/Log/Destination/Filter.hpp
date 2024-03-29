#pragma once
#include <functional>
#include <But/NotNull.hpp>
#include <But/Log/Destination/Sink.hpp>

namespace But
{
namespace Log
{
namespace Destination
{

/** @brief filtering backend, that pass through only logs, that are match by a given functor.
 */
class Filter: public Sink
{
public:
  /** @brief define a filter to accept logs.
   *  @param f functor, that returns true, if a given log is to be forwarded; false otherwise.
   *  @param sink destination that shall receive matching logs.
   */
  template<typename F>
  explicit Filter(F&& f, NotNullShared<Sink> sink):
    filter_{ std::forward<F>(f) },
    sink_{ std::move(sink) }
  {
    BUT_ASSERT(filter_);
  }

private:
  void logImpl(std::string&& str) override
  {
    if( filter_(str) )
      sink_->log( std::move(str) );
  }
  void reloadImpl() override { sink_->reload(); }
  void flushImpl() override { sink_->flush(); }

  std::function< bool(std::string const&) > filter_;
  NotNullShared<Sink> sink_;
};

}
}
}
