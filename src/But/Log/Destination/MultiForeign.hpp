#pragma once
#include <vector>
#include "But/NotNull.hpp"
#include "Foreign.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

/** @brief template that can hold multiple foreign destinations and forwards all calls to all of them.
 *
 *  @note interface allows to change destinations at run time, eg. via config file.
 *
 *  @note this destination does NOT forward exceptions from any destinations it forwards log to.
 */
class MultiForeign final: public Foreign
{
public:
  using value_type = NotNullShared<Foreign>;
  using collection_type = std::vector<value_type>;

  explicit MultiForeign(collection_type dsts): dsts_{ std::move(dsts) } { }

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
