#pragma once
#include <vector>
#include <memory>
#include "Foregin.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

/** @brief template that can hold multiple foregin destinations and forwards all calls to all of them.
 *  @note interface allows to change destinations at run time, eg. via config file.
 */
class MultiForegin final: public Foregin
{
public:
  using value_type = std::shared_ptr<Foregin>;
  using collection_type = std::vector<value_type>;

  explicit MultiForegin(collection_type dsts): dsts_{ std::move(dsts) } { }

private:
  void logImpl(Backend::Entry e) override;
  void reloadImpl() override;
  void flushImpl() override;

  collection_type dsts_;
};

}
}
}
