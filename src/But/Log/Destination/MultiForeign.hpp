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
 *  @note interface allows to change destinations at run time, eg. via config file.
 */
class MultiForeign final: public Foreign
{
public:
  using value_type = NotNullShared<Foreign>;
  using collection_type = std::vector<value_type>;

  explicit MultiForeign(collection_type dsts): dsts_{ std::move(dsts) } { }

private:
  void logImpl(Backend::Entry const& e) override;
  void reloadImpl() override;
  void flushImpl() override;

  collection_type dsts_;
};

}
}
}
