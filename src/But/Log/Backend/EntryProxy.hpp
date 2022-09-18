#pragma once
#include <string>
#include <cassert>
#include "detail/EntryImplFwd.hpp"

namespace But::Log::Backend
{

struct EntryRoot;

struct EntryProxy
{
  EntryProxy() = delete;

  EntryProxy(EntryProxy const&) = default;
  EntryProxy& operator=(EntryProxy const&) = default;

  EntryProxy(EntryProxy&&) = default;
  EntryProxy& operator=(EntryProxy&&) = default;

  // TODO: adding elements goes here

private:
  friend struct EntryProxy;

  explicit EntryProxy(detail::EntryImpl* impl): impl_{impl} { assert(impl_); }

  detail::EntryImpl& impl()
  {
    assert(impl_);
    return *impl_;
  }

  detail::EntryImpl* impl_;
};

}
