#pragma once
#include <vector>
#include "But/NotNull.hpp"
#include "Sink.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

/** @brief template that can hold multiple foreign sink and forwards all calls to all of them.
 *
 *  @note this sink does NOT forward exceptions from any destinations it forwards log to.
 */
class MultiSink final: public Sink
{
public:
  using value_type = NotNullShared<Sink>;
  using collection_type = std::vector<value_type>;

  explicit MultiSink(collection_type dsts): dsts_{ std::move(dsts) } { }

private:
  void logImpl(Backend::FieldInfo const& fi) override { logImplNe(fi); }
  void reloadImpl() override { reloadImplNe(); }
  void flushImpl() override { flushImplNe(); }

  void logImplNe(Backend::FieldInfo const& fi) noexcept;
  void reloadImplNe() noexcept;
  void flushImplNe() noexcept;

  collection_type dsts_;
};

}
}
}
