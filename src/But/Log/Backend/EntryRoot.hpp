#pragma once
#include <string>
#include <memory>
#include "EntryProxy.hpp"


namespace But::Log::Backend
{

struct EntryProxy;

struct EntryRoot
{
  EntryRoot();
  ~EntryRoot();

  EntryRoot(EntryRoot const&) = default;
  EntryRoot& operator=(EntryRoot const&) = default;

  EntryRoot(EntryRoot&&) = default;
  EntryRoot& operator=(EntryRoot&&) = default;

  EntryRoot independentCopy() const { return EntryRoot{*impl_}; }

  EntryProxy proxy();
  std::string json() const;

private:
  struct Impl;

  explicit EntryRoot(Impl const& impl);

  std::shared_ptr<Impl> impl_;
};

}
