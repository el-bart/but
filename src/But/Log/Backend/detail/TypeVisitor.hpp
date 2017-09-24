#pragma once
#include <string>
#include "But/Log/Backend/toType.hpp"

namespace But
{
namespace Log
{
namespace Backend
{
namespace detail
{

struct TypeVisitor final
{
  template<typename T>
  void operator()(T const& t) { type_ = toType(t); }

  Type type_;
};

}
}
}
}
