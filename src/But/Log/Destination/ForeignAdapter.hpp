#pragma once
#include "But/NotNull.hpp"

namespace But
{
namespace Log
{
namespace Destination
{

template<typename FT>
class ForeignAdapter final
{
public:
  template<typename T>
  explicit ForeignAdapter(T&& foreign): foreign_{ std::forward<T>(foreign) } { }
  ~ForeignAdapter() = default;

  ForeignAdapter(ForeignAdapter const&) = default;
  ForeignAdapter& operator=(ForeignAdapter const&) = default;
  ForeignAdapter(ForeignAdapter&&) = default;
  ForeignAdapter& operator=(ForeignAdapter&&) = default;

  template<typename ...Args>
  void log(Args&& ...args)
  {
    e_.clear();
    e_.reserve( sizeof...(args) );
    foreign_->log(e_, std::forward<Args>(args)...);
  }

  void reload() { foreign_->reload(); }
  void flush() { foreign_->flush(); }

  auto operator->() { return this; }

private:
  FT foreign_;
  Backend::Entry e_;
};

}
}
}
