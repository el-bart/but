#pragma once
#include <memory>
#include <functional>
#include "But/NotNull.hpp"
#include "Foreign.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

/** @brief filtering backend, that pass through only logs, that are match by a given functor.
 */
class Filter: public Foreign
{
public:
  /** @brief define a filter to accept logs.
   *  @param f functor, that returns true, if a given log is to be forwarded; false otherwise.
   *  @param destination destination that shall receive matching logs.
   */
  template<typename F>
  explicit Filter(F&& f, NotNullShared<Foreign> destination):
    filter_{ std::forward<F>(f) },
    destination_{ std::move(destination) }
  {
    assert(filter_);
  }

private:
  void logImpl(Backend::Entry e) override
  {
    if( filter_(e) )
      destination_->log( std::move(e) );
  }
  void reloadImpl() override { destination_->reload(); }
  void flushImpl() override { destination_->flush(); }

  std::function<bool(Backend::Entry const&)> filter_;
  NotNullShared<Foreign> destination_;
};

}
}
}
