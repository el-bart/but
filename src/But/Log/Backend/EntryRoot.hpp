#pragma once
#include <string>
#include <memory>
#include "detail/EntryImplFwd.hpp"


namespace But::Log::Backend
{

struct EntryRoot
{
  EntryRoot();
  ~EntryRoot();

  EntryRoot(EntryRoot const&) = default;
  EntryRoot& operator=(EntryRoot const&) = default;

  EntryRoot(EntryRoot&&) = default;
  EntryRoot& operator=(EntryRoot&&) = default;

  EntryProxy proxy();
  std::string_view json() const;

private:
  std::shared_ptr<detail::EntryImpl> impl_;
};

}
