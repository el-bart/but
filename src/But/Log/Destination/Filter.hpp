#pragma once
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
    BUT_ASSERT(filter_);
  }

private:
  void logImpl(Backend::FieldInfo const& fi) override
  {
    if( filter_(fi) )
      destination_->log(fi);
  }
  void reloadImpl() override { destination_->reload(); }
  void flushImpl() override { destination_->flush(); }

  std::function< bool(Backend::FieldInfo const&) > filter_;
  NotNullShared<Foreign> destination_;
};

}
}
}
