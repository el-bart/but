#pragma once
#include "EntryRoot.hpp"
#include "detail/EntryImpl.hpp"


namespace But::Log::Backend
{

EntryPoint(): impl_{ std::make_shared<detail::EntryImpl>() } { }


~EntryPoint() = default;


EntryProxy proxy()
{
  // TODO
  throw 42;
}


std::string_view json() const
{
  // TODO
  throw 42;
}

}
